#pragma once

extern struct Keyboard keyboard;
extern struct Window window;

void playOneBeat(int eax);
void loadMusicFile(char *fileName,struct Task *task,struct Sheet *musicPlayerSheet);
void playTask_Main(struct Task *task);
