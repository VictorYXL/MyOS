#pragma once

extern struct Keyboard keyboard;
extern struct Window window;

void loadFile (char *fileName,struct Sheet *textViewerSheet,int* curPosX,int* curPosY,char curInput[21][75],int page);
void textViewerTask_Main(struct Task *task);
