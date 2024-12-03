#include "stm32f10x.h"
#include "task.h"
#include "include.h" 

//��ʼ������ң�����ṹ��
remote_control RC = {0,1,0,0,0,0,0};

u8 checksum(u8 *temp){
	u8 back = 0;
	back = temp[3] ^ temp[4] ^ temp[5];
}

//ң��ָ���
void remote_control_command(void){
	
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)//
	{	
		//Uart2_Start_DMA_Tx(5);
		if(RC.low_battery == 0){
			key_flag = KEY_Scan(0);
		}

		
		//�жϰ�����������
		switch (key_flag)
        {
			//û�а������µ�ʱ�������������
        	case 0:
			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
        		break;
			
			//��ͣ��ť����
        	case 1:
				RC.car_num++;
				if(RC.car_num > 8)
					RC.car_num = 8;
        		break;
			
			//���ż���
			case 2:
				RC.car_num--;
				if(RC.car_num < 1)
					RC.car_num = 1;
        		break;
				
			//��ͣһ¥
			case 3:
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x01;
				//��ͣ
				Uart2_Tx[5] = 0x01;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 16;
				u2_SendArray(Uart2_Tx,8);
				
				
        		break;
			
			//��ͣ��¥
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
				
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x02;
				//��ͣ
				Uart2_Tx[5] = 0x01;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 16;
				u2_SendArray(Uart2_Tx,8);
				
				
        		break;
			//�Լ�һ¥
			case 5:
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x01;
				//�Լ�
				Uart2_Tx[5] = 0x02;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			//�Լ��¥
			case 6:
				
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x02;
				//�Լ�
				Uart2_Tx[5] = 0x02;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			//����һ¥
			case 7:
				
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x01;
				//����
				Uart2_Tx[5] = 0x03;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			//���ٶ�¥
			case 8:
				//usart2_send_data("8577778\r\n");
				
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x02;
				//����
				Uart2_Tx[5] = 0x03;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			
			//����һ¥
			case 9:
				//usart2_send_data("8577778\r\n");
			
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x01;
				//�Լ�
				Uart2_Tx[5] = 0x04;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			
			//���ٶ�¥
			case 10:
				//usart2_send_data("8577778\r\n");
			
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x02;
				//�Լ�
				Uart2_Tx[5] = 0x04;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			
			//����һ¥
			case 11:
				//usart2_send_data("8577778\r\n");
			
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x01;
				//�Լ�
				Uart2_Tx[5] = 0x05;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
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
			
			//���ٶ�¥
			case 12:
				//usart2_send_data("8577778\r\n");
			
				//֡ͷ
				Uart2_Tx[0] = 0xfe;
				Uart2_Tx[1] = 0xb1;
				//����
				Uart2_Tx[2] = 0x03;
				//����
				Uart2_Tx[3] = RC.car_num;
				//���
				Uart2_Tx[4] = 0x02;
				//�Լ�
				Uart2_Tx[5] = 0x05;
				
				//У��
				Uart2_Tx[6] = Uart2_Tx[3] ^ Uart2_Tx[4] ^ Uart2_Tx[5];
				
				//֡β
				Uart2_Tx[7] = 0xef;
				
				RC.car_status = 12;
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
        }
		
		LED_Display1 = RC.car_status;
		LED_Display2 = RC.car_num;
		
		//��¼�����Ƿ��£��Լ�δ����ʱ��
		if(key_flag == 0){
			RC.control_idle_time++;
		}else if(key_flag!= 0){
			RC.control_idle_time = 0;
		}
		
	}
}

void TIM2_IRQHandler(void)
{
	{
		
		//��׼��ѹ����
		RC.reference_voltage = (1.2f/adcSampleValue[1]) * 4095;
		//��ص�ѹ����
		RC.battery_voltage = (adcSampleValue[0] / 4095.0f) * RC.reference_voltage * 2.0;
		
		if(RC.battery_voltage < 3.5){
			LED_Display1 = 12;
			LED_Display2 = 15;
		}
		
		//�͵�����ʾ
		if(RC.reference_voltage < 3.15f){
			RC.low_battery_num++;
			if(RC.low_battery_num >= 1000){
				//�͵�ѹ״̬λ��1
				RC.low_battery = 1;
				//����״̬����
				key_flag = 0;
				LED_Display1 = 12;
				LED_Display2 = 15;
				
				RC.low_battery_num = 1200;
				LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowAlphabetData[LED_Display1], gShowAlphabetData[LED_Display2]);
			}
			
			
			
		}else{
			RC.low_battery_num = 0;
		}
		

		
		//ң��������ָ��
		remote_control_command();
		

		
		 
		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}

