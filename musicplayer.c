#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"musicplayer.h" 
#include<stdio.h>

void musicPlayerTask_Main(struct Task *task)
{
	//初始化缓冲区 
	char bufferArray[128];
	struct Buffer bufferTime;
	initBuffer(&bufferTime,128,bufferArray);

	//初始化定时器 
	struct Timer *timerCur;
	timerCur=allocTimer();
	initTimer(timerCur,&bufferTime,1);
	setTimer(timerCur,50);

	//显示窗口
	struct Sheet *textViewerSheet;
	unsigned char *textViewerBuffer;
	textViewerSheet=allocSheet();
	textViewerBuffer=(unsigned char *)allocMem_4k(200*68,"Music Player UI");//申请内存空间 
	setBufInSheet(textViewerSheet,textViewerBuffer,200,68,-1);
	char s[60];
	sprintf (s,"Music Player: %s",task->par[0]);
	makeWindow(textViewerSheet,200,68,"Music Player");
	slideSheet(textViewerSheet,180,72);
	updownSheet(textViewerSheet,task->winID+1);
	
	//输入的信息 
	char curInput[128];
	int curPosX=0,length=0;//光标 
	unsigned char data;
	//struct Command command;//命令 
	char str[128];
	int flag=0,f1=0;
	int t=0;
	while (1);
}
