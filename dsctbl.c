#include"nasmfunc.h"
#include"dsctbl.h" 
void init_gdtidt(void)
{
	struct Segment_Descriptor *gdt=(struct Segment_Descriptor *) ADR_GDT;
	struct Gate_Descriptor *idt=(struct Gate_Descriptor *) ADR_IDT;
	int i;
	
	for (i=0;i<8192;i++)
		set_segmdesc(gdt+i,0,0,0);
	
	set_segmdesc(gdt+1,0xffffffff,0x00000000,AR_DATA32_RW);
	set_segmdesc(gdt+2,LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
	//将GDTR写入寄存器，他记录了全局段表的基址和界限 
	load_gdtr(LIMIT_GDT, ADR_GDT);
	
	for (i=0;i<256;i++)
		set_gatedesc(idt+i,0,0,0);
	load_idtr(LIMIT_IDT, ADR_IDT);
	
	//21号中断，中断处理程序在第二个段的nsm_inthandler21中
	//第二个段是全局 
	set_gatedesc(idt+0x21,(int)nsm_inthandler21,2<<3,AR_INTGATE32);
	return;
} 
void set_segmdesc(struct Segment_Descriptor *sd,unsigned int limit,int base,int ar)
{
	if (limit > 0xfffff)
	{
		ar|=0x8000;
		limit/=0x1000;
	}
	sd->limit_low=limit & 0xffff;
	sd->base_low=base & 0xffff;
	sd->base_mid=(base >> 16) & 0xff;
	//ar格式为xxxx0000xxxxxxxx，其中高4位写入limit_high中，低8位写入access_right中 
	sd->access_right=ar & 0xff; 
	//limit_high高4位是控制信息，低4位与limit_low组成段上限 
	sd->limit_high=((limit>>16)& 0x0f) |((ar>>8) & 0xf0);
	sd->base_high=(base>>24)& 0xff;
	return;
} 

void set_gatedesc(struct Gate_Descriptor *gd,int offset,int selector,int ar)
{
	gd->offset_low=offset & 0xffff;
	gd->selector=selector;
	gd->dw_count=(ar>>8)&0xff;
	gd->access_right=ar&0xff;
	gd->offset_high=(offset>>16)&0xffff;
	return;
}
