#pragma once

#include "asio.h"
#include "wavHandler.h"
#define RUISENCARD 0

typedef bool(*ploadAsioDriver)(char *name);
typedef bool(*ploadAsioDriver)(char *name);
typedef void(*preleaseAsioDriver)();
typedef int(*pgetAsioDriverCount)();
typedef long(*pgetAsioDriverName)(int index, char *name);
typedef ASIOError(*pInitialize)();
typedef long(*pGetBufferSizeMin)();
typedef long(*pGetBufferSizeMax)();
typedef long(*pGetBufferSizePreferred)();
typedef ASIOError(*pSetSampleRate)(ASIOSampleRate rate);
typedef ASIOError(*pPrepare)();
typedef long(*pGetInputChannelCount)();
typedef long(*pGetOutputChannelCount)();
typedef ASIOSampleRate(*pGetCurrentSampleRate)();
typedef ASIOError(*pPlayBuff)(BufferInfo bufferList[], int length);
typedef ASIOError(*pCaptureBuff)(BufferInfo bufferList[], int length);
typedef void(*pStopPlayback)();
typedef void(*pStopCapture)();
typedef ASIOError(*pFinish)();
typedef long(*pGetInputLatency)();
typedef long(*pGetOutputLatency)();
typedef bool(*pIsCaptureRunning)();
typedef bool(*pIsRscardInsert)();
typedef ASIOError(*pSetPlayBuffer)(void* buffer, int len, int port);
typedef ASIOError(*pSetCaptureBuffer)(void* buffer, int len, int port);
typedef void(*pStartPlayback)();
typedef void(*pStartCapture)();
typedef bool(*pIsPortCaptureRunning)(int port);
typedef bool(*pIsPortPlaybackRunning)(int port);



struct MyPartmeter
{
	int nPort;
	char *cRecordBuf;
	int nRecordLen;
	LPVOID pThis;
};

struct RecordPartmeter
{
	int nTotalPort;
	int nOneRecordLen;
	MyPartmeter par[8];
	bool bRecord[8];
	LPVOID pThis;
	

	char filepath[200];
	//uint16 nch;
	uint32 samplerate;
	uint16 bps;
	int port;
	int len;
	int* data;

	int port2;
	int len2;
	int* data2;
	
};


class CRuiSenCard
{
public:
	CRuiSenCard();
	~CRuiSenCard();



	ploadAsioDriver loadAsioDriver;
	preleaseAsioDriver releaseAsioDriver;
	pgetAsioDriverCount getAsioDriverCount;
	pgetAsioDriverName getAsioDriverName;
	pInitialize Initialize;
	pGetBufferSizeMin GetBufferSizeMin;
	pGetBufferSizeMax GetBufferSizeMax;
	pGetBufferSizePreferred GetBufferSizePreferred;
	pSetSampleRate SetSampleRate;
	pPrepare Prepare;
	pGetInputChannelCount GetInputChannelCount;
	pGetOutputChannelCount GetOutputChannelCount;
	pGetCurrentSampleRate GetCurrentSampleRate;
	pPlayBuff PlayBuff;
	pCaptureBuff CaptureBuff;
	pStopPlayback StopPlayback;
	pStopCapture StopCapture;
	pFinish Finish;
	pGetInputLatency GetInputLatency;
	pGetOutputLatency GetOutputLatency;
	pIsCaptureRunning IsCaptureRunning;
	pIsRscardInsert IsRscardInsert;
	pSetPlayBuffer SetPlayBuffer;
	pSetCaptureBuffer SetCaptureBuffer;
	pStartPlayback StartPlayback;
	pStartCapture StartCapture;
	pIsPortCaptureRunning IsPortCaptureRunning;
	pIsPortPlaybackRunning IsPortPlaybackRunning;


	HMODULE hModuleDll;
	void LoadAsioLib();
	void FreeAsioLib();
	bool initCard();

	RecordPartmeter m_RecordParameter;
	bool isruning = false;


	void playwithRecord(int* data, int len, int port, int* res = NULL, int* res2 = NULL, bool isrecord = true);
	void record(int* data, int len, int port);
	void Testrecord(int* data, int len, int port, int* data2, int len2, int port2);
	bool isCardInsert();


	
};

