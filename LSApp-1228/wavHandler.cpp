#include "stdafx.h"
#include "wavHandler.h"
#include <malloc.h>



bool wavread(const char * filename, wave_t *pWave)
{

	unsigned char temp = 0;
	uint32 byts;
	uint32 i = 0, w = 0, iLength;
	uint32 j;
	char iHigh;

	pWave->fp = fopen(filename, "rb");

	if (pWave->fp == NULL)                              // open file 
	{
		printf("file open failure!\n");
		return false;
	}
	
	if (4 != fread(&pWave->pHeader.riff, sizeof(uint8), 4, pWave->fp))           // RIFF chunk 
	{
		printf("read riff error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.size), sizeof(uint32), 1, pWave->fp))         //SIZE : from here to file end 
	{
		printf("read size error!\n");
		return false;
	}
	if (4 != fread(&pWave->pHeader.wave_flag, sizeof(uint8), 4, pWave->fp))      // wave file flag 
	{
		printf("read wave_flag error!\n");
		return false;
	}
	if (4 != fread(&pWave->pHeader.fmt, sizeof(uint8), 4, pWave->fp))             //fmt chunk   
	{
		printf("read fmt error!\n");
		return false;
	}
	if (1 != fread(&pWave->pHeader.fmt_len, sizeof(uint32), 1, pWave->fp))       //fmt length 
	{
		printf("read fmt_len error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.tag), sizeof(uint16), 1, pWave->fp))          // tag : PCM or not
	{
		printf("read tag error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.channels), sizeof(uint16), 1, pWave->fp))      //channels 
	{
		printf("read channels error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.samp_freq), sizeof(uint32), 1, pWave->fp))      // samp_freq 
	{
		printf("read samp_freq error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.byte_rate), sizeof(uint32), 1, pWave->fp))      // byte_rate : decode how many bytes per second  
	{                                                                       // byte_rate = samp_freq * bit_samp 
		printf("read byte_rate error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.block_align), sizeof(uint16), 1, pWave->fp))       //quantize bytes for per samp point 
	{
		printf("read byte_samp error!\n");
		return false;
	}
	if (1 != fread(&(pWave->pHeader.bit_samp), sizeof(uint16), 1, pWave->fp))        //quantize bits for per samp point
	{                                                                        // bit_samp = byte_samp * 8 
		printf("read bit_samp error!\n");
		return false;
	}

	do  	// jump to "data" for reading data   
	{
		fread(&temp, sizeof(uint8), 1, pWave->fp);
	} while ('d' != temp);
	pWave->data_flag[0] = temp;
	pWave->data_flag[4] = '\0';
	for (i = 1; i <= 3; i++)/* data chunk */
	{
		if (1 != fread(&pWave->data_flag[i], sizeof(uint8), 1, pWave->fp))
		{
			printf("read header data error!\n");
			return false;
		}
	}
	if (1 != fread(&pWave->length, sizeof(uint32), 1, pWave->fp))                  //data length 
	{
		printf("read length error!\n");
		return false;
	}


	byts = pWave->pHeader.bit_samp / 8;
	/*if (byts == 4 && pWave->pHeader->channels == 2) //32位PCM
	{
		uint32 idata;
		pWave->pData = (uint32*)malloc(pWave->length * sizeof(uint32) * 2);

		iLength = (pWave->length) / (pWave->pHeader->bit_samp / 8 * 2);
		for (j = 0; j < iLength; j++)
		{
			fread(&idata, 8, 1, pWave->fp);
			pWave->pData[j] = idata;
		}
		pWave->length = iLength;
	}*/
	if (byts == 4 && pWave->pHeader.channels == 1) //32位PCM
	{
		int idata;
		pWave->pData = (int*)malloc(pWave->length * sizeof(int));
		iLength = (pWave->length) / (pWave->pHeader.bit_samp / 8);
		for (j = 0; j < iLength; j++)
		{
			fread(&idata, 4, 1, pWave->fp);
			pWave->pData[j] = idata;
		}
		pWave->length = iLength;
	}

	if (byts == 3 && pWave->pHeader.channels == 1) //32位PCM
	{
		int idata;
		pWave->pData = (int*)malloc(pWave->length * sizeof(int));
		iLength = (pWave->length) / (pWave->pHeader.bit_samp / 8);
		for (j = 0; j < iLength; j++)
		{
			fread(&idata, 4, 1, pWave->fp);
			pWave->pData[j] = idata;
		}
		pWave->length = iLength;
	}
	
	if (byts == 2 && pWave->pHeader.channels == 2) //16位双字节PCM
	{//zcj wdm 
		float idata;
		//pWave->pData = (int*)malloc(pWave->length * sizeof(int));
		pWave->pfData = (float*)malloc(pWave->length * sizeof(float));
		iLength = (pWave->length) / (pWave->pHeader.bit_samp / 8 * 2);
		for (j = 0; j < iLength; j++)
		{
			fread(&idata, 4, 1, pWave->fp);
			//(pWave->pData[j]) = idata;
			(pWave->pfData[j]) = idata;
		}
	}
	if (byts == 2 && pWave->pHeader.channels == 1) //16位双字节PCM		
	{
		uint16 idata;
		pWave->pData = (int*)malloc(pWave->length * sizeof(uint32));
		iLength = (pWave->length) / (pWave->pHeader.bit_samp / 8);
		for (j = 0; j < iLength; j++)
		{
			fread(&idata, 2, 1, pWave->fp);
			pWave->pData[j] = idata;
		}
		pWave->length = iLength;
	}

	if (byts == 1 && pWave->pHeader.channels == 1)//8位单字节PCM	
	{
		uint8 idata;
		pWave->pData = (int*)malloc(pWave->length * sizeof(uint32) / 4);
		iLength = (pWave->length);
		for (j = 0; j < iLength; j++)
		{
			fread(&iHigh, 1, 1, pWave->fp);
			idata = (uint32)iHigh;
			pWave->pData[j] = idata;
		}
		pWave->length = iLength;
	}

	return true;
}


void wavwrite(const char * filename, uint16 nch, uint32 samplerate, uint16 bps, char* data, int len)
{
	long  iByts, iLength;

	wave_t *pWaveOut;


	pWaveOut = (wave_t *)malloc(sizeof(wave_t));
	//pWaveOut->pHeader = &headr;// (wave_header_t *)malloc(sizeof(wave_header_t));

	memset(pWaveOut, 0, sizeof(wave_t));
	//memset(pWaveOut->pHeader, 0, sizeof(wave_header_t));

	pWaveOut->pHeader.riff[0] = 'R';
	pWaveOut->pHeader.riff[1] = 'I';
	pWaveOut->pHeader.riff[2] = 'F';
	pWaveOut->pHeader.riff[3] = 'F';

	pWaveOut->pHeader.size = 44 - 8 + len;

	pWaveOut->pHeader.wave_flag[0] = 'W';
	pWaveOut->pHeader.wave_flag[1] = 'A';
	pWaveOut->pHeader.wave_flag[2] = 'V';
	pWaveOut->pHeader.wave_flag[3] = 'E';

	pWaveOut->pHeader.fmt[0] = 'f';
	pWaveOut->pHeader.fmt[1] = 'm';
	pWaveOut->pHeader.fmt[2] = 't';
	pWaveOut->pHeader.fmt[3] = ' ';

	pWaveOut->pHeader.fmt_len = 0x10;
	pWaveOut->pHeader.tag = 1;
	pWaveOut->pHeader.channels = nch;
	pWaveOut->pHeader.samp_freq = samplerate;
	pWaveOut->pHeader.byte_rate = samplerate * nch * bps / 8;
	pWaveOut->pHeader.block_align = nch * bps / 8;
	pWaveOut->pHeader.bit_samp = bps;

	pWaveOut->data_flag[0] = 'd';
	pWaveOut->data_flag[1] = 'a';
	pWaveOut->data_flag[2] = 't';
	pWaveOut->data_flag[3] = 'a';

	pWaveOut->length = len;

	pWaveOut->fp = fopen(filename, "wb+");
	//rewind(pWaveOut->fp);//@zcj add

	if (pWaveOut->fp == NULL)//@zcj add for modify rs bug
		return;

	fwrite(&(pWaveOut->pHeader), sizeof(wave_header_t), 1, pWaveOut->fp); //将头写入.wav文件;
	fwrite(&(pWaveOut->data_flag), sizeof(uint8), 4, pWaveOut->fp); //写入.wav文件; 
	fwrite(&(pWaveOut->length), sizeof(uint32), 1, pWaveOut->fp);
	/*
	if (pWaveOrigin->pHeader->channels == 2)
	{
		iByts = pWaveOrigin->pHeader->bit_samp / 8;
		iLength = pWaveOrigin->length / pWaveOrigin->pHeader->block_align;

		if (iByts == 2)
		{
			uint32 idata;
			while (pWaveOrigin->length > j)
			{
				for (i = 0; i<160; i++)
				{
					if (j >(pWaveOrigin->length - 1)) continue;
					idata = pWaveOrigin->pData[j++];
					readbuffL[i] = idata & 0x0000ffff;
					readbuffH[i] = (idata & 0Xffff0000) >> 16;
				}
				WebRtcNsx_Process(pNsxInst, readbuffL, NULL, writebuffL, NULL);
				WebRtcNsx_Process(pNsxInst, readbuffH, NULL, writebuffH, NULL);
				for (i = 0; i<160; i++)
				{
					if (j >(pWaveOrigin->length - 1)) continue;
					fwrite(&writebuffL[i], 2, 1, fp1);
					fwrite(&writebuffH[i], 2, 1, fp1);
				}
			}
		}

		if (iByts == 1)
		{
			while (pWaveOrigin->length > j)
			{
				for (i = 0; i<160; i++)
				{
					if (j >(pWaveOrigin->length - 1)) continue;
					readbuff[i] = (short)pWaveOrigin->pData[j++];
				}
				WebRtcNsx_Process(pNsxInst, readbuff, NULL, writebuff, NULL);
				for (i = 0; i<160; i++)
				{
					if (j >(pWaveOrigin->length - 1)) continue;
					writebuff[i] = (char)writebuff[i];

				}
				fwrite(writebuff, 1, 160, fp1);
			}
		}

	}
	*/

	/*
	if (pWaveOut->pHeader.channels == 1)
	{

		iByts = pWaveOut->pHeader.bit_samp / 8;
		iLength = pWaveOut->length / (pWaveOut->pHeader.bit_samp / 8);

		fwrite(data, iByts, iLength, pWaveOut->fp);

	}
	*/


	
	  

	if (fwrite(data, len, 1, pWaveOut->fp) < len)
	{
		//AfxMessageBox(_T("wav write err!"))
		
	}
	fclose(pWaveOut->fp);
	//free(pWaveOut->pHeader);
	free(pWaveOut);
	
}




void MultiChannelWavWrite()
{
}

//void wavwrite(const char * filename, uint16 nch, uint32 samplerate, uint16 bps, int* data, int len)
//{
//	long  iByts, iLength;
//
//	wave_t *pWaveOut;
//
//
//	pWaveOut = (wave_t *)malloc(sizeof(wave_t));
//	//pWaveOut->pHeader = &headr;// (wave_header_t *)malloc(sizeof(wave_header_t));
//
//	memset(pWaveOut, 0, sizeof(wave_t));
//	//memset(pWaveOut->pHeader, 0, sizeof(wave_header_t));
//
//	pWaveOut->pHeader.riff[0] = 'R';
//	pWaveOut->pHeader.riff[1] = 'I';
//	pWaveOut->pHeader.riff[2] = 'F';
//	pWaveOut->pHeader.riff[3] = 'F';
//
//	pWaveOut->pHeader.size = 44 - 8 + len;
//
//	pWaveOut->pHeader.wave_flag[0] = 'W';
//	pWaveOut->pHeader.wave_flag[1] = 'A';
//	pWaveOut->pHeader.wave_flag[2] = 'V';
//	pWaveOut->pHeader.wave_flag[3] = 'E';
//
//	pWaveOut->pHeader.fmt[0] = 'f';
//	pWaveOut->pHeader.fmt[1] = 'm';
//	pWaveOut->pHeader.fmt[2] = 't';
//	pWaveOut->pHeader.fmt[3] = ' ';
//
//	pWaveOut->pHeader.fmt_len = 0x10;
//	pWaveOut->pHeader.tag = 1;
//	pWaveOut->pHeader.channels = nch;
//	pWaveOut->pHeader.samp_freq = samplerate;
//	pWaveOut->pHeader.byte_rate = samplerate * nch * bps / 8;
//	pWaveOut->pHeader.block_align = nch * bps / 8;
//	pWaveOut->pHeader.bit_samp = bps;
//
//	pWaveOut->data_flag[0] = 'd';
//	pWaveOut->data_flag[1] = 'a';
//	pWaveOut->data_flag[2] = 't';
//	pWaveOut->data_flag[3] = 'a';
//
//	pWaveOut->length = len;
//
//	pWaveOut->fp = fopen(filename, "wb+");
//
//	fwrite(&(pWaveOut->pHeader), sizeof(wave_header_t), 1, pWaveOut->fp); //将头写入.wav文件;
//	fwrite(&(pWaveOut->data_flag), sizeof(uint8), 4, pWaveOut->fp); //写入.wav文件; 
//	fwrite(&(pWaveOut->length), sizeof(uint32), 1, pWaveOut->fp);
//
//
//		
//
//	fwrite(data, sizeof(data), 1, pWaveOut->fp);
//
//	fclose(pWaveOut->fp);
//	//free(pWaveOut->pHeader);
//	free(pWaveOut);
//	
//}
