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
/*extern struct SheetControl *scl;
extern struct BootInfo *binfo;
extern struct MemoryList *meml;*/
struct SheetControl* SCL_init(struct MemoryList *meml,struct BootInfo *binfo);
struct Sheet *sheet_alloc(struct SheetControl *scl);
void sheet_setbuf(struct Sheet *sht,unsigned char *buf,int xsize,int ysize,int col_inv);
void sheet_updown(struct SheetControl *scl,struct Sheet *sht,int height);
void sheet_slide(struct SheetControl *scl,struct Sheet* sht,int vx0,int vy0);
void sheet_free(struct SheetControl *scl,struct Sheet* sht);
void sheet_refreshSub(struct SheetControl *scl,int x0,int y0,int pxsize,int pysize,int h0);
void sheet_refreshAll(struct SheetControl *scl);
void sheet_refreshSheet(struct SheetControl *scl,struct Sheet *sht);
void sheet_refreshSheetSub(struct SheetControl *scl,struct Sheet *sht,int x0,int y0,int xsize,int ysize);
