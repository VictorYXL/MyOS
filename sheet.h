
#pragma once
#define MAXSHEET 256 
#define SHEETFREE 0
#define SHEETUSE 1

struct Sheet
{
	unsigned char *buffer;//像素
	int x0,y0,xsize,ysize;//起始坐标，长宽
	int col_inv,height,flags; //透明色色号、图层高度、其他信息 
};
struct SheetControl
{
	unsigned char *vram;
	int xsize,ysize,top;//top表示最上层图层的高度 
	struct Sheet *sheetp[MAXSHEET];//图层的地址，方便进行排序 
	struct Sheet sheet[MAXSHEET];//具体图层 
};
extern struct SheetControl *scl;
/*extern struct BootInfo *binfo;
extern struct MemoryList *meml;*/
struct SheetControl* initSCL(struct BootInfo *binfo);
struct Sheet *allocSheet();
void setBufInSheet(struct Sheet *sht,unsigned char *buf,int xsize,int ysize,int col_inv);
void updownSheet(struct Sheet *sht,int height);
void slideSheet(struct Sheet* sht,int vx0,int vy0);
void freeSheet(struct Sheet* sht);
void refreshSubSheet(int x0,int y0,int pxsize,int pysize,int h0);
void refreshAllSheet();
void refreshSheet(struct Sheet *sht);
void refreshSubInSheet(struct Sheet *sht,int x0,int y0,int xsize,int ysize);
