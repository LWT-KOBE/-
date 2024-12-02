#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����


#define 	RECV_BUF_SIZE 	400

#define 	SEND_BUF_SIZE 	200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��

#define UART2_RX_LEN     1024+50
#define UART2_TX_LEN     256
#define UART1_RX_LEN     90
#define UART1_TX_LEN     256
#define USART1_MAX_RECV_LEN		100				//�����ջ����ֽ���
extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8 USART1_RX_counter;			        //���ڽ������ݼ���

extern u8 USART1_RX_FINISH_FLAG;		        //���ڽ���������ɱ�?
extern u8 USART1_RX_Len;				//���ڽ������ݳ���
extern u32 WiFidelaycounter;

extern u8 Uart2_Tx[UART2_TX_LEN];

void uart_init(u32 bound);
void usart1_send(uint8_t c);
void usart1_send_data(char *buf);
void DMA_USART1_Configuration(void);
void uart2_init(u32 bound);
void usart2_send(uint8_t c);
void usart2_send_data(char *buf);
void u2_SendArray(uint8_t *Array, uint16_t Length);
void DMA_USART2_Configuration(void);
void DMA1_Channel4_IRQHandler(void);
void Uart2_Start_DMA_Tx(u16 size);
void WIFIdataSend(u8 DataLen);

#define USART_WIFI_RX_NUM                            (uc8)100
#endif


