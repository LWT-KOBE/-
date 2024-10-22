#include "app.h"
#include "can.h"
#include "key.h"
#include "iap_APP.h"
#include "motor.h"
CanRxMsg RxMessage;
CanTxMsg TxMessage;

u8 LED_Display1;
u8 LED_Display2;

u8 LastDeliverResult_Flag = 0;//0x01�ϱ���־λ
u8 Applictiao_ADDR_Flag = 0x00;//0x01�����ַ  0x02���յ�����ĵ�ַ    0x10�ϱ�Ͷ�ݽ�� 0x20--�յ��ظ�
u8 ApplicFlag = 0;
u32 DeliverResultACKFlag = 0;//Ͷ�ݽ���ϱ�Ӧ���־λ21��λ����21�������Ӧ��

u32 DeliverAddr = 0;//APP�·��ĵ�ַ
u32 LastDeliverResult = 0;//��һ��Ͷ�ݽ��
u8 DeliverTaskFlag = 0;//1--Ͷ������ʼ��־

u8 EnableApplicationAddr = 0;//0--disable 1--enable  ָ��װ��λ��

u16 HSpeedPocketNum = 0;//����ʱͶ�ݸ�ں�
u16 LSpeedPocketNum = 0;//����ʱͶ�ݸ�ں�
u16 CarSpeed = 0;//С���ٶ�

u8 DeliverMode = 0;//Ͷ��ģʽ  0---��Ƭģʽ   1---RFIDģʽ
u8 DeliverScussFlag = 15;//Ͷ�ݳɹ���־λ  1--�ɹ�   0--ʧ��
u8 DeliverActionFlag = 0;//Ͷ�ݶ�����־λ  1--ִ�ж���  0--��
u16 Pocket_A_Count,Pocket_A_CountOld,Pocket_A_CountNew = 0;
u8 TrainStateOld,TrainStateNew,TrainState = 0;
u16 SwitchCount1 = 0;
u8 SwitchFlag1 = 0;
u8 SetDateFlag = 0;//���ñ�־λ	
u32 APPSendADDRFlag = 0;//APP�·���ַ��־λ  21��λ����21�������·��ĵ�ַ
u16 OpendoorDelay = 0;
u16 OpendoorDelayCount = 0;
u8 OpendoorDelayFlag = 0;
u32 PocketCaseNum = 0;
u16 DisplayDelayCount = 0;
u8 DisplayFlag = 0;
u16 PulseMotor,PulseMotorOld = 0;
u16 PulseMotorCount = 0;
/////////////////////////////////���ò���/////////////////////////////////
u16 CaseNum = 77;//�������  ADDR 0-1
u8 Pocket4 = 12; //װ�ص�Ƭ ADDR 2
u8 TrainBasketMaxNum = 11;//��������  ADDR 3
u8 ScanCodeMode = 1;//0--�ֶ�ɨ��    1--�Զ�ɨ��   ADDR 4

u16 RecycleCaseNum = 77;//���ո��  ADDR 15-16
/////////////////////////////////���ò���/////////////////////////////////


