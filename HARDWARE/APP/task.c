#include "stm32f10x.h"
#include "task.h"
#include "include.h" 

//初始化车辆遥控器结构体
remote_control RC = {0,1,0,0,0,0,0};
<<<<<<< HEAD

u8 checksum(u8 *temp){
	u8 back = 0;
	back = temp[3] ^ temp[4] ^ temp[5];
}
=======
>>>>>>> d21cc8e588ad563df3fa175ce38d0cf27afa87e1

//遥控指令函数
void remote_control_command(void){
	
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)//
	{	
<<<<<<< HEAD
		//Uart2_Start_DMA_Tx(5);
		RC.num++;
		if(RC.low_battery == 0){
			key_flag = KEY_Scan(0);
=======
		
		//如果蓝灯亮起说明wifi没连接上
		if(PBout(14) == 0){
			RC.num ++;
		}else{
			RC.num = 0;
>>>>>>> d21cc8e588ad563df3fa175ce38d0cf27afa87e1
		}

			
		
			
		
		
		//检测wifi掉线是否超过12秒
		if(RC.num >= 12000){
			//超过12秒拉低模块RST脚
			PAout(12) = 0;
			
			//两秒后拉高模块RST脚完成重启
			if(RC.num > 14000){
				RC.num = 0;
				PAout(12) = 1;
			}
			
			
		}
		
		//心跳计数
		RC.time ++;
		
		//计时4秒发送心跳
		if(RC.time >= 4000){
			Uart2_Tx[0] = 0xAA;
			Uart2_Tx[1] = 0x55;
			Uart2_Tx[2] = 0x01;
			Uart2_Tx[3] = 0x55;
			Uart2_Tx[4] = 0xAA;
			u2_SendArray(Uart2_Tx,5);
			RC.time = 0;
			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
		}
		
		//按键检测
		key_flag = KEY_Scan(0);
		
		//判断按键发送内容
		switch (key_flag)
        {
			//没有按键按下的时候清除发送数组
        	case 0:
			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
        		break;
			
			//急停按钮按下
        	case 1:
<<<<<<< HEAD
				RC.car_num++;
				if(RC.car_num > 8)
					RC.car_num = 8;
        		break;
			
			//车号减少
			case 2:
				RC.car_num--;
				if(RC.car_num < 1)
					RC.car_num = 1;
        		break;
				
			//急停一楼
			case 3:
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x01;
				//急停
				Uart2_Tx[5] = 0x01;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 16;
				u2_SendArray(Uart2_Tx,8);
				
				
        		break;
			
			//急停二楼
			case 4:
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 0x04;
//			
//				
//				RC.car_status = 16;
//				u2_SendArray(Uart2_Tx,5);
				
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x02;
				//急停
				Uart2_Tx[5] = 0x01;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 16;
				u2_SendArray(Uart2_Tx,8);
				
				
        		break;
			//自检一楼
			case 5:
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x01;
				//自检
				Uart2_Tx[5] = 0x02;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 3;
				u2_SendArray(Uart2_Tx,8);
			
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 0x05;
//			
//				RC.car_status = 3;
//				u2_SendArray(Uart2_Tx,5);
				
        		break;
			//自检二楼
			case 6:
				
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x02;
				//自检
				Uart2_Tx[5] = 0x02;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 3;
				u2_SendArray(Uart2_Tx,8);
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 0x06;
//			
//				RC.car_status = 3;
//				u2_SendArray(Uart2_Tx,5);
//				

        		break;
			//高速一楼
			case 7:
				
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x01;
				//高速
				Uart2_Tx[5] = 0x03;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 8;
				u2_SendArray(Uart2_Tx,8);
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 0x07;
//			
//				RC.car_status = 8;
//				u2_SendArray(Uart2_Tx,5);
        		break;
			//高速二楼
			case 8:
				//usart2_send_data("8577778\r\n");
				
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x02;
				//高速
				Uart2_Tx[5] = 0x03;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 8;
				u2_SendArray(Uart2_Tx,8);
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 8;
//			
//				RC.car_status = 8;
//				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			//中速一楼
			case 9:
				//usart2_send_data("8577778\r\n");
			
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x01;
				//自检
				Uart2_Tx[5] = 0x04;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 13;
				u2_SendArray(Uart2_Tx,8);
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 0x09;
//			
//				RC.car_status = 13;
//				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			//中速二楼
			case 10:
				//usart2_send_data("8577778\r\n");
			
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x02;
				//自检
				Uart2_Tx[5] = 0x04;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 13;
				u2_SendArray(Uart2_Tx,8);
				
				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 10;
//			
//				RC.car_status = 13;
//				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			//低速一楼
			case 11:
				//usart2_send_data("8577778\r\n");
			
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x01;
				//自检
				Uart2_Tx[5] = 0x05;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 12;
				u2_SendArray(Uart2_Tx,8);
			
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 11;
//				
//				RC.car_status = 12;
//				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			//低速二楼
			case 12:
				//usart2_send_data("8577778\r\n");
			
				//帧头
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//长度
				Uart2_Tx[2] = 0x03;
				//车号
				Uart2_Tx[3] = RC.car_num;
				//层号
				Uart2_Tx[4] = 0x02;
				//自检
				Uart2_Tx[5] = 0x05;
				
				//校验
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//帧尾
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 3;
				u2_SendArray(Uart2_Tx,8);
				
			
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = RC.car_num;
//				Uart2_Tx[4] = 12;
//			
//				RC.car_status = 12;
//				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
        	default:
        		break;
=======
				if(PAin(1) == 1){
					Uart2_Tx[0] = 0xfe;
					Uart2_Tx[1] = 0xb2;
					Uart2_Tx[2] = 0x03;
					Uart2_Tx[3] = 0xFF;
					Uart2_Tx[4] = 0x01;
					Uart2_Tx[5] = 0x01;
					Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
					Uart2_Tx[7] = 0xef;
				}else{
					Uart2_Tx[0] = 0xfe;
					Uart2_Tx[1] = 0xb2;
					Uart2_Tx[2] = 0x03;
					Uart2_Tx[3] = 0xFF;
					Uart2_Tx[4] = 0x02;
					Uart2_Tx[5] = 0x01;
					Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
					Uart2_Tx[7] = 0xef;
				}
				
				
				u2_SendArray(Uart2_Tx,8);
        		break;
				
			default:
				break;
>>>>>>> d21cc8e588ad563df3fa175ce38d0cf27afa87e1
        }
		
		
		
<<<<<<< HEAD
		//记录按键是否按下，以及未按下时间
		if(key_flag == 0){
			RC.control_idle_time++;
		}else if(key_flag!= 0){
			RC.control_idle_time = 0;
		}
		
		if(RC.num == 50){
			LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowAlphabetData[LED_Display1], gShowNumberData[LED_Display2]);
			RC.num = 0;
		}
		
		//Display();
		//LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowAlphabetData[LED_Display1], gShowNumberData[LED_Display2]);
		
=======
>>>>>>> d21cc8e588ad563df3fa175ce38d0cf27afa87e1
	}
}

