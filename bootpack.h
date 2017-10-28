#pragma once
#define BOOTADDR		0x00000ff0
#define MEMORYLISTADDR	0x003c0000
void HariMain();
struct BufferAll allbuf;
struct TimerCTL timerctl;
/*struct BootInfo *binfo;
struct MemoryList *meml;
struct SheetControl *scl;*/ 
