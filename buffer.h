#pragma once
struct Buffer
{
	int bufferSize;
	unsigned char *data;
	int h,r,free,flag;
	struct Task *task;//BufferËùÊôÈÎÎñ 
};

struct BufferAll
{
	struct Buffer key;
	struct Buffer mouse;
};
extern struct BufferAll allbuf;

void initBuffer(struct Buffer *buffer,int bufferSize,unsigned char *tmp,struct Task *task);
int putBuffer(struct Buffer *buffer,unsigned char data);
int getBuffer(struct Buffer *buffer,unsigned char *data);
int fullCountBuffer(struct Buffer *buffer);
int emptyCountBuffer(struct Buffer *buffer);
