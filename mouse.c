#include"keyboard.h"
#include"mouse.h"
#include"nasmfunc.h"
#include"buffer.h"
struct Mouse_Dec mdec;
//¼¤»îÊó±ê
void enable_mouse(struct Mouse_Dec *mdec)
{
	wait_KBC_sendready();
	//·¢ËÍÊó±ê¼¤»îµÄÃüÁî
	io_out8(PORT_KEYCMD,KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT,MOUSECMD_ENABLE);
	mdec->status=0;
	return;
}

int mouse_decode(struct Mouse_Dec *mdec,unsigned char data)
{
	if (mdec->status==0)
	{
		if (data==0xfa)
		{
			mdec->status++;
			return 0;
		}
	}
	else 
	{
		mdec->buf[mdec->status-1]=data;
		mdec->status=mdec->status%3+1;
	}
	if (mdec->status==1)
	{
		mdec->lbtn=((mdec->buf[0]&0x01)!=0);
		mdec->rbtn=((mdec->buf[0]&0x02)!=0);
		mdec->mbtn=((mdec->buf[0]&0x04)!=0);
		if ((mdec->buf[0]&0x10)!=0)
			mdec->dx=mdec->buf[1]|0xffffff00;
		else mdec->dx=mdec->buf[1];
		if ((mdec->buf[0]&0x20)!=0)
			mdec->dy=-(mdec->buf[2]|0xffffff00);
		else mdec->dy=-mdec->buf[2];		
	}
	return mdec->status==1;
}
