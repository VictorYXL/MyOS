void io_hlt();
void write_mem8(int addr, int data);
void io_cli();
void io_out8(int port,int data);
int io_load_eflags();
void io_store_eflags(int eflags);

void init_palette();
void set_palette();
void boxfill(unsigned char *vram,int xsize,unsigned char c,int x0,int y0,int x1,int y1);
void HariMain()
{

	char *vram;
	/*
	char * -> BYTE
	short * -> WORD
	int * -> DWORD
	*/
	
	//设定调色板
	init_palette();
	
	vram=(char*)0xa0000; 
	/*boxfill(p,320,2,20,30,140,150);
	boxfill(p,320,3,50,60,170,180);
	boxfill(p,320,4,100,110,120,130);*/
	int xsize=320,ysize=200;
	boxfill(vram,xsize,14,0,0,xsize-1,ysize-29);
	boxfill(vram,xsize,8,0,ysize-28,xsize-1,ysize-28);
	boxfill(vram,xsize,7,0,ysize-27,xsize-1,ysize-27);
	boxfill(vram,xsize,8,0,ysize-26,xsize-1,ysize-1);

	boxfill(vram,xsize,7,3,ysize-24,59,ysize-24);
	boxfill(vram,xsize,7,2,ysize-24,2,ysize-4);
	boxfill(vram,xsize,15,3,ysize-4,59,ysize-4);
	boxfill(vram,xsize,15,59,ysize-23,59,ysize-5);
	boxfill(vram,xsize,0,2,ysize-3,59,ysize-3);
	boxfill(vram,xsize,0,60,ysize-24,60,ysize-3);

	boxfill(vram,xsize,15,xsize-47,ysize-24,xsize-4,ysize-24);
	boxfill(vram,xsize,15,xsize-47,ysize-23,xsize-47,ysize-4);
	boxfill(vram,xsize,7,xsize-47,ysize-3,xsize-4,ysize-3);
	boxfill(vram,xsize,7,xsize-3,ysize-24,xsize-3,ysize-3);
	
	while(1)
		io_hlt();
	
}
void boxfill(unsigned char *vram,int xsize,unsigned char c,int x0,int y0,int x1,int y1)
{
	int i,j;
	for (j=y0;j<=y1;j++)
		for (i=x0;i<=x1;i++)
			vram[j*xsize+i]=c;
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