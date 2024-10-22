#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H

#include "include.h" 

#define EEPROM_SCL_PIN 												GPIO_Pin_13
#define EEPROM_SDA_PIN												GPIO_Pin_14
#define EEPROM_IO															GPIOC
#define RCC_APBxPeriph_EEPROM_IO							RCC_APB2Periph_GPIOC

//typedef enum {FALSE = 0, TRUE}bool;
#define EEPROM_SLAVE_ADDRESS7    0xA0        //������ַ д
#define EEPROM_READ_ADDRESS7     0xA1        //������ַ ��

#define SET_EEPROM_SCL         	 GPIO_SetBits(EEPROM_IO, EEPROM_SCL_PIN)
#define RESET_EEPROM_SCL         GPIO_ResetBits(EEPROM_IO, EEPROM_SCL_PIN)
#define SET_EEPROM_SDA        	 GPIO_SetBits(EEPROM_IO, EEPROM_SDA_PIN)
#define RESET_EEPROM_SDA         GPIO_ResetBits(EEPROM_IO, EEPROM_SDA_PIN)

#define SCL_read      EEPROM_IO->IDR  & EEPROM_SCL_PIN
#define SDA_read      EEPROM_IO->IDR  & EEPROM_SDA_PIN

typedef enum {FALSE = 0, TRUE = !FALSE}bool; 

float EepromReadFloat(u8 WriteAddr);
void EEPROM_delay(void);

bool EEPROM_Start(void);

void EEPROM_Stop(void);

void EEPROM_Ack(void);

void EEPROM_NoAck(void);

bool EEPROM_WaitAck(void);  //����Ϊ:=1��ACK,=0��ACK

void EEPROM_SendByte(u8 SendByte);//���ݴӸ�λ����λ//
void EEPROM_GPIO_Init(void);
u8 EEPROM_ReceiveByte(void); //���ݴӸ�λ����λ//

bool EEPROM_Write_Byte(u8 WriteAddr, u8 WriteData);
bool EEPROM_Write_u16(u8 WriteAddr, u16 WriteData);

 
bool EEPROM_Write2(u8 WriteAddr, u8 WriteData);
void EepromWriteFloat(uint8_t addr_, float dat_);

//����1������
u8 EEPROM_Read_Byte(u8 WriteAddr);
u16 EEPROM_Read_u16(u8 WriteAddr);

//����1������
u8 EEPROM_Read2(u8 WriteAddr);

#endif // __BSP_EEPROM_H
