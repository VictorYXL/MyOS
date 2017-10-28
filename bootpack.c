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
#include<stdio.h>
void HariMain()
{	
	//中断初始化 
	init_gdtidt();
	init_pic();
	io_sti();
	io_out8(PIC0_IMR, 0xf9);//允许PIC中断与键盘中断
	io_out8(PIC1_IMR, 0xef);//允许鼠标中断
	
	//初始化缓冲区 
	unsigned char data;
	unsigned char keyb[32],mouseb[128];
	struct BufferAll allbuf;
	buffer_init(&allbuf.key,32,keyb);
	buffer_init(&allbuf.mouse,128,mouseb);
	
	//鼠标->鼠标控制电路->CPU
	//先初始化鼠标控制电路（隐藏在键盘控制电路里）
	//再激活鼠标
	struct Mouse_Dec mdec;
	init_keyboard();//先初始化键盘控制电路
	enable_mouse(&mdec);//再激活鼠标
	
	//初始化屏幕 
	struct BootInfo *binfo=(struct BootInfo *) BOOTADDR;//屏幕长宽，图像缓冲区起始位置
	init_palette();//设定调色板 
	putstr_srn(binfo,0,20,LIGHTRED,"Welcome To MyOS");
	putstr_srn(binfo,0,50,LIGHTRED,"Check the memory now...");

	
	//初始化内存
	struct MemoryList *meml=(struct MemoryList *)MEMORYLISTADDR;
	int size=memtest(0x00400000,0xbfffffff); 
	mem_init(meml);
	mem_free(meml,0x00001000,0x0009e000);
	mem_free(meml,0x00400000,size-0x00400000);
	
	//初始化图层表
	struct SheetControl *scl;
	scl=SCL_init(meml,binfo);
	//初始化桌面图层
	struct Sheet *sht_back;
	unsigned char *back_buf;
	sht_back=sheet_alloc(scl);//申请图层 
	back_buf=(unsigned char *)mem_alloc_4k(meml,sizeof (binfo->scrnx*binfo->scrny));//申请内存空间 
	sheet_setbuf(sht_back,back_buf,binfo->scrnx,binfo->scrny,-1);//初始化图层，其中无透明色 
	init_screen_sht(sht_back);//画图层
	sheet_slide(scl,sht_back,0,0);//移动图层位置
	sheet_updown(scl,sht_back,0);//挪动图层高度，到底层 
	while (1);
	/*//初始化屏幕 
	binfo=(struct BootInfo *) BOOTADDR;//屏幕长宽，图像缓冲区起始位置
	meml=(struct MemoryList *)MEMORYLISTADDR;
	meml与binfo指向相同的区域 
	考虑解决方案吧。。  
	if(meml==binfo && binfo==scl) 
		while (1);
	else return;
	
	//中断初始化 
	init_gdtidt();
	init_pic();
	io_sti();
	io_out8(PIC0_IMR, 0xf9);//允许PIC中断与键盘中断
	io_out8(PIC1_IMR, 0xef);//允许鼠标中断
	
	//鼠标->鼠标控制电路->CPU
	//先初始化鼠标控制电路（隐藏在键盘控制电路里）
	//再激活鼠标
	struct Mouse_Dec mdec;
	init_keyboard();//先初始化键盘控制电路
	enable_mouse(&mdec);//再激活鼠标
	//初始化缓冲区 
	unsigned char data;
	unsigned char keyb[32],mouseb[128];
	buffer_init(&allbuf.key,32,keyb);
	buffer_init(&allbuf.mouse,128,mouseb);

	//初始化内存
	int size=memtest(0x00400000,0xbfffffff); 
	mem_init();
	mem_free(0x00001000,0x0009e000);
	mem_free(0x00400000,size-0x00400000);
		
	/*char sss[128];
	sprintf (sss,"%d,%d,%d,%d,%d",meml->freesize,meml->free[0].size,meml->free[1].size,sizeof (struct SheetControl),mem_alloc_4k(sizeof(struct SheetControl)));
	//binfo=(struct BootInfo *) 0x0ff0;
	Tputstr(0,0,7,sss);
	while (1);
	char str[60];
	int mx=100,my=100;
	
	init_palette();//设定调色板 
	
	//初始化图层 
	struct Sheet *sht_back,*sht_mouse;
	unsigned char *buf_back,buf_mouse[256]; 
	
	
	SCL_init();//初始化图层表 
	//背景图层 
	sht_back=sheet_alloc();//申请背景图层 
	buf_back=(unsigned char *)mem_alloc_4k(binfo->scrnx*binfo->scrny);//申请空间 
	sheet_setbuf(sht_back,buf_back,binfo->scrnx,binfo->scrny,-1);//设置参数 
	init_screen(sht_back);//画内容 
	sheet_slide(sht_back,0,0);//移动背景图层 
	sheet_updown(sht_back,0);//放置到0层 
	//鼠标图层 
	sht_mouse=sheet_alloc();
	sheet_setbuf(sht_mouse,buf_back,16,16,99);
	init_mouse_cursor(sht_mouse);
	sheet_slide(sht_back,mx,my);
	sheet_updown(sht_mouse,1);
	

	
	
	sprintf (str,"Checking The Memory");
	boxfill(sht_back,0,0,30*8,16,LIGHTGRAY);
	putstr(sht_back,0,0,LIGHTRED,str);
	sheet_refresh();
	//内存检查 
	sprintf (str,"Memory: %dM",size/1024/1024);
	boxfill(sht_back,0,0,30*8,16,LIGHTGRAY);
	putstr(sht_back,0,0,LIGHTRED,str);
	sheet_refresh();

	while(1)
	{
		io_cli();
		if (buffer_get(&(allbuf.key),&data))
		{
			io_sti();
			sprintf (str,"Key:%x",data);
			boxfill(0,1*16,30*8,16,LIGHTGRAY);
			putstr(0,16*1,LIGHTRED,str);
			
		}else if (buffer_get(&allbuf.mouse,&data))
		{
			io_sti();
			if (mouse_decode(&mdec,data))
			{
				sprintf (str,"Mouse:");
				if (mdec.lbtn)
					sprintf (str,"%sL",str);
				if (mdec.mbtn)
					sprintf (str,"%sM",str);				
				if (mdec.rbtn)
					sprintf (str,"%sR",str);
				
				boxfill(mx,my,16,16,LIGHTBLUE);
				
				mx+=mdec.dx;
				if (mx<0)
					mx=0;
				if (mx>binfo->scrnx-16)
					mx=binfo->scrnx-16;
				my+=mdec.dy;
				if (my<0)
					my=0;
				if (my>binfo->scrny-16)
					my=binfo->scrny-16;
				
				sprintf (str,"%s (%d,%d,%d,%d)",str,mx,my,mdec.dx,mdec.dy);
				boxfill(0,2*16,30*8,16,LIGHTGRAY);
				putstr(0,16*2,LIGHTRED,str);
				
				
				put_block(mx,my,16,16,mcursor);
				
			}
		}else io_stihlt();
	}*/
}
