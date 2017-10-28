#pragma once
#define MAXCOMMANDCOUNT 	6 
#define MAXCOMMANDLENGTH	20
enum CommandType
{
	Calculator,	//计算器 
	Open,		//打开文件 
	Play,		//播放音乐 
	TaskList,	//任务管理器 
	MemoryList,	//内存表 
	Shutdown	//关机 
};
struct Command
{
	int commandType;
	int parCount;
	char par[MAXCOMMANDCOUNT][MAXCOMMANDLENGTH];
};
extern struct Keyboard keyboard;

void analyseCommand(char *commandString,struct Command *command);
void consoleTask_Main(struct Task *task);
