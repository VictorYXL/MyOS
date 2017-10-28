#include"memory.h"
#include"nasmfunc.h"
/*
内存的管理使用空闲表法
使用内存前先检查内存 
*/ 
struct MemoryList *meml;
unsigned int memtest(unsigned int start,unsigned int end)
{
	/*
	检查内存的方法：向内存写入值，再读取
	检查前需关闭缓存（CPU486以上才有），因为有缓存的情况下优先从缓存读写
	先判断是否为486以上的CPU
	486以下EFLAGS第18位永远是0，故先写入1在读取看是否为1*/
	
	char flg486=0;
	unsigned int eflg,cr0,i;
	eflg=io_load_eflags();
	eflg|=EFLAGS_AC_BIT;//将第18位写入1
	io_store_eflags(eflg);
	eflg=io_load_eflags();
	flg486=((eflg & EFLAGS_AC_BIT)!=0);//判断是否恢复为0
	eflg&=~EFLAGS_AC_BIT;//恢复EFLAGS
	io_store_eflags(eflg);
	
	if (flg486)
	{
		cr0=load_cr0();
		cr0|=CR0_CACHE_DISABLE;//禁止缓存
		store_cr0(cr0);
	}
	
	i=memtestSub(start,end);//内存检查

	if (flg486)
	{
		cr0=load_cr0();
		cr0&=~CR0_CACHE_DISABLE;//允许缓存
		store_cr0(cr0);
	}
	
	return i;
		
}
unsigned int memtestSub(unsigned int start,unsigned int end)
{
	unsigned int i,*p,old,pat0=0xaa55aa55,pat1=0x55aa55aa;
	for (i=start;i<=end;i+=0x1000)
	{
		p=(unsigned int *)i+0xffc;//只用检查最后的4个字节即可
		//对每个单元格的测试方法：写入->反转->判断->再反转->判断->恢复
		old=*p;
		*p=pat0;//写入 
		*p^=0xffffffff;//反转
		if (*p!=pat1)//判断
			return i;
			
		*p^=0xffffffff;//再反转
		if (*p!=pat0)
			return i;
		*p=old;
	}
	return i;
}

void initMem()
{
	meml->freesize=0;
	meml->maxfreesize=0;
}
unsigned int freeTotalMem()
{
	int i,t=0;
	for (i=0;i<meml->maxfreesize;i++) 
		t+=meml->free[i].size;
	return t;
}
//分配空间 
unsigned int allocMem(unsigned int size)
{
	
	int i,a,j;
	for (i=0;i<meml->freesize;i++)
	{
		if (meml->free[i].size>=size)
		{
			a=meml->free[i].addr;
			meml->free[i].size-=size;
			meml->free[i].addr+=size;
			//删除表项 
			if (meml->free[i].size==0)
			{
				for (j=i;j<meml->freesize;j++)
					meml->free[j]=meml->free[j+1];
			}
			return a;
		}
	}
}
int freeMem(unsigned int addr,unsigned int size)
{
	int i,j;
	//获取待插入位置 
	for (i=0;i<meml->freesize;i++)
		if (meml->free[i].addr>addr)
			break;
	//与前表项可以合并 
	if (i>0 && meml->free[i-1].addr+meml->free[i-1].size==addr)
	{
		meml->free[i-1].size+=size;
		//与后表项也可以合并 
		if (i<meml->freesize && meml->free[i].addr==addr+size)
		{
			meml->free[i-1].size+=meml->free[i].size;
			//删除后表项
			meml->freesize--;
			for (j=i;j<meml->maxfreesize;j++) 
				meml->free[j]=meml->free[j+1];
		}
		return 0;
	}
	//仅与后表项可以合并 
	if (i<meml->freesize && meml->free[i].addr==addr+size)
	{
		meml->free[i].addr=addr; 
		meml->free[i].size=addr;
		return 0;
	}
	//不可合并
	if (meml->freesize>=MEMORYLISTFREES-1)//超过表项的最大值 
		return -1;
	for (j=i;j<meml->freesize;j++) 
		meml->free[j+1]=meml->free[j];
	meml->free[i].addr=addr;
	meml->free[i].size=size;
	if (meml->free[i].size>meml->maxfreesize)
		meml->maxfreesize=meml->free[i].size;
	meml->freesize++; 
	return 0;
}
//一次申请4KB
unsigned int allocMem_4k(unsigned int size)
{
	unsigned int a;
	size=(size+0xfff)&0xfffff000;
	a=allocMem(size);
	return a;
}
//一次释放4KB
int freeMem_4k(unsigned int addr,unsigned int size)
{
	unsigned int a;
	size=(size+0xfff)&0xfffff000;
	a=freeMem(addr,size);
	return a;
}
