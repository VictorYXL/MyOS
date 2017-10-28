#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"imageviewer.h" 
#include<stdio.h>

void imageViewerTask_Main(struct Task *task)
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
	struct Sheet *consoleSheet;
	unsigned char *consoleBuffer;
	consoleSheet=allocSheet();
	consoleBuffer=(unsigned char *)allocMem_4k(200*68,"image Viewer UI");//申请内存空间 
	setBufInSheet(consoleSheet,consoleBuffer,200,68,-1);
	makeWindow(consoleSheet,200,68,"Image Viewer");
	slideSheet(consoleSheet,180,72);
	updownSheet(consoleSheet,task->winID+1);
	
	//输入的信息 
	char curInput[128];
	int curPosX=0,length=0;//光标 
	unsigned char data;
	char str[128];
	int flag=0,f1=0;
	int t=0;
	while (1);
}
