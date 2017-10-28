#pragma once
//easyOS.nsm
struct BootInfo
{
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;
	char *vram;
};

//nasmfunc.nsm
void io_hlt();
void write_mem8(int addr, int data);
void io_cli();
void io_out8(int port,int data);
int io_load_eflags();
void io_store_eflags(int eflags);

