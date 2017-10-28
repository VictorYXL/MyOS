#pragma once

extern struct Keyboard keyboard;
extern struct Window window;

void loadFile (char *fileName,struct Sheet *textViewerSheet,int* curPosX,int* curPosY,char curInput[20][75]);
void textViewerTask_Main(struct Task *task);
