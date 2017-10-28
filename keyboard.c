#include"keyboard.h"
#include"nasmfunc.h"
#include"buffer.h"
struct Keyboard keyboard;
//等待键盘控制电路准备完毕
void wait_KBC_sendready()
{
	//键盘控制电路准备完毕时 PORT_KEYSTA地址的倒数第二位应该是0
	while (1)
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY)==0)
			break;
	return;
}
//初始化键盘控制电路
void init_keyboard()
{
	//等待可以发送信息
	wait_KBC_sendready();
	//发送模式指令
	io_out8(PORT_KEYCMD,KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	//发送指令编号
	io_out8(PORT_KEYDAT,KBC_MODE);
}


