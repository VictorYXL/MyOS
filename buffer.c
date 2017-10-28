#include"buffer.h"
#include <stdlib.h>
#include <malloc.h>
void buffer_init(struct Buffer *buffer)
{
	buffer->flag=0;
	buffer->free=BUFFERSIZE;
	buffer->h=0;
	buffer->r=0;
	return; 
}
int buffer_put(struct Buffer *buffer,unsigned char data)
{
	if (buffer->free==0)
	{
		buffer->flag=1;
		return 0;
	}
	buffer->data[buffer->r]=data;
	buffer->r++;
	buffer->r%=BUFFERSIZE;
	buffer->free--;
	return 1;
}
int buffer_get(struct Buffer *buffer,unsigned char *data)
{
	if (buffer->free==BUFFERSIZE)
		return 0;
	*data=buffer->data[buffer->h];
	buffer->h++;
	buffer->h%=BUFFERSIZE;
	buffer->free++;
	return 1;
}
