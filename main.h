#pragma once
#define BOOTADDR		0x00000ff0
#define MEMORYLISTADDR	0x003c0000
void HariMain();
void consoleTask_Main(struct SheetControl *scl,struct Sheet *sht_back); 
extern struct BufferAll allbuf;
extern struct TimerCTL timerctl;
extern struct MemoryList *meml;
extern struct SheetControl *scl;
/*struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];
sprintf (s,"12");
putStrOnSrn(binfo,0,200,1,s);while (1);*/
/*struct BootInfo *binfo;
struct MemoryList *meml;
struct SheetControl *scl;*/ 
