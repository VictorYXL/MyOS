#include"nasmfunc.h"
#include"graphic.h"
#include"dsctbl.h" 
#include"int.h"
#include"buffer.h"
#include<stdio.h>

extern struct Buffer buffer;
void HariMain()

{
	//屏幕长宽，图像缓冲区起始位置
	struct BootInfo *binfo=(struct BootInfo *) 0x0ff0;
	
	//设置中断 
	init_gdtidt();
	init_pic();
	io_sti();
	
	//设定调色板
	init_palette();
	init_screen(binfo);
	
	char mcursor[256];	
	init_mouse_cursor(mcursor);
	put_block(binfo,16,16,200,100,mcursor);
	
	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);
	
	
	/*keybuf.len=0;
	keybuf.h=0;
	keybuf.r=0;*/
	unsigned char data;
	char str[20];
	int i=0;
	buffer_init(&buffer);
	while(1)
	{
		io_cli();
		if (buffer_get(&buffer,&data))
		{
			io_sti();
			
			int j=i%10;
			sprintf (str,"%d)  %02x",i,data);
			boxfill(binfo,LIGHTGRAY,0,j*16,20*8,j*16+16);
			putstr(binfo,0,16*j,LIGHTRED,str);
			i++;
			
		}else io_stihlt();
	}

	
}
