#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"dsctbl.h"  
#include"buffer.h"
#include"timer.h" 
#include"keyboard.h"
#include"mouse.h"
#include"mtask.h"
#include"int.h"
#include"console.h" 
#include"calculator.h" 
#include"main.h"
#include<stdio.h>
void HariMain()
{	
	//中断初始化 
	init_gdtidt();
	init_pic();
	io_sti();
	io_out8(PIC0_IMR, 0xf8);//允许PIC中断与键盘中断
	io_out8(PIC1_IMR, 0xef);//允许鼠标中断
	
	//初始化鼠标、键盘、定时器缓冲区（主任务键鼠缓冲区定义在栈中，子任务缓冲定义在堆中） 
	unsigned char data;
	unsigned char keyb[32],mouseb[1024],timeb[8];
	struct Buffer timeBuffer;
	initBuffer(&allBuf.key,32,keyb);
	initBuffer(&allBuf.mouse,1024,mouseb);
	initBuffer(&timeBuffer,8,timeb);
	
	//定时器初始化
	initPit();
	//设定定时器 
	struct Timer *timer1;//timer1为光标闪烁定时器,timer2为倒计时定时器 
	timer1=allocTimer();
	initTimer(timer1,&timeBuffer,10);
	setTimer(timer1,50);
	
	//初始化屏幕 
	struct BootInfo *binfo=(struct BootInfo *) BOOTADDR;//屏幕长宽，图像缓冲区起始位置
	initPalette();//设定调色板 
	putStrOnSrn(binfo,0,20,LIGHTRED,"Welcome To my OS");
	putStrOnSrn(binfo,0,50,LIGHTRED,"Check the memory now...");
	
	//鼠标->鼠标控制电路->CPU
	//先初始化鼠标控制电路（隐藏在键盘控制电路里）
	//再激活鼠标
	mdec.x=100;
	mdec.y=100;
	init_keyboard();//先初始化键盘控制电路
	enable_mouse(&mdec);//再激活鼠标
	


	
	//初始化内存
	//struct MemoryList *
	meml=(struct MemoryList *)MEMORYLISTADDR;
	int size;
	initMem();
	//size=memtest(0x00400000,0xbfffffff);//内存测试 
	freeMem(0x00001000,0x0009e000);
	freeMem(0x00400000,size-0x00400000);
	
	//初始化键盘数据
	char keyTable[0x160]=
	{
		 0 , 0 ,'1','2','3','4','5','6','7','8','9','0','-','=', 0 , 0 ,
		'q','w','e','r','t','y','u','i','o','p','[',']', 0 , 0 ,'a','s',
		'd','f','g','h','j','k','l',';','\'','`', 0 ,'\\','z','x','c','v',
		'b','n','m',',','.','/', 0 ,'*', 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'7','8','9','-','4','5','6','+','1',
		'2','3','0','.', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 ,'_', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'\\', 0 , 0 ,
 		 0 , 0 ,'!','@','#','$','%','^','&','*','(',')','_','+', 0 , 0 ,
		'Q','W','E','R','T','Y','U','I','O','P','[',']', 0 , 0 ,'A','S',
		'D','F','G','H','J','K','L',';','|','~', 0 ,'\\','Z','X','C','V',
		'B','N','M',',','.','/', 0 ,'*', 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'7','8','9','_','4','5','6','+','1',
		'2','3','0','.', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 ,'_', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'\\', 0 , 0
	};
	for (int i=0x00;i<0x160;i++)
		keyboard.keyTable[i]=keyTable[i];
	char curInput[128];
	int curPos = 0;

	//初始化图层表
	scl=initSCL(binfo);

	//初始化桌面图层
	struct Sheet *sht_back;
	unsigned char *buf_back;
	sht_back=allocSheet();//申请图层 
	buf_back=(unsigned char *)allocMem_4k(binfo->scrnx*binfo->scrny);//申请内存空间 
	setBufInSheet(sht_back,buf_back,binfo->scrnx,binfo->scrny,-1);//初始化图层，其中无透明色 
	initScreenOnSht(sht_back);//画图层
	slideSheet(sht_back,0,0);//移动图层位置
	setHeightSheet(sht_back,0);
	
	//初始化鼠标图层
	struct Sheet *sht_mouse;
	char mousebuf[256];
	sht_mouse=allocSheet();
	setBufInSheet(sht_mouse,mousebuf,16,16,99);
	initMouseCursor(sht_mouse);
	slideSheet(sht_mouse, mdec.x, mdec.y);
	setHeightSheet(sht_mouse,255); 
	
	//显示基本信息 
	char str[128]; 
	//显示内存大小
	sprintf (str,"Memory: %dM",size/1024/1024);
	putStrAndBackOnSht(sht_back,0,0,LIGHTRED,LIGHTGRAY,str,-1);
	//显示Focus窗口
	sprintf (str,"Fouse: Console");
	putStrAndBackOnSht(sht_back,0,1*16,LIGHTRED,LIGHTBLUE,str,40);
	refreshAllSheet();//所有图层准备 
	
	//初始化多任务
	initTaskCTL();
	
	//设定主任务
	struct Task *mainTask;
	mainTask=getMainTask();
	
	//初始化焦点，开始焦点为Console任务 
	initWindow();
	
	//定义Console任务
	struct Task *consoleTask;
	consoleTask=allocTask();
	initTask(consoleTask,(int)&consoleTask_Main);
	createWindow(consoleTask,"Console");
	runTask(consoleTask);
	while(1);
	//定义calculator任务
	/*struct Task *calculatorTask;
	calculatorTask=allocTask();
	window.winCount++;
	initTask(calculatorTask,(int)&calculatorTask_Main,window.winCount);
	sprintf (window.winName[window.winCount],"Calculator");
	runTask(calculatorTask);*/ 
	
	int flag;
	keyboard.isShift=0;
	//程序大循环 
	while(1)
	{
		flag=0;
		if (window.isChanged)
		{
			sprintf (str,"Focus: %s",window.winName[window.focus]);//,window.focus,window.winCount,taskctl->now,taskctl->runningCount,taskctl->runningCount);
			putStrAndBackOnSht(sht_back,0,1*16,LIGHTRED,LIGHTBLUE,str,40);
			window.isChanged=0;
		}
		//检查各类中断 
		io_cli();
		if (getBuffer(&allBuf.key,&data))
		{
			//键盘 
			io_sti();
			flag=1;
			sprintf (str,"Key: %x",data);
			putStrAndBackOnSht(sht_back,0,2*16,LIGHTRED,LIGHTBLUE,str,26);//显示键盘信息 
			//执行键盘动作 
			switch (data)
			{
				case 0x0f://Tab键
					window.focus++; 
					window.focus%=window.winCount;
					window.isChanged=1;
					break;
				case 0x2a:
				case 0x36://Shift按住 
					keyboard.isShift=1;
					break;
				case 0xaa:
					case 0xb6://Shift松开 
					keyboard.isShift=0;
					break;
				default://剩余键盘动作 传给子任务 
					putBuffer(&taskctl->tasks[1+window.focus]->bufAll.key,data);//主任务没有窗口，故窗口号+1=任务号 
					break;
			}
			/*if (window.focus==0)
			{
				if (data<0x80 && keyTable[data]>0 && curPos<=15)//字母，数字 
				{
					curInput[curPos++] = keyTable[data];
					curInput[curPos] = '\0';
				}else switch (data)
				{
					case 0x0e://退格键 
						if (curPos>0)
							curInput[--curPos] = '\0';
						break; 
					case 0x0f://Tab键
						window.focus++; 
						window.focus%=window.winCount;
						break;
				}
				sprintf (str,"%s",curInput);
				putStrAndBackOnSht(sht_window,16,44,BLACK,WHITE,str,16);
			}*/
			
		}
		if (getBuffer(&allBuf.mouse,&data))
		{
			//鼠标 
			io_sti();
			flag=2;
			if (mouse_decode(&mdec,data))
			{
				mdec.x+=mdec.dx;
				if (mdec.x<0)
					mdec.x=0;
				if (mdec.x>binfo->scrnx-1)
					mdec.x=binfo->scrnx-1;
				mdec.y+=mdec.dy;
				if (mdec.y<0)
					mdec.y=0;
				if (mdec.y>binfo->scrny-1)
					mdec.y=binfo->scrny-1;
				
				
				sprintf (str,"Mouse: ");
				if (mdec.lbtn)
					sprintf (str,"%sL",str);
				if (mdec.mbtn)  
					sprintf (str,"%sM",str);				
				if (mdec.rbtn)
					sprintf (str,"%sR",str);
				sprintf (str,"%s (%d,%d)",str,mdec.x,mdec.y);
				putStrAndBackOnSht(sht_back,0,3*16,LIGHTRED,LIGHTBLUE,str,25);//显示鼠标信息 
				slideSheet(sht_mouse,mdec.x,mdec.y);
				
				//剩余鼠标动作传给子任务 
				//主任务没有窗口，故图层号+1=窗口号+1=任务号 
				if (mdec.lbtn)
					putBuffer(&taskctl->tasks[window.focus+1]->bufAll.mouse,0);
				else if (mdec.mbtn)
					putBuffer(&taskctl->tasks[window.focus+1]->bufAll.mouse,1);
				else if (mdec.rbtn)
					putBuffer(&taskctl->tasks[window.focus+1]->bufAll.mouse,2);
				
			}
		}
		/*if (getBuffer(&timeBuffer,&data))
		{
			//定时器 
			io_sti();
			flag=3;
			switch (data)//为了区分不同的定时器 
			{
			case 10://timer1显示光标 
				initTimer(timer1,&timeBuffer,11);
				boxfillOnSht(sht_window,16+8*curPos,44,8,15,BLACK);
				refreshSubInSheet(sht_window,16+8*curPos,44,8,15);   
				setTimer(timer1,50);
				break;
			case 11://timer1隐藏光标 
				initTimer(timer1,&timeBuffer,10);
				boxfillOnSht(sht_window,16+8*curPos,44,8,15,WHITE);
				refreshSubInSheet(sht_window,16+8*curPos,44,8,15);
				setTimer(timer1,50);
				break; 
			}
		}*/
		if (flag==0)
		{
			//没有事件就休眠 
			//sleepTask(mainTask);
			io_sti();
		}
	}
}



