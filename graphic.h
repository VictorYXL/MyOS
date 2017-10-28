#pragma once
#define BLACK 0
#define LIGHTRED 1
#define LIGHTGREEN 2
#define LIGHTYELLOW 3
#define LIGHTBLUE 4
#define LIGHTPURPLE 5
#define LIGHTGRAYBLUE 6
#define WHITE 7
#define LIGHTGRAY 8
#define DULLRED 9
#define DULLGREEN 10
#define DULLYELLOW 11
#define DULLINDIGO 12
#define DULLPURPLE 13
#define DULLGRAYBLUE 14
#define DULLGRAY 15 

extern struct BootInfo *binfo;
extern char fontlib[4096];
extern struct SheetControl *scl;
void initPalette();
void setPalette();
void boxfillOnSht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,unsigned char c);
void initScreenOnSht(struct Sheet *sht);
void putfont8Onsht(struct Sheet *sht,int x,int y,char color,char c);
void putBlockOnSht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,char *block);
void putStrOnSht(struct Sheet *sht,int x,int y,char color,char *str);
void initMouseCursor(struct Sheet *sht);
void putfont8OnSrn(struct BootInfo *binfo,int x,int y,char color,char c);
void putStrOnSrn(struct BootInfo *binof,int x,int y,char color,char *str);
void putStrAndBackOnSht(struct Sheet *sht,int x,int y,int color,int backcolor,char *str,int length);
void makeWindow(struct Sheet *sht,int xsize,int ysize,char *title);
void makeTextBox(struct Sheet *sht, int x0, int y0, int sx, int sy, int c);
