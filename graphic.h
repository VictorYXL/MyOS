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
void init_palette();
void set_palette();
void boxfill_sht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,unsigned char c);
void init_screen_sht(struct Sheet *sht);
void putfont8_sht(struct Sheet *sht,int x,int y,char color,char c);
void put_block_sht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,char *block);
void putstr_sht(struct Sheet *sht,int x,int y,char color,char *str);
void init_mouse_cursor(struct Sheet *sht);
void putfont8_srn(struct BootInfo *binfo,int x,int y,char color,char c);
void putstr_srn(struct BootInfo *binof,int x,int y,char color,char *str);
