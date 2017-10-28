#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"timer.h"
#include"player.h" 
#include<stdio.h>
#include<string.h>
void playOneBeat(int eax)
{
	int i=0;
	//停止播放 
	if (eax==0)
	{
		i=io_in8(0x61);
		io_out8(0x61,i&0x0d);
	}else//播放 
	{
		i=1193180000/eax;
		io_out8(0x43,0xb6);
		io_out8(0x42,i&0xff);
		io_out8(0x42,i>>8);
		i=io_in8(0x61);
		io_out8(0x61,(i | 0x03) & 0x0f);
	}
}
void loadMusicFile(char *fileName,struct Task *task,struct Sheet *musicPlayerSheet)
{
	int tone[12] = {1071618315,1135340056,1202850889,1274376125,1350154473,1430438836,
		1515497155,1605613306,1701088041,1802240000,1909406767,2022946002}; 
	int note[7] = {9,11,0,2,4,5,7};
	//初始化缓冲区 
	char bufferArray;
	struct Buffer bufferTime;
	initBuffer(&bufferTime,1,&bufferArray);

	//初始化定时器 
	struct Timer *timerPlayer;
	timerPlayer=allocTimer();
	initTimer(timerPlayer,&bufferTime,1);
	char data;
	
	int i,o=4,t=0;
	char str[10];
	if (strcmp(fileName,"\\Music\\Mus1")==0)
	{
		char music[50]="CCGGAAG.FFDDEEC.GGFFEED.GGFFEED.CCGGAAG.FFDDEEC...";
		sprintf (str,"Playing");
		putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
		while (1)
		{
			for (int p=0;p<50;p++)
			{
				if (music[p]!='.')
				{
					i=o*12+note[music[p]-'A']+12;
					playOneBeat(tone[i%12]>>(17-i/12));
				}else playOneBeat(0);
			
				setTimer(timerPlayer,20);
				while (1)
					if (getBuffer(&bufferTime,&data))
						break;
					else if (getBuffer(&task->bufAll.key,&data) && keyboard.keyTable[data]==' ')
					{
						playOneBeat(0);
						sprintf (str,"Suspend");
						putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
						while (!(getBuffer(&task->bufAll.key,&data) && keyboard.keyTable[data]==' '));
						sprintf (str,"Playing");
						putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
					}
				playOneBeat(0);
			
				setTimer(timerPlayer,10);
				while (1)
					if (getBuffer(&bufferTime,&data))
						break;
					else if (getBuffer(&task->bufAll.key,&data) && keyboard.keyTable[data]==' ')
					{
						playOneBeat(0);
						sprintf (str,"Suspend");
						putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
						while (!(getBuffer(&task->bufAll.key,&data) && keyboard.keyTable[data]==' '));
						sprintf (str,"Playing");
						putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
					}
			}
		}
	}
	playOneBeat(0);
}
void playTask_Main(struct Task *task)
{
	//初始化缓冲区 
	char bufferArray[128];
	struct Buffer bufferTime;
	initBuffer(&bufferTime,128,bufferArray);
	
	//显示窗口
	struct Sheet *musicPlayerSheet;
	unsigned char *musicPlayerBuffer;
	musicPlayerSheet=allocSheet();
	slideSheet(musicPlayerSheet,500,120);
	musicPlayerBuffer=(unsigned char *)allocMem(200*68,"Player UI");//申请内存空间 
	setBufInSheet(musicPlayerSheet,musicPlayerBuffer,200,68,-1);
	makeWindow(musicPlayerSheet,200,68,"Player");
	setHeightSheet(musicPlayerSheet,task->winID+1);
	
	int flag=0;
	unsigned char data;
	loadMusicFile(task->par[0],task,musicPlayerSheet);
	while (1)
	{
		flag=0;
		io_cli();
		if (getBuffer(&task->bufAll.mouse,&data))
		{
			io_sti();
			flag=2;
			switch(data)
			{
				//左键移动 
				case 0:
					slideSheet(musicPlayerSheet,mdec.x,mdec.y);
					break;
				//右键关闭
				case 2:
					freeSheet(musicPlayerSheet);
					freeMem((unsigned int)musicPlayerBuffer,200*68);
					deleteWindow(task);
					deleteTask(task);
					break;
			}
		}
		if (flag==0)
			io_sti();
	}
}
