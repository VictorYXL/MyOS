#include"nasmfunc.h"
#include"dsctbl.h" 
void init_gdtidt(void)
{
	struct Segment_Descriptor *gdt=(struct Segment_Descriptor *) 0x00270000;
	struct Gate_Descriptor *idt=(struct Gate_Descriptor *) 0x0026f800;
	int i;
	
	for (i=0;i<8192;i++)
		set_segmdesc(gdt+i,0,0,0);
	set_segmdesc(gdt+1,0xffffffff,0x00000000,0x4092);
	set_segmdesc(gdt+2,0x0007ffff,0x00280000,0x409a);
	load_gdtr(0xffff,0x00270000);
	
	for (i=0;i<256;i++)
		set_gatedesc(idt+i,0,0,0);
	load_idtr(0x7ff,0x0026f800);
	
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
}
