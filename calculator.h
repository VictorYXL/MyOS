#pragma once
#define NUMCOUNT	40
struct Exp
{
	int data[NUMCOUNT];
	int type[NUMCOUNT];
	int length;
};

extern struct Keyboard keyboard;
extern struct Window window;

int calculator (char *str);
void calculatorTask_Main(struct Task *task); 
void analysize(char *str,struct Exp *exp);
int calSub(struct Exp *exp,int h,int r);

