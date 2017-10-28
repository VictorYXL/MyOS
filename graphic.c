#include"nasmfunc.h"
#include"graphic.h"
void boxfill(struct BootInfo *binfo,unsigned char c,int x0,int y0,int x1,int y1)
{
	int i,j;
	for (j=y0;j<=y1;j++)
		for (i=x0;i<=x1;i++)
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
	//Êó±ê×ø±êÍ¼
	static char cursor[16][16] = {
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
	for(int i=0;i<16;i++)
		for (int j=0;j<16;j++)
			switch(cursor[i][j])
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

void put_block(struct BootInfo *binfo,int pxsize,int pysize,int x0,int y0,char *block)
{
	for (int y=0;y<pysize;y++)
		for (int x=0;x<pxsize;x++)
			if (block[y*pxsize+x]>0)
				binfo->vram[(y0+y)*binfo->scrnx+x0+x]=block[y*pxsize+x];
}

void init_palette()
{	
	static unsigned char table_rgb[16][3] = 
	{
		{0x00, 0x00, 0x00},	//ºÚ
		{0xff, 0x00, 0x00},	//ÁÁºì
		{0x00, 0xff, 0x00},	//ÁÁÂÌ
		{0xff, 0xff, 0x00},	//ÁÁ»Æ
		{0x00, 0x00, 0xff},	//ÁÁÀ¶
		{0xff, 0x00, 0xff},	//ÁÁ×Ï
		{0x00, 0xff, 0xff},	//Ç³ÁÁÀ¶
		{0xff, 0xff, 0xff},	//°×
		{0xc6, 0xc6, 0xc6},	//ÁÁ»Ò
		{0x84, 0x00, 0x00},	//°µºì
		{0x00, 0x84, 0x00},	//°µÂÌ
		{0x84, 0x84, 0x00},	//°µ»Æ
		{0x00, 0x00, 0x84},	//°µÇà
		{0x84, 0x00, 0x84},	//°µ×Ï
		{0x00, 0x84, 0x84},	//Ç³°µÀ¶
		{0x84, 0x84, 0x84}	//°µ»Ò
	};
	set_palette(0, 15, table_rgb);
	return;
}

void set_palette(int start,int end,unsigned char rgb[16][3])
{
	int i,eflags;
	eflags=io_load_eflags();
	io_cli();
	io_out8(0x03c8,start);//´Ó0ºÅµ÷É«°å¿ªÊ¼Éè¶¨
	
	for (i=start;i<=end;i++)
	{
		//°´RGBÐ´Èë£¬µ÷É«°åºÅÂë×Ô¶¯ºóÒÆ
		io_out8(0x03c9,rgb[i][0]/4);//³ý4ÊÇÎªÁË¸Ä±äÉîÇ³
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
	
	//»­ÈÎÎñ¿ò
	boxfill(binfo,4,0,0,x-1,y-29);
	boxfill(binfo,8,0,y-28,x-1,y-28);
	boxfill(binfo,8,0,y-27,x-1,y-27);
	boxfill(binfo,7,0,y-26,x-1,y-1);

	boxfill(binfo,7,3,y-24,59,y-24);
	boxfill(binfo,7,2,y-24,2,y-4);
	boxfill(binfo,15,3,y-4,59,y-4);
	boxfill(binfo,15,59,y-23,59,y-5);
	boxfill(binfo,0,2,y-3,59,y-3);
	boxfill(binfo,0,60,y-24,60,y-3);

	boxfill(binfo,15,x-47,y-24,x-4,y-24);
	boxfill(binfo,15,x-47,y-23,x-47,y-4);
	boxfill(binfo,7,x-47,y-3,x-4,y-3);
	boxfill(binfo,7,x-3,y-24,x-3,y-3);
	return;
}
