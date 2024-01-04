#pragma once

#ifdef   __cplusplus  
extern "C" {
#endif  

#include <stdio.h>

	typedef unsigned char uint8;
	typedef unsigned short int uint16;
	typedef unsigned int uint32;

	typedef struct WaveHeader
	{
		uint8  riff[4];             //资源交换文件标志;
		uint32 size;               //从下个地址开始到文件结尾的字节数; 
		uint8  wave_flag[4];        //wave文件标识; 
		uint8  fmt[4];              //波形格式标识 ;
		uint32 fmt_len;            //过滤字节(一般为00000010H) ;
		uint16 tag;                //格式种类，值为1时，表示PCM线性编码 ;
		uint16 channels;           //通道数，单声道为1，双声道为2 ;
		uint32 samp_freq;          //采样频率 ;
		uint32 byte_rate;          //数据传输率 (每秒字节＝采样频率×每个样本字节数) ;
		uint16 block_align;        //块对齐字节数 = channles * bit_samp / 8 ;
		uint16 bit_samp;           //bits per sample (又称量化位数) ;
	} wave_header_t;


	typedef struct WaveStruct
	{
		FILE *fp;                  //file pointer ;
		wave_header_t pHeader;      //header; 
		uint8 data_flag[4];        //数据标识符 ;
		uint32 length;             //采样数据总数 ;
		int *pData;             //data ;
		float* pfData;
	} wave_t;

	bool wavread(const char * filename, wave_t *pWave);
	void wavwrite(const char * filename, uint16 nch, uint32 samplerate, uint16 bps, char* data, int len);
	//void wavwrite(const char * filename, uint16 nch, uint32 samplerate, uint16 bps, int* data, int len);

	


#ifdef   __cplusplus  
}
#endif   /* end of __cplusplus */ 

