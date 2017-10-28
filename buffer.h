#pragma once
#define BUFFERSIZE 32
struct Buffer
{
	unsigned char data[BUFFERSIZE];
	int h,r,free,flag;
};

void buffer_init(struct Buffer *buffer);
int buffer_put(struct Buffer *buffer,unsigned char data);
int buffer_get(struct Buffer *buffer,unsigned char *data);
