// fifo.cpp : Defines the entry point for the console application.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "fifo.h"

#ifndef SEND_BUF_MAX_NUM2
#define SEND_BUF_MAX_NUM2 40//35//60 //300
#endif

#ifndef CMD_LENGTH2 
#define CMD_LENGTH2       400*1024 //10 //not over 512*1024
//200000
//512000
#endif
#define MAX_CHANNEL_NUM	4
//Circular queue

struct _my_queue{ 
    unsigned char sendBufForUsart[SEND_BUF_MAX_NUM2][CMD_LENGTH2];
    int font, rear;
    int num;
	int flag;
    int len[SEND_BUF_MAX_NUM2];
}; 

typedef struct _mult_queue
{
	struct _my_queue channel_queue[MAX_CHANNEL_NUM];
	int channel_num;
}mult_queue;

static pthread_mutex_t current_job_lock[MAX_CHANNEL_NUM];
static mult_queue multqueue;
//static struct _my_queue myQueue;
static int _have_init_fifo = 0;


//Init queue
void InitQueue()
{
	int i=0;
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		multqueue.channel_queue[i].font = 0;
		multqueue.channel_queue[i].rear = 0;
		multqueue.channel_queue[i].num = 0;
		pthread_mutex_init(&current_job_lock[i],NULL);
	}
	_have_init_fifo = 1;
}

int GetStreamNum(int fd)
{
	return multqueue.channel_queue[fd].num;
}
//Write Queue
void WriteQueue(unsigned char *buffer,int length,int channel)
{	
	if(!_have_init_fifo)
		InitQueue();
	int i =channel;
	//printf("%s:channel = %d \n",__func__,channel);
	//if(multqueue.channel_queue[i].num>30)usleep(2000);//30000
	pthread_mutex_lock(&current_job_lock[i]);
	
	//if(length>(400*1024))printf("over size is %d...! \n",length);
	
	multqueue.channel_queue[i].num++;
#ifdef debug_print
	if(multqueue.channel_queue[i].num>30)
	{
		printf("%s:write num=%d, channel = %d\n",__func__,multqueue.channel_queue[i].num,channel);
	}
#endif
	if(multqueue.channel_queue[i].num>SEND_BUF_MAX_NUM2)
	{
#ifdef debug_print
		printf("fifo is full,when recv data******%d*****!\n",channel);
#endif
		/*myQueue.num = SEND_BUF_MAX_NUM;
		myQueue.font++;
		if(myQueue.font>=SEND_BUF_MAX_NUM)
		{
			myQueue.font = 0;
		}*/
		
		multqueue.channel_queue[i].font = 0;
		multqueue.channel_queue[i].rear = 0;
		multqueue.channel_queue[i].num = 1;
		
		//exit(1);
	}
	
	multqueue.channel_queue[i].len[multqueue.channel_queue[i].rear]=length;
	memcpy(multqueue.channel_queue[i].sendBufForUsart[multqueue.channel_queue[i].rear],buffer,length);
	multqueue.channel_queue[i].rear++;
	
	if(multqueue.channel_queue[i].rear>=SEND_BUF_MAX_NUM2)
	{
		multqueue.channel_queue[i].rear=0;
	}
	pthread_mutex_unlock(&current_job_lock[i]);
	//unsigned char *p;
	//p=buffer;
	//printf("ch%d,put %d: %x %x %x %x %x %x\n",channel,(length),p[0],p[1],p[2],p[3],p[4],p[5]);
	
}

int ReadQueue(unsigned char *buffer,int *length,int channel)
{
	if(!_have_init_fifo)
		InitQueue();
	int i = channel;
	pthread_mutex_lock(&current_job_lock[i]);
	
	if(multqueue.channel_queue[i].num<=0){
		pthread_mutex_unlock(&current_job_lock[i]);
		//printf("%s:return -1 \n",__func__);
		return -1;
	}

	*length=multqueue.channel_queue[i].len[multqueue.channel_queue[i].font];
	memcpy(buffer,multqueue.channel_queue[i].sendBufForUsart[multqueue.channel_queue[i].font],*length);

#ifdef debug_print
	if(multqueue.channel_queue[i].font<=0)
		printf("fifo is empty,when read ****%d****!\n",channel);
#endif
	multqueue.channel_queue[i].font++;	
	
	if(multqueue.channel_queue[i].font>=SEND_BUF_MAX_NUM2)
	{
		multqueue.channel_queue[i].font = 0;
	}
	multqueue.channel_queue[i].num--;

#ifdef debug_print
	if(multqueue.channel_queue[i].num>30)
		printf("%s:read num = %d ,channel =%d \n",__func__,multqueue.channel_queue[i].num,channel);
#endif

	pthread_mutex_unlock(&current_job_lock[i]);

#ifdef save_file_264
	unsigned char *p;
	p=buffer;
	//printf("size = %d \n",(*length));
	//if((*p+4)&0x1f==7)
	//printf("ch%d,get %d: %x %x %x %x %x %x\n",channel,(*length),p[0],p[1],p[2],p[3],p[4],p[5]);
	//printf("channel = %d ...............\n",channel);
	static int savefile[4] ={0};
	static FILE *fp1 = NULL;
	static FILE *fp2 = NULL;
	if(savefile[channel] == 0)
	{
		printf("channel = %d \n",channel);
		if(channel==0)
		fp1= fopen("readqueue1.h264","wb");
		if(channel==1)
		fp2= fopen("readqueue2.h264","wb");
		savefile[channel] = 1;
	}
	if(savefile[channel]==1)
	{
		if(channel==0)
		fwrite(buffer,1,(*length),fp1);
		if(channel==1)
		fwrite(buffer,1,(*length),fp2);
	}
#endif
	return 0;
}

