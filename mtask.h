#pragma once
#define AR_TSS32	0x0089
#define MAX_TASKS	1000
#define TASK_GDT0	3
#define TASK_UNALLOCED	0
#define TASK_ALLOCED	1
#define TASK_USING		2 
#define MAXWINDOWCOUNT	200
#define MAXWINDOWNAME	50
struct TSS
{
	int backlink,esp0,ss0,esp1,ss1,esp2,ss2,cr3;	//任务切换中的信息 ，任务切换时不被CPU写入 
	int eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi;	//EIP 跳转到任务时起始地址 （段内偏移） esp为任务的栈 
	int es,cs,ss,ds,fs,gs;
	int ldtr,iomap;									//任务切换中的信息 ，任务切换时不被CPU写入
};
struct Task
{
	int sel,flags;
	struct TSS tss;	
	struct BufferAll bufAll;
	int winID;
	int parCount;
	char par[5][30];
};
struct TaskCTL
{
	int runningCount;
	int now;
	struct Task *tasks[MAX_TASKS];	//动态指向任务 
	struct Task tasks0[MAX_TASKS];	//存储任务 
};
struct Window
{
	int focus,winCount,x;
	char winName[MAXWINDOWCOUNT][MAXWINDOWNAME];
	int isChanged;
};
extern struct TaskCTL *taskctl;
extern struct Timer *taskTimer;
extern struct Window window;
struct Task *getMainTask();
struct Task *allocTask();
void initTaskCTL();
void initTask(struct Task *task,int eip,char *str,int f);//,unsigned char *keyb,unsigned char *mouseb);
void runTask(struct Task *task);
struct Task *getMainTask(); 
void switchTask();
void deleteTask(struct Task *task);
void initWindow();
void createWindow(struct Task *task,char *name);
void deleteWindow(struct Task *task);
