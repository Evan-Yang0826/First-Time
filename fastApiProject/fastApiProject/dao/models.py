from tortoise import Model, fields


class Student(Model):
    id = fields.BigIntField(pk=True)
    name = fields.TextField(max_length=255)
    gradeName = fields.TextField(max_length=255)
    className = fields.TextField(max_length=255)
    dormitoryName = fields.TextField(max_length=255)
    cardNo = fields.TextField(max_length=255)

    class Meta:
        table = "student"

    def to_dict(self):
        return {
            "id": self.id,
            "name": self.name,
            "gradeName": self.gradeName,
            "className": self.className,
            "dormitoryName": self.dormitoryName,
            "cardNo": self.cardNo,
        }


class Device(Model):
    id = fields.BigIntField(pk=True)
    devicename = fields.TextField(max_length=255)
    devicemac = fields.TextField(max_length=255)
    devicelocation = fields.TextField(max_length=255)
    types = fields.IntField()
    startprohibit = fields.TextField(max_length=255)
    endprohibit = fields.TextField(max_length=255)

    class Meta:
        table = "device"

    def to_dict(self):
        return {
            "id": self.id,
            "devicename": self.devicename,
            "devicemac": self.devicemac,
            "devicelocation": self.devicelocation,
            "types": self.types,
            "startprohibit": self.startprohibit,
            "endprohibit": self.endprohibit,
        }


class User(Model):
    id = fields.BigIntField(pk=True)
    name = fields.TextField(max_length=255)
    username = fields.TextField(max_length=255)
    password = fields.TextField(max_length=255)
    roles = fields.IntField()

    class Meta:
        table = "user"

    def to_dict(self):
        return {
            "id": self.id,
            "name": self.name,
            "username": self.username,
            "password": self.password,
            "roles": self.roles,
        }


class Track(Model):
    id = fields.BigIntField(pk=True)
    status = fields.IntField()
    stuname = fields.TextField(max_length=255)
    location = fields.TextField(max_length=255)
    cardno = fields.TextField(max_length=255)
    ctime = fields.TextField(max_length=255)

    class Meta:
        table = "track"

    def to_dict(self):
        return {
            "id": self.id,
            "status": self.status,
            "stuname": self.stuname,
            "location": self.location,
            "cardno": self.cardno,
            "ctime": self.ctime,
        }

