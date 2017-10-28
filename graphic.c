#include"nasmfunc.h"
#include"graphic.h"
void boxfill(struct BootInfo *binfo,int x0,int y0,int pxsize,int pysize,unsigned char c)
{
	int i,j;
	for (j=y0;j<y0+pysize;j++)
		for (i=x0;i<x0+pxsize;i++)
			binfo->vram[j*binfo->scrnx+i]=c;
}

void putfont8(struct BootInfo *binfo,char *fontlib,int x,int y,char color,char c)
{
	int i,j;
	char tmp[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	char *font=fontlib+c*16;
	for (i=0;i<16;i++)
		for (j=0;j<8;j++)
			if ((font[i]&tmp[j])!=0)
				binfo->vram[(y+i)*binfo->scrnx+x+j]=color;
				
}
void putstr(struct BootInfo *binfo,int x,int y,char color,char *str)
{
	extern char fontlib[4096];
	char *t=str;
	while (*t!=0x00)
	{
		putfont8(binfo,fontlib,x,y,color,*t);
		x+=8;
		t++;
	}
}

void init_mouse_cursor(char *mouse)
{
	//鼠标坐标图
	static char cursor[18][17] = {
		"****************",
		"****************",
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	//此处无法理解，前两层为何无法显示，故跳过
	for(int i=0;i<16;i++)
		for (int j=0;j<16;j++)
			switch(cursor[i+2][j])
			{
				case '*':
					mouse[i*16+j]=BLACK;
					break;
				case 'O':
					mouse[i*16+j]=WHITE;
					break;
				default:
					mouse[i*16+j]=-1;
			}
}

void put_block(struct BootInfo *binfo,int x0,int y0,int pxsize,int pysize,char *block)
{
	for (int y=0;y<pysize;y++)
		for (int x=0;x<pxsize;x++)
			if (block[y*pxsize+x]>=0)
				binfo->vram[(y0+y)*binfo->scrnx+x0+x]=block[y*pxsize+x];
}

void init_palette()
{	
	static unsigned char table_rgb[16][3] = 
	{
		{0x00, 0x00, 0x00},	//黑
		{0xff, 0x00, 0x00},	//亮红
		{0x00, 0xff, 0x00},	//亮绿
		{0xff, 0xff, 0x00},	//亮黄
		{0x00, 0x00, 0xff},	//亮蓝
		{0xff, 0x00, 0xff},	//亮紫
		{0x00, 0xff, 0xff},	//浅亮蓝
		{0xff, 0xff, 0xff},	//白
		{0xc6, 0xc6, 0xc6},	//亮灰
		{0x84, 0x00, 0x00},	//暗红
		{0x00, 0x84, 0x00},	//暗绿
		{0x84, 0x84, 0x00},	//暗黄
		{0x00, 0x00, 0x84},	//暗青
		{0x84, 0x00, 0x84},	//暗紫
		{0x00, 0x84, 0x84},	//浅暗蓝
		{0x84, 0x84, 0x84}	//暗灰
	};
	set_palette(0, 15, table_rgb);
	return;
}

void set_palette(int start,int end,unsigned char rgb[16][3])
{
	int i,eflags;
	eflags=io_load_eflags();
	io_cli();
	io_out8(0x03c8,start);//从0号调色板开始设定
	
	for (i=start;i<=end;i++)
	{
		//按RGB写入，调色板号码自动后移
		io_out8(0x03c9,rgb[i][0]/4);//除4是为了改变深浅
		io_out8(0x03c9,rgb[i][1]/4);
		io_out8(0x03c9,rgb[i][2]/4);
	}
	
	io_store_eflags(eflags);
}

void init_screen(struct BootInfo *binfo)
{
	char *vram=binfo->vram;
	int x=binfo->scrnx;
	int y=binfo->scrny;
	
	//画任务框
	//void boxfill(struct BootInfo *binfo,unsigned char c,int x0,int y0,int x1,int y1)
	//boxfill(binfo,4,0,0,x-1,y-29);
	boxfill(binfo,0,0,x,y-28,4);
	boxfill(binfo,0,y-28,x,1,8);
	boxfill(binfo,0,y-27,x,1,8);
	boxfill(binfo,0,y-26,x,26,7);

	boxfill(binfo,3,y-24,57,1,7);
	boxfill(binfo,2,y-24,1,21,7);
	boxfill(binfo,3,y-4,57,1,15);
	boxfill(binfo,59,y-23,1,19,15);
	boxfill(binfo,2,y-3,57,1,0);
	boxfill(binfo,60,y-24,1,22,0);

	boxfill(binfo,x-47,y-24,44,1,15);
	boxfill(binfo,x-47,y-23,1,20,15);
	boxfill(binfo,x-47,y-3,44,1,7);
	boxfill(binfo,x-3,y-24,1,22,7);
	return;
}