void DeliverTask (void)//��Ƭģʽ
{
//	if((PocketCount%2)==0)//���ٵ�Ƭ
//		LSpeedPocketNum = PocketCount/2;
//	else//���ٵ�Ƭ
//		HSpeedPocketNum = PocketCount/2 + 1;
	if(DeliverTaskFlag == 0)
	{
		OpendoorDelayFlag = 0;
		OpendoorDelayCount =0;
	}
		
	if(DeliverTaskFlag)//��ʼ����
	{	
////////////////���⴦���ַ�·�����ȥ����/////////////////////			
		if(PocketCount >= (CaseNum-2)*2)//
		{
			if(PocketCount > DeliverAddr*2)
				BasketErrorFlag |= 0x40;
		}				
////////////////���⴦���ַ�·�����ȥ����/////////////////////				
	
///////////////////////�쳣ȥ���տ�///////////////////////////////			
		if(TrainState != 1 && BasketErrorFlag != 0)
		{
			DeliverAddr = CaseNum - 1;
		}
///////////////////////�쳣ȥ���տ�///////////////////////////////			
			
		if(CarSpeed >= 125)//����Ͷ��
		{
			if(DeliverAddr == HSpeedPocketNum)//
			{
				LastDeliverResult = HSpeedPocketNum;
				LastDeliverResult_Flag = 1;//�ڴ˴��ϱ�Ͷ�ݽ�����ܲ�׼����Ƭû��������Ǵ��
				BasketFlagQ = 1;//����
				BasketFlagH = 1;			
				ert = 0;
				DeliverAddr = 0;
				DeliverTaskFlag = 0;
				DeliverActionFlag = 1;
			}
		}
		else if(CarSpeed >=95 && CarSpeed <125)
		{
			if(DeliverAddr == HSpeedPocketNum)//
			{	
				OpendoorDelayFlag =1;
				OpendoorDelay = 10/CarSpeed*1000+100;
				if(OpendoorDelayCount >= OpendoorDelay)
				{
					LastDeliverResult = HSpeedPocketNum;
					LastDeliverResult_Flag = 1;//�ڴ˴��ϱ�Ͷ�ݽ�����ܲ�׼����Ƭû��������Ǵ��
					BasketFlagQ = 1;//����
					BasketFlagH = 1;			
					ert = 0;
					DeliverAddr = 0;
					DeliverTaskFlag = 0;
					DeliverActionFlag = 1;						
				}
			}				
		}	
		else if(CarSpeed >45 && CarSpeed <95)
		{
			if(DeliverAddr == LSpeedPocketNum)//
			{			
				LastDeliverResult = LSpeedPocketNum;
				LastDeliverResult_Flag =1;//�ڴ˴��ϱ�Ͷ�ݽ�����ܲ�׼����Ƭû��������Ǵ��
				BasketFlagQ = 1;//����
				BasketFlagH = 1;
				DeliverTaskFlag = 0;				
				ert = 0;
				DeliverAddr = 0;
				DeliverActionFlag = 1;	
			}

		}			
		else if(CarSpeed <= 45)//����Ͷ��
		{
			if(DeliverAddr == LSpeedPocketNum)//
			{
				if(PulseMotorCount >= 10)
				{
					LastDeliverResult = LSpeedPocketNum;
					LastDeliverResult_Flag =1;//�ڴ˴��ϱ�Ͷ�ݽ�����ܲ�׼����Ƭû��������Ǵ��
					BasketFlagQ = 1;//����
					BasketFlagH = 1;
					DeliverTaskFlag = 0;				
					ert = 0;
					DeliverAddr = 0;
					DeliverActionFlag = 1;
				}					
			}  
		}
	
		if(LastDeliverResult == CaseNum-1)
			BasketErrorFlag &= ~0x40;//�������״̬
	}	

}