void TIM2_IRQHandler(void)
{
	{
		
<<<<<<< HEAD
		
//		if(RC.battery_voltage < 3.5){
//			LED_Display1 = 12;
//			LED_Display2 = 15;
//		}
		
		//低电量显示
		if(RC.reference_voltage < 3.15f){
			RC.low_battery_num++;
			if(RC.low_battery_num >= 1000){
				//低电压状态位置1
				RC.low_battery = 1;
				//按键状态清零
				key_flag = 0;
				LED_Display1 = 12;
				LED_Display2 = 15;
				
				RC.low_battery_num = 1200;
				LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowAlphabetData[LED_Display1], gShowAlphabetData[LED_Display2]);
			}
			
			
			
		}else{
			RC.low_battery_num = 0;
		}
		
//		switch (RC.flag)
//        {
//        	case 0:
//				
//        		break;
//        	case 1:
//				usart2_send_data("AT+RESTORE\r\n");
//        		break;
//			case 2:
//				
//				usart2_send_data("AT+CWJAP=\"XHCTEST\",\"test66668888\",\"s.y\"\r\n");
//				usart2_send_data("AT+CIPMUX=0\r\n");
//				usart2_send_data("AT+SAVETRANSLINK=1,\"192.168.0.255\",10000,\"UDP\",10000\r\n");
//        		break;
//        	default:
//        		break;
//        }
//		
		//遥控器控制指令
=======

>>>>>>> d21cc8e588ad563df3fa175ce38d0cf27afa87e1
		remote_control_command();
		

		
		 
		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}

