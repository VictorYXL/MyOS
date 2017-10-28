#include"system.h"
#include"io.h"
#include"graphic.h"
#include"window.h"
#include"buffer.h"
#include"memory.h"
#include"file.h"
#include<stdio.h>
int main(int argc,char **argv)
{
	if (argc!=1)
		return 0;
	char *fileName=argv[0];
	//Open File
	struct File *fp;
	if ((fp=fileOpen(fileName))==0)
		return 0;
	int length=fileLength(fp);
	//File Buffer
	unsigned char *fileBufferC = (unsigned char *)allocMem(length,"File Buffer");
	struct Buffer fileBuffer;
	initBuffer(&fileBuffer,length,fileBufferC);
	readFile(fp,0,length,fileBuffer); 

	//Sheet Buffer
	unsigned char *playerBufferC=(unsigned char *)allocMem(70*28,"Player");
	struct Buffer fileBuffer;
	initBuffer(playerBufferC,70*28,fileBuffer);
	//Alloc Sheet
	struct Sheet *playerSheet;
	playerSheet=allocSheet();
	setBufInSheet(playerSheet,playerBufferC,70,28,-1);
	//Init Sheet
	makeWindow(playerSheet,70,28,"Player");
	slideSheet(playerSheet,180,72);
	updownSheet(playerSheet);
	char s[8],data;
	sprintf (s,"Playing");
	putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
	
	//Init Timer
	struct Timer *timerPlayer;
	char bufferTime;
	timerPlayer=allocTimer();
	initTimer(timerPlayer,&bufferTime,1);
	
	char oneBeat;
	while (1)
	{
		for (int i=0;i<length;i++)
		{
			//Close
			if(isClosedWindow(playerSheet))
			{
				freeSheet(playerSheet);
				freeMem(fileBufferC,length);
				freeMem(playerBufferC,70*28);
				PlayOneBeat(0);
				return;
			}
			//Suspend
			if (iskey(&data) && data==' ')
			{
				sprintf (s,"Suspend");
				putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
				while (!(iskey(&data) && data==' '));
				//COntinue
				sprintf (s,"Playing");
				putStrAndBackOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);
			}
			//OneBeat
			getBuffer(fileBuffer,oneBeat);
			playOneBeat(oneBeat);
			//20ms
			setTimer(timerPlayer,20);
			while (1)
				if (getBuffer(&bufferTime,&data))
					break;
			//Stop
			PlayOneBeat(0);
			//15ms
			setTimer(timerPlayer,15);
			while (1)
				if (getBuffer(&bufferTime,&data))
					break;			
		}
		
	} 
	return 0;
}
