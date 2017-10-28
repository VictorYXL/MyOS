#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
void boxfill_sht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,unsigned char c)
{
	int i,j;
	for (j=y0;j<y0+pysize;j++)
		for (i=x0;i<x0+pxsize;i++)
			sht->buffer[j*sht->xsize+i]=c;
}

void putfont8_sht(struct Sheet *sht,int x,int y,char color,char c)
{
	int i,j;
	char tmp[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	char *font=fontlib+c*16;
	for (i=0;i<16;i++)
		for (j=0;j<8;j++)
			if ((font[i]&tmp[j])!=0)
				sht->buffer[(y+i)*sht->xsize+x+j]=color;
}
void putstr_sht(struct Sheet *sht,int x,int y,char color,char *str)
{
	char *t=str;
	while (*t!=0x00)
	{
		putfont8_sht(sht,x,y,color,*t);
		x+=8;
		t++;
	}
}
void putstr_back_sht(struct Sheet *sht,int x,int y,int color,int backcolor,char *str,int length)
{
	boxfill_sht(sht,x,y,length*8,16,backcolor);
	putstr_sht(sht,x,y,color,str);
	sheet_refreshSheetSub(sht,x,y,length*8,16);
}
void init_mouse_cursor(struct Sheet *sht)
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
					sht->buffer[i*16+j]=BLACK;
					break;
				case 'O':
					sht->buffer[i*16+j]=WHITE;
					break;
				default:
					sht->buffer[i*16+j]=sht->col_inv;
			}
}

void put_block_sht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,char *block)
{
	for (int y=0;y<pysize;y++)
		for (int x=0;x<pxsize;x++)
			if (block[y*pxsize+x]>=0)
				sht->buffer[(y0+y)*sht->xsize+x0+x]=block[y*pxsize+x];
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
void init_screen_sht(struct Sheet *sht)
{
	//char *vram=sht->buffer;
	
	
	int x=sht->xsize; 
	int y=sht->ysize;
	
	//画任务框
	boxfill_sht(sht,0,0,x,y-28,LIGHTBLUE);
	boxfill_sht(sht,0,y-28,x,1,LIGHTGRAY);
	boxfill_sht(sht,0,y-27,x,1,LIGHTGRAY);
	boxfill_sht(sht,0,y-26,x,26,WHITE);

	boxfill_sht(sht,3,y-24,57,1,WHITE);
	boxfill_sht(sht,2,y-24,1,21,WHITE);
	boxfill_sht(sht,3,y-4,57,1,DULLGRAY);
	boxfill_sht(sht,59,y-23,1,19,DULLGRAY);
	boxfill_sht(sht,2,y-3,58,1,BLACK);
	boxfill_sht(sht,60,y-24,1,22,BLACK);

	boxfill_sht(sht,x-47,y-24,44,1,DULLGRAY);
	boxfill_sht(sht,x-47,y-23,1,20,DULLGRAY);
	boxfill_sht(sht,x-47,y-3,44,1,WHITE);
	boxfill_sht(sht,x-3,y-24,1,22,WHITE);
	return;
}

void putfont8_srn(struct BootInfo *binfo,int x,int y,char color,char c)
{
	int i,j;
	char tmp[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	char *font=fontlib+c*16;
	for (i=0;i<16;i++)
		for (j=0;j<8;j++)
			if ((font[i]&tmp[j])!=0)
				binfo->vram[(y+i)*binfo->scrnx+x+j]=color;
				
}
void putstr_srn(struct BootInfo *binfo,int x,int y,char color,char *str)
{
	char *t=str;
	while (*t!=0x00)
	{
		putfont8_srn(binfo,x,y,color,*t);
		x+=8;
		t++;
	}
}
void make_window(struct Sheet *sht,int xsize,int ysize,char *title)
{
	//boxfill_sht(sht,0,0,xsize,ysize,LIGHTRED);
	static char closebtn[14][16] = 
		{
		"OOOOOOOOOOOOOOO@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQQQ@@QQQQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"O$$$$$$$$$$$$$$@",
		"@@@@@@@@@@@@@@@@"
		};
	int x, y;
	char c;
	boxfill_sht(sht, 0,         0,         xsize ,		1, 			LIGHTGRAY);
	boxfill_sht(sht, 1,         1,         xsize - 2, 	1, 			WHITE);
	boxfill_sht(sht, 0,         0,         1,     		ysize , 	LIGHTGRAY);
	boxfill_sht(sht, 1,         1,         1,        	ysize - 2, 	WHITE);
	boxfill_sht(sht, xsize - 2, 1,         1,		 	ysize - 2, 	DULLGRAY);
	boxfill_sht(sht, xsize - 1, 0,         1, 			ysize, 		BLACK);
	boxfill_sht(sht, 2,         2,         xsize - 4, 	ysize - 4, 	WHITE);
	boxfill_sht(sht, 3,         3,         xsize - 6, 	18, 		DULLINDIGO);
	boxfill_sht(sht, 1,         ysize - 2, xsize - 2, 	1, 			DULLGRAY);
	boxfill_sht(sht, 0,         ysize - 1, xsize, 		1,	 		BLACK);
	putstr_sht(sht,16, 4, WHITE, title);
	for (y = 0; y < 14; y++) 
	{
		for (x = 0; x < 16; x++) 
		{
			c = closebtn[y][x];
			if (c == '@') 
			{
				c = BLACK;
			} else if (c == '$') 
			{
				c = DULLGRAY;
			} else if (c == 'Q') 
			{
				c = LIGHTGRAY;
			} else 
			{
				c = WHITE;
			}
			sht->buffer[(5 + y) * xsize + (xsize - 21 + x)] = c;
		}
	}
	return;
}
