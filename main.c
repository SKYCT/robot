#include "bsrSD.h"
#include "robot.h"

extern void RobotSystemInit(void);
extern unsigned int IsFirstDownLoad(void);
extern void FormatFlash(void);
extern void TrainFiveCommand(void);
extern void SaveFiveCommand(unsigned int uiAddr);
extern void ImportFiveCommand(unsigned int uiAddr_Import);
extern unsigned int TrainWord(unsigned int WordID,unsigned int SndIndex);
extern void PlaySnd(unsigned int SndIndex);
extern void Robot_DanceAgain(unsigned int n);
extern void Robot_Dance(unsigned int n);
extern void Robot_Go(unsigned int n);
extern void Robot_Backup(unsigned int n);
extern void Robot_TurnLeft(unsigned int n);
extern void Robot_TurnRight(unsigned int n);
extern void Robot_HeadTurnLeft(unsigned int n);
extern void Robot_HeadTurnRight(unsigned int n);
extern void Robot_Shoot_Prepare(unsigned int n);
extern void Robot_Shoot2(unsigned int n);
extern void Robot_Shoot_Five(unsigned int n);
//extern void Robot_Shoot_wait(unsigned int n);

extern void F_FlashWrite1Word(unsigned int,unsigned int);
extern void F_FlashErase(int);

extern void Delay(unsigned int);

