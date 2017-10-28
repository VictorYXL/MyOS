#pragma once
#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000
#define MEMORYLISTFREES		4096
#define MEMORYLISTADDR		0x003c0000
struct FreeInfo
{
	unsigned int addr,size;
};
struct MemoryList
{
	int freesize,maxfreesize;
	struct FreeInfo free[MEMORYLISTFREES];
};
extern struct MemoryList *meml;
unsigned int memtest(unsigned int start,unsigned int end);
unsigned int memtest_sub(unsigned int start,unsigned int end);
void mem_init();
unsigned int mem_freetotal();
unsigned int mem_alloc(unsigned int size);
int mem_free(unsigned int addr,unsigned int size);
unsigned int mem_alloc_4k(unsigned int size);
int mem_free_4k(unsigned int addr,unsigned int size);