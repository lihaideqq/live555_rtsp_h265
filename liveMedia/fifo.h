#ifndef __FIFO_H__
#define __FIFO_H__
//#define save_file_264
//#define debug_print
void initQueue();
int GetStreamNum(int fd);
void WriteQueue(unsigned char *buffer,int length,int channel);
int ReadQueue(unsigned char *buffer,int *length,int channel);
//void WriteQueue(unsigned char *buffer,int length);
//int ReadQueue(unsigned char *buffer,int *length);

#endif
