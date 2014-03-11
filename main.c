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
	unsigned int uiFlagFirst;				//是否为第一次下载
	unsigned int uiRes;						//识别结果
	unsigned int uiActivated;				//是否出于待命状态
	unsigned int uiTimerCount;				//时间是否超时
	unsigned int uiBS_Team;					//用于标识现在是第几组命令在内存当中
	unsigned int uiKey;						//存储键盘值，按下将重新训练
	
	RobotSystemInit();
	uiActivated = 0;

	uiFlagFirst = IsFirstDownLoad();		//判断是否为第一次下载程序
	
	if(uiFlagFirst == 1)
	{
		FormatFlash();						//格式化需要存储命令的存储器
		
		TrainFiveCommand();					//训练需要的五条命令（一组）
		SaveFiveCommand(0xf700);			//存储五条命令
		PlaySnd(OK);						//播放Ok,Let's go,表示一组命令存储结束
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
	BSR_InitRecognizer(BSR_MIC);			//初始化识别器
	while(1)
	{	
		*(unsigned int *)0x7012 = 1;		//清看门狗
		//开始识别命令
		uiRes = BSR_GetResult();			//取得识别结果
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
						case 0:				//第一组的第一个命令（杰克）
						//播放Ok,Les's go,Yeah
						PlaySnd(OK);
						break;
						case 1:				//第二组的第一个命令（开始）
						PlaySnd(OK);
						PlaySnd(LETUSGO);
						PlaySnd(YEAH);
						//播放Ok,Les's go
						break;
						case 2:				//第三组的第一个命令（准备）
						PlaySnd(OK);
						//播放Ok
						break;
					}	
					break;
					
					
					case Command_One_ID:
					switch(uiBS_Team)
					{
						case 0:				//第一组的第二个命令（开始）
						
						//导入第二组命令，修改uiBS_Team为1，播放Wo,Ho,Hoo,Hooo,跳出循环
						ImportFiveCommand(0xf900);
						uiBS_Team = 1;
						PlaySnd(HO);
						PlaySnd(HOO);
						PlaySnd(HOO);
						PlaySnd(HOOO);
						goto Loop;
						break;
						case 1:				//第二组的第二个命令（向前走）
						
						//播放喀咔声音，控制电机向前走
						PlaySnd(OK);						//播放Ok,Let's go
		                PlaySnd(LETUSGO);                   //
						
						Robot_Go(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
						case 2:				//第三组的第二个命令（向左瞄准）
						
						//播放旋转声音，头部电机向左转
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
					uiActivated = 0;		//杰克为触发命令，每完成一个动作需要出发一次
					break;
					
					
					case Command_Two_ID:
					switch(uiBS_Team)
					{
						case 0:				//第一组的第三个命令（准备）
						
						//导入第三组命令，修改uiBS_Team为2，播放Wo,Ho,Hoo,Hooo,跳出循环
						ImportFiveCommand(0xfb00);
						uiBS_Team = 2;
						PlaySnd(HO);
						PlaySnd(HOO);
						PlaySnd(HOO);
						PlaySnd(HOOO);
						goto Loop;
						break;
						case 1:				//第二组的第三个命令（倒退）
						PlaySnd(FOLLOWME);  //
						//播放喀咔声音，控制电机向后走
						Robot_Backup(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						break;
						case 2:				//第三组的第三个命令（向右瞄准）
						
						//播放旋转声音，头部电机向右转
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
						case 0:				//第一组的第四个命令（跳舞）
						PlaySnd(OK);
						PlaySnd(FOLLOWME);
						//跳舞，清看门狗，时间比较长
						Robot_Dance(2);
						uiActivated = 0;
						*(unsigned int *)0x7012 = 0x0001;
						break;
						case 1:				//第二组的第四个命令（左转）
						
						//播放旋转声音，向左旋转
						Robot_TurnLeft(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
						case 2:				//第三组的第四个命令（发射）
						
						//播放咚咚声音，发射飞盘
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
						case 0:				//第一组的第五个命令（再来一曲）
						
						//在来一曲
						Robot_DanceAgain(2);
						uiActivated = 0;
						break;
						case 1:				//第二组的第五个命令（右转）
						
						//播放声音，向右旋转，导到第一组命令，设置uiBS_Team为0，推出循环
						Robot_TurnRight(1);
						PlaySnd(WALK);
						ImportFiveCommand(0xf700);
						uiBS_Team = 0;
						uiActivated = 0;
						goto Loop;
						break;
						case 2:				//第三组的第五个命令（连续发射）
						
						//连续发射飞盘，导到第一组命令，设置uiBS_Team为0，推出循环
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
					PlaySnd(OK);					//播放应答OK
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
		}	//擦除标志位
	}	
}