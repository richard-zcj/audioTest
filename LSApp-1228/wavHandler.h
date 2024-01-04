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
		uint8  riff[4];             //��Դ�����ļ���־;
		uint32 size;               //���¸���ַ��ʼ���ļ���β���ֽ���; 
		uint8  wave_flag[4];        //wave�ļ���ʶ; 
		uint8  fmt[4];              //���θ�ʽ��ʶ ;
		uint32 fmt_len;            //�����ֽ�(һ��Ϊ00000010H) ;
		uint16 tag;                //��ʽ���ֵ࣬Ϊ1ʱ����ʾPCM���Ա��� ;
		uint16 channels;           //ͨ������������Ϊ1��˫����Ϊ2 ;
		uint32 samp_freq;          //����Ƶ�� ;
		uint32 byte_rate;          //���ݴ����� (ÿ���ֽڣ�����Ƶ�ʡ�ÿ�������ֽ���) ;
		uint16 block_align;        //������ֽ��� = channles * bit_samp / 8 ;
		uint16 bit_samp;           //bits per sample (�ֳ�����λ��) ;
	} wave_header_t;


	typedef struct WaveStruct
	{
		FILE *fp;                  //file pointer ;
		wave_header_t pHeader;      //header; 
		uint8 data_flag[4];        //���ݱ�ʶ�� ;
		uint32 length;             //������������ ;
		int *pData;             //data ;
		float* pfData;
	} wave_t;

	bool wavread(const char * filename, wave_t *pWave);
	void wavwrite(const char * filename, uint16 nch, uint32 samplerate, uint16 bps, char* data, int len);
	//void wavwrite(const char * filename, uint16 nch, uint32 samplerate, uint16 bps, int* data, int len);

	


#ifdef   __cplusplus  
}
#endif   /* end of __cplusplus */ 

