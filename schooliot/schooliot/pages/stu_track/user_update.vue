<template>
	<view class="container">
		<!-- <uni-card :is-shadow="false" is-full>
			<text class="uni-h6">uni-forms 组件一般由输入框、选择器、单选框、多选框等控件组成，用以收集、校验、提交数据。</text>
		</uni-card> -->
		<uni-section title="用户信息" type="line">
			<view class="example">
				<!-- 基础用法，不包含校验规则 -->
				<uni-forms ref="baseForm" :rules="rules" :model="baseFormData" labelWidth="80px">
					<uni-forms-item label="姓名" required name="name">
						<uni-easyinput v-model="baseFormData.name" placeholder="请输入姓名" />
					</uni-forms-item>
					<uni-forms-item label="用户名" required name="username">
						<uni-easyinput v-model="baseFormData.username" placeholder="请输入用户名" />
					</uni-forms-item>
					<uni-forms-item label="密码" required name="password">
						<uni-easyinput v-model="baseFormData.password" type="password" placeholder="请输入密码" />
					</uni-forms-item>
					<uni-forms-item label="选择技能">
						<uni-data-select v-model="baseFormData.roles" :localdata="rolesRange" >
						</uni-data-select>
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
				userId:'',
				// 基础表单数据
				baseFormData: {
					name: '',
					username: '',
					password: '',
					roles: 0,
				},
				// 校验规则
				rules: {
					name: {
						rules: [{
							required: true,
							errorMessage: '姓名不能为空'
						}]
					},
					username: {
						rules: [{
							required: true,
							errorMessage: '用户名不能为空'
						}]
					},
					password: {
						rules: [{
							required: true,
							errorMessage: '密码不能为空'
						}]
					},
					roles: {
						rules: [{
							required: true,
							errorMessage: '角色不能为空'
						}]
					}
				},
				rolesRange: [{
						value: 0,
						text: "一般工作人员"
					},
					{
						value: 1,
						text: "管理员"
					},
				]
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
			this.userId = option.id;
		},
		onReady() {
			// 设置自定义表单校验规则，必须在节点渲染完毕后执行
			// this.$refs.customForm.setRules(this.customRules)
		},
		onShow() {
			this.getUser(this.userId)
		},
		methods: {
			getUser(id){
				uni.request({
				    url: '/api/user/'+id, //仅为示例，并非真实接口地址。
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
				uni.navigateBack({
				            //保留当前页面，跳转到应用内的某个页面
				            delta:1
				        })					
			},
			add() {
				this.dynamicFormData.domains.push({
					label: '域名',
					value: '',
					rules: [{
						'required': true,
						errorMessage: '域名项必填'
					}],
					id: Date.now()
				})
			},
			submit(ref) {
				this.$refs[ref].validate().then(res => {
					uni.request({
					    url: '/api/user/update/'+this.userId, //仅为示例，并非真实接口地址。
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
