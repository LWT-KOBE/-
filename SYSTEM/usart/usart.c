#include "sys.h"
#include "usart.h"	
#include "string.h"
#include "stm32f10x_it.h" 
#include "app.h"
#include "exti.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
	
//����洢�����ַ���������
char g_usart1_recv_buf[RECV_BUF_SIZE] = {0};


//����洢�����ַ���������
char g_usart1_send_buf[SEND_BUF_SIZE] = {0};

//��������ַ��ļ���
int g_usart1_recv_cnt = 0;	

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART2);
}
#endif



u8 Uart1_Rx[UART1_RX_LEN]={0};
u8 Uart1_Tx[UART1_TX_LEN]={0};
u16 Uart1_Rx_length = 0;
u16 Uart1_Tx_length = 0;	 
	
	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.

u8 USART1_RX_counter = 100;			//���ڽ������ݼ���
u8 USART1_RX_FINISH_FLAG = 0;		        //���ڽ���������ɱ�־
u8 USART1_RX_Len = 0;				//���ڽ������ݳ���
	
u8 Uart2_Rx[UART2_RX_LEN]={0};
u8 Uart2_Tx[UART2_TX_LEN]={0}; 
 
u16 Uart2_Rx_length = 0;
u16 Uart2_Tx_length = 0;
u8 SendDataBuf2[10];	


void uart_init(u32 bound) {
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_TX   GPIOA.9
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1

}

void DMA_USART1_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		//USART1_DMA��������
		DMA_DeInit(DMA1_Channel5); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR); 
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart1_Rx; 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
		DMA_InitStructure.DMA_BufferSize = UART1_RX_LEN; 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 	
		DMA_Init(DMA1_Channel5,&DMA_InitStructure); 
		DMA_Cmd(DMA1_Channel5,ENABLE);
		//USART2_DMA ���� ����
		DMA_DeInit(DMA1_Channel4);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart1_Tx;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = UART1_TX_LEN;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel4,&DMA_InitStructure);
		DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel4, DISABLE);
		
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
}

void Uart1_Start_DMA_Tx(u16 size)
{
		DMA_SetCurrDataCounter(DMA1_Channel4,size);
		DMA_Cmd(DMA1_Channel4, ENABLE);
}
void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_FLAG_TC4))
    {       
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_Cmd(DMA1_Channel4, DISABLE);
        USART_ITConfig(USART1, USART_IT_TC, ENABLE);        
    }
}

void usart1_send(uint8_t c)
{	
	USART_SendData(USART1, c);//��
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}


void usart1_send_data(char *buf)
{
	while(*buf != '\0') 
	{
		usart1_send(*buf);
		buf++;
	}
}


//����1���շ��ʹ�����
void Serial1Data(uint8_t ucData){
	
	
		static unsigned char ucRxBuffer[250];
		static unsigned char ucRxCnt = 0;	
		ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
		
		//������յ��ַ����ַ�������Ķ�Ӧ��λ��
		g_usart1_recv_buf[g_usart1_recv_cnt] =ucData;
		//�����ַ��ļ���
		g_usart1_recv_cnt++;
		
		
			
	
		//�����յ����ϸ�ʽ���ַ��������߽��մﵽ�������ޣ��ͽ����ַ������ݵĴ���
		if(g_usart1_recv_cnt > RECV_BUF_SIZE-1 || g_usart1_recv_buf[g_usart1_recv_cnt-1] == '\n')
		{
			//strncmp�����������Ƚ������ַ���ǰN���ֽ��Ƿ���ͬ��strcmp�����ǱȽ������ַ����Ƿ���ͬ
			/*
				����1��2��Ҫ�Ƚϵ������ַ�����ַ
				����3��Ҫ�Ƚϵ������ַ�����ǰn���ֽ�
				����0�����ʾ�ַ���ǰN���ֽ���ͬ
			*/
			/*
						printf,scanf�����Ǳ�׼������뺯���������Զ��ӱ�׼�豸�н������ݵ���������루stdout,stdin��
						sprintf,sscanf�������ǵ���������Ѿ�ʵ���ض��������Զ������ݴ���ָ�����ڴ�ռ��н�����������루buf��
						����1��Ҫ���/����������ڴ�ռ�
						����2��Ҫ���/������ַ�����ʽ����ʽ������%d�����ͣ�%c���ַ���%s���ַ���
						����3��Ҫ��ʽ�����/����ı���
				*/
			
			
			
			
			if(strncmp(g_usart1_recv_buf, "REST=\n ", 5) == 0)//����WiFi
			{
				//sscanf(g_usart1_recv_buf, "F=%d\n", &balanceData.flag);//�ٶ��޸�
				//usart2_send_data("AT+RESTORE\r\n");
				RC.flag = 1;
			}
			
			if(strncmp(g_usart1_recv_buf, "RUN=\n ", 4) == 0)//����WiFi
			{
				//sscanf(g_usart1_recv_buf, "F=%d\n", &balanceData.flag);//�ٶ��޸�
				//usart2_send_data("AT+RESTORE\r\n");
				RC.flag = 2;
			}
			
			//����ַ�������
			memset(g_usart1_recv_buf, 0, sizeof(g_usart1_recv_buf));
			
			//����ֵ���
			g_usart1_recv_cnt = 0;
		}
		
		
	
	
}


