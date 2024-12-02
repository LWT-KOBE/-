#include "timer.h"
#include "delay.h"
#include "motor.h"
#include "app.h"
#include "exti.h"
static u32 garry_ch0[10] = {0};
static u32 garry_ch1[10] = {0};
static u16 gGetAdcCounter = 0;
char flag = 0;

u8 car_num = 1;
u8 car;
u16 SendCanDelay = 0;
u16 Timer10msDelay = 0;

void time2_init(u16 arr,u16 psc)//
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrct;
	NVIC_InitTypeDef NVIC_InitStrct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStrct.TIM_Period=arr;
	TIM_TimeBaseInitStrct.TIM_Prescaler=psc;
	TIM_TimeBaseInitStrct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStrct.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStrct);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);//ʧ��
	
	NVIC_InitStrct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStrct.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStrct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStrct);	
}

u16 g_sum_timer2;
float time;




//void TIM2_IRQHandler(void)
//{

//	char *atCommand = "AT+CWJAP=\"RedmiLWT\",\"12345678\",\"s.y\"\r\n";
//	char *atCommand1 = "AT+CWJAP=\"XHCTEST\",\"test66668888\",\"s.y\"\r\n";
//	char *atCommand2 = "AT+CWJAP=\"dd\",\"dd66668888\",\"s.y\"\r\n";
//	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)//
//	{	
//		//Uart2_Start_DMA_Tx(5);
//		key_flag = KEY_Scan(0);
//		//KeyScan();
//		//usart2_send_data("okkkk");
//		switch (key_flag)
//        {
//			//û�а������µ�ʱ�������������
//        	case 0:
//			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
//        		break;
//			
//			//��������
//        	case 1:
//				car_num++;
//				if(car_num > 8)
//					car_num = 8;
//        		break;
//			
//			//���ż���
//			case 2:
//				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.88.77\",1000,1000\r\n");
//				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.0.255\",1000,1002\r\n");
//				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.0.105\",1000,1002\r\n");
//				car_num--;
//				if(car_num < 1)
//					car_num = 1;
//				
////				LED_Display1 = 1;
////				LED_Display2 = 10;
//				//usart2_send_data("AT+CIPSTART=\"UDP\",\"192.168.0.106\",1000,1002\r\n");
//        		break;
//				
//			//��ͣ
//			case 3:
//				//usart2_send_data("AT+CIPSTATUS\r\n");
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = car_num;
//				Uart2_Tx[4] = 0x01;
//				
//				car = 2;
//				u2_SendArray(Uart2_Tx,5);
//				
//				
////				LED_Display1 = 8;
////				LED_Display2 = 9;
//				//printf("AT+CIPSEND\r\n");
//        		break;
//			
//			//�Լ�
//			case 4:
//				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = car_num;
//				Uart2_Tx[4] = 0x02;
//			
//				car = 1;
//				u2_SendArray(Uart2_Tx,5);
//			
//				
////				LED_Display1 = 0;
////				LED_Display2 = 0;
//        		break;
//			//����
//			case 5:
//				//usart2_send_data("wsd58\r\n");
//				//usart2_send_data("AT+SAVETRANSLINK=1,\"192.168.0.105\",1000,\"UDP\",1002\r\n");
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = car_num;
//				Uart2_Tx[4] = 0x03;
//			
//				car = 3;
//				u2_SendArray(Uart2_Tx,5);
//				
//        		break;
//			//����
//			case 6:
//				//usart2_send_data("AT+CWQAP\r\n");
//				
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = car_num;
//				Uart2_Tx[4] = 0x04;
//			
//				car = 4;
//				u2_SendArray(Uart2_Tx,5);
//				
//				//usart2_send_data(",s.y\r\n");
//				//LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowNumberData[6], gShowNumberData[6]);
//        		break;
//			//����
//			case 7:
//				//usart2_send_data("8577778\r\n");
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = car_num;
//				Uart2_Tx[4] = 0x05;
//			
//				car = 5;
//				u2_SendArray(Uart2_Tx,5);
//				//usart2_send_data(",s.y\r\n");
//				//LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowNumberData[6], gShowNumberData[6]);
//        		break;
//			//��������Ļ�л�
//			case 8:
//				//usart2_send_data("8577778\r\n");
//				Uart2_Tx[0] = 0xaa;
//				Uart2_Tx[1] = 0x55;
//				Uart2_Tx[2] = 0x03;
//				Uart2_Tx[3] = car_num;
//				Uart2_Tx[4] = 0x06;
//				u2_SendArray(Uart2_Tx,5);
//				
//        		break;
//        	default:
//        		break;
//        }
//		
//		LED_Display1 = car;
//		LED_Display2 = car_num;
//		
//		
//		//��¼�����Ƿ��£��Լ�δ����ʱ��
//		if(key_flag == 0){
//			time++;
//		}else if(key_flag!= 0){
//			time = 0;
//		}
//			
////		switch (key_flag)
////        {
////			
////        	case 0:
////			memset(Uart2_Tx, 0, sizeof(Uart2_Tx));
////        		break;
////        	case 1:
////				//����
////				Uart2_Tx[0] = 0xaa;
////				Uart2_Tx[1] = 0x55;
////				Uart2_Tx[2] = 0x03;
////				Uart2_Tx[3] = 0x01;
////				Uart2_Tx[4] = 0x01;
////				
////        		break;
////			case 2:
////				//����
////				Uart2_Tx[0] = 0xaa;
////				Uart2_Tx[1] = 0x55;
////				Uart2_Tx[2] = 0x03;
////				Uart2_Tx[3] = 0x01;
////				Uart2_Tx[4] = 0x02;
////        		break;
////			case 3:
////				//����
////				Uart2_Tx[0] = 0xaa;
////				Uart2_Tx[1] = 0x55;
////				Uart2_Tx[2] = 0x03;
////				Uart2_Tx[3] = 0x01;
////				Uart2_Tx[4] = 0x03;
////        		break;
////			case 4:
////				//��ͣ
////				Uart2_Tx[0] = 0xaa;
////				Uart2_Tx[1] = 0x55;
////				Uart2_Tx[2] = 0x03;
////				Uart2_Tx[3] = 0x01;
////				Uart2_Tx[4] = 0x04;
////        		break;
////			case 5:
////				//�Լ�
////				Uart2_Tx[0] = 0xaa;
////				Uart2_Tx[1] = 0x55;
////				Uart2_Tx[2] = 0x03;
////				Uart2_Tx[3] = 0x01;
////				Uart2_Tx[4] = 0x05;
////        		break;
////			case 6:
////				//��Ļҳ���л�
////				usart2_send_data("AT+CWQAP\r\n");
////				Uart2_Tx[0] = 0xaa;
////				Uart2_Tx[1] = 0x55;
////				Uart2_Tx[2] = 0x03;
////				Uart2_Tx[3] = 0x01;
////				Uart2_Tx[4] = 0x06;
////        		break;
////			case 7:
////				//��Ļҳ���л�
////				usart2_send_data("AT+CWQAP\r\n");
////        		break;
////        	default:
////        		break;
////        }
//		
//		
//		 
//		TIM_ClearFlag(TIM2,TIM_IT_Update);
//	}
//}



