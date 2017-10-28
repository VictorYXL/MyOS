#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"mouse.h"
#include"memorylist.h"
#include<stdio.h>
void showMemory(struct Sheet *memoryListSheet)
{
	int curPosY=0;
	makeWindow(memoryListSheet,470,710,"Memory List");
	char str[70]="   From         To           Status";
	putStrAndBackOnSht(memoryListSheet,18,32+curPosY*30,BLACK,WHITE,str,36);
	curPosY=1;
	for (int i=0;i<128;i++)
	{
		if (meml->used[i].flag==1)
		{
			sprintf (str,"0x%08x  0x%08x  %s",meml->used[i].addr,meml->used[i].addr+meml->used[i].size,meml->used[i].status);
			putStrAndBackOnSht(memoryListSheet,18,32+curPosY*30,BLACK,WHITE,str,36);
			curPosY++;
		}
	}
	
}

void memoryListTask_Main(struct Task *task)
{
	//初始化缓冲区 
	char bufferArray[128];
	struct Buffer bufferTime;
	initBuffer(&bufferTime,128,bufferArray);
	//初始化定时器 
	struct Timer *timerCur;
	timerCur=allocTimer();
	initTimer(timerCur,&bufferTime,1);
	setTimer(timerCur,100);
	
	
	//显示窗口
	struct Sheet *memoryListSheet;
	unsigned char *memoryListBuffer;
	memoryListSheet=allocSheet();
	slideSheet(memoryListSheet,400,20);
	memoryListBuffer=(unsigned char *)allocMem(470*710,"Memory List UI");//申请内存空间 
	setBufInSheet(memoryListSheet,memoryListBuffer,470,710,-1);
	makeWindow(memoryListSheet,470,710,"Memory List");
	setHeightSheet(memoryListSheet,task->winID+1);
	
	//显示信息 
	showMemory(memoryListSheet);
	refreshSheet(memoryListSheet);
	unsigned char data;
	int flag=0;
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
					slideSheet(memoryListSheet,mdec.x,mdec.y);
					break;
				//右键关闭
				case 2:
					freeSheet(memoryListSheet);
					freeMem((unsigned int)memoryListBuffer,470*710);
					deleteWindow(task);
					deleteTask(task);
					break;
			}
		}
		//定时刷新 
		if (getBuffer(&bufferTime,&data))
		{
			io_sti();
			flag=3;
			switch (data)
			{
			case 1:
				initTimer(timerCur,&bufferTime,1);
				//显示信息 
				showMemory(memoryListSheet);
				refreshSheet(memoryListSheet);
				setTimer(timerCur,100);
				break;
			}
		} 
		if (flag==0)
			io_sti();
	}
} 
