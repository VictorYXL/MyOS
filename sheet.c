#include"memory.h"
#include"nasmfunc.h" 
#include"sheet.h"
#include<stdio.h>
struct SheetControl *scl;
struct SheetControl* initSCL(struct BootInfo *binfo)
{
	int i;
	scl=(struct SheetControl *)allocMem(sizeof(struct SheetControl),"Sheet Control");
	scl->vram=binfo->vram;
	scl->xsize=binfo->scrnx;
	scl->ysize=binfo->scrny;
	scl->top=-1;//没有图层 
	for (i=0;i<MAXSHEET;i++)
		scl->sheet[i].flags=SHEETFREE;//标记为未使用
	return scl; 
}
struct Sheet *allocSheet()
{
	struct Sheet *sht;
	int i;
	//分配空闲的图层 
	for (i=0;i<MAXSHEET;i++)
		if (scl->sheet[i].flags==SHEETFREE)
		{
			sht=&scl->sheet[i];
			sht->flags=SHEETUSE;
			sht->height=-1;//-1表示不显示 
			return sht;
		}
	return 0;
}
//设置图层 
void setBufInSheet(struct Sheet *sht,unsigned char *buf,int xsize,int ysize,int col_inv)
{
	sht->buffer=buf;
	sht->xsize=xsize; 
	sht->ysize=ysize;
	sht->col_inv=col_inv;
}

//设置图层高度
void setHeightSheet(struct Sheet *sht,int height) 
{
	sht->height=height;
	if (scl->top<height)
		scl->top=height;
	if (height>=0)
		scl->sheetp[height]=sht;
	//refreshAllSheet();
	refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,0);
}

