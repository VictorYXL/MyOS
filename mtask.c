#include"timer.h"
#include"nasmfunc.h" 
#include"mtask.h"
#include"dsctbl.h"
#include"memory.h"
#include"mtask.h"
struct TaskCTL *taskctl;
void initTask()
{
	int i=0;
	struct Segment_Descriptor *gdt=(struct Segment_Descriptor *)ADR_GDT;
	taskctl=(struct TaskCTL *)mem_alloc_4k(sizeof (struct TaskCTL));
	for (i=0;i<MAX_TASKS;i++) 
	{
		taskctl->tasks0[i].flags=TASK_UNALLOCED;
		taskctl->tasks0[i].sel=(TASK_GDT0+i)*8;
		set_segmdesc(gdt+TASK_GDT0+i,103,(int)&taskctl->tasks0[i].tss,AR_TSS32);	//设定段 
	}
	return;
}
struct Task *getMainTask()
{
	//初始化主任务 
	struct Task *task;
	task=allocTask();
	task->flags=TASK_USING;
	taskctl->runningCount=1;
	taskctl->tasks[0]=task;
	taskctl->now=0;
	load_tr(task->sel);
	taskTimer=allocTimer();
	setTimer(taskTimer,2);
	//返回
	return task;
}
//分配任务 
struct Task *allocTask()
{
	int i;
	struct Task *task;
	for (i=0;i<MAX_TASKS;i++)
	{
		if (taskctl->tasks0[i].flags==TASK_UNALLOCED)//找到第一个可以分配的任务 
		{
			//设定任务的属性 
			task=&taskctl->tasks0[i];
			task->flags=TASK_ALLOCED;
			task->tss.eflags=0x00000202; //IF=1
			task->tss.eax=0;
			task->tss.ecx=0;
			task->tss.edx=0;
			task->tss.ebx=0;
			task->tss.ebp=0;
			task->tss.esi=0;
			task->tss.edi=0;
			task->tss.es=0;
			task->tss.ds=0;
			task->tss.fs=0;
			task->tss.gs=0;
			task->tss.ldtr=0;
			task->tss.iomap=0x40000000;
			return task;
		}
	}
	return 0;
}
void runTask(struct Task *task)
{
	task->flags=TASK_USING;
	taskctl->tasks[taskctl->runningCount]=task;//将task添加到运行队列中，运行 
	taskctl->runningCount++;
}

void switchTask()
{
	setTimer(taskTimer,2);
	if (taskctl->runningCount>=2)
	{
		taskctl->now++;
		if (taskctl->runningCount==taskctl->now)
			taskctl->now=0;
		farjmp(0,taskctl->tasks[taskctl->now]->sel);
	}
}
void sleepTask(struct Task *task)
{
	int i;
	char ts=0;
	if (task->flags==2)
	{
		//如果需要休眠正在运行的任务 ，则需要进行任务切换 
		if (task==taskctl->tasks[taskctl->now])
			ts=1;
			
		//寻找需要休眠的任务 
		for(i=0;i<taskctl->runningCount;i++) 
			if (task==taskctl->tasks[i])
				break;
				
		//修改正在运行的编号
		if (taskctl->now>i) 
			taskctl->now--;
		//删除需要休眠的任务 
		taskctl->runningCount--;
		for (;i<taskctl->runningCount;i++)
			taskctl->tasks[i]=taskctl->tasks[i+1];
		task->flags=TASK_ALLOCED;
		
		//任务切换 
		if (ts!=0)
		{  
			if (taskctl->now>=taskctl->runningCount)
				taskctl->now=0;
			farjmp(0,taskctl->tasks[taskctl->now]->sel);
		}
	}
}
