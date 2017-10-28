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

void buffer_init(struct Buffer *buffer,int bufferSize,unsigned char *tmp,struct Task *task);
int buffer_put(struct Buffer *buffer,unsigned char data);
int buffer_get(struct Buffer *buffer,unsigned char *data);
int buffer_fullCount(struct Buffer *buffer);
int buffer_emptyCount(struct Buffer *buffer);