//滑动图层 （修改高度） 
void updownSheet(struct Sheet *sht,int height)
{
	int old=sht->height,i;
	//修正错误数值 
	if (height>=scl->top+1)
		height=scl->top+1;
	else if (height<-1)
		height=-1;
	//修改某一图层高度有4种情况 
	sht->height=height;
	if (old>height && height>=0)
	{
		//降低图层且仍然显示 ,将中间图层向上提，并修改图层表中高度 
		for (i=old;i>height;i--)
		{
			scl->sheetp[i]=scl->sheetp[i-1];
			scl->sheetp[i]->height=i;
		}
		scl->sheetp[height]=sht;
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,height+1);
	}else if (old>height && height<0)
	{
		//隐藏图层,将上面图层向下拉,并修改图层表中高度 
		for (i=old;i<scl->top;i++)
		{ 
			scl->sheetp[i]=scl->sheetp[i+1];
			scl->sheetp[i]->height=i;
		}
		scl->top--;
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,0);
	}else if (old <height && old>=0)
	{
		//提高图层且原图层是显示状态 ,将中间图层向下拉，并修改图层表中高度 
		for (i=old;i<height;i++)
		{
			scl->sheetp[i]=scl->sheetp[i+1];
			scl->sheetp[i]->height=i;
		}
		scl->sheetp[height]=sht;
	}else if (old<height && old<0)
	{
		//显示隐藏的图层 ,将上面图层向上提，并修改图层表中高度 
		for (i=scl->top+1;i>height;i--)
		{
			scl->sheetp[i]=scl->sheetp[i-1];
			scl->sheetp[i]->height=i;
			
		}
		scl->sheetp[height]=sht;
		scl->top++;
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,height);
	}
}
//移动图层 
void slideSheet(struct Sheet* sht,int vx0,int vy0)
{
	int old_x0=sht->x0,old_y0=sht->y0;
	sht->x0=vx0;
	sht->y0=vy0;
	if (sht->height>=0)//正在显示 
	{
		//不考虑优化直接刷新整个屏幕
		//refreshAllSheet(scl);
		//考虑优化 ，只刷新有变动的区域（移动前位置与移动后位置） 
		//原位置需要全部重新绘制 
		refreshSubSheet(old_x0,old_y0,sht->xsize,sht->ysize,0);
		//新位置只用从目标图层开始绘制 
		refreshSubSheet(vx0,vy0,sht->xsize,sht->ysize,sht->height);
	}
	return;
}
//释放图层 
void freeSheet(struct Sheet* sht)
{
	//隐藏该图层并置未使用标志
	if (sht->height>=0) 
		updownSheet(sht,-1);
	//setHeightSheet(sht,-1);
	sht->flags=0;
	return;
}
//刷新一部分屏幕 (x0,y0)是相对屏幕坐标 
void refreshSubSheet(int x0,int y0,int pxsize,int pysize,int h0)
{
	int h,vx,vy,sx,sy;//(vx,vy) 表示整个画面中的坐标 (sx,sy)表示图层中的坐标
	int vx0,vx1,vy0,vy1;
	unsigned char *buf,c,*vram=scl->vram;
	struct Sheet *sht;
	if (x0<0) 
		x0=0;
	if (x0+pxsize>scl->xsize)
		pxsize=scl->xsize-x0;
	if (y0<0) 
		y0=0;
	if (y0+pysize>scl->ysize)
		pysize=scl->ysize-y0;
	//从低到高绘制图层 
	for (h=h0;h<=scl->top;h++)
	{
		sht=scl->sheetp[h];
		buf=sht->buffer;
		/*
		vx0~vx1,vy0~vy1	该图层与绘制区域的交集  
		*/
		vy0=y0>sht->y0?y0:sht->y0;
		vy1=y0+pysize<sht->y0+sht->ysize?y0+pysize:sht->y0+sht->ysize;
		vx0=x0>sht->x0?x0:sht->x0;
		vx1=x0+pxsize<sht->x0+sht->xsize?x0+pxsize:sht->x0+sht->xsize;
		for (vy=vy0;vy<vy1;vy++)
			for (vx=vx0;vx<vx1;vx++)
			{
				sy=vy-sht->y0;
				sx=vx-sht->x0;
				c=buf[sy*sht->xsize+sx];
				//该像素不是透明 
				if (c!=sht->col_inv)
					vram[vy*scl->xsize+vx]=c;
			}
	}
	
	//鼠标图层 
	sht=&scl->sheet[1];
	buf=sht->buffer;
	vy0=y0>sht->y0?y0:sht->y0;
	vy1=y0+pysize<sht->y0+sht->ysize?y0+pysize:sht->y0+sht->ysize;
	vx0=x0>sht->x0?x0:sht->x0;
	vx1=x0+pxsize<sht->x0+sht->xsize?x0+pxsize:sht->x0+sht->xsize;
	for (vy=vy0;vy<vy1;vy++)
		for (vx=vx0;vx<vx1;vx++)
		{
			sy=vy-sht->y0;
			sx=vx-sht->x0;
			c=buf[sy*sht->xsize+sx];
			//该像素不是透明 
			if (c!=sht->col_inv)
				vram[vy*scl->xsize+vx]=c;
		}
}
//绘刷新整个屏幕 
void refreshAllSheet()
{
	int h,sx,sy,vx,vy;//(sx,sy)表示单个图层中的坐标 (vx,vy) 表示整个画面中的坐标 
	unsigned char *buf,c,*vram=scl->vram;
	struct Sheet *sht;
	
	//从低到高绘制图层 
	for (h=0;h<=scl->top;h++)
	{
		sht=scl->sheetp[h];
		buf=sht->buffer;
		for (sy=0;sy<sht->ysize;sy++)
		{
			vy=sht->y0+sy;
			for (sx=0;sx<sht->xsize;sx++)
			{
				vx=sht->x0+sx;
				c=buf[sy*sht->xsize+sx];
				//该像素不是透明 
				if (c!=sht->col_inv)
					vram[vy*scl->xsize+vx]=c;
			}
		}
	}
	
	//鼠标图层 
	sht=&scl->sheet[1];
	buf=sht->buffer;
	for (sy=0;sy<sht->ysize;sy++)
	{
		vy=sht->y0+sy;
		for (sx=0;sx<sht->xsize;sx++)
		{
			vx=sht->x0+sx;
			c=buf[sy*sht->xsize+sx];
			//该像素不是透明 
			if (c!=sht->col_inv)
				vram[vy*scl->xsize+vx]=c;
		}
	}
}
//刷新一个图层
void refreshSheet(struct Sheet *sht) 
{
	//如果图层在显示，则刷新图层所在区域 
	if (sht->height>=0)
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,sht->height);
}
//刷新一个图层的一部分（相对图层的位置） 
void refreshSubInSheet(struct Sheet *sht,int x0,int y0,int xsize,int ysize) 
{
	//如果图层在显示，则刷新图层所在区域的一部分 
	if (sht->height>=0)
		refreshSubSheet(sht->x0+x0,sht->y0+y0,xsize,ysize,sht->height);	
}
