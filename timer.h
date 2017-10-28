#pragma once
#define PIT_CTRL		0x0043 
#define PIT_CNT0		0x0040
#define TIMER_MAX		500
#define TIMER_UNALLOCED	0
#define TIMER_ALLOCED	1
#define TIMER_USING		2 
#define TIME_MAX		0xffffffff
struct Timer
{
	int timeout;
	int flag;
	struct Buffer *timeoutBuffer; 
	char timeoutData;

};
struct TimerCTL
{
	unsigned int count,next;//next为下一次超时时间 
	struct Timer timer[TIMER_MAX];
}; 
extern struct TimerCTL timerctl;
void initPit();
struct Timer *allocTimer();
void freeTimer(struct Timer *timer);
void initTimer(struct Timer *timer,struct Buffer *buffer,unsigned char timeoutData);
void setTimer(struct Timer *timer,unsigned int timeout);
