import json
import threading
import time

import serial
import uvicorn
from fastapi import FastAPI, Request, Body, Query
from fastapi.responses import JSONResponse
from tortoise.contrib.fastapi import register_tortoise

from dao.models import User, Student, Device, Track

app = FastAPI()

# 数据库绑定
register_tortoise(app, db_url="mysql://root:Jinxincun2023%23%23@localhost:3306/schoollocal",
                  modules={"models": ['dao.models']},
                  add_exception_handlers=True,
                  generate_schemas=True)

if __name__ == '__main__':
    uvicorn.run(app)


@app.get("/api/")
def index():
    return JSONResponse(status_code=200, content={"msg": "get user"})


@app.get("/api/stm32/open")
def open_stm32():
    ser_thread = threading.Thread(target=open_ser)
    ser_thread.start()


def open_ser():
    port = 'COM4'  # 串口号
    baudrate = 9600  # 波特率
    try:
        global ser, data_pre, track_pre, trackTag
        trackTag = False
        ser = serial.Serial(port, baudrate, timeout=2)
        if ser.isOpen() == False:
            ser.open()
        while True:
            n = ser.readline().hex()  # serial包的用法 读取串口的一行数据
            data_pre = str(n)
            # track_pre = str(n)
            print("data_pre:", data_pre)
            trackTag = False
            if data_pre is None:
                continue
            if len(data_pre) < 20:
                continue  # 把读取的数据转换成字符串
            trackTag = True
            track_pre = str(n)
            print("mac:", data_pre[8:20])
            print("card:", data_pre[20:28])

    except Exception as exc:
        print("串口打开异常", exc)


@app.get("/api/stm32/track")
async def stm32track():
    if trackTag == False:
        return JSONResponse(status_code=200, content={"msg": "success"})
    if (track_pre is not None and len(track_pre) > 20):
        cardstr = track_pre[20:28]
        macstr = track_pre[8:20]
        device = await Device.filter(devicemac=macstr).first()
        if device.types == 1:
            time1 = time.strptime(device.startprohibit, "%H:%M")
            time2 = time.strptime(device.endprohibit, "%H:%M")
            beginTime = time1.tm_hour * 60 + time1.tm_min
            endTime = time2.tm_hour * 60 + time2.tm_min
            nowTime = time.localtime().tm_hour * 60 + time.localtime().tm_min
            print("beginTime", beginTime)
            print("endTime", endTime)
            print("nowTime", nowTime)
            if beginTime <= endTime:
                if beginTime <= nowTime <= endTime:
                    stu = await Student.filter(cardNo=cardstr).first()
                    await Track(status=1, stuname=stu.name, location=device.devicelocation, cardno=stu.cardNo,
                                ctime=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())).save()
            if beginTime >= endTime:
                if nowTime >= beginTime or (endTime >= nowTime >= 0):
                    stu = await Student.filter(cardNo=cardstr).first()
                    await Track(status=1, stuname=stu.name, location=device.devicelocation, cardno=stu.cardNo,
                                ctime=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())).save()
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.get("/api/stm32/card")
async def stm32():
    tag = 0
    if data_pre is not None and len(data_pre) > 20:
        tag = data_pre[20:28]
    return JSONResponse(status_code=200, content={"msg": tag})


@app.get("/api/stm32/mac")
async def mac_add():
    tag = 0
    if data_pre is not None and len(data_pre) > 20:
        tag = data_pre[8:20]
    return JSONResponse(status_code=200, content={"msg": tag})


@app.get("/api/users")
async def users(req: Request):
    users = await User.all()
    jsonData = []
    if len(users) > 0:
        for user in users:
            jsonData.append(user.to_dict())
    return JSONResponse(status_code=200, content={"msg": json.dumps(jsonData, ensure_ascii=False)})


@app.get("/api/user/{user_id}")
async def user_id(user_id: int):
    user = await User.get(id=user_id)
    return JSONResponse(status_code=200, content={"msg": user.to_dict()})


@app.post("/api/user/del/{user_id}")
async def de_user_id(user_id: int):
    user = await User.get(id=user_id)
    if user:
        await User.delete(user)
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.post("/api/user/update/{user_id}")
async def update_user_id(user_id: int, name=Body(None), username=Body(None), password=Body(None), roles=Body(None)):
    user = await User.get(id=user_id)
    if user:
        if name is not None:
            user.name = name
        if username is not None:
            user.username = username
        if password is not None:
            user.password = password
        if roles is not None:
            user.roles = roles
        await User.filter(id=user_id).update(name=user.name, username=user.username, password=user.password,
                                             roles=user.roles)
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.post("/api/user/insert")
async def insert_user_id(name=Body(None), username=Body(None), password=Body(None), roles=Body(None)):
    await User(name=name, username=username, password=password,
               roles=roles).save()
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.get("/api/students")
async def students():
    students = await Student.all()
    jsonData = []
    if len(students) > 0:
        for student in students:
            jsonData.append(student.to_dict())
    return JSONResponse(status_code=200, content={"msg": json.dumps(jsonData, ensure_ascii=False)})