/* 
//任务calculator 
void calculatorTask_Main(struct Task *task)
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
	
	//初始化控制台图层
	struct Sheet *consoleSheet;
	unsigned char *consoleBuffer;	
	consoleBuffer=(unsigned char *)allocMem_4k(512*310);
	consoleSheet=allocSheet();
	setBufInSheet(consoleSheet,consoleBuffer,512,310,-1);//没有透明色
	slideSheet(consoleSheet,202,8);
	makeWindow(consoleSheet,512,310,"Calculator");
	makeTextBox(consoleSheet,8,27,496,276,BLACK);
	setHeightSheet(consoleSheet,1);
	
	unsigned char data;
	int curPos=0;
	int flag=0;
	while (1)
	{
		flag=0;
		if (window.focus!=task->winID)//焦点不在，取消光标 
		{
			boxfillOnSht(consoleSheet,8+8*curPos,28,8,15,BLACK);
			refreshSubInSheet(consoleSheet,8+8*curPos,28,8,15); 
			continue;
		}else if (timerCur->flag==TIMER_ALLOCED)//重新获得焦点，重启光标 
		{
			initTimer(timerCur,&bufferTime,1);
			setTimer(timerCur,50);
		}
		io_cli();
		if (getBuffer(&bufferTime,&data))
		{
			io_sti();
			flag=3;
			switch (data)
			{
			case 0:
				initTimer(timerCur,&bufferTime,1);
				boxfillOnSht(consoleSheet,8+8*curPos,28,8,15,WHITE);
				refreshSubInSheet(consoleSheet,8+8*curPos,28,8,15);   
				setTimer(timerCur,50);
				break;
			case 1:
				initTimer(timerCur,&bufferTime,0);
				boxfillOnSht(consoleSheet,8+8*curPos,28,8,15,BLACK);
				refreshSubInSheet(consoleSheet,8+8*curPos,28,8,15);
				setTimer(timerCur,50);
				break; 
			}
		} 
		if (flag==0)
		{
			io_sti();
		} 
	}
}*/ 
