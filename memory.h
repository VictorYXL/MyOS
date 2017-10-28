#pragma once
#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000
#define MEMORYLISTFREES		4096
struct FreeInfo
{
	unsigned int addr,size;
};
struct MemoryList
{
	int freesize,maxfreesize;
	struct FreeInfo free[MEMORYLISTFREES];
};
//extern struct MemoryList *meml;
unsigned int memtest(unsigned int start,unsigned int end);
unsigned int memtest_sub(unsigned int start,unsigned int end);
void mem_init(struct MemoryList *meml);
unsigned int mem_freetotal(struct MemoryList *meml);
unsigned int mem_alloc(struct MemoryList *meml,unsigned int size);
int mem_free(struct MemoryList *meml,unsigned int addr,unsigned int size);
unsigned int mem_alloc_4k(struct MemoryList *meml,unsigned int size);
int mem_free_4k(struct MemoryList *meml,unsigned int addr,unsigned int size);
