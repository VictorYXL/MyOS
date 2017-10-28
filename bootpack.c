#include"nasmfunc.h"
#include"graphic.h"
#include"dsctbl.h" 
#include"int.h"
#include<stdio.h>
void HariMain()

{
	//屏幕长宽，图像缓冲区起始位置
	struct BootInfo *binfo;
	extern char fontlib[4096]; 
	
	//设定调色板
	init_palette();
	init_pic();
	
	//在easyOS.nsm中定义
	binfo=(struct BootInfo *) 0x0ff0; 
	init_screen(binfo);
	
	char s[120],mcursor[256];
	sprintf (s,"scrnx=%d",binfo->scrnx);
	putstr(binfo,fontlib,8,8,WHITE,s); 
	
	init_mouse_cursor(mcursor);
	put_block(binfo,16,16,200,100,mcursor);
	
	while(1)
		io_hlt();

	
}
