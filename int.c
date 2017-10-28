#include"nasmfunc.h"
#include"buffer.h"
#include"int.h"
#include<stdio.h>
/*
PIC是可编程中断控制器 有两个PIC（主，从）每个PIC有8路IRQ（中断请求） 
IMR 中断屏蔽器，8位对应8个IRQ信号 是PIC的寄存器 
io_out8向对应端口写数据 
*/
void init_pic(void)
{
	//禁止所有中断 
	io_out8(PIC0_IMR,0xff);	
	io_out8(PIC1_IMR,0xff);
	
	//设置主PIC 
	//ICW有4个2字节 ICW1与ICW4 为主板特性，固定值
	//ICW2决定IRQ用哪一种中断通知CPU 
	//ICW3是主从PIC连接设定 0x00000100  从PIC连接到主PIC的IRQ2上 固定值 
	io_out8(PIC0_ICW1,0x11); //边沿触发模式
	io_out8(PIC0_ICW2,0x20); //IRQ0-7由INT20-27接收
	io_out8(PIC0_ICW3,1<<2); //从PIC由IRQ2连接 
	io_out8(PIC0_ICW4,0x01); //无缓冲区模式
	
	//设置从PIC 
	io_out8(PIC1_ICW1,0x11); //边沿触发模式
	io_out8(PIC1_ICW2,0x28); //IRQ8-15由INT28-2f接收
	io_out8(PIC1_ICW3,2); 	 //从PIC由IRQ2连接 
	io_out8(PIC1_ICW4,0x01); //无缓冲区模式
	
	io_out8(PIC0_IMR,0xfb);	//11111011 PIC1以外 全部禁止 
	io_out8(PIC1_IMR,0xff);	//11111111 禁止所有中断

	return; 
	 
}
//键盘中断处理程序 
void inthandler21(int *esp,struct Buffer *keybuf)
{
	//通知PIC中断已经接受 
	//键盘中断是IRQ1，向PIC0发送0x(1+60)即可
	io_out8(PIC0_OCW2,0x61);
	unsigned char data=io_in8(PORT_KEYDAT);
	buffer_put(keybuf,data);
	return;
}
//不明白是干啥的
void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67);
	return;
}

//鼠标中断处理程序 
void inthandler2c(int *esp,struct Buffer *mousebuf)
{
	//鼠标中断是IRQ12，向IRQ0发送2，也要向IRQ1发送4（从PIC的第5个中断）
	io_out8(PIC1_OCW2,0x64);
	io_out8(PIC0_OCW2,0x62);
	unsigned char data=io_in8(PORT_KEYDAT);
	buffer_put(mousebuf,data);
	return;
} 
