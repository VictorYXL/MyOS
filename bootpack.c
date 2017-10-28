#include"nasmfunc.h"
#include"graphic.h"
void HariMain()
{
	//屏幕长宽，图像缓冲区起始位置
	struct BootInfo *binfo;
	extern char fontlib[4096]; 
	
	//设定调色板
	init_palette();
	
	//在easyOS.nsm中定义
	binfo=(struct BootInfo *) 0x0ff0;
	init_screen(binfo);
	
	putstr(binfo,fontlib,8,8,WHITE,"You are my friend");
	while(1)
		io_hlt();

	
}
