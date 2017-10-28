#include"buffer.h"
#include"mtask.h"

struct BufferAll allbuf;

void buffer_init(struct Buffer *buffer,int bufferSize,unsigned char *tmp,struct Task *task)
{
	buffer->bufferSize=bufferSize;
	buffer->data=tmp;
	buffer->flag=0;
	buffer->free=buffer->bufferSize;
	buffer->h=0;
	buffer->r=0;
	buffer->task=task; 
	return; 
}
int buffer_put(struct Buffer *buffer,unsigned char data)
{
	//写入数据 
	if (buffer->free==0)
	{
		buffer->flag=1;
		return 0;
	}
	buffer->data[buffer->r]=data;
	buffer->r++;
	buffer->r%=buffer->bufferSize;
	buffer->free--;
	
	//唤醒任务 
	if (buffer->task!=0)
		if (buffer->task->flags!=TASK_USING)
			runTask(buffer->task);
	return 1;
}
int buffer_get(struct Buffer *buffer,unsigned char *data)
{
	if (buffer->free==buffer->bufferSize)
		return 0;
	*data=buffer->data[buffer->h];
	buffer->h++;
	buffer->h%=buffer->bufferSize;
	buffer->free++;
	return 1;
}
int buffer_fullCount(struct Buffer *buffer)
{
	return buffer->bufferSize-buffer->free;
}
int buffer_emptyCount(struct Buffer *buffer)
{
	return buffer->free;
}
