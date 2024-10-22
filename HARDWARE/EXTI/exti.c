#include "exti.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"
//#include "vofa.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "stdlib.h"
#include "motor.h"
#include "app.h"
u32 PocketCountQ = 0;
u32 PocketCountH = 0;
u32 PocketCount = 0;
u16 CageNumber = 0;//ʵʱ��ں�
u32 NFCNUM = 0;
u8 MFSW_Flag = 0;//0--��  1--��⵽Ͷ����Ʒ
u8 CheckDirction = 0;//0--ǰ�� 1--����
u8 Q_GDSW_Flag = 0;
u8 H_GDSW_Flag = 0;
u8 H_GDSW_Flag_Count;
u8 test1 = 0;
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	//GPIOA5 �ж����Լ��жϳ�ʼ������   �½��ش��� //Q_GDSW  ǰ���
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5); 
//	EXTI_InitStructure.EXTI_Line=EXTI_Line5;	//
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���



//// //GPIOA4	  �ж����Լ��жϳ�ʼ������ �½��ش��� //H_GDSW  ����
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
//	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
//	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	
 //GPIOA1	  �ж����Լ��жϳ�ʼ������ �½��ش��� //MFSW  ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	
// //GPIOC15	  �ж����Լ��жϳ�ʼ������ �½��ش��� //����
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);
//	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		
//	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ��Q_GDSWǰ������ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�1�� 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); 
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ��H_GDSW�������ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�1�� 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ��MFSW���������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�1�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 	


//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�1�� 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); 

}

//�ⲿ�ж�5�������  Q_GDSWǰ���
//void EXTI9_5_IRQHandler(void)
//{
////	delay_ms(10);
//	if(Q_GDSW == 0)
//	{
//		Q_GDSW_Flag = 1;
//		My_Sprintf(0,"QIAN = %d\r\n", 1);
//		if(H_GDSW_Flag)
//		{
//			//PocketCount--;
//			H_GDSW_Flag = 0;
//			Q_GDSW_Flag =0;
//			My_Sprintf(0,"NO = %d\r\n", 1);
//		}
//	}
//	EXTI_ClearITPendingBit(EXTI_Line5); //���LINE0�ϵ��жϱ�־λ  
//}

//�ⲿ�ж�4�������  H_GDSW����
void EXTI4_IRQHandler(void)
{

	
	EXTI_ClearITPendingBit(EXTI_Line4); //���LINE0�ϵ��жϱ�־λ  
}
 
//�ⲿ�ж�1�������  MFSW������
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)			//��־λ��ֵλ�������жϣ�
	{	
		if(ScanCodeMode == 0)
		{
			delay_ms(2);
			if(MFSW == 0 && TrainState != 0) 
			{
				if(BasketErrorFlag == 0)//���ϲ������ַ
				{
					if(EnableApplicationAddr == 1)
					{
						if(DeliverTaskFlag == 0 && TrainState != 1)
						{
							test1 ++; 
							Applictiao_ADDR_Flag &= ~0x0f;//
							Applictiao_ADDR_Flag |= 0x01;//�����ַ	
						}			
					}
				}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1); //���LINE0�ϵ��жϱ�־λ 
	}		
}



//�ⲿ�ж�15�������  MFSW������
void EXTI15_10_IRQHandler(void)
{
//	if(KEY1 == 0)
//	{
//		iQ = 0;jQ = 0;iH = 0;jH = 0;iQOP = 0;jQOP = 0;iHOP = 0;jHOP = 0;//��ʱ�����0		
//		if(BasketFlagQ ==0)
//			BasketFlagQ = 1;
//		else if(BasketFlagQ ==2)
//			BasketFlagQ = 3;

//		if(BasketFlagH ==0)
//			BasketFlagH = 1;
//		else if(BasketFlagH ==2)
//			BasketFlagH = 3;
//	}
	EXTI_ClearITPendingBit(EXTI_Line15); //���LINE0�ϵ��жϱ�־λ 
}








 
