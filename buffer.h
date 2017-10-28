#pragma once
struct Buffer
{
	int buffersize;
	unsigned char *data;
	int h,r,free,flag;
};

struct BufferAll
{
	struct Buffer key;
	struct Buffer mouse;
};


void buffer_init(struct Buffer *buffer,int buffersize,unsigned char *tmp);
int buffer_put(struct Buffer *buffer,unsigned char data);
int buffer_get(struct Buffer *buffer,unsigned char *data);
int buffer_fullCount(struct Buffer *buffer);
int buffer_emptyCount(struct Buffer *buffer);