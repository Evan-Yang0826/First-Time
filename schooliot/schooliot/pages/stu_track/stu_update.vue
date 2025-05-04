<template>
	<view class="container">
		<!-- <uni-card :is-shadow="false" is-full>
			<text class="uni-h6">uni-forms 组件一般由输入框、选择器、单选框、多选框等控件组成，用以收集、校验、提交数据。</text>
		</uni-card> -->
		<uni-section title="学生信息" type="line">
			<view class="example">
				<!-- 基础用法，不包含校验规则 -->
				<uni-forms ref="baseForm" :rules="rules" :model="baseFormData" labelWidth="80px">
					<uni-forms-item label="姓名" required name="name">
						<uni-easyinput v-model="baseFormData.name" placeholder="请输入姓名" />
					</uni-forms-item>
					<uni-forms-item label="年级" required name="gradeName">
						<uni-easyinput v-model="baseFormData.gradeName" placeholder="请输入年级" />
					</uni-forms-item>
					<uni-forms-item label="班级" required name="className">
						<uni-easyinput v-model="baseFormData.className" placeholder="请输入班级" />
					</uni-forms-item>
					<uni-forms-item label="寝室" required name="dormitoryName">
						<uni-easyinput v-model="baseFormData.dormitoryName" placeholder="请输入寝室" />
					</uni-forms-item>
					<uni-forms-item label="卡号" disabled required name="cardNo">
						<uni-easyinput disabled v-model="baseFormData.cardNo" placeholder="请输入卡号" />
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
				stu_id:'',
				// 基础表单数据
				baseFormData: {
					id:'',
					name: '',
					gradeName: '',
					className: '',
					dormitoryName: '',
					cardNo: '',
				},
				// 校验规则
				rules: {
					name: {
						rules: [{
							required: true,
							errorMessage: '姓名不能为空'
						}]
					},
					gradeName: {
						rules: [{
							required: true,
							errorMessage: '年级不能为空'
						}]
					},
					className: {
						rules: [{
							required: true,
							errorMessage: '班级不能为空'
						}]
					},
					dormitoryName: {
						rules: [{
							required: true,
							errorMessage: '寝室不能为空'
						}]
					},
					cardNo: {
						rules: [{
							required: true,
							errorMessage: '卡号不能为空'
						}]
					}
				},
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
			this.stu_id = option.id;
		},
		onReady() {
			// 设置自定义表单校验规则，必须在节点渲染完毕后执行
			// this.$refs.baseFormData.setRules(this.rules)
		},
		onShow() {
			this.getStu(this.stu_id)
			this.getCardNo()
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
			getCardNo(){
				this.timer = setInterval(() => {
				  uni.request({
				      url: '/api/stm32/card', //仅为示例，并非真实接口地址。
				      method:'GET',
				      header: {
				      		'content-type': 'application/json'
				      	},
				      success: (res) => {
						  if(res.data.msg != null && res.data.msg.length > 0){
							  this.baseFormData.cardNo = res.data.msg
						  }
				      }
				  });
				}, 2000)
			},
			getStu(id){
				uni.request({
				    url: '/api/student/'+id, //仅为示例，并非真实接口地址。
				    method:'GET',
				    header: {
				    		'content-type': 'application/json'
				    	},
				    success: (res) => {
				        // console.log(res.data.msg);
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
					    url: '/api/student/update/'+this.stu_id, //仅为示例，并非真实接口地址。
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
	.dark {
		background-color: #d3dce6;
	}
	
	.light {
		background-color: #e5e9f2;
	}
	.demo-uni-col {
		height: 36px;
		border-radius: 5px;
	}
	.demo-uni-row {
		margin-bottom: 10px;
	
		// 组件在小程序端display为inline
		// QQ、字节小程序文档写有 :host，但实测不生效
		// 百度小程序没有 :host
		/* #ifdef MP-TOUTIAO || MP-QQ || MP-BAIDU */
		display: block;
		/* #endif */
	}
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