int main(void)
{
	unsigned int uiFlagFirst;				//�Ƿ�Ϊ��һ������
	unsigned int uiRes;						//ʶ����
	unsigned int uiActivated;				//�Ƿ���ڴ���״̬
	unsigned int uiTimerCount;				//ʱ���Ƿ�ʱ
	unsigned int uiBS_Team;					//���ڱ�ʶ�����ǵڼ����������ڴ浱��
	unsigned int uiKey;						//�洢����ֵ�����½�����ѵ��
	
	RobotSystemInit();
	uiActivated = 0;

	uiFlagFirst = IsFirstDownLoad();		//�ж��Ƿ�Ϊ��һ�����س���
	
	if(uiFlagFirst == 1)
	{
		FormatFlash();						//��ʽ����Ҫ�洢����Ĵ洢��
		
		TrainFiveCommand();					//ѵ����Ҫ���������һ�飩
		SaveFiveCommand(0xf700);			//�洢��������
		PlaySnd(OK);						//����Ok,Let's go,��ʾһ������洢����
		PlaySnd(LETUSGO);	
		
		TrainFiveCommand();
		SaveFiveCommand(0xf900);
		PlaySnd(FOLLOWME);
			
		TrainFiveCommand();
		SaveFiveCommand(0xfb00);
		PlaySnd(HO);
		PlaySnd(HOO);
		PlaySnd(HOO);
		PlaySnd(HOOO);
		PlaySnd(HO);
		PlaySnd(HOO);
		PlaySnd(HOO);
		PlaySnd(HOOO);
		
		uiFlagFirst = 0xaaaa;
		F_FlashWrite1Word(0xfd00,0xaaaa);
	}
	
	ImportFiveCommand(0xf700);
	uiBS_Team = 0;
	
Loop:	
	BSR_InitRecognizer(BSR_MIC);			//��ʼ��ʶ����
	while(1)
	{	
		*(unsigned int *)0x7012 = 1;		//�忴�Ź�
		//��ʼʶ������
		uiRes = BSR_GetResult();			//ȡ��ʶ����
		if(uiRes > 0)
		{	
			if(uiActivated)
			{
				uiTimerCount = 0;
				switch(uiRes)
				{
					case NAME_ID:
					switch(uiBS_Team)
					{
						case 0:				//��һ��ĵ�һ������ܿˣ�
						//����Ok,Les's go,Yeah
						PlaySnd(OK);
						break;
						case 1:				//�ڶ���ĵ�һ�������ʼ��
						PlaySnd(OK);
						PlaySnd(LETUSGO);
						PlaySnd(YEAH);
						//����Ok,Les's go
						break;
						case 2:				//������ĵ�һ�����׼����
						PlaySnd(OK);
						//����Ok
						break;
					}	
					break;
					
					
					case Command_One_ID:
					switch(uiBS_Team)
					{
						case 0:				//��һ��ĵڶ��������ʼ��
						
						//����ڶ�������޸�uiBS_TeamΪ1������Wo,Ho,Hoo,Hooo,����ѭ��
						ImportFiveCommand(0xf900);
						uiBS_Team = 1;
						PlaySnd(HO);
						PlaySnd(HOO);
						PlaySnd(HOO);
						PlaySnd(HOOO);
						goto Loop;
						break;
						case 1:				//�ڶ���ĵڶ��������ǰ�ߣ�
						
						//���ſ������������Ƶ����ǰ��
						PlaySnd(OK);						//����Ok,Let's go
		                PlaySnd(LETUSGO);                   //
						
						Robot_Go(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
						case 2:				//������ĵڶ������������׼��
						
						//������ת������ͷ���������ת
						Robot_HeadTurnLeft(1);
						PlaySnd(TURNHEAD);
						PlaySnd(TURNHEAD);
						PlaySnd(TURNHEAD);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
					}	
					*(unsigned int *)0x7012 = 1;
					uiActivated = 0;		//�ܿ�Ϊ�������ÿ���һ��������Ҫ����һ��
					break;
					
					
					case Command_Two_ID:
					switch(uiBS_Team)
					{
						case 0:				//��һ��ĵ��������׼����
						
						//�������������޸�uiBS_TeamΪ2������Wo,Ho,Hoo,Hooo,����ѭ��
						ImportFiveCommand(0xfb00);
						uiBS_Team = 2;
						PlaySnd(HO);
						PlaySnd(HOO);
						PlaySnd(HOO);
						PlaySnd(HOOO);
						goto Loop;
						break;
						case 1:				//�ڶ���ĵ�����������ˣ�
						PlaySnd(FOLLOWME);  //
						//���ſ������������Ƶ�������
						Robot_Backup(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						break;
						case 2:				//������ĵ��������������׼��
						
						//������ת������ͷ���������ת
						Robot_HeadTurnRight(1);
						PlaySnd(TURNHEAD);
						PlaySnd(TURNHEAD);
						PlaySnd(TURNHEAD);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
					}
					*(unsigned int *)0x7012 = 1;
					uiActivated = 0;
					break;
					
					
					case Command_Three_ID:
					switch(uiBS_Team)
					{
						case 0:				//��һ��ĵ��ĸ�������裩
						PlaySnd(OK);
						PlaySnd(FOLLOWME);
						//���裬�忴�Ź���ʱ��Ƚϳ�
						Robot_Dance(2);
						uiActivated = 0;
						*(unsigned int *)0x7012 = 0x0001;
						break;
						case 1:				//�ڶ���ĵ��ĸ������ת��
						
						//������ת������������ת
						Robot_TurnLeft(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
						case 2:				//������ĵ��ĸ�������䣩
						
						//���������������������
						Robot_Shoot_Prepare(4);
						Delay(1500);
						Robot_Shoot2(2);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
					}
					*(unsigned int *)0x7012 = 1;
					uiActivated = 0;
					break;
					
					
					case Command_Four_ID:
					switch(uiBS_Team)
					{
						case 0:				//��һ��ĵ�����������һ����
						
						//����һ��
						Robot_DanceAgain(2);
						uiActivated = 0;
						break;
						case 1:				//�ڶ���ĵ���������ת��
						
						//����������������ת��������һ���������uiBS_TeamΪ0���Ƴ�ѭ��
						Robot_TurnRight(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
						case 2:				//������ĵ��������������䣩
						
						//����������̣�������һ���������uiBS_TeamΪ0���Ƴ�ѭ��
						PlaySnd(LETUSGO);
						PlaySnd(FOLLOWME);
						Robot_Shoot_Five(2);
						 Delay(15);
						Robot_Shoot_Prepare(4);////
						Delay(1500);
						Robot_Shoot_Five(2);
						
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						
						uiActivated = 0;
						goto Loop;
						break;
					}
					*(unsigned int *)0x7012 = 1;
					uiActivated = 0;
					break;
				}
			}
			else
			{
				if(uiRes == NAME_ID)
				{
					*(unsigned int *)0x7012 = 1;
					uiActivated = 1;
					uiTimerCount = 0;
					PlaySnd(OK);					//����Ӧ��OK
				}
			}				
		}
		else if(uiActivated)
		{
			if(++uiTimerCount > 700)
			{
				*(unsigned int *)0x7012 = 1;
				uiActivated = 0;
				uiTimerCount = 0;
				PlaySnd(HOOO);	
			}
		}
		uiKey = *P_IOA_Data;
		uiKey = uiKey&0x0004;
		if(uiKey == 0x0004)
		{
			F_FlashErase(0xfd00);
		}	//������־λ
	}	
}