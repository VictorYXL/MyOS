#pragma once
#define BOOTADDR		0x00000ff0
#define MEMORYLISTADDR	0x003c0000
void HariMain();
//void windowTask_Main(struct Task *task);
//void calculatorTask_Main(struct Task *task); 
//extern struct window window; 
extern struct Window window;
extern struct Keyboard keyboard; 
extern struct BufferAll allBuf;
extern struct TimerCTL timerctl;
extern struct Mouse_Dec mdec;
extern struct MemoryList *meml;
extern struct SheetControl *scl;
/*struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];sprintf (s,"12");putStrOnSrn(binfo,0,200,1,s);while (1);*/
/*struct BootInfo *binfo;
struct MemoryList *meml;
struct SheetControl *scl;*/ 