@app.get("/api/student/{stu_id}")
async def stu_id(stu_id: int):
    stu = await Student.get(id=stu_id)
    return JSONResponse(status_code=200, content={"msg": stu.to_dict()})


@app.post("/api/student/del/{stu_id}")
async def de_stu_id(stu_id: int):
    stu = await Student.get(id=stu_id)
    if stu:
        await Student.delete(stu)
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.post("/api/student/update/{stu_id}")
async def update_stu_id(stu_id: int, name=Body(None), gradeName=Body(None), className=Body(None),
                        dormitoryName=Body(None), cardNo=Body(None)):
    stu = await Student.get(id=stu_id)
    if stu:
        if name is not None:
            stu.name = name
        if gradeName is not None:
            stu.gradeName = gradeName
        if className is not None:
            stu.className = className
        if dormitoryName is not None:
            stu.dormitoryName = dormitoryName
        if cardNo is not None:
            stu.cardNo = cardNo
        await Student.filter(id=stu_id).update(name=stu.name, gradeName=stu.gradeName, className=stu.className,
                                               dormitoryName=stu.dormitoryName, cardNo=stu.cardNo)
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.post("/api/student/insert")
async def insert_student_id(name=Body(None), gradeName=Body(None), className=Body(None),
                            dormitoryName=Body(None), cardNo=Body(None)):
    await Student(name=name, gradeName=gradeName, className=className,
                  dormitoryName=dormitoryName, cardNo=cardNo).save()
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.get("/api/devices")
async def devices():
    devices = await Device.all()
    jsonData = []
    if len(devices) > 0:
        for device in devices:
            jsonData.append(device.to_dict())
    return JSONResponse(status_code=200, content={"msg": json.dumps(jsonData, ensure_ascii=False)})


@app.get("/api/device/{device_id}")
async def dev_id(device_id: int):
    device = await Device.get(id=device_id)
    return JSONResponse(status_code=200, content={"msg": device.to_dict()})


@app.post("/api/device/del/{device_id}")
async def de_device_id(device_id: int):
    device = await Device.get(id=device_id)
    if device:
        await Device.delete(device)
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.post("/api/device/update/{device_id}")
async def update_device_id(device_id: int, devicename=Body(None), devicemac=Body(None), devicelocation=Body(None),
                           types=Body(None),
                           startprohibit=Body(None), endprohibit=Body(None)):
    device = await Device.get(id=device_id)
    if device:
        if devicename is not None:
            device.devicename = devicename
        if devicemac is not None:
            device.devicemac = devicemac
        if devicelocation is not None:
            device.devicelocation = devicelocation
        if types is not None:
            device.types = types
        if startprohibit is not None:
            device.startprohibit = startprohibit
        if endprohibit is not None:
            device.endprohibit = endprohibit
        await Device.filter(id=device_id).update(devicename=device.devicename, devicemac=device.devicemac,
                                                 devicelocation=device.devicelocation,
                                                 types=device.types, startprohibit=device.startprohibit,
                                                 endprohibit=device.endprohibit)
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.post("/api/device/insert")
async def insert_device_id(devicename=Body(None), devicemac=Body(None), devicelocation=Body(None),
                           types=Body(None),
                           startprohibit=Body(None), endprohibit=Body(None)):
    await Device(devicename=devicename, devicemac=devicemac, devicelocation=devicelocation,
                 types=types, startprohibit=startprohibit, endprohibit=endprohibit).save()
    return JSONResponse(status_code=200, content={"msg": "success"})


@app.get("/api/tracks")
async def tracks(req: Request):
    tracks = await Track.all()
    jsonData = []
    if len(tracks) > 0:
        for track in tracks:
            jsonData.append(track.to_dict())
    return JSONResponse(status_code=200, content={"msg": json.dumps(jsonData, ensure_ascii=False)})


# 刷卡逻辑，判断有无此卡号关联学生，判断有无该mac地址设备，有则添加记录，无则不操作；向页面发送卡号、mac地址、

@app.post("/api/login")
async def login(username=Query(None), password=Query(None)):
    user = await User.filter(username=username, password=password).first()
    if user:
        return JSONResponse(status_code=200, content={"msg": "success"})
    else:
        return JSONResponse(status_code=200, content={"msg": "failed"})
