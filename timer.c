#include"timer.h"
#include"nasmfunc.h"
#include"buffer.h"
/*
设置定时中断频率为100Hz（设定值为11932,0x2e9c）:
向0x43写入0x34
向0x40写入设定值的低8位 
向0x40写入设定值的高8位
*/
struct TimerCTL timerctl;
struct Timer *taskTimer;
void initPit()
{
	io_out8(PIT_CTRL,0x34);
	io_out8(PIT_CNT0,0x9c);
	io_out8(PIT_CNT0,0x2e);
	timerctl.count=0;
	timerctl.next=TIME_MAX;
	int i;
	for (i=0;i<TIMER_MAX;i++)
		timerctl.timer[i].flag=TIMER_UNALLOCED;
	return;
}
struct Timer *allocTimer()
{
	int i;
	for (i=0;i<TIMER_MAX;i++)
		if (timerctl.timer[i].flag==TIMER_UNALLOCED)
		{
			timerctl.timer[i].flag=TIMER_ALLOCED;
			return &timerctl.timer[i];
		}
	return 0;
}
void freeTimer(struct Timer *timer)
{
	timer->flag=TIMER_UNALLOCED;
	return;
}
void initTimer(struct Timer *timer,struct Buffer *buffer,unsigned char timeoutData)
{
	timer->timeoutBuffer=buffer;
	timer->timeoutData=timeoutData;
}
void setTimer(struct Timer *timer,unsigned int timeout)
{
	timer->timeout=timeout+timerctl.count;
	timer->flag=TIMER_USING;
	if (timer->timeout<timerctl.next)
		timerctl.next=timer->timeout;
	return;
}
