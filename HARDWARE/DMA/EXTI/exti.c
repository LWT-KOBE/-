#include "exti.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"
#include "vofa.h"
#include "encoder.h"
#include "State.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	//GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);

	EXTI_InitStructure.EXTI_Line=EXTI_Line13;	//KEY2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���



 //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���




	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 



 
}

//�ⲿ�ж�0������� 
void EXTI15_10_IRQHandler(void)
{
	u8 i = 0;
	u32 Databuffe[6];
//		zeroFlag = 0;
//		zeroCount++;
//		if(zeroCount == 1) 
//		Wheel_Data.Distance_travelled = 0;
	//	MOTOR_ENA_B = 0;
		
		Wheel_Data.encoder_CNT= Read_Encoder(4);
		if(Wheel_Data.encoder_CNT > 0)
			Wheel_Data.Sum_CNT = Wheel_Data.encoder_CNT +	Read_EncoderFlag*65536;
		else
			Wheel_Data.Sum_CNT = Wheel_Data.encoder_CNT -	Read_EncoderFlag*65536;		
			
		Wheel_Data.Distance_travelled = (abs(Wheel_Data.Sum_CNT));// 	���	
		
		OldTrainMileage = TrainMileage;
		TrainMileage = Wheel_Data.Distance_travelled;
		Databuffe[0] = 0xfa;
		Databuffe[1] = (TrainMileage-OldTrainMileage)>>24;
		Databuffe[2] = (TrainMileage-OldTrainMileage)>>16;
		Databuffe[3] = (TrainMileage-OldTrainMileage)>>8;
		Databuffe[4] = (TrainMileage-OldTrainMileage);
		Databuffe[5] = 0xfb;
	
		for(i=0;i<6;i++)
		{		
			while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
			USART_SendData(USART1,Databuffe[i]); 		
		}

//	printf("TrainMileage:%d\r\n",TrainMileage-OldTrainMileage);
	EXTI_ClearITPendingBit(EXTI_Line13); //���LINE0�ϵ��жϱ�־λ  
	EXTI_ClearITPendingBit(EXTI_Line14); //���LINE0�ϵ��жϱ�־λ
}
 

 
