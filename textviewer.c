#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"textviewer.h" 
#include<stdio.h>
#include<string.h> 
void loadTextFile (char *fileName,struct Sheet *textViewerSheet,int* curPosX,int* curPosY,char curInput[20][75])
{
	if (strcmp(fileName,"\\Text\\Welcome")==0)
	{
		sprintf (curInput[(*curPosY)++],"                             Welcome to my OS");
		sprintf (curInput[(*curPosY)++],"    This is my OS which can run on bare computer.");
		sprintf (curInput[(*curPosY)++],"    My OS support visualization,multitasking and the management of memory,");
		sprintf (curInput[(*curPosY)++],"process and file.The detail of bottom layout includes start sector,system ");
		sprintf (curInput[(*curPosY)++],"boot,protected mode,graphics,pallet,interrupt,segmentation,PCB and file.Th");
		sprintf (curInput[(*curPosY)++],"e detail of upper layout includes buffer,multitask and communication,mouse");
		sprintf (curInput[(*curPosY)++]," and keyboard's driver,typeface,timer,sheet,window and focus,and API.The a");
		sprintf (curInput[(*curPosY)++],"pplication on this OS includes calculator,drawing,music player,note,task m");
		sprintf (curInput[(*curPosY)++],"anager,memory manager and file manager.");
		sprintf (curInput[(*curPosY)++],"    The management of memory uses free table and first fit algorithm,inclu");
		sprintf (curInput[(*curPosY)++],"ding memory test,memory alloc and auto free.The management of memory uses ");
		sprintf (curInput[(*curPosY)++],"round robin algorithm,including task alloc,shift,sleep and communication b");
		sprintf (curInput[(*curPosY)++],"etween tasks.The file system based on FAT,including text file,image file,m");
		sprintf (curInput[(*curPosY)++],"usic file and menu.");
		sprintf (curInput[(*curPosY)++],"    This OS embodies my efforts.");
	}
	
	for (int i=0;i<*curPosY;i++)
		putStrAndBackOnSht(textViewerSheet,16,28+i*16,BLACK,WHITE,curInput[i],75);
}

void textViewerTask_Main(struct Task *task)
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
	textViewerBuffer=(unsigned char *)allocMem_4k(650*400,"Text Viewer UI");//申请内存空间 
	setBufInSheet(textViewerSheet,textViewerBuffer,650,400,-1);
	char s[60];
	sprintf (s,"Text Viewer: %s",task->par[0]);
	makeWindow(textViewerSheet,650,400,"Text Viewer");
	slideSheet(textViewerSheet,180,72);
	updownSheet(textViewerSheet,task->winID+1);
	
	//输入的信息 
	char curInput[20][75];
	int curPosX=0,curPosY=0,lengthX=0,lengthY=0;//光标 
	unsigned char data;
	//struct Command command;//命令 
	char str[128];
	int flag=0,f1=0;
	
	for (int i=0;i<50;i++)
		curInput[i][0]='\0';
	loadTextFile(task->par[0],textViewerSheet,&curPosX,&curPosY,curInput);
	
	while (1)
	{
		flag=0; 
		if (window.focus!=task->winID)//焦点不在，取消光标 
		{
			if (f1==0)
			{
				boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,WHITE);
				refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15); 
				f1=1;
			}
			continue;
		}else if (timerCur->flag==TIMER_ALLOCED)//重新获得焦点，重启光标 
		{
			initTimer(timerCur,&bufferTime,1);
			setTimer(timerCur,50);
		}
		f1=0;
		io_cli();
		//检查鼠标键盘事件 
		if (getBuffer(&task->bufAll.key,&data))
		{
			//键盘 
			io_sti();
			flag=1;
			switch (data)
			{
				case 0x0e://退格键 
					if (curPosX>2)
					{
						curPosX--;
						lengthX--; 
						for (int j=curPosX;j<=lengthX;j++)
							curInput[curPosY][j]=curInput[curPosY][j+1];
						curInput[curPosY][lengthX] = '\0';
					} 
					break; 
				case 0x4b:
					if (curPosX>0)
						curPosX--;
					break;
				case 0x4d:
					if (curPosX<lengthX)
						curPosX++;
					break;
				case 0x48:
					if (curPosY>0)
					{
						curPosY--;
						lengthX=0;
						while (curInput[curPosY][lengthX] != '\0')
							lengthX++;
					}
					break;
				case 0x50:
					if (curPosY<lengthY)
					{
						curPosY++;
						lengthX=0;
						while (curInput[curPosY][lengthX] != '\0')
							lengthX++;
					}
					break;
				case 0x1c://回车键 
					boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,WHITE);
					refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15); 
					curPosY++;
					if (curPosY>lengthY)
						lengthY=curPosY;
					curInput[curPosY][0] = '\0';
					curPosX=0;
					lengthX=0;
					break;
				default:
					if (data<0x80 && keyboard.keyTable[data]>0 && curPosX<50)//字母，数字 
					{
						curInput[curPosY][curPosX++] = keyboard.keyTable[data+keyboard.isShift*0x80];
						if (curPosX>lengthX)
						{
							curInput[curPosY][curPosX] = '\0';
							lengthX=curPosX;
						}
					}
					break;
			}
			sprintf (str,"%s",curInput[curPosY]);
			putStrAndBackOnSht(textViewerSheet,16,28+curPosY*16,BLACK,WHITE,str,75);
		}
		if (getBuffer(&task->bufAll.mouse,&data))
		{
			io_sti();
			flag=2;
			switch(data)
			{
				case 0:
					slideSheet(textViewerSheet,mdec.x,mdec.y);
					break;
				case 2:
					freeSheet(textViewerSheet);
					//freeMem((unsigned int)curInput,20);
					freeMem((unsigned int)textViewerBuffer,312*210);
					deleteWindow(task);
					deleteTask(task);
			}
		}
		if (getBuffer(&bufferTime,&data))
		{
			io_sti();
			flag=3;
			switch (data)
			{
			case 0:
				initTimer(timerCur,&bufferTime,1);
				boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,WHITE);
				refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15);   
				setTimer(timerCur,50);
				break;
			case 1:
				initTimer(timerCur,&bufferTime,0);
				boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,BLACK);
				refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15);
				setTimer(timerCur,50);
				break; 
			}
		} 
		if (flag==0)
			io_sti();
	}
}
