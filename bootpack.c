#include"mtask.h"
#include"buffer.h"
#include"int.h"
#include"nasmfunc.h"
#include"dsctbl.h"  
#include"keyboard.h"
#include"mouse.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"bootpack.h"
#include"sheet.h"
#include"timer.h" 
#include<stdio.h>



void HariMain()
{	
	//中断初始化 
	init_gdtidt();
	init_pic();
	io_sti();
	io_out8(PIC0_IMR, 0xf8);//允许PIC中断与键盘中断
	io_out8(PIC1_IMR, 0xef);//允许鼠标中断
	
	//初始化鼠标、键盘、定时器缓冲区 
	unsigned char data;
	unsigned char keyb[32],mouseb[1024],timeb[8];
	struct Buffer timeBuffer;
	buffer_init(&allbuf.key,32,keyb,0);
	buffer_init(&allbuf.mouse,1024,mouseb,0);
	buffer_init(&timeBuffer,8,timeb,0);
	
	//定时器初始化
	initPit();
	//设定定时器 
	struct Timer *timer1;//timer1为光标闪烁定时器,timer2为倒计时定时器 
	timer1=allocTimer();
	initTimer(timer1,&timeBuffer,10);
	setTimer(timer1,50);
	
	//初始化屏幕 
	struct BootInfo *binfo=(struct BootInfo *) BOOTADDR;//屏幕长宽，图像缓冲区起始位置
	init_palette();//设定调色板 
	putstr_srn(binfo,0,20,LIGHTRED,"Welcome To my OS");
	putstr_srn(binfo,0,50,LIGHTRED,"Check the memory now...");
	
	//鼠标->鼠标控制电路->CPU
	//先初始化鼠标控制电路（隐藏在键盘控制电路里）
	//再激活鼠标
	struct Mouse_Dec mdec;
	mdec.x=100;
	mdec.y=100;
	init_keyboard();//先初始化键盘控制电路
	enable_mouse(&mdec);//再激活鼠标
	


	
	//初始化内存
	//struct MemoryList *
	meml=(struct MemoryList *)MEMORYLISTADDR;
	int size;
	mem_init();
	//size=memtest(0x00400000,0xbfffffff);//内存测试 
	mem_free(0x00001000,0x0009e000);
	mem_free(0x00400000,size-0x00400000);
	
	//初始化键盘数据
	static char keyTable[0x80] =
	{
 		 0 , 0 ,'1','2','3','4','5','6','7','8','9','0','-','^', 0 , 0 ,
		'Q','W','E','R','T','Y','U','I','O','P','[',']', 0 , 0 ,'A','S',
		'D','F','G','H','J','K','L',';','\'','`', 0 ,'\\','Z','X','C','V',
		'B','N','M',',','.','/', 0 ,'*', 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'7','8','9','-','4','5','6','+','1',
		'2','3','0','.', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 ,'_', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'\\', 0 , 0 
	};
	char curInput[128];
	int curPos = 0;

	//初始化图层表
	scl=SCL_init(binfo);

	//初始化桌面图层
	struct Sheet *sht_back;
	unsigned char *buf_back;
	sht_back=sheet_alloc();//申请图层 
	buf_back=(unsigned char *)mem_alloc_4k(binfo->scrnx*binfo->scrny);//申请内存空间 
	sheet_setbuf(sht_back,buf_back,binfo->scrnx,binfo->scrny,-1);//初始化图层，其中无透明色 
	init_screen_sht(sht_back);//画图层
	sheet_slide(sht_back,0,0);//移动图层位置
	sheet_updown(sht_back,0);//挪动图层高度，到底层 

	//显示内存大小
	char str[128];
	sprintf (str,"Memory: %dM",size/1024/1024);
	putstr_back_sht(sht_back,0,0,LIGHTRED,LIGHTGRAY,str,13);
	
	//显示窗口
	struct Sheet *sht_window;
	unsigned char *buf_window;
	sht_window=sheet_alloc();
	buf_window=(unsigned char *)mem_alloc_4k(160*68);//申请内存空间 
	sheet_setbuf(sht_window,buf_window,160,68,-1);
	make_window(sht_window,160,68,"window");
	putstr_sht(sht_window,16,28,BLACK,"Welcome to my OS");
	sheet_slide(sht_window,80,72);
	sheet_updown(sht_window,1);
	
	//初始化鼠标图层
	struct Sheet *sht_mouse;
	char mousebuf[256];
	sht_mouse=sheet_alloc();
	sheet_setbuf(sht_mouse,mousebuf,16,16,99);
	init_mouse_cursor(sht_mouse);
	sheet_slide( sht_mouse, mdec.x, mdec.y);
	sheet_updown(sht_mouse,2);
	sheet_refreshAll();//所有图层准备 
	
	//初始化多任务
	initTask();
	
	//设定主任务
	struct Task *mainTask;
	mainTask=getMainTask();
	//关联键鼠缓冲与主任务 
	allbuf.key.task=mainTask;
	allbuf.mouse.task=mainTask;
	//定义任务B
	struct Task *taskb;
	taskb=allocTask();
	taskb->tss.esp=mem_alloc_4k(64*1024)+64*1024-12;//栈底指针为内存的末尾·为了让地址不超过范围：sht_back为esp+8到esp+12 
	taskb->tss.eip=(int) &task_b_main;
	taskb->tss.es=1*8;
	taskb->tss.cs=2*8;
	taskb->tss.ss=1*8;
	taskb->tss.ds=1*8;
	taskb->tss.fs=1*8;
	taskb->tss.gs=1*8;
	*((int *)(taskb->tss.esp+4)) = (int)scl;//压入栈，当作函数的参数 
	*((int *)(taskb->tss.esp+8)) = (int)sht_back;
	runTask(taskb);
	
	
	int count=0;
	int flag;
	int pst=0;//光标状态 
	
	//程序大循环 
	while(1)
	{
		count++;
		flag=0;
		//检查各类中断 
		io_cli();
		if (buffer_get(&(allbuf.key),&data))
		{
			//键盘 
			io_sti();
			flag=1;
			sprintf (str,"Key:%x",data);
			putstr_back_sht(sht_back,0,1*16,LIGHTRED,LIGHTBLUE,str,6);//显示键盘信息 
			
			if (data<0x80 && keyTable[data]>0)
			{
				curInput[curPos++] = keyTable[data];
				curInput[curPos] = '\0';
				sprintf (str,"%s",curInput);
				putstr_back_sht(sht_window,16,44,BLACK,WHITE,str,16);
			}else if (data==0x0e)
			{
				if (curPos>0)
					curInput[--curPos] = '\0';
				sprintf (str,"%s",curInput);
				putstr_back_sht(sht_window,16,44,BLACK,WHITE,str,16);
			}
			
		}
		if (buffer_get(&allbuf.mouse,&data))
		{
			//鼠标 
			io_sti();
			flag=2;
			if (mouse_decode(&mdec,data))
			{
				sprintf (str,"Mouse:");
				if (mdec.lbtn)
					sprintf (str,"%sL",str);
				if (mdec.mbtn)  
					sprintf (str,"%sM",str);				
				if (mdec.rbtn)
					sprintf (str,"%sR",str);
				
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
				
				sprintf (str,"%s (%d,%d,%d,%d)",str,mdec.x,mdec.y,mdec.dx,mdec.dy);
				putstr_back_sht(sht_back,0,2*16,LIGHTRED,LIGHTBLUE,str,25);//显示键盘信息 
				sheet_slide(sht_mouse,mdec.x,mdec.y);
				
			}
		}
		if (buffer_get(&timeBuffer,&data))
		{
			//定时器 
			io_sti();
			flag=3;
			switch (data)//为了区分不同的定时器 
			{
			case 10://timer1显示光标 
				initTimer(timer1,&timeBuffer,11);
				boxfill_sht(sht_window,16+8*curPos,44,8,15,BLACK);
				sheet_refreshSheetSub(sht_window,16+8*curPos,44,8,15);   
				setTimer(timer1,50);
				break;
			case 11://timer1隐藏光标 
				initTimer(timer1,&timeBuffer,10);
				boxfill_sht(sht_window,16+8*curPos,44,8,15,WHITE);
				sheet_refreshSheetSub(sht_window,16+8*curPos,44,8,15);
				setTimer(timer1,50);
				break; 
			/*case 20://5秒后切换到任务B（休眠） 
				farjmp(0,4*8);
				setTimer(timer2,2);*/ 
			}
		}
		if (flag==0)
		{
			//没有事件就休眠 
			sleepTask(mainTask);
			io_sti();
		}
	}
}


//任务B不需要关心任务的切换 
void task_b_main(struct SheetControl *scl,struct Sheet *sht_back)
{
	
	int count=1000000001,num=0;
	char str[128];
	while (1)
	{
		if (count>1000000000)
		{
			count=0;
			num++;
			sprintf (str,"Task B:%x(G)",num);
			putstr_back_sht(sht_back,0,3*16,LIGHTRED,LIGHTBLUE,str,30);//显示任务B
		}
		count++;
	} 
}