void CheckCarCanCmd(void)
{
	if(CAN_ID == (0x53))
	{
//		if(RxRAM0[7] != 0)
//		{
//			TrainStateOld = TrainStateNew;
//			TrainStateNew = TrainState;
//			CarSpeed = RxRAM0[4];
//			TrainState = RxRAM0[5];		
//		}
		if(RxRAM0[7] == 0)//���ò���
		{
			CaseNum = RxRAM0[1] <<8 | RxRAM0[2];
			Pocket4 = RxRAM0[6];
			TrainBasketMaxNum = RxRAM0[5];
			ScanCodeMode = RxRAM0[0];//ɨ��ģʽ
	//		SetDateFlag = 1;//���ñ�־λ		
		}
		
		else if(RxRAM0[7] == 1)//�����������
		{
			if(RxRAM0[1] == TrainBasket)
			{
				if(RxRAM0[0] == 3)//���γ���
					BasketErrorFlag |= 0x20;
				else if(RxRAM0[0] == 4)//������γ���
					BasketErrorFlag &= ~0x20;
				else if(RxRAM0[0] == 5)//ȥ���տ�
				{
					BasketErrorFlag |= 0x40;		
					DeliverAddr = CaseNum-1;//���ո������
					iQ = 0;jQ = 0;iH = 0;jH = 0;iQOP = 0;jQOP = 0;iHOP = 0;jHOP = 0;//���ض���ʱ�����0
					DeliverTaskFlag = 1; 
				}					
			}
			
			DeliverResultACKFlag = RxRAM0[4]<<16 | RxRAM0[5]<<8 | RxRAM0[6];
			if(((DeliverResultACKFlag >> (TrainBasket-1)) & 0x01) == 0x01)
			{
				Applictiao_ADDR_Flag &= ~0xf0;//������ϱ���־λ
				Applictiao_ADDR_Flag |= 0x20;//�յ��ϱ����Ӧ���־λ		
			}						
		}
	}
	
	
	if(CAN_ID == (0x52))//��ͷID
	{
		TrainStateOld = TrainStateNew;
		TrainStateNew = TrainState;	
		TrainState = RxRAM0[1];
		APPSendADDRFlag = RxRAM0[1]<<16 | RxRAM0[2]<<8 | RxRAM0[3];
		CarSpeed = RxRAM0[6];
		Pocket_A_Count = RxRAM0[7];
		
		PulseMotorOld = PulseMotor;
		PulseMotor = RxRAM0[0];
		if(PulseMotorOld != RxRAM0[0])
			PulseMotorCount++;
		
		
		if(TrainState == 0x01)//�Լ���������Ͷ�ݵ�ַ
		{	
			DeliverTaskFlag = 0;
			DeliverAddr = 0;
			Applictiao_ADDR_Flag &= ~0x0f;//�����־λ
			Applictiao_ADDR_Flag |= 0x02;//�յ��·���ַ	
			LastDeliverResult = 0;//0814
		}	
		else if(TrainState == 0)
		{
			PocketCount = 0;//722
			CageNumber = 0;//0814
			Applictiao_ADDR_Flag &= ~0x0f;//�����־λ
			Applictiao_ADDR_Flag |= 0x02;//�յ��·���ַ
			DeliverTaskFlag = 0;
			DeliverAddr = 0;	
			HSpeedPocketNum = 0;
			LSpeedPocketNum = 0;
		}
		
		if(ScanCodeMode == 0)
		{		
			if(Pocket_A_Count >= (Pocket4 + TrainBasket-1) && Pocket_A_Count <= (Pocket4 + TrainBasket +1))//���������ַ����  NFCNUM-->RxRAM0[7]  װ�ص�λҲ�������ַ
				EnableApplicationAddr =1;
			else if(Pocket_A_Count > (Pocket4 + TrainBasket +2))
			{
				EnableApplicationAddr =0;
				Applictiao_ADDR_Flag &= ~0x0f;//��������־λ	
				Applictiao_ADDR_Flag |= 0x03;	
			}
			
			if((Applictiao_ADDR_Flag & 0x0f) == 0x01 || (Applictiao_ADDR_Flag & 0x0f) == 0x03)//��Ҫ�����ٴλص�װ���������ַ�Ƿ���һ���·��ļ�¼���ǣ���ͷ�ڷ��·�״̬ʱ�������ַ��
			{	
				if((RxRAM0[2] & 0x03) == 1)//�յ���ͷ�ĵ�ַ
				{
					if(RxRAM0[3] == TrainBasket)
					{
						LastDeliverResult = 0;//0814
						DeliverAddr =  RxRAM0[4]<<8 | RxRAM0[5];//��ȡͶ�ݵ�ַ��
						Applictiao_ADDR_Flag &= ~0x0f;//�����־λ
						Applictiao_ADDR_Flag |= 0x02;//�յ��·���ַ
						MFSW_Flag = 0;//ȡ�������ַ״̬
						DeliverTaskFlag = 1;	//Ͷ�������γɱ�־
						iQ = 0;jQ = 0;iH = 0;jH = 0;iQOP = 0;jQOP = 0;iHOP = 0;jHOP = 0;//���ض���ʱ�����0
					}
				}
				
			}
				
//			if((RxRAM0[0] & 0x0c) == 0x04)//�ϱ�����յ�Ӧ���ź�
//			{
//				Applictiao_ADDR_Flag &= ~0xf0;//������ϱ���־λ
//				Applictiao_ADDR_Flag |= 0x20;//�յ��ϱ����Ӧ���־λ						
//			}			
		}
		
		else//�Զ�ɨ��
		{
			if(Pocket_A_Count >= (4 + TrainBasket+1) && Pocket_A_Count <= (Pocket4 + TrainBasket + TrainBasketMaxNum+3))//�Զ�ɨ�����Ͷ�ݵ�ַ����
		//	if(TrainState >1 && TrainState <6)
			{
				if((Applictiao_ADDR_Flag & 0x0f) == 0x03 || (Applictiao_ADDR_Flag & 0x0f) == 0x00)
				{	
					if((RxRAM0[2] & 0x03) == 1)//�յ���ͷ�ĵ�ַ
					{
						if(RxRAM0[3] == TrainBasket)
						{
							LastDeliverResult = 0;//0814
							Applictiao_ADDR_Flag &= ~0x0f;//��������־λ
							Applictiao_ADDR_Flag |= 0x02;//�յ��·���ַ						
							DeliverAddr =  RxRAM0[4]*256+RxRAM0[5];//��ȡͶ�ݵ�ַ��
							DeliverTaskFlag = 1;	//Ͷ�������γɱ�־
							iQ = 0;jQ = 0;iH = 0;jH = 0;iQOP = 0;jQOP = 0;iHOP = 0;jHOP = 0;//���ض���ʱ�����0	
						}
					}
				}
			}
			
			
/////////////////////�Զ�ɨ��//////////////////////////	
			if(TrainState == 6)
			{
				Applictiao_ADDR_Flag &= ~0x0f;//��������־λ	
				Applictiao_ADDR_Flag |= 0x03;
			}
//			else if(TrainState == 5)
//			{
//				Applictiao_ADDR_Flag &= ~0xf0;//������ϱ���־λ
//				Applictiao_ADDR_Flag |= 0x20;//�յ��ϱ����Ӧ���־λ	
//				PocketCount = 0;
//				LSpeedPocketNum = 0;
//				HSpeedPocketNum = 0;	
//				iQ = 0;jQ = 0;iH = 0;jH = 0;iQOP = 0;jQOP = 0;iHOP = 0;jHOP = 0;//���ض���ʱ�����0				
//			}
			
/////////////////////�Զ�ɨ��//////////////////////////					
		}

		
		if((RxRAM0[0] & 0x0c) == 0x04)//�ϱ�����յ�Ӧ���ź�
		{
			Applictiao_ADDR_Flag &= ~0xf0;//������ϱ���־λ
			Applictiao_ADDR_Flag |= 0x20;//�յ��ϱ����Ӧ���־λ						
		}
			
		if(Pocket_A_Count == (1 + TrainBasket)) // �ϱ�Ͷ�ݽ�� ��׼ȷ���
		{
			Applictiao_ADDR_Flag &= ~0xf0;//������յ��ϱ������־λ
			Applictiao_ADDR_Flag |= 0x10;//�ϱ������־λ  
		//	DeliverActionFlag = 0;0809
		}	
	
		else if(Pocket_A_Count == (2 + TrainBasket)) // �ϱ�Ͷ�ݽ�� ��׼ȷ���
		{
			Applictiao_ADDR_Flag &= ~0xf0;//������յ��ϱ������־λ
			Applictiao_ADDR_Flag |= 0x10;//�ϱ������־λ  
		//	DeliverActionFlag = 0;0809
		}			
		
		else if(Pocket_A_Count == (Pocket4 + TrainBasket-1))//��Ƭ������0 
		{	
//			Applictiao_ADDR_Flag &= ~0xf0;//������ϱ���־λ  //0809ע�� ��Ϊ�ϱ���ʱ���ɹ�
//			Applictiao_ADDR_Flag |= 0x20;//�յ��ϱ����Ӧ���־λ	
			PocketCount = 0;
			CageNumber = 0;//0814
			LSpeedPocketNum = 0;
			HSpeedPocketNum = 0;	
			iQ = 0;jQ = 0;iH = 0;jH = 0;iQOP = 0;jQOP = 0;iHOP = 0;jHOP = 0;//���ض���ʱ�����0				
		}		
		
		if(TrainState == 6)
		{		
			Applictiao_ADDR_Flag &= ~0xf0;//������ϱ���־λ
			Applictiao_ADDR_Flag |= 0x20;//�յ��ϱ����Ӧ���־λ			
		}		
//		if(TrainState == 3)
//		{
//			if(Pocket_A_Count == (2 + TrainBasket))//�жϵ�Ƭ�Ƿ�©��
//			{
//				PocketCaseNum = CaseNum*2;
//				if(PocketCount == PocketCaseNum)
//				{
//					BasketErrorFlag &= ~0x10;		
//				}
//				else//��Ƭ����
//				{
//					BasketErrorFlag |= 0x10;
//				}		
//			}	
//		}
		

		
	
	}
	
	
	
	

//		{
//			if(RxRAM0[7] == (StionStop + TrainBasket) || RxRAM0[7] == (2 + TrainBasket))//�жϵ�Ƭ�Ƿ�©��
//			{
//				if(PocketCount == CaseNum*2)
//				{
//					BasketErrorFlag &= ~0x10;
//				}
//				else//��Ƭ����
//				{
//					BasketErrorFlag |= 0x10;
//				}		
//			}
//		}
//		
//		if(RxRAM0[7] >= (3 + TrainBasket + 1) && RxRAM0[7] <= (Pocket4 + TrainBasket))//��Ƭ������0
//		{
//			PocketCount = 0;
//		}		
	
	
	

	
	
	else if(CAN_ID == 0x62)//����������ʽ
	{
		if(RxRAM0[0] == 0xfa)
		{
			if(RxRAM0[1] ==1)
				UpdateModoe = 1;
			else if(RxRAM0[1] ==0)
				UpdateModoe = 0;
		}
		else if(RxRAM0[0] == 0xfb)
		{
			if(RxRAM0[1] ==1)
				UpdataFlag = 1; 
			else if(RxRAM0[1] ==0)
				UpdataFlag = 0;
			
//			if(RxRAM0[2] ==1)
//				systeam_ReStart();  
		}
		
	}
//	else if(CAN_ID == 0x53)//
//	{
//		if(RxRAM0[0] == 1)
//		{
//			LastDeliverResult_Flag = 0;
//		}
//		
//		
//	}
	
}




void Display (void)
{
	
//	if(CanLiveFlag ==0)//CAN������
	{
//		buf = TrainBasketID/10%10;
//		buf1 = TrainBasketID%10;	
//		buf2 = TrainBasketID/100;
		LED_Tube_Choose_DisPlay1(gShowNumberData[1], gShowAlphabetData[LED_Display1], gShowNumberData[LED_Display2]);			
	}

}



void ReadE2promData(void)//��ȡ���ò���
{
	CaseNum = EEPROM_Read_u16(0);
	Pocket4 = EEPROM_Read_Byte(2);
	TrainBasketMaxNum = EEPROM_Read_Byte(3);
	ScanCodeMode = EEPROM_Read_Byte(4);
	
	if(CaseNum < 10 || CaseNum == 0xff || Pocket4 >120 || Pocket4 == 0 || TrainBasketMaxNum == 0  || TrainBasketMaxNum == 0xff || ScanCodeMode >1)
	{
		BasketErrorFlag |= 0x10;
	}
//	RecycleCaseNum = EEPROM_Read_u16(15);

}





