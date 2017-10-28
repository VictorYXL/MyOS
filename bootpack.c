#include"buffer.h"
#include"bootpack.h"
#include"int.h"
#include"nasmfunc.h"
#include"graphic.h"
#include"dsctbl.h" 
#include"keyboard.h"
#include"mouse.h"
#include<stdio.h>
void HariMain()
{

	//屏幕长宽，图像缓冲区起始位置
	struct BootInfo *binfo=(struct BootInfo *) 0x0ff0;
	
	//设置中断 
	init_gdtidt();
	init_pic();
	io_sti();
		
	//允许PIC中断与键盘中断
	io_out8(PIC0_IMR, 0xf9);
	//允许鼠标中断
	io_out8(PIC1_IMR, 0xef);
	
	
	
	/*鼠标->鼠标控制电路->CPU
	先初始化鼠标控制电路（隐藏在键盘控制电路里）
	再激活鼠标*/
	
	init_keyboard();//先初始化键盘控制电路
	
	unsigned char data;
	unsigned char keyb[32],mouseb[128];
	struct Mouse_Dec mdec;
	
	buffer_init(&allbuf.key,32,keyb);
	buffer_init(&allbuf.mouse,128,mouseb);
	

	/*这里有明显的错误
	两个缓冲区对应同一块地址。
	需要解决。
	如果不能解决，可以考虑改写Buffer，将两个缓冲区写到一起*/
	//sprintf (str,"(%d) (%d)",&(allbuf.key),&(allbuf.mouse));
	//putstr(binfo,0,0,LIGHTRED,str);
	
	char str[60];
	int i=0;
	int mx=100,my=100;

	//设定调色板
	init_palette();
	//初始化屏幕
	init_screen(binfo);
	//画鼠标
	char mcursor[256];	
	init_mouse_cursor(mcursor);
	put_block(binfo,mx,my,16,16,mcursor);
	
	enable_mouse(&mdec);//再激活鼠标
	while(1)
	{
		io_cli();
		if (buffer_get(&(allbuf.key),&data))
		{
			io_sti();
			int j=i%10;
			sprintf (str,"%d) %x",i,data);
			boxfill(binfo,0,j*16,20*8,16,LIGHTGRAY);
			putstr(binfo,0,16*j,LIGHTRED,str);
			i++;
			
		}else if (buffer_get(&allbuf.mouse,&data))
		{
			io_sti();
			int j=i%10;
			if (mouse_decode(&mdec,data))
			{
				sprintf (str,"%d) ",i);
				if (mdec.lbtn)
					sprintf (str,"%sL",str);
				if (mdec.mbtn)
					sprintf (str,"%sM",str);				
				if (mdec.rbtn)
					sprintf (str,"%sR",str);
				
				boxfill(binfo,mx,my,16,16,LIGHTBLUE);
				
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
				boxfill(binfo,0,j*16,20*8,16,LIGHTGRAY);
				putstr(binfo,0,16*j,LIGHTRED,str);
				
				
				put_block(binfo,mx,my,16,16,mcursor);
				
				i++;
				
			}
		}else io_stihlt();
	}
}
