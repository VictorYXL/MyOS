#include"buffer.h"
void buffer_init(struct Buffer *buffer,int buffersize,unsigned char *tmp)
{
	buffer->buffersize=buffersize;
	buffer->data=tmp;
	buffer->flag=0;
	buffer->free=buffer->buffersize;
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
	buffer->r%=buffer->buffersize;
	buffer->free--;
	return 1;
}
int buffer_get(struct Buffer *buffer,unsigned char *data)
{
	if (buffer->free==buffer->buffersize)
		return 0;
	*data=buffer->data[buffer->h];
	buffer->h++;
	buffer->h%=buffer->buffersize;
	buffer->free++;
	return 1;
}
int buffer_fullCount(struct Buffer *buffer)
{
	return buffer->buffersize-buffer->free;
}
int buffer_emptyCount(struct Buffer *buffer)
{
	return buffer->free;
}