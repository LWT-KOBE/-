#include "include.h"   
#include "iap_APP.h"
#include "motor.h"
#include "SEGGER_RTT.h"
#include "app.h"
#include "stm32f10x_pwr.h"
u8 key_flag = 0;
u8 ert = 0;
u16 qwe = 0; 
u8 delaynum1;
u8 delaynum2;
u8 InStionFlag = 0;


                      
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
	delay_init();	    			//延时函数初始化
	KEY_Init();						// IO口配置
	uart_init(115200);				//初始化串口1
	uart2_init(115200);				//初始化串口2
	time2_init(99, 719); //1MS  
	DMA_USART2_Configuration();
	TM1620_Config();
	TM1620_init();
	//AD_Init();
	Adc_Init();
	//DMA_USART1_Configuration();

	/* 使能电源管理单元的时钟,必须要使能时钟才能进入待机模式 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	while(1)  
	{		
		
		//数码管显示
		Display();
//	
//	/* 如果120秒后没有按键按下则进入待机模式 */
	if(RC.control_idle_time >= 90000){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
		/*清除WU状态位*/
		PWR_ClearFlag (PWR_FLAG_WU);
		
		}		
	}
}