void USART1_IRQHandler(void) 
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		//USART_SendData(USART1,Res);
//		Serial1Data(Res);
		USART_SendData(USART2,Res);
        if((USART_RX_STA&0x8000)==0)//����δ���
        {
            if(USART_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
                else USART_RX_STA|=0x8000;	//���������
            }
            else //��û�յ�0X0D
            {
                if(Res==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
}


void WIFIdataSend(u8 DataLen) 
{
	u8 i=0;
	
////////////////HEAD/////////////////	
	SendDataBuf2[0] = 0xfc;
	SendDataBuf2[1] = 1;
	SendDataBuf2[2] = DataLen;

////////////////V////////////////////	
	
	SendDataBuf2[1+2] = CageNumber>>8;
	SendDataBuf2[2+2] = CageNumber;
	
////////////////TAIL/////////////////	
	SendDataBuf2[3+DataLen] = 0x11;
	SendDataBuf2[4+DataLen] = 0xfc;		
	
	
		for(i=0;i<DataLen+5;i++)
		{
			while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
			USART_SendData(USART1,SendDataBuf2[i]);          
		}	
}


void uart2_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART1��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    USART_DeInit(USART2);  //��λ����1

    //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2

    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
    
    //���ô���
   USART_InitStructure.USART_BaudRate = bound;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
   USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
   USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

   USART_Init(USART2, &USART_InitStructure);//���ô���2
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//ʹ�ܴ��ڽ����ж�  
   //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//���ڷ����ж��ڷ�������ʱ����
   USART_Cmd(USART2, ENABLE);//ʹ�ܴ���3

   //�����ж�����
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����USART2�ж�
   //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�жϵȼ�
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);                
}









void DMA_USART2_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		//USART1_DMA��������
		DMA_DeInit(DMA1_Channel6); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR); 
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart2_Rx; 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
		DMA_InitStructure.DMA_BufferSize = UART2_RX_LEN; 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 	
		DMA_Init(DMA1_Channel6,&DMA_InitStructure); 
		DMA_Cmd(DMA1_Channel6,ENABLE);
		//USART2_DMA ���� ����
		DMA_DeInit(DMA1_Channel7);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart2_Tx;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = UART2_TX_LEN;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel7,&DMA_InitStructure);
		DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel7, DISABLE);
		
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
}

void usart2_send(uint8_t c)
{	
	USART_SendData(USART2, c);//��
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}


void usart2_send_data(char *buf)
{
	while(*buf != '\0') 
	{
		usart2_send(*buf);
		buf++;
	}
}


void u2_SendArray(uint8_t *Array, uint16_t Length)	//����һ��8λ������
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		usart2_send(Array[i]);
	}
}

void Uart2_Start_DMA_Tx(u16 size)
{
		DMA_SetCurrDataCounter(DMA1_Channel7,size);
		DMA_Cmd(DMA1_Channel7, ENABLE);
}
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_FLAG_TC7))
    {       
        DMA_ClearFlag(DMA1_FLAG_TC7);
        DMA_Cmd(DMA1_Channel7, DISABLE);
        USART_ITConfig(USART2, USART_IT_TC, ENABLE);        
    }
}



void USART2_IRQHandler(void)
{
	u8 Res;
//    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
//    { 
//			DMA_Cmd(DMA1_Channel6,DISABLE);
//			Uart1_Rx_length = USART2->SR; 
//			Uart1_Rx_length = USART2->DR; 
//			Uart1_Rx_length = UART1_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel6); 
//			if(Uart1_Rx_length<UART1_RX_LEN)
//			{

//				
//			}
//			

//			
//			DMA1_Channel6->CNDTR = UART1_RX_LEN;
//			DMA_Cmd(DMA1_Channel6, ENABLE);
//    }
//		
//    if(USART_GetITStatus(USART2, USART_IT_TC)!= RESET)//���������ʱ���봮��1�ж�
//    {    
//			NVIC_ClearPendingIRQ(USART2_IRQn);//����жϱ�־
//			USART_ClearITPendingBit(USART2,USART_IT_TC);//�������2������ɱ�־��־
//    }		
		
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //Check if data is received //�ж��Ƿ���յ�����
	{
		Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
		USART_SendData(USART1,Res);
		//USART_SendData(USART2,Res);
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
				else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}
		
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	}
}




#endif	