//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OC1InitStructure;
	TIM_OCInitTypeDef  TIM_OC2InitStructure;
	TIM_OCInitTypeDef  TIM_OC3InitStructure;
	TIM_OCInitTypeDef  TIM_OC4InitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 

	
   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO	
	
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOA.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO		
	
   //���ø�����Ϊ�����������,���TIM3 CH3��PWM���岨��	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO	
	
   //���ø�����Ϊ�����������,���TIM3 CH4��PWM���岨��	GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO			
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OC1InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OC1InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC1InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OC1InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OC2InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OC2InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC2InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�	
	TIM_OC2Init(TIM3, &TIM_OC2InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���	
	
	//��ʼ��TIM3 Channel3 PWMģʽ	 
	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�	
	TIM_OC3Init(TIM3, &TIM_OC3InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

	//��ʼ��TIM3 Channel4 PWMģʽ	 
	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OC4InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC4
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���		
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM4
	

}


void OC1_Cmd(FunctionalState NewState)
{
	TIM_OCInitTypeDef  TIM_OC1InitStructure;
	
	
	TIM_OC1InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  if (NewState != DISABLE)
  {
    /* Enable */
    TIM_OC1InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  }	
	else
	{
		TIM_OC1InitStructure.TIM_OutputState = TIM_OutputState_Disable;	
	}
 	
	TIM_OC1InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OC1InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1	
	
}

void OC2_Cmd(FunctionalState NewState)
{
	TIM_OCInitTypeDef  TIM_OC2InitStructure;
	
	
	TIM_OC2InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  if (NewState != DISABLE)
  {
    /* Enable */
    TIM_OC2InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  }	
	else
	{
		TIM_OC2InitStructure.TIM_OutputState = TIM_OutputState_Disable;	
	}
 	
	TIM_OC2InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OC2InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1	
	
}

void OC3_Cmd(FunctionalState NewState)
{
	TIM_OCInitTypeDef  TIM_OC3InitStructure;
	
	
	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  if (NewState != DISABLE)
  {
    /* Enable */
    TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  }	
	else
	{
		TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Disable;	
	}
 	
	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM3, &TIM_OC3InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1	
	
}

void OC4_Cmd(FunctionalState NewState)
{
	TIM_OCInitTypeDef  TIM_OC4InitStructure;
	
	
	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  if (NewState != DISABLE)
  {
    /* Enable */
    TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  }	
	else
	{
		TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Disable;	
	}
 	
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OC4InitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1	
	
}



