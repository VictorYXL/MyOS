#pragma once

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
struct Mouse_Dec
{
	unsigned char buf[3];
	int status;
	int dx,dy;
	int lbtn,rbtn,mbtn;
};

void enable_mouse(struct Mouse_Dec *mdec);
int mouse_decode(struct Mouse_Dec *mdec,unsigned char data);