#include "stm32f10x.h"
#include "task.h"
#include "include.h" 

//��ʼ������ң�����ṹ��
remote_control RC = {0,1,0,0,0,0,0};

//ң��ָ���
void remote_control_command(void){
	
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)//
	{	
		
		//�����������˵��wifiû������
		if(PBout(14) == 0){
			RC.num ++;
		}else{
			RC.num = 0;
		}

			
		
			
		
		
		//���wifi�����Ƿ񳬹�12��
		if(RC.num >= 12000){
			//����12������ģ��RST��
			PAout(12) = 0;
			
			//���������ģ��RST���������
			if(RC.num > 14000){
				RC.num = 0;
				PAout(12) = 1;
			}
			
			
		}
		
		//��������
		RC.time ++;
		
		//��ʱ4�뷢������
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
		
		//�������
		key_flag = KEY_Scan(0);
		
		//�жϰ�����������
		switch (key_flag)
        {
			//û�а������µ�ʱ�������������
        	case 0:
			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
        		break;
			
			//��ͣ��ť����
        	case 1:
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
        }
		
		
		
	}
}

void TIM2_IRQHandler(void)
{
	{
		

		remote_control_command();
		

		
		 
		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}

