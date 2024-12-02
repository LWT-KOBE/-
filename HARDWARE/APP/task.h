#ifndef __TASK_H
#define __TASK_H

#include "include.h" 

typedef struct  
{
	u8 car_status;						//ң�ص�״̬
	u8 car_num;							//ң�صĳ���
	u8 low_battery;						//�͵���״̬
	u8 function_display;				//������ʾ״̬
	u8 digital_display;					//������ʾ
	u8 flag;							//״̬λ
	int low_battery_num;				//�͵�ѹά��ʱ��
	float control_idle_time;			//δ����ң������ʱ��
	float battery_voltage;				//��ص�ѹ
	float reference_voltage;			//�ڲ���׼��ѹ
	int num;
}remote_control;

extern remote_control RC;

#endif // __BSP_EEPROM_H
