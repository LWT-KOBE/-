#include "stm32f10x.h"
#include "task.h"
#include "include.h" 

//��ʼ������ң�����ṹ��
remote_control RC = {0,1,0,0,0};

//ң��ָ���
void remote_control_command(void){
	char *atCommand = "AT+CWJAP=\"RedmiLWT\",\"12345678\",\"s.y\"\r\n";
	char *atCommand1 = "AT+CWJAP=\"XHCTEST\",\"test66668888\",\"s.y\"\r\n";
	char *atCommand2 = "AT+CWJAP=\"dd\",\"dd66668888\",\"s.y\"\r\n";
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)//
	{	
		//Uart2_Start_DMA_Tx(5);
		if(RC.low_battery == 0){
			key_flag = KEY_Scan(0);
		}
		
		//KeyScan();
		//usart2_send_data("okkkk");
		switch (key_flag)
        {
			//û�а������µ�ʱ�������������
        	case 0:
			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
        		break;
			
			//��������
        	case 1:
				RC.car_num++;
				if(RC.car_num > 8)
					RC.car_num = 8;
        		break;
			
			//���ż���
			case 2:
				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.88.77\",1000,1000\r\n");
				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.0.255\",1000,1002\r\n");
				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.0.105\",1000,1002\r\n");
				RC.car_num--;
				if(RC.car_num < 1)
					RC.car_num = 1;
				
//				LED_Display1 = 1;
//				LED_Display2 = 10;
				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.0.106\",1000,1002\r\n");
        		break;
				
			//��ͣ
			case 3:
				//usart2_send_data("AT+CIPSTATUS\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x03;
				
				RC.car_status = 16;
				u2_SendArray(Uart2_Tx,5);
				
			
        		break;
			
			//�Լ�
			case 4:
				
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x04;
			
				RC.car_status = 15;
				u2_SendArray(Uart2_Tx,5);
			
				
//				LED_Display1 = 0;
//				LED_Display2 = 0;
        		break;
			//����
			case 5:
				//usart2_send_data("wsd58\r\n");
				//usart2_send_data("AT+SAVETRANSLINK=1,\"192.168.0.105\",1000,\"UDP\",1002\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x05;
			
				RC.car_status = 3;
				u2_SendArray(Uart2_Tx,5);
				
        		break;
			//����
			case 6:
				//usart2_send_data("AT+CWQAP\r\n");
				
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x06;
			
				RC.car_status = 4;
				u2_SendArray(Uart2_Tx,5);
				
				//usart2_send_data(",s.y\r\n");
				//LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowNumberData[6], gShowNumberData[6]);
        		break;
			//����
			case 7:
				//usart2_send_data("8577778\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x07;
			
				RC.car_status = 5;
				u2_SendArray(Uart2_Tx,5);
				//usart2_send_data(",s.y\r\n");
				//LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowNumberData[6], gShowNumberData[6]);
        		break;
			//��������Ļ�л�
			case 8:
				//usart2_send_data("8577778\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x08;
				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			case 9:
				//usart2_send_data("8577778\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x08;
				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			case 10:
				//usart2_send_data("8577778\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x08;
				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			case 11:
				//usart2_send_data("8577778\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x08;
				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
			case 12:
				//usart2_send_data("8577778\r\n");
				Uart2_Tx[0] = 0xaa;
				Uart2_Tx[1] = 0x55;
				Uart2_Tx[2] = 0x03;
				Uart2_Tx[3] = RC.car_num;
				Uart2_Tx[4] = 0x08;
				u2_SendArray(Uart2_Tx,5);
				
        		break;
			
        	default:
        		break;
        }
		
		//if(RC.reference_voltage > 2.8f){
			LED_Display1 = RC.car_status;
			LED_Display2 = RC.car_num;
		//}
		
		
		
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
		if(RC.reference_voltage < 3.0f){
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
//		
		//ң��������ָ��
		remote_control_command();
		

		
		
		 
		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}

