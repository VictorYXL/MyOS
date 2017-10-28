#pragma once
#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000
#define MEMORYLISTFREES		128
struct FreeInfo
{
	unsigned int addr,size;
};
struct UsedInfo
{
	unsigned int addr,size,flag;
	char status[30];
};
struct MemoryList
{
	int maxsize;
	int freesize,maxfreesize;
	struct FreeInfo free[MEMORYLISTFREES];
	struct UsedInfo used[MEMORYLISTFREES];
};
extern struct MemoryList *meml;
unsigned int memtest(unsigned int start,unsigned int end);
unsigned int memtestSub(unsigned int start,unsigned int end);
void initMem();
unsigned int freeTotalMem();
unsigned int allocMem(unsigned int size,char *status);
int freeMem(unsigned int addr,unsigned int size);
unsigned int allocMem_4k(unsigned int size,char *status);
int freeMem_4k(unsigned int addr,unsigned int size);
