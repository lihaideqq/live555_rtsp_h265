#ifndef __LIVE_H265_BUFFER_TYPE_HH
#define __LIVE_H265_BUFFER_TYPE_HH

#ifdef __cplusplus
extern "C"{
#endif

#define TRANSPORT_SYNC_BYTE 0x47        // TS流同步字节
#define REC_BUF_MAX_LEN 256*1024+2*188  // 每个buffer的有效数据长度为256K字节,多出的2*188字节存放PAT表和PMT表

typedef struct s_buffer 
{
    unsigned char   buf_writing;        // buffer是否处于被写状态，buffer在写入状态不稳定，需要等待写完成才能读取
    unsigned int    buf_len;            // 写入数据的实际长度
    unsigned int    buf_read_counter;   // 记录已经读取的字节数
    unsigned char   buf_data[REC_BUF_MAX_LEN];  // 存放数据的buffer
} t_h265_Buffer;

#ifdef __cplusplus
}
#endif


#endif




