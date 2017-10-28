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
void init_palette();
void set_palette();
void boxfill(struct BootInfo *binfo,int x0,int y0,int pxsize,int pysize,unsigned char c);
void init_screen(struct BootInfo *);
void putfont8(struct BootInfo *binfo,char *fontlib,int x,int y,char color,char c);
void put_block(struct BootInfo *binfo,int x0,int y0,int pxsize,int pysize,char *block);
void putstr(struct BootInfo *binfo,int x,int y,char color,char *str);
void init_mouse_cursor(char *mouse);
