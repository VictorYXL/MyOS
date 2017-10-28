#pragma once
//全局描述符表 
struct Segment_Descriptor
{
	/*
	base 段基址（考虑兼容性，分成了三段） 
	limit 段上限总20位（limit_low与limit_high低4位），limit单位是页，1页为4KB，共总可表示4KB*1M= 4GB
	段属性12位（access_right与limit_high高4位） 
	*/
	short limit_low,base_low;
	char base_mid,access_right;
	char limit_high,base_high;
};
//中断记录表 
struct Gate_Descriptor
{
	/*
	offset 偏移
	selector 选择符
	发生中断后 处理程序所在位置：选择符确定段，偏移确定具体位置
	*/
	 
	short offset_low,selector;
	char dw_count,access_right;
	short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct Segment_Descriptor *sd,unsigned int limit,int base,int ar);
void set_gatedesc(struct Gate_Descriptor *gd,int offset,int selector,int ar);
