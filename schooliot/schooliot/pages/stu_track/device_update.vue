<template>
	<view class="container">
		<uni-section title="设备信息" type="line">
			<view class="example">
				<!-- 基础用法，不包含校验规则 -->
				<uni-forms ref="baseForm" :rules="rules" :model="baseFormData" labelWidth="80px">
					<uni-forms-item label="设备名称" required>
						<uni-easyinput v-model="baseFormData.devicename" placeholder="请输入设备名称" />
					</uni-forms-item>
					<uni-forms-item label="mac地址" required>
						<uni-easyinput disabled v-model="baseFormData.devicemac" placeholder="请输入mac地址" />
					</uni-forms-item>
					<uni-forms-item label="设备位置" required>
						<uni-easyinput v-model="baseFormData.devicelocation" placeholder="请输入设备位置" />
					</uni-forms-item>
					<uni-forms-item label="设备类型" required>
						<uni-data-select v-model="baseFormData.types" :localdata="typesRange" >
						</uni-data-select>
					</uni-forms-item>
					<uni-forms-item label="禁止开始时间">
							<picker mode="time" :value="baseFormData.startprohibit" start="00:00" end="23:59" @change="startTimeChange" 
							v-model="baseFormData.startprohibit">
								<view class="uni-input">{{baseFormData.startprohibit}}</view>
							</picker>
					</uni-forms-item>
					<uni-forms-item label="禁止结束时间">
							<picker mode="time" :value="baseFormData.endprohibit" start="00:00" end="23:59" @change="endTimeChange" 
							v-model="baseFormData.endprohibit">
								<view class="uni-input">{{baseFormData.endprohibit}}</view>
							</picker>
					</uni-forms-item>
				</uni-forms>
				<uni-row class="demo-uni-row" >
					<view class="example-body box">
						<uni-col :span="12">
							<button type="primary" @click="submit('baseForm')">提交</button>
						</uni-col>
						<uni-col :span="12">
								<button type="primary" @click="goback">取消</button>
						</uni-col>
					</view>
				</uni-row>
			</view>
		</uni-section>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				timer:null,
				dev_id:'',
				// 基础表单数据
				baseFormData: {
					devicename: '',
					devicemac: '',
					devicelocation: '',
					types: 0,
					startprohibit: '00:01',
					endprohibit: '23:59'
				},
				
				typesRange: [
					{
						value: 0,
						text: "一般设备"
					},
					{
						value: 1,
						text: "栅栏"
					},
				],
				// 校验规则
				rules: {
					name: {
						rules: [{
							required: true,
							errorMessage: '姓名不能为空'
						}]
					},
					age: {
						rules: [{
							required: true,
							errorMessage: '年龄不能为空'
						}, {
							format: 'number',
							errorMessage: '年龄只能输入数字'
						}]
					}
				}
			}
		},
		computed: {
			// 处理表单排列切换
			alignment() {
				if (this.current === 0) return 'left'
				if (this.current === 1) return 'top'
				return 'left'
			}
		},
		onLoad: function (option) {
			this.dev_id = option.id;
		},
		onReady() {
			// 设置自定义表单校验规则，必须在节点渲染完毕后执行
			// this.$refs.customForm.setRules(this.customRules)
		},
		onShow() {
			this.getDev(this.dev_id)
			this.getMac()
		},
		onHide(){
			clearInterval(this.timer)
		},
		onUnload(){
			clearInterval(this.timer)
		},
		destroyed(){
			clearInterval(this.timer)
		},
		methods: {
			getMac(){
				this.timer = setInterval(() => {
				  uni.request({
				      url: '/api/stm32/mac', //仅为示例，并非真实接口地址。
				      method:'GET',
				      header: {
				      		'content-type': 'application/json'
				      	},
				      success: (res) => {
						  if(res.data.msg != null && res.data.msg.length > 0){
							  this.baseFormData.devicemac = res.data.msg
						  }
				      }
				  });
				}, 2000)
			},
			startTimeChange: function(e) {
				this.baseFormData.startprohibit = e.detail.value
			},
			endTimeChange: function(e) {
				this.baseFormData.endprohibit = e.detail.value
			},
			getDev(id){
				uni.request({
				    url: '/api/device/'+id, //仅为示例，并非真实接口地址。
				    method:'GET',
				    header: {
				    		'content-type': 'application/json'
				    	},
				    success: (res) => {
						this.baseFormData = res.data.msg;
				    }
				});
			},
			goback(){
				clearInterval(this.timer)
				uni.navigateBack({
				            //保留当前页面，跳转到应用内的某个页面
				            delta:1
				        })					
			},
			submit(ref) {
				this.$refs[ref].validate().then(res => {
					uni.request({
					    url: '/api/device/update/'+this.dev_id, //仅为示例，并非真实接口地址。
					    method:'POST',
						dataType:'json',
						data:this.baseFormData,
					    header: {
					    		'content-type': 'application/json'
					    	},
					    success: (res) => {
					        // console.log(res.data.msg);
							this.goback()
					    }
					});
				}).catch(err => {
					console.log('err', err);
				})
			},
		}
	}
</script>

<style lang="scss">
	.example {
		padding: 15px;
		background-color: #fff;
	}

	.segmented-control {
		margin-bottom: 15px;
	}

	.button-group {
		margin-top: 15px;
		display: flex;
		justify-content: space-around;
	}

	.form-item {
		display: flex;
		align-items: center;
		flex: 1;
	}

	.button {
		display: flex;
		align-items: center;
		height: 35px;
		line-height: 35px;
		margin-left: 10px;
	}
</style>
