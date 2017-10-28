#pragma once

extern struct Keyboard keyboard;
extern struct Window window;

void memoryListTask_Main(struct Task *task); 
void showMemory(struct Sheet *memoryListSheet);
