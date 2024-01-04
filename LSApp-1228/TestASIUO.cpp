
#include "stdafx.h"
#include "TestASIUO.h"
//#include "LSAppView.h"
#include <thread>
#include <iostream>
#include <fstream>
#include "LSApp.h"
#include "wavHandler.h"

#include "mmdeviceapi.h"
#include "PolicyConfig.h"
#include "propapi.h"
#include "FunctionDiscoveryKeys_devpkey.h"
#include "AudioDevice.h"
#include <Audioclient.h>



#pragma comment(lib,"winmm.lib")

using namespace std;



//TestASIUO glbasio 联实声卡驱动ID;
CLSID IID_ASIO_DRIVER = { 0x188135e1, 0xd565, 0x11d2, { 0x85, 0x4f, 0x0, 0xa0, 0xc9, 0x9f, 0x5d, 0x19 } };

CLSID IID_RME = { 0xECE3DEFE, 0xB1FA, 0x4927, {0xAB,0xBF,  0xF2, 0x16, 0x56, 0xF2, 0x7C, 0x39 } };

CLSID   RME = { 0xECE3DEFE, 0xB1FA, 0x4927, { 0xAB, 0xBF, 0xF2, 0x16, 0x56, 0xF2, 0x7C, 0x39 } };
CLSID   RS = { 0xFEC47612, 0x7933, 0x4B66, { 0x8A, 0x18, 0xB8, 0x6D, 0xFF, 0xED, 0xD9, 0xB3 } };
CLSID   LI = { 0x6347B8C4, 0x267B, 0x4087, { 0xA2, 0x2D, 0x40, 0x6E, 0xA6, 0xA4, 0x26, 0x5C } };
//CLSID   LI = { 0x5D071BE3, 0x1E7A, 0x4978, { 0x9E, 0xC0, 0xA3, 0x72, 0xBC, 0x91, 0xB6, 0xDE } };
CLSID   YM = { 0xCB7F9FFD, 0xA33B, 0x48B2, { 0x8B, 0xC0, 0x43, 0x7D, 0x94, 0xF3, 0x71, 0x42 } };

CLSID AUDIOSET = { 0x870af99c, 0x171d, 0x4f9e, { 0xaf, 0x0d,0xe6,0x3d,0xf4,0x0c,0x2b,0xc9 } };


//struct __declspec(uuid_t("91408006-120B-4DBD-A7B1-948846D3DE3B"))  IActiveDesktop;




using namespace std;

a4aInterface* asio4all;

#define CHANNELS 60
ASIOBufferInfo* buf;
int globle_in, globle_out,device_in;
int** playdata;//实际使用的播放数据
int* playindex;//实际播放通道
int* recorddata;
int** rd;//实际使用的录音数据
int* recordindex;//实际录音通道


/********第二版使用单独播放与录音控制*********/
int* bufferRecord;
vector<int*> record_data;

bool doubleBufferIndexmme;

vector<int> indexOfRecordMME;

size_t block_mme = 0;

WAVEHDR* wHdr1;//数据缓存


mutex m;


bool asioBufswitch;

int buffsize;//声卡缓冲区大小

int* pld;//临时保存数据
//extern CJiLiSet jiliset;

extern bool bstopASIO;
//微积分可以把非均匀变化的计算转化为均匀变化的计算
//线性变化  非线性变化
/*

返回值决定调用音频处理函数
*/
long asioMessage (long selector, long value, void* message, double* opt)
{


	return 0;

}
size_t len = 0;// 播放数据点数
size_t recordlen = 0;
size_t step = 0;
size_t recordstep = 0;
/*
按照字节处理音频数据
record input data is relation to play output
*/
void bufferSwitch (long doubleBufferIndex, ASIOBool directProcess)
{
	/*char* fileName1 = "D:\\TestRecord\\testplay.wav";
	wavwrite(fileName1, 1, 44100, 32, (char*)playdata[0], 4* 44100*10);*/
	
	try{

		asioBufswitch = true;
		if (len == 0)
		{



			/*char* fileName1 = "D:\\TestRecord\\recordrme.wav";
			wavwrite(fileName1, 1, 44100, 32, (char*)rd, sizeof(int)*44100);*/

			//delete[] playdata;
			/***********************播放结束后去除噪音*******************************/
			for (size_t i = 0; i < globle_out; i++)
			{
				memset(buf[device_in + playindex[i]].buffers[doubleBufferIndex], 0, buffsize * 4);
				//memset(buf[9].buffers[doubleBufferIndex], 0, 1024 * 4);
				//memset(buf[32].buffers[doubleBufferIndex], 0, 1024 * 4);
				//memset(buf[33].buffers[doubleBufferIndex], 0, 1024 * 4);
			}

			if (recordlen == 0)
				return;
		}


		if (directProcess)
		{


			if (len >= buffsize * 4)
			{
				//int dd[1024] = {0};
				//
				//memcpy(buf[8].buffers[doubleBufferIndex], playdata + step, 1024*4);

				for (size_t i = 0; i < globle_out; i++)
				{
					if (playdata[i] != NULL)
					{
						for (size_t j = step; j < step + buffsize; j++)
						{
							pld[j - step] = playdata[i][j];
						}
						memcpy(buf[device_in + playindex[i]].buffers[doubleBufferIndex], pld, buffsize * 4);
					}
						
					
					
					//memcpy(buf[i].buffers[doubleBufferIndex], pld + step, 1024 * 4);
					//memcpy(buf[33].buffers[doubleBufferIndex], playdata + step, 1024 * 4);
					//memcpy(buf[8].buffers[doubleBufferIndex], playdata + step, 1024 * 4);
					//memcpy(buf[9].buffers[doubleBufferIndex], playdata + step, 1024 * 4);
				}

				//delete[] pld;

				//录音
				//for (size_t i = 0; i < globle_in; i++)
				//{
				//	
				//	memcpy(rd[recordindex[i]] + step, buf[i].buffers[doubleBufferIndex == 0 ? 1 : 0], buffsize * 4);
				//	//memcpy(rd + step, buf[0].buffers[doubleBufferIndex == 0 ? 1 : 0], 1024 * 4);
				//}

				step += buffsize;
				len -= buffsize * 4;
			}
			else if (len > 0)
			{
				//step -= buffsize;
				if (pld)
				    delete[] pld;
				pld = new int[len / 4]{0};

				for (size_t i = 0; i < globle_out; i++)
				{
					if (playdata[i] != NULL)
					{
						for (size_t j = step; j < step + (len / 4); j++)
						{
							pld[j - step] = playdata[i][j];
						}
						memset(buf[device_in + playindex[i]].buffers[doubleBufferIndex], 0, buffsize * 4);
						memcpy(buf[device_in + playindex[i]].buffers[doubleBufferIndex], pld, len);
					
					}
					else
					{
						AfxMessageBox(_T("paly err"));
					}
						

					
					
				}

				//delete[] pld;

				//录音
				//for (size_t i = 0; i < globle_in; i++)
				//{
				//	memcpy(rd[recordindex[i]] + step, buf[i].buffers[doubleBufferIndex == 0 ? 1 : 0], len);
				//	//memcpy(rd + step, buf[0].buffers[doubleBufferIndex == 0 ? 1 : 0], len);
				//}

				step = 0;
				len = 0;




			}


			if (recordlen >= buffsize * 4)
			{



				//录音
				for (size_t i = 0; i < globle_in; i++)
				{
					//recordindex[i] 因为这里暂时只支持一个录音通道 使用i
					memcpy(rd[i] + recordstep, buf[recordindex[i]].buffers[doubleBufferIndex == 0 ? 1 : 0], buffsize * 4);
					//memcpy(rd + step, buf[0].buffers[doubleBufferIndex == 0 ? 1 : 0], 1024 * 4);
				}

				recordstep += buffsize;//下一次将要，pc指针
				recordlen -= buffsize * 4;//本次已经

			}
			else if (recordlen > 0)
			{

				for (size_t i = 0; i < globle_in; i++)
				{
					//recordindex[i] 因为这里暂时只支持一个录音通道 使用i
					memcpy(rd[i] + recordstep, buf[recordindex[i]].buffers[doubleBufferIndex == 0 ? 1 : 0], recordlen);
					//memcpy(rd + step, buf[0].buffers[doubleBufferIndex == 0 ? 1 : 0], len);

				}

				recordstep = 0;
				recordlen = 0;

			}

		}
		else
		{
			//非直接处理数据
			memcpy(rd[0] + step, buf[0].buffers[0], buffsize * 4);
			memcpy(rd[0] + step, buf[0].buffers[1], buffsize * 4);

		}


	}
	catch (...)
	{
		AfxMessageBox(_T("buffswitc err"));
	}

}

void bufferSwitch2(long doubleBufferIndex, ASIOBool directProcess)
{


	if (recordlen >= buffsize * 4)
	{



		//录音
		for (size_t i = 0; i < globle_in; i++)
		{
			//recordindex[i] 因为这里暂时只支持一个录音通道 使用i
			memcpy(rd[i] + recordstep, buf[recordindex[i]].buffers[doubleBufferIndex == 0 ? 1 : 0], buffsize * 4);
			//memcpy(rd + step, buf[0].buffers[doubleBufferIndex == 0 ? 1 : 0], 1024 * 4);
		}

		recordstep += buffsize;//下一次将要，pc指针
		recordlen -= buffsize * 4;//本次已经

	}
	else if (recordlen > 0)
	{

		for (size_t i = 0; i < globle_in; i++)
		{
			//recordindex[i] 因为这里暂时只支持一个录音通道 使用i
			memcpy(rd[i] + recordstep, buf[recordindex[i]].buffers[doubleBufferIndex == 0 ? 1 : 0], recordlen);
			//memcpy(rd + step, buf[0].buffers[doubleBufferIndex == 0 ? 1 : 0], len);

		}

		recordstep = 0;
		recordlen = 0;
		bufferRecord = new int[buffsize];
		record_data.push_back(bufferRecord);

	}


}
/*
不使用
*/
void sampleRateDidChange (ASIOSampleRate sRate)
{
	int a = 1 + 2;
}

ASIOTime* bufferSwitchTimeInfo (ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess)
{
	ASIOTime* time=new ASIOTime;


	return time;

}

LONG WINAPI ASIOErrHandle(PEXCEPTION_POINTERS pExceptions)
{
	
	return EXCEPTION_CONTINUE_EXECUTION;
}


	
TestASIUO::TestASIUO() : bstop(false), initRME(false), bfirstrun(true), valote(0),
wdmrecorddone(false), wdmplaydone(false), initcardtimes1(0), m_IsNoPlay(true)
{
	wHdr1 = NULL;
	
	globle_in = 0;
	device_in = 0;
	long* l=0;
	buffsize = 0;
	IUnknown* un=NULL;
	LPUNKNOWN a=NULL;
	asioBufswitch = false;
	/*if (!ASIOExit())
	{
		AfxMessageBox(_T("err"));
	}*/
	//AsioSample asio(a, l);
	/*char* sys = "1.0";
	ASIOBool bok = asio.init((void*)sys);*/


	//indata = new double[44100]{0};
	//outdata = new double[44100];

	/*int f = 1000;
	for (size_t i = 0; i < 44100; i++)
	{
		indata[i] = sin(2.0*3.1415926*f*i / 44100);
	}*/

	//playdata = new int[44100]{0};
	//recorddata = new int[44100]{0};
	//rd = new int[44100];
	//valote = 0.05;
	//for (size_t i = 0; i < 44100; i++)
	//{
	//	playdata[i] = valote*1000000000*sin(2.0*3.1415926 * f * i / 44100.0);
	//	//playdata[i] = 10*sin(2.0*3.1415926 * f * i / 44100.0);
	//	
	//	
	//}

	

#if RUNMODE
	std::thread testrme(&TestASIUO::LoadDll, this);
    std::thread::id rmeid=	testrme.get_id();
	testrme.detach();
#endif

	
	//LoadDll();
	//LoadASIO4ALL();
	//LoadASIO4ALLFunc();


	
	

	
	//LoadWaveData();

	

	
	//LoadWinMM();

	//LoadYamahaASIODriever();
#if RUNTESTASIOMODE
	vector<wstring> wdmDeviceName;
	vector<string> asioChName;
	LoadRSASIO(wdmDeviceName, asioChName,true,3);
	int index = 0;
	int** indata;
	int** playdat = new int*[8];
	size_t testlen = 44100 * 2.6293197278911564;
	playdat[0] = new int[testlen];

	for (size_t i = 0; i < testlen; i++)
	{
		playdat[0][i]=pow(2, 31)*sin(2.0*3.1415926 * 1000 * i / 44100);
	}
	int runcout = 0;
	setplaydata(playdat, 1, &index);
	for (size_t i = 0; i < 50; i++)
	{
		try{
			recordASIO(&index, &indata, 1, 44100, 2.6293197278911564);
		}
		catch (...)
		{
			AfxMessageBox(_T("ASIO err!"));
		}
		

		delete[] rd[0];
		runcout = i;
	}

#endif RUNTESTASIOMODE


	SetUnhandledExceptionFilter(ASIOErrHandle);

}

void TestASIUO::LoadWinMM()
{
	MCIERROR err=-1;
	err=mciSendStringA("open play.wav type waveaudio alias music", NULL, 0, NULL);
	err=mciSendStringA("set music time format ms", NULL, 0, NULL);
	err = mciSendStringA("play music from 0 to 1000 wait", NULL, 0, NULL);

}

void TestASIUO::LoadASIO4ALLFunc()
{

	hModuleDll = NULL;
	init = NULL;

	hModuleDll = LoadLibrary(_T("asio4all64.dll"));
	
}


BOOL EnumCallback(void* param)
{
	int DeviceIndex = -1;
	int InterfaceIndex = -1;

	
	char* name = new char[255]{0};
	while (A4A_PRIVATE_NOERROR == asio4all->GetDeviceProperty(kA4A_Device_lpszName, ++DeviceIndex, name))
	{
		if (name == "RME Fireface 802")
		{
			break;
		}

	}

	while (A4A_PRIVATE_NOERROR == asio4all->GetInterfaceProperty(kA4A_Interface_lpszName, DeviceIndex, ++InterfaceIndex, (void*)name))
	{
		int dwMaxChannels = -1;
		asio4all->GetPinProperty(kA4A_Pin_dwMaxChannels, DeviceIndex, InterfaceIndex, 0, (void*)&dwMaxChannels);

		//A4AError err = asio4all->SetPinProperty(kA4A_Pin_dwDataFlow, DeviceIndex, InterfaceIndex, 0, indata);
		//asio4all->Enumerate();
	}



	return TRUE;

}

void TestASIUO::LoadASIO4ALL()
{
	DeviceIndex = -1;
	InterfaceIndex = -1;
	CoInitialize(0);							// Will use COM

	 asio4all = new a4aInterface;

	DWORD id = 0;
	id=asio4all->GetVersion();
	if (id)
	{

		asio4all->SetCallback(EnumCallback, 0);
		asio4all->Enumerate();
	}

	

	/*std::thread play(&TestASIUO::playThread, this);
	play.detach();

	std::thread record(&TestASIUO::recordThread, this);
	record.detach();
	*/
	

	

	

	delete asio4all;


}


//this function  device name is "music"
void TestASIUO::playThread()
{

	/*int f = 1000;
	for (size_t i = 0; i < 44100; i++)
	{
		indata[i] = 1000000000*sin(2.0*3.1415926*f*i / 44100);
	}

	char* fileName1 = "play.wav";*/
	//wavwrite(fileName1 , 1 , 44100 , 32 , (char*)pWave.pData , pWave.length*4);
	//wavwrite(fileName1, 1, 44100, 24, (char*)indata, sizeof(int)*44100);

	
	MCIERROR err = -1;
	/*err = mciSendStringA("open play.wav type waveaudio alias music", NULL, 0, NULL);
	err = mciSendStringA("set music time format ms", NULL, 0, NULL);
	err = mciSendStringA("play music from 0 to 1000 wait", NULL, 0, NULL);*/

	
	//palyname = "play.wav";
	int time = re_time;
	char comm[1024];
	char* filename = getcharp(palyname);
	sprintf(comm,"open %s alias music", filename);
    err = mciSendStringA(comm, NULL, 0, NULL);
	memset(comm, 0, 1024);
	sprintf(comm, "set music output %d", wdmindex_out);
	err = mciSendStringA(comm, NULL, 0, NULL);
	err = mciSendStringA("set music time format ms", NULL, 0, NULL);
	

	//获取文件时长

	char filelen[100]{0};
	err = mciSendStringA("status music length", filelen, sizeof(filelen), NULL);

	re_time = atof(filelen);
	time = re_time;
	//err = mciSendStringA("record music overwrite", NULL, 0, NULL);//测试同时播放录音
	memset(comm, 0, 1024);
	sprintf(comm, "play music from 0 to %d wait", time);
	err = mciSendStringA(comm, NULL, 0, NULL);


	//err = mciSendStringA("stop music", NULL, 0, NULL);//测试同时播放录音
	//err = mciSendStringA("save music mutiplay.wav wait", NULL, 0, NULL);//测试同时播放录音
	err = mciSendStringA("close music", NULL, 0, NULL);


	delete [] filename;
	

}

//所有调用该函数的地方需要做内存释放
char* TestASIUO::getcharp(CString str)
{

	int len1 = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char* ch = new char[len1 + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
	return ch;
}



void TestASIUO::LoadWaveData()
{
	wave_t data;
	data.length = SAMPLENUMS;
	if (!wavread("D:\\Projects\\LSApp\\123.wav", &data))
		return;
	  
	std::fclose(data.fp);

	std::ofstream outfile;

	CString path("D:\\Projects\\LSApp\\data.txt");
	//path.Append(_T("\\conf1.ini"));
	outfile.open(path, std::ios::out | std::ios::trunc);

	
	

	//for (size_t i = 0; i < data.length; i++)
	//{
	//   bufferrme[i] = data.pData[i];
	//   outfile << data.pData[i] << endl;
	//}

	outfile.close();

}

int TestASIUO::getWdmChsIndex(bool isInput, CString chsName)
{
	int wdmIndex = 0;
	UINT ndev = 0;

	if (isInput)
	{
		WAVEINCAPS pwic;
		ndev = waveInGetNumDevs();
		for (size_t i = 0; i < ndev; i++)
		{
			
		    waveInGetDevCaps(i, &pwic, sizeof(WAVEINCAPS));

			if (chsName.Compare(pwic.szPname) == 0)
			{
				//切换window输入
				wdmIndex = i;
				break;
			}

		}
	}
	else
	{
		WAVEOUTCAPS pwoc;
		ndev = waveOutGetNumDevs();
		for (size_t i = 0; i < ndev; i++)
		{

			waveOutGetDevCaps(i, &pwoc, sizeof(WAVEOUTCAPS));

			if (chsName.Compare(pwoc.szPname) == 0)
			{
				//切换window输入
				wdmIndex = i;
				break;
			}

		}
	}


	return wdmIndex;
}

void TestASIUO::recordThread()
{
	
	

	/*if (wdmrecordrunning)
	{//这里考虑到多线程调用
		return;
	}*/
	wdmrecorddone = false;
	//打开音频设备
	UINT ndev = waveInGetNumDevs();
	WAVEINCAPS pwic;
	int ret = -1;
	for (size_t i = 0; i < ndev; i++)
	{
		//MMSYSERR_BADDEVICEID
		ret = waveInGetDevCaps(i, &pwic, sizeof(WAVEINCAPS));

		if (wdmindex_in == i)
		{
			//切换window输入

		}

	}
	
	

	
	
	MCIERROR err = -1;
	char comm[1024];
	err = mciSendStringA("open new type waveaudio alias rme", NULL, 0, NULL);


	err = mciSendStringA("set rme bitspersample  16", "", 0, NULL);
	err = mciSendStringA("set rme samplespersec  44100", "", 0, NULL);
	err = mciSendStringA("set rme channels 2", "", 0, NULL);
	err = mciSendStringA("set rme format tag pcm", "", 0, 0);
	
	sprintf(comm, "set rme input %d", wdmindex_in);
	err = mciSendStringA(comm, NULL, 0, NULL);

	err = mciSendStringA("record rme overwrite", NULL, 0, 0);
	if (err != 0)
		mainf->setStatusStr(_T("WDM开启录音失败..."));

	Sleep((re_time+0.5)*1000);
	err = mciSendStringA("stop rme", 0, 0, NULL);
	err = mciSendStringA("save rme D:\\TestRecord\\wdmRecord1.wav", NULL, 0, NULL);
	err = mciSendStringA("close rme", NULL, 0, NULL);
	
	
	wdmrecorddone = true;

}

using namespace AudioEndPoint;
HRESULT TestASIUO::SetDefaultAudio(PCWSTR devId, int setType)
{

	/*typedef HRESULT(__stdcall *pfnAudioDevice)(REFCLSID, REFIID, void**);
	typedef HRESULT(_stdcall *PDllRegisterServer)();
	HMODULE hModule = LoadLibrary(_T("AudioSes.dll"));

	pfnAudioDevice fHello = NULL;

	fHello = (pfnAudioDevice)GetProcAddress(hModule, "DllGetClassObject");*/

	///********************这里注册其他驱动************************/
	//PDllRegisterServer DllRegisterServer = NULL;
	//DllRegisterServer = (PDllRegisterServer)GetProcAddress(hModuleDll, "DllRegisterServer");
	//if (DllRegisterServer)
	//{
	//	DllRegisterServer();
	//}


	/*IClassFactory* pcf = NULL;
	HRESULT hr = (fHello)(AUDIOSET, IID_IClassFactory, (void**)&pcf);


	

	AudioDevice * ad;
	IPolicyConfig* pcof;

	hr = pcf->CreateInstance(NULL, AUDIOSET, (void**)&ad);*/

	//ad->SetDefault(eConsole);

	//bok = samp->init(NULL);

	//HRESULT hr = 0;


	IMMDeviceEnumerator *pEnum=NULL;
	LPWSTR setstr=NULL;
	IMMDeviceCollection *pCollection = NULL;
	IPropertyStore *pStore = NULL;
	
	HRESULT hr2 = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)&pEnum);

	PCWSTR tempstr;


	if (SUCCEEDED(hr2))
	{
		IMMDevice* pDevice = NULL;

		//hr2 = pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);

		pEnum->EnumAudioEndpoints((EDataFlow)setType, DEVICE_STATE_ACTIVE, &pCollection);
		

		if (FAILED(hr2))
		{
			if (pEnum)
			{
				pEnum->Release();
				return hr2;
			}
		}

		UINT count;

		pCollection->GetCount(&count);
		if (FAILED(hr2))
		{
			if (pCollection)
			{
				pCollection->Release();
			}
			if (pEnum)
			{
				pEnum->Release();
				
			}
			return hr2;
		}

		for (UINT i = 0; i < count; i++)
		{
			pCollection->Item(i, &pDevice);
			if (SUCCEEDED(hr2))
			{


				hr2 = pDevice->OpenPropertyStore(STGM_READ, &pStore);
				
				
				//CPolicyConfigClient* pcobfg;
				//hr2 = pDevice->Activate(__uuidof(MMDeviceEnumerator), CLSCTX_ALL, NULL, (void**)&pcobfg);
				if (SUCCEEDED(hr2))
				{
					PROPVARIANT friendlyName;

					PropVariantInit(&friendlyName);

					pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
					if (SUCCEEDED(hr2))
					{
						tempstr = friendlyName.pwszVal;
						CString s1(devId);
						CString s2(tempstr);
						if (s1.GetLength()==s2.GetLength())
						{
							if (s1 == s2)
							{
								pDevice->GetId(&setstr);
								break;
							}
							
						}
						else
						{
							if (s2.Find(s1) ==s1.GetLength())
							{
								pDevice->GetId(&setstr);
								break;
							}
						}


					}

				}
			}


		}
	}


	


	IPolicyConfig* pPolicyConfig = NULL;
	ERole reserved = eConsole;

	HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_SERVER, __uuidof(IPolicyConfig), (LPVOID*)&pPolicyConfig);
	

	if (SUCCEEDED(hr))
	{
		WAVEFORMATEX* dd;
		auto str = L"{0.0.0.00000000}.{0e719da0-06f8-4fb1-a9b7-063868d3c2e0}";
		//hr = pPolicyConfig->SetEndpointVisibility(devId, true);
		hr = pPolicyConfig->SetDefaultEndpoint(setstr, reserved);
		pPolicyConfig->Release();

		
	}



	/*HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IMMDeviceEnumerator *pEnum = NULL;

		hr = CoCreateInstance(__uuidof(IMMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		if (SUCCEEDED(hr))
		{

			bool bExit = false;
			IMMDevice *pDefDevice = NULL;

			pEnum->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefDevice);
		}

	}*/




	/*DefSound::CEndpoint endpoint;

	endpoint.m_State.state = DefSound::EDeviceState::Active;
	endpoint.m_DeviceId = devId;
	

	AudioDevice* ad = new AudioDevice(endpoint, AudioDeviceType::Recording);


	ad->SetDefault(eConsole);*/




	return hr2;

}

void TestASIUO::mmelayAndRecordthread()
{
	IMMDeviceEnumerator *pEnum = NULL;
	IMMDevice* pDevice = NULL;
	HRESULT hr2 = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)&pEnum);

	hr2 = pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
	
	IAudioClient* pAudioClinet;
	pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClinet);

	WAVEFORMATEX* pDeviceFormat;

	pAudioClinet->GetMixFormat(&pDeviceFormat);

	const int REFTIMES_PER_SEC = 10000000;
	const int REFTIMES_PER_MILLSEC = 10000;

	const REFERENCE_TIME hnsRequestDur = 2 * REFTIMES_PER_SEC;
	pAudioClinet->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestDur, 0, pDeviceFormat,nullptr);

	
	/*UINT32* data{};
	pAudioClinet->GetBufferSize(data);
	IAudioCaptureClient* pCapClient;
	pAudioClinet->GetService(__uuidof(IAudioCaptureClient), (void**)&pCapClient);*/
	IAudioRenderClient* pRender;
	pAudioClinet->GetService(__uuidof(IAudioRenderClient), (void**)&pRender);

	BYTE* playdata = new BYTE[44100 * 2];

	for (size_t i = 0; i < 44100*2; i++)
	{
		playdata[i] = 255*sin(2.0*3.1415926 * 1000 * i / 44100.0);
	}

	pRender->GetBuffer(44100 * 2, &playdata);


}

void TestASIUO::setWDMplaych(int index, PCWSTR Name, bool isInput)
{

	//UINT ndev = waveOutGetNumDevs();
	//WAVEOUTCAPS pwic;
	//int ret = -1;
	//for (size_t i = 0; i < ndev; i++)
	//{
	//	//MMSYSERR_BADDEVICEID
	//	ret = waveOutGetDevCaps(i, &pwic, sizeof(WAVEOUTCAPS));

	//	if (index == i)
	//	{
	//		//切换window输入
	//		break;
	//	}

	//}
	if (isInput)
	{

		SetDefaultAudio(Name, eCapture);
	}
	else
	{
		SetDefaultAudio(Name, eRender);
	}
	

 //MMRESULT hr=	waveOutMessage((HWAVEOUT)WAVE_MAPPER, WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE, 1, 0);

}
bool TestASIUO::wdmisRecordDone()
{
	return wdmrecorddone;

}
void TestASIUO::setwdmisRecordDone(bool b)
{
	wdmrecorddone = b;
}

//播放时间将从文件中读取
void TestASIUO::play(CString filename, double recordtime, int index, CString palyChsName)
{
	
	palyname = filename;
	re_time = recordtime;
	wdmindex_out = 0;
	SetDefaultAudio(palyChsName, eRender);
	std::thread play(&TestASIUO::playThread, this);
	play.detach();
}



void TestASIUO::record(int time, void* dlg)
{
	
	//memset(bufferrme, 10, SAMPLENUMS);

	//waveform.wFormatTag = WAVE_FORMAT_PCM;
	//waveform.nSamplesPerSec = 48000;
	//waveform.wBitsPerSample = 16;
	//waveform.nChannels = 2;
	//waveform.nAvgBytesPerSec = 48000 * 4;
	//waveform.nBlockAlign = 4;
	//waveform.cbSize = 0;

	////打开音频设备
	//UINT ndev=waveInGetNumDevs();
	//LPWAVEINCAPS pwic=NULL;
	//for (size_t i = 0; i < ndev; i++)
	//{
	//	waveInGetDevCaps(i, pwic, sizeof(LPWAVEINCAPS));
	//}
	//

	//MMRESULT ret = waveInOpen(&hWaveIn, WAVE_MAPPED, &waveform, (DWORD_PTR)AudioRecordProc, (DWORD_PTR)dlg, CALLBACK_FUNCTION);

	//


	////音频缓存初始化
	//for (size_t i = 0; i < 2; i++)
	//{

	//	

	//	wHdr1[i].lpData = new char[CHUNK_SIZE*3];
	//	wHdr1[i].dwBufferLength = CHUNK_SIZE*3;
	//	wHdr1[i].dwBytesRecorded = 0;
	//	wHdr1[i].dwUser = NULL;
	//	wHdr1[i].dwFlags = 0;
	//	wHdr1[i].dwLoops = 1;
	//	wHdr1[i].lpNext = NULL;
	//	wHdr1[i].reserved = 0;

	//	waveInPrepareHeader(hWaveIn, &wHdr1[i], sizeof(WAVEHDR));
	//	waveInAddBuffer(hWaveIn, &wHdr1[i], sizeof(WAVEHDR));
	//}


	//waveInStart(hWaveIn);

	

	
}

//char bufferrme[SAMPLENUMS2];
//char bufferrme2[SAMPLENUMS];
//char bufferrme3[SAMPLENUMS];
//char bufferrme4[SAMPLENUMS];
//int wmdstep = 0;
//int wmdstep2 = 0;
//int wmdstep3 = 0;
//int wmdstep4 = 0;
vector<HWAVEIN> recordWdms;
vector<char*> recordDatas;
vector<HANDLE> recordThreadId;
struct RecordParam
{
	unsigned long m_fs;
	double m_recordTimes;
	int m_chs;
	int m_bits;

};
void TestASIUO::stopRe()
{
	for (size_t i = 0; i < recordWdms.size(); i++)
	{
		
		MMRESULT mmret2 = waveInStop(recordWdms[i]);
		MMRESULT mmret1 = waveInClose(recordWdms[i]);
	}
	
}
int MMECALLBACKCOUNT = 0;
void CALLBACK AudioRecordProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	
	
	//short* buffer = new short[SAMPLENUMS * 4];

	
	
	if (WIM_DATA == uMsg)
	{
		

		
		for (size_t i = 0; i < recordWdms.size(); i++)
		{
			
			if (hwi == recordWdms[i])
			{
				LPWAVEHDR pwh = (LPWAVEHDR)dwParam1;
				RecordParam* rdParam = (RecordParam*)dwInstance;
				//if (MMECALLBACKCOUNT < 4)
				{
					OutputDebugString(_T("sub thread enter\n"));
					//几个通道就除以几
					if (MMECALLBACKCOUNT / recordWdms.size()*pwh->dwBufferLength<_msize(recordDatas[i]))
						memcpy(recordDatas[i] + MMECALLBACKCOUNT / recordWdms.size()*pwh->dwBufferLength, pwh->lpData, pwh->dwBufferLength);
					
					
					//wavwrite("D:\\TestRecord\\wdmRecord1.wav", 1, 44100, 32, pwh->lpData, pwh->dwBufferLength);
					free(pwh->lpData);
					pwh->lpData = NULL;
					//waveInAddBuffer(hwi, pwh, sizeof(WAVEHDR));
					
					//for (size_t j = 0; j < 2; j++)
					{
						//waveInPrepareHeader(hwi, pwh, sizeof(WAVEHDR));
						
						//waveInPrepareHeader(hwi, &wHdr1[indexOfRecordMME[i] * 2 + j], sizeof(WAVEHDR));
						//waveInAddBuffer(hwi, &wHdr1[indexOfRecordMME[i] * 2 + j], sizeof(WAVEHDR));
						

					}
					
					
					MMECALLBACKCOUNT++;//@zcj marked at 2023.12.13,因为录音缓冲区只有一个，不会多次进入该函数
						
					//pwh->lpData = (char*)malloc(1024);
				   // waveInAddBuffer(hwi, pwh, sizeof(WAVEHDR));
				//if (hwi == recordWdms[i])
				//{
				//	char* fileName1 = "D:\\TestRecord\\wdmRecord.wav";
				//	wavwrite(fileName1, 1, 44100, 24, recordDatas[i], 44100 * 10 * 4);
				//	//wmdstep = 0;
				//}
				//else if (hwi == recordWdms[1])
				
				//if (MMECALLBACKCOUNT * 1024 > _msize(recordDatas[i]))
				{
					
					//recordWdms[i] = NULL;
				}
				
					
					//wmdstep2 = 0;
				

				

				//

				}

				
				/*free(recordDatas[i]);
				free(pwh->lpData);
				MMRESULT mmret2 = waveInStop(hwi);
				waveInReset(hwi);
				MMRESULT mmret1 = waveInClose(hwi);
				*/
			}
		}

		
		
	}
	else if (WIM_CLOSE == uMsg)
	{

	}
	else
	{
		//Sleep(10);

	}

	

}

void CALLBACK AudioRecordProc2(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{


	//short* buffer = new short[SAMPLENUMS * 4];
	if (WIM_DATA == uMsg)
	{



		for (size_t i = 0; i < recordWdms.size(); i++)
		{

			if (hwi == recordWdms[i])
			{
				LPWAVEHDR pwh = (LPWAVEHDR)dwParam1;
				RecordParam* rdParam = (RecordParam*)dwInstance;
				//if (MMECALLBACKCOUNT < 4)
				{
					OutputDebugString(_T("sub thread enter\n"));
					//几个通道就除以几
					
						memcpy(recordDatas[i], pwh->lpData, pwh->dwBufferLength);


					//wavwrite("D:\\TestRecord\\wdmRecord1.wav", 1, 44100, 32, pwh->lpData, pwh->dwBufferLength);
					free(pwh->lpData);
					pwh->lpData = NULL;
					//waveInAddBuffer(hwi, pwh, sizeof(WAVEHDR));

					//for (size_t j = 0; j < 2; j++)
					{
						//waveInPrepareHeader(hwi, pwh, sizeof(WAVEHDR));

						//waveInPrepareHeader(hwi, &wHdr1[indexOfRecordMME[i] * 2 + j], sizeof(WAVEHDR));
						//waveInAddBuffer(hwi, &wHdr1[indexOfRecordMME[i] * 2 + j], sizeof(WAVEHDR));


					}


					MMECALLBACKCOUNT++;//@zcj marked at 2023.12.13,因为录音缓冲区只有一个，不会多次进入该函数

					//pwh->lpData = (char*)malloc(1024);
					// waveInAddBuffer(hwi, pwh, sizeof(WAVEHDR));
					//if (hwi == recordWdms[i])
					//{
					//	char* fileName1 = "D:\\TestRecord\\wdmRecord.wav";
					//	wavwrite(fileName1, 1, 44100, 24, recordDatas[i], 44100 * 10 * 4);
					//	//wmdstep = 0;
					//}
					//else if (hwi == recordWdms[1])

					//if (MMECALLBACKCOUNT * 1024 > _msize(recordDatas[i]))
					{

						//recordWdms[i] = NULL;
					}


					//wmdstep2 = 0;




					//

				}


				/*free(recordDatas[i]);
				free(pwh->lpData);
				MMRESULT mmret2 = waveInStop(hwi);
				waveInReset(hwi);
				MMRESULT mmret1 = waveInClose(hwi);
				*/
			}
		}



	}
	else if (WIM_CLOSE == uMsg)
	{

	}
	else
	{
		//Sleep(10);

	}



}


DWORD WINAPI runMMERecordThread(LPVOID lParam)
{
	MSG msg;
	MMRESULT mmRet;
	RecordParam* rdParam = (RecordParam*)lParam;
	char fileName1[512];
	WAVEHDR* pHdr = NULL;
	bool isDone = false;
	
	while (GetMessage(&msg,0,0,0))
	{
		switch (msg.message)
		{


		case WIM_DATA:
			m.lock();
			pHdr = (WAVEHDR*)msg.lParam;
			
			for (size_t i = 0; i < recordWdms.size(); i++)
			{
				if ((HWAVEIN)msg.wParam == recordWdms[i])
				{
					memcpy(recordDatas[i], pHdr->lpData, pHdr->dwBufferLength);
					//mmRet = waveInUnprepareHeader((HWAVEIN)msg.wParam, (WAVEHDR*)msg.lParam, sizeof(WAVEHDR));
					mmRet = waveInPrepareHeader((HWAVEIN)msg.wParam, (WAVEHDR*)msg.lParam, sizeof(WAVEHDR));
					mmRet = waveInAddBuffer((HWAVEIN)msg.wParam, (WAVEHDR*)msg.lParam, sizeof(WAVEHDR));
					sprintf(fileName1, "D:\\TestRecord\\wdmRecord_%d.wav", i);	
					wavwrite(fileName1, rdParam->m_chs, rdParam->m_fs, rdParam->m_bits, recordDatas[i], rdParam->m_fs * rdParam->m_recordTimes * 4);
					mmRet = waveInStop((HWAVEIN)msg.wParam);
					mmRet = waveInClose((HWAVEIN)msg.wParam);
					
					free(recordDatas[i]);
					free(pHdr->lpData);
					
					isDone = true;
				}
			}

			m.unlock();
			
			break;

		case WIM_CLOSE:

			//发送完成事件
			break;
		case WIM_OPEN:


			break;
		}

		if (isDone)
		{

			break;
		}

	}

	
	return 0;

}
#define MMETHREAD_MAC 1
//void TestASIUO::writeWaveFile(const MMERecordParams& mmesparam, int nIndex, char* pData)
//{
//	char fileName1[512] = "\0";
//	if (mmesparam.mode == 0)
//	{
//		sprintf(fileName1, "D:\\TestRecord\\mic_wdmRecord_%d.wav", nIndex);
//	}
//	else
//	{
//		sprintf(fileName1, "D:\\TestRecord\\spk_wdmRecord_%d.wav", nIndex);
//	}
//
//	wavwrite(fileName1, mmesparam.chs, mmesparam.fs, mmesparam.bit, pData, mmesparam.fs * time * 4);
//}

//void TestASIUO::MMEinit(int n, double time, const MMERecordParams& mmesparam, int nIndex)
//{
//#if MMETHREAD_MAC
//	
//	stopRe();
//
//	for (size_t i = 0; i < recordDatas.size(); i++)
//	{
//		if (i == 0)
//		{
//			writeWaveFile(mmesparam, nIndex, recordDatas[i]);
//		}
//
//		delete recordDatas[i];
//	}
//
//	if (wHdr1)
//	{
//		free(wHdr1);
//		wHdr1 = NULL;
//
//	}
//#endif
//	recordWdms.clear();
//	recordDatas.clear();
//	for (size_t i = 0; i < recordThreadId.size(); i++)
//	{
//		
//		CloseHandle(recordThreadId[i]);
//	}
//	recordThreadId.clear();
//	
//	//wHdr1 = (WAVEHDR*)malloc(n*sizeof(WAVEHDR));
//}

void TestASIUO::MMEFree(double nTime, bool bSpk, int nIndex)
{
	stopRe();

	bool bTestOnce = false;
	char szFileName[256] = "\0";
	int nSlot = 0;

	if (nIndex > 0)
	{
		bTestOnce = true;
		nSlot = nIndex - 1;
	}

	for (size_t i = 0; i < recordDatas.size(); i++)
	{
		if (recordDatas[i] == NULL)
		{
			continue;
		}

		if (!bTestOnce || strlen(szFileName) == 0)
		{
			slotParam* pSlot = GetSlotParam(nSlot++);
			if (pSlot == NULL)
			{
				break;
			}

			// 找到下一个启用的通道
			while (!pSlot->bEnable)
			{
				pSlot = GetSlotParam(nSlot++);
				if (pSlot == NULL)
				{
					break;
				}
			}

			prParam* pPlayRecord = nullptr;

			if (bSpk)
			{
				pPlayRecord = GetPlayRecord(pSlot->nSpkPlayRecord, true);
				//stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);
			}
			else
			{
				pPlayRecord = GetPlayRecord(pSlot->nMicPlayRecord, false);
			}

			devParam* pDev = GetDevParam(pPlayRecord->nRecordIndex, true);

			memset(szFileName, 0, sizeof(szFileName));
			if (bSpk)
			{
				sprintf(szFileName, "D:\\TestRecord\\spk_wdmRecord_DUT%d.wav", pSlot->nSlotIndex);
			}
			else
			{
				sprintf(szFileName, "D:\\TestRecord\\mic_wdmRecord_DUT%d.wav", pSlot->nSlotIndex);
			}


			wavwrite(szFileName, 1, pDev->nSamplingRate, pDev->nBitDepth,
				recordDatas[i], pDev->nSamplingRate * nTime * 4);
			
			if (ReadBool(_T("Common"), _T("SaveFile"), false, GetConfigIniFile()))
			{

				memset(szFileName, 0, sizeof(szFileName));
				sprintf(szFileName, "D:\\TestRecord\\mic_wdmRecord_DUT%d.txt", pSlot->nSlotIndex);
				std::ofstream outfile;
				CString path(szFileName);
				outfile.open(path, std::ios::out | std::ios::trunc);

				for (size_t t = 0; t < _msize(recordDatas[i]); t++)
				{

				outfile << (int)recordDatas[i][t] << endl;
				}
				outfile.close();
			}

			
		}

		free(recordDatas[i]);
	}

	if (wHdr1)
	{
		free(wHdr1);
		wHdr1 = NULL;
	}

	recordWdms.clear();
	recordDatas.clear();
	for (size_t i = 0; i < recordThreadId.size(); i++)
	{

		CloseHandle(recordThreadId[i]);
	}
	recordThreadId.clear();

	//wHdr1 = (WAVEHDR*)malloc(n*sizeof(WAVEHDR));
}

void TestASIUO::delMMEbuffer()
{

	for (size_t i = 0; i < recordDatas.size(); i++)
	{
		delete[] recordDatas[i];
	}

	recordDatas.clear();
}

void TestASIUO::MMEinit(int n, double time, const vector<MMERecordParams>& mmesparam, int nIndex)
{
#if MMETHREAD_MAC

	stopRe();

	bool bTestOnce = false;

	for (size_t i = 0; i < recordDatas.size(); i++)
	{
		if (recordDatas[i] == NULL)
		{
			continue;
		}

		if (mmesparam[i].mode == 0)
		{
			nIndex = i;
		}

		if (!bTestOnce)
		{
			char fileName1[512] = "\0";
			if (mmesparam[nIndex].mode == 0)
			{
				sprintf(fileName1, "D:\\TestRecord\\mic_wdmRecord_%d.wav", nIndex);
			}
			else
			{
				bTestOnce = true;
				sprintf(fileName1, "D:\\TestRecord\\spk_wdmRecord_%d.wav", nIndex);
			}

			MMERecordParams stParam = mmesparam[nIndex];
			wavwrite(fileName1, 1, stParam.fs, stParam.bit, recordDatas[i], stParam.fs * time * 4);
		}

		delete recordDatas[i];
	}

	if (wHdr1)
	{
		free(wHdr1);
		wHdr1 = NULL;
	}
#endif
	recordWdms.clear();
	recordDatas.clear();
	for (size_t i = 0; i < recordThreadId.size(); i++)
	{

		CloseHandle(recordThreadId[i]);
	}
	recordThreadId.clear();

	//wHdr1 = (WAVEHDR*)malloc(n*sizeof(WAVEHDR));
}

void TestASIUO::saveMMEFile(double fs, int chs, int bit,double time)
{
	OutputDebugString(_T("del enter\n"));
	wavwrite("D:\\TestRecord\\wdmRecord1.wav", chs, fs, bit, recordDatas[0], fs * time * 4);
	recordDatas.clear();
	//delete[] recordDatas[0];
}

void TestASIUO::createMMEbuffer(int n)
{
	MMECALLBACKCOUNT = 0;
	block_mme = 0;
	indexOfRecordMME.clear();
	doubleBufferIndexmme = true;
	wHdr1 = (WAVEHDR*)malloc(1*n*sizeof(WAVEHDR));
}

void TestASIUO::createMMEbuffer()
{
	MMECALLBACKCOUNT = 0;
	block_mme = 0;
	indexOfRecordMME.clear();
	doubleBufferIndexmme = true;
	UINT ndev = waveInGetNumDevs();
	wHdr1 = (WAVEHDR*)malloc(1 * ndev * sizeof(WAVEHDR));
}

void TestASIUO::recordMME(double recordTimes, void* obj, int chs, int bits, DWORD fs, int i)
{
	
	/*for (size_t i = 0; i < recordDatas.size(); i++)
	{
		memset(recordDatas[i], 0, sizeof(recordDatas[i]));
	}*/
	

	

	//打开音频设备
	
	//LPWAVEINCAPS pwic = NULL;
	
	char* data = NULL;
	
	//for (size_t i = 0; i < ndev; i++)
	{
		
		//if (i != 1)
		{
			
			waveform.wFormatTag = WAVE_FORMAT_PCM;
			waveform.nSamplesPerSec = fs;
			waveform.wBitsPerSample = bits;
			waveform.nChannels = 1;// chs;
			waveform.nBlockAlign = waveform.nChannels*waveform.wBitsPerSample / 8;
			waveform.nAvgBytesPerSec = waveform.nBlockAlign*waveform.nSamplesPerSec;
			waveform.cbSize = 0;

		}
		/*else
		{
			waveform.wFormatTag = WAVE_FORMAT_PCM;
			waveform.nSamplesPerSec = SAMPLES;
			waveform.wBitsPerSample = 16;
			waveform.nChannels = 2;
			waveform.nBlockAlign = waveform.nChannels*waveform.wBitsPerSample / 8;
			waveform.nAvgBytesPerSec = waveform.nBlockAlign*waveform.nSamplesPerSec;
			waveform.cbSize = 0;
		}*/
		
		RecordParam* rdParam = new RecordParam;
		rdParam->m_fs = waveform.nSamplesPerSec;
		rdParam->m_recordTimes = recordTimes;
		rdParam->m_chs = 1;// waveform.nChannels;
		rdParam->m_bits = waveform.wBitsPerSample;
#if MMETHREAD_MAC
		//Sleep(20);
		MMRESULT ret = waveInOpen(&hWaveIn, i, &waveform, (DWORD_PTR)AudioRecordProc, (DWORD_PTR)rdParam, CALLBACK_FUNCTION);
		
#else
		DWORD threadId;
		HANDLE tid= CreateThread(NULL, 0, runMMERecordThread, rdParam, CREATE_SUSPENDED, &threadId);
		recordThreadId.push_back(tid);
		//i变量在这里指代mme设备的编号
		MMRESULT ret = waveInOpen(&hWaveIn, i, &waveform, (DWORD_PTR)threadId, (DWORD_PTR)rdParam, CALLBACK_THREAD);
		
#endif
		if (hWaveIn)
		{
			data = (char*)malloc(waveform.nSamplesPerSec * 4 * recordTimes);
			recordDatas.push_back(data);
			recordWdms.push_back(hWaveIn);
			WriteDebugLog(_T("采集缓存初始化！"));
		}
		else
		{
			WriteDebugLog(_T("打开录制失败！"));
			recordDatas.push_back(NULL);
			return;
		}
		



		//音频缓存初始化
		//block_mme = waveform.nSamplesPerSec * recordTimes * 4 / 2048;
		for (size_t j = 0; j < 1; j++)
		{

			//if (i != 1)
			{
				wHdr1[i * 1 + j].lpData = (char*)malloc(waveform.nSamplesPerSec * 4 * recordTimes);// new char[CHUNK_SIZE2];
				wHdr1[i * 1 + j].dwBufferLength = waveform.nSamplesPerSec * 4 * recordTimes;
				wHdr1[i * 1 + j].dwBytesRecorded = 0;
				wHdr1[i * 1 + j].dwUser = NULL;
				wHdr1[i * 1 + j].dwFlags = 0;
				wHdr1[i * 1 + j].dwLoops = 1;
				wHdr1[i * 1 + j].lpNext = NULL;
				wHdr1[i * 1 + j].reserved = 0;
			}
			/*else
			{
				wHdr1[i * 2 + j].lpData = new char[CHUNK_SIZE];
				wHdr1[i * 2 + j].dwBufferLength = CHUNK_SIZE;
				wHdr1[i * 2 + j].dwBytesRecorded = 0;
				wHdr1[i * 2 + j].dwUser = NULL;
				wHdr1[i * 2 + j].dwFlags = 0;
				wHdr1[i * 2 + j].dwLoops = 1;
				wHdr1[i * 2 + j].lpNext = NULL;
				wHdr1[i * 2 + j].reserved = 0;
			}*/

			

			waveInPrepareHeader(hWaveIn, &wHdr1[i * 1+j], sizeof(WAVEHDR));
			waveInAddBuffer(hWaveIn, &wHdr1[i * 1+j], sizeof(WAVEHDR));
			
			
		}
		
		

	}	

	indexOfRecordMME.push_back(i);
	
}

void TestASIUO::startMM()
{
	for (size_t i = 0; i < recordThreadId.size(); i++)
	{
		ResumeThread(recordThreadId[i]);
	}

	for (size_t i = 0; i < recordWdms.size(); i++)
	{
		OutputDebugString(_T("TestASIUO::startMM exter\n"));
		waveInStart(recordWdms[i]);
	}
}
HWAVEOUT hwo;
WAVEHDR wh;
void CALLBACK AudioPlayProc(HWAVEOUT shwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if (uMsg == WOM_DONE)
	{
		TestASIUO* insten = (TestASIUO*)dwInstance;
		
		
		//if (insten->getPlayCount() * 1024 < 44100 * 4)
		{
			//insten->submitPlayData(1024);
		}
	

		//if(data_ptr - temp_ptr < _msize(playdata))
		//{
		//	wh.lpData = data_ptr;
		//	wh.dwBufferLength = playbuf;
		//	wh.dwFlags = 0;
		//	wh.dwLoops = 1;
		//	wh.dwUser = NULL;
		//	wh.lpNext = NULL;
		//	wh.reserved = 0;

		//	data_ptr += playbuf;
		//	//playsize += playbuf;

		//	waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));
		//	waveOutWrite(hwo, &wh, sizeof(WAVEHDR));

		//	
		//}

		//waveOutClose(hwo);
	}
}

void TestASIUO::submitPlayData(DWORD len)
{/*
	WAVEHDR wh;
	wh.lpData = (char*)playdatamme + paly_count * 1024;
	if (44100*4-paly_count * 1024 < 1024)
	{
		wh.dwBufferLength = 44100*4 - paly_count * 1024;
	}
	else
	{
		wh.dwBufferLength = 1024;
	}
	paly_count++;
	
	wh.dwFlags = 0;
	wh.dwLoops = 0;
	wh.dwUser = NULL;
	wh.lpNext = NULL;
	wh.reserved = 0;
	waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));
	waveOutWrite(hwo, &wh, sizeof(WAVEHDR));*/

	/*wh2.lpData = (char*)playdatamme2;
	wh2.dwBufferLength = _msize(playdatamme2);
	wh2.dwFlags = 0;
	wh2.dwLoops = 0;
	wh2.dwUser = NULL;
	wh2.lpNext = NULL;
	wh2.reserved = 0;
	waveOutPrepareHeader(hwo2, &wh2, sizeof(WAVEHDR));
	waveOutWrite(hwo2, &wh2, sizeof(WAVEHDR));*/

	wh.lpData = (char*)playdatamme;
	wh.dwBufferLength = _msize(playdatamme);
	wh.dwFlags = 0;
	wh.dwLoops = 0;
	wh.dwUser = NULL;
	wh.lpNext = NULL;
	wh.reserved = 0;
	waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));
	waveOutWrite(hwo, &wh, sizeof(WAVEHDR));
	
	
}

int TestASIUO::getPlayCount()
{
	return paly_count;
}

void TestASIUO::playMME(int recordTimes, void* obj)
{
	double fs = 48000;
	int playtimes = fs*2 ;
	
	
	for (size_t i = 0; i < playtimes; i++)
	{
		playdatamme[i] = 0.1*pow(2,32)*sin(2.0*3.1415926 * 1000 * i / fs);
	}
	play_len = playtimes*4;
	WAVEFORMATEX wfx = { 0 };

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = fs;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels*wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign*wfx.nSamplesPerSec;
	

	//HANDLE wait = CreateEvent(NULL, 0, 0, NULL);


	
	waveOutOpen(&hwo, 0, &wfx, (DWORD_PTR)AudioPlayProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
	//waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0, CALLBACK_EVENT);

	

	int playbuf = 1024 ;
	paly_count = 0;
	WAVEHDR wh;
	wh.lpData = (char*)playdatamme;
	wh.dwBufferLength = play_len;
	wh.dwFlags = 0;
	wh.dwLoops = 0;
	wh.dwUser = NULL;
	wh.lpNext = NULL;
	wh.reserved = 0;
	submitPlayData(1024);
	//int playsize = 0;

	

	

	//while (data_ptr - temp_ptr<_msize(playdatamme))
	{
		
		//playsize += playbuf;

		
		

		//WaitForSingleObject(wait,INFINITE);


	}

	// waveOutPause(hwo);
	//CloseHandle(wait);
	//delete[] playdatamme;
	
	
}

void TestASIUO::playMME(int* data, double fs, int chs, int bit, int dev_id, double outvp)
{

	WAVEFORMATEX wfx = { 0 };

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = chs;
	wfx.nSamplesPerSec = fs;
	wfx.wBitsPerSample = bit;
	wfx.nBlockAlign = wfx.nChannels*wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign*wfx.nSamplesPerSec;
	playdatamme = new int[_msize(data) / 4];


	

	for (size_t i = 0; i < _msize(data) / 4; i++)
	{
		playdatamme[i] = data[i];
	}
	waveOutOpen(&hwo, dev_id, &wfx, (DWORD_PTR)AudioPlayProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
	submitPlayData(0);
}

void TestASIUO::cal_vpByMME(double fs, int chs, int bit, int dev_id, double outvp)
{
	int playtimes = fs*3;

	if (playdatamme)
		delete[] playdatamme;

	playdatamme = new int[playtimes];
	WAVEFORMATEX wfx = { 0 };

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = chs;
	wfx.nSamplesPerSec = fs;
	wfx.wBitsPerSample = bit;
	wfx.nBlockAlign = wfx.nChannels*wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign*wfx.nSamplesPerSec;
	for (size_t i = 0; i < playtimes; i++)
	{
		playdatamme[i] = 0.1*pow(2, wfx.wBitsPerSample - 1)*sqrt(2) / outvp*sin(2.0*3.1415926 * 1000 * i / fs);
	}


	


	waveOutOpen(&hwo, dev_id, &wfx, (DWORD_PTR)AudioPlayProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
	submitPlayData(0);

}

//使用wdm驱动录音的质量与麦克风有关，电脑自带麦克风录音底噪较大
void TestASIUO::openRecordThread(double time, int index)
{
	
	re_time = time;
	wdmindex_in = 0;
	SetDefaultAudio(wdmName_in, eCapture);
	std::thread record(&TestASIUO::recordThread, this);
	record.detach();//分离线程，在后台运行，不能获取其状态

}


//旧版本
void TestASIUO::playAndRecord(int ms, CString name)
{

	MCIERROR err = -1;
	int time = ms;
	char comm[1024] = { 0 };
	//err = mciSendStringA("open play.wav type waveaudio alias music", NULL, 0, NULL);
	//err = mciSendStringA("open test.mp3  alias music", NULL, 0, NULL);
	char* filename = getcharp(name);
	sprintf(comm, "open %s alias music", filename);
	err = mciSendStringA(comm, NULL, 0, NULL);
	err = mciSendStringA("set music time format ms", NULL, 0, NULL);

	


	/******************************************************/
	//err = mciSendStringA("open new type waveaudio alias rme", NULL, 0, NULL);//new type waveaudio alias rme
	err = mciSendStringA("record music overwrite", NULL, 0, NULL);
	//err = mciSendStringA("play music from 0 to 1000 wait", NULL, 0, NULL);
	sprintf(comm, "play music from 0 to %d wait", time);
	err = mciSendStringA(comm, NULL, 0, NULL);

	//Sleep(1000);
	err = mciSendStringA("stop music", NULL, 0, NULL);
	err = mciSendStringA("save music 123.wav wait", NULL, 0, NULL);

	if (err != 0)
	{
		AfxMessageBox(_T("录音出错！"));
	}
	err = mciSendStringA("close music", NULL, 0, NULL);

	err = mciSendStringA("close music", NULL, 0, NULL);

}
void TestASIUO::wdmplayAndRecord(int time, int index_in, CString playfilename, int index_out, CString inChsName,double fs)
{
	palyname = playfilename;
	wdmindex_out = index_out;

	re_time = time*1000;
	wdmindex_in = index_in;

	
	m_fs = fs;
	//设置默认通道
	wdmName_in = inChsName;
	SetDefaultAudio(wdmName_out, eRender);
	SetDefaultAudio(wdmName_in, eCapture);

	wdmindex_in = getWdmChsIndex(true, inChsName);
	wdmindex_out = getWdmChsIndex(false, wdmName_out);
	
	wdmplayAndRecordthread();
	/*std::thread record(&TestASIUO::wdmplayAndRecordthread, this);
	record.detach();*/



}

void TestASIUO::mmeplayAndRecord(double fs, int bits, int recordmmeIndex, int playmmeIndex, double outvp)
{
	//录音
	recordWdms.clear();
	if (recordDatas.size() > 0)
	{
		delete[] recordDatas[0];
	}
	recordDatas.clear();
	char* data = NULL;
	waveform.wFormatTag = WAVE_FORMAT_PCM;
	waveform.nSamplesPerSec = fs;
	waveform.wBitsPerSample = bits;
	waveform.nChannels = 1;// chs;
	waveform.nBlockAlign = waveform.nChannels*waveform.wBitsPerSample / 8;
	waveform.nAvgBytesPerSec = waveform.nBlockAlign*waveform.nSamplesPerSec;
	waveform.cbSize = 0;


	MMRESULT ret = waveInOpen(&hWaveIn, recordmmeIndex, &waveform, (DWORD_PTR)AudioRecordProc, (DWORD_PTR)0, CALLBACK_FUNCTION);


	if (hWaveIn)
	{
		data = (char*)malloc(waveform.nSamplesPerSec * 4 * 3);
		recordDatas.push_back(data);
		recordWdms.push_back(hWaveIn);

	}




	//音频缓存初始化
	//block_mme = waveform.nSamplesPerSec * recordTimes * 4 / 2048;
	for (size_t j = 0; j < 1; j++)
	{

		
		{
			wHdr1[recordmmeIndex * 1 + j].lpData = (char*)malloc(waveform.nSamplesPerSec * 4 * 3);// new char[CHUNK_SIZE2];
			wHdr1[recordmmeIndex * 1 + j].dwBufferLength = waveform.nSamplesPerSec * 4 * 3;
			wHdr1[recordmmeIndex * 1 + j].dwBytesRecorded = 0;
			wHdr1[recordmmeIndex * 1 + j].dwUser = NULL;
			wHdr1[recordmmeIndex * 1 + j].dwFlags = 0;
			wHdr1[recordmmeIndex * 1 + j].dwLoops = 1;
			wHdr1[recordmmeIndex * 1 + j].lpNext = NULL;
			wHdr1[recordmmeIndex * 1 + j].reserved = 0;
		}
		



		waveInPrepareHeader(hWaveIn, &wHdr1[recordmmeIndex * 1 + j], sizeof(WAVEHDR));
		waveInAddBuffer(hWaveIn, &wHdr1[recordmmeIndex * 1 + j], sizeof(WAVEHDR));


	}


	startMM();//开始录音


	//播放

	if (playmmeIndex!=-1)
		cal_vpByMME(fs, 1, bits, playmmeIndex, outvp);


}

//新版本 wdm播放与录音函数，带通道选择与噪音消除处理
void TestASIUO::wdmplayAndRecordthread()
{
	//先执行录音
	wdmrecorddone = false;
	MCIERROR err = -1;
	char comm[1024];

	char* filename = getcharp(palyname);
	sprintf(comm, "open %s alias music", filename);
	err = mciSendStringA(comm, NULL, 0, NULL);

	//获取文件时长
	
	char filelen[100]{0};
	err = mciSendStringA("status music length", filelen, sizeof(filelen), NULL);

	re_time=atof(filelen);

	err = mciSendStringA("open new type waveaudio alias recorder", NULL, 0, NULL);
	err = mciSendStringA("set recorder bitspersample  16", "", 0, NULL);//16
	memset(comm, 0, 1024);
	sprintf(comm, "set recorder samplespersec %d", (int)m_fs);
	err = mciSendStringA(comm, "", 0, NULL);
	if (m_fs == 44100)
	{
		//err = mciSendStringA("set recorder bitspersample  24", "", 0, NULL);//16
		if (err != 0 && mainf)
			mainf->setStatusStr(_T("WDM设置量化位数失败..."));
		err = mciSendStringA("set recorder channels 2", "", 0, NULL);//2
	}
	else if (m_fs == 96000)
	{
		//err = mciSendStringA("set recorder bitspersample  16", "", 0, NULL);//16
		err = mciSendStringA("set recorder channels 2", "", 0, NULL);//1
	}
	else if (m_fs == 192000)
	{
		//err = mciSendStringA("set recorder bitspersample  16", "", 0, NULL);//24
		err = mciSendStringA("set recorder channels 1", "", 0, NULL);//2
	}

	/*memset(comm, 0, 1024);
	sprintf(comm, "set recorder samplespersec %d", (int)m_fs);
	err = mciSendStringA(comm, "", 0, NULL);*/
	
	err = mciSendStringA("set recorder format tag pcm", "", 0, 0);
	memset(comm, 0, 1024);
	sprintf(comm, "set recorder input %d", wdmindex_in);
	err = mciSendStringA(comm, NULL, 0, NULL);

	

	
	err = mciSendStringA("record recorder overwrite", NULL, 0, 0);
	
	if (err != 0 && mainf)
	    mainf->setStatusStr(_T("WDM开启录音失败..."));

	/***************这里执行wdm播放功能*********************/

//	int time = re_time;
	/*char* filename = getcharp(palyname);
	sprintf(comm, "open %s alias music", filename);
	err = mciSendStringA(comm, NULL, 0, NULL);*/
	memset(comm, 0, 1024);
	sprintf(comm, "set music output %d", wdmindex_out);
	err = mciSendStringA(comm, NULL, 0, NULL);
	err = mciSendStringA("set music time format ms", NULL, 0, NULL);

	memset(comm, 0, 1024);
	sprintf(comm, "play music from 0 to %d wait", re_time);
	err = mciSendStringA(comm, NULL, 0, NULL);
	if (err != 0 && mainf)
		mainf->setStatusStr(_T("WDM开启播放失败..."));
	

	delete[] filename;

	Sleep(500);

	/**************************************/
	err = mciSendStringA("stop music", 0, 0, NULL);
	err = mciSendStringA("stop recorder", 0, 0, NULL);
	err = mciSendStringA("save recorder D:\\TestRecord\\wdmRecord1.wav", NULL, 0, NULL);
	err = mciSendStringA("close music", NULL, 0, NULL);
	err = mciSendStringA("close recorder", NULL, 0, NULL);


	wdmrecorddone = true;

}



void TestASIUO::LoadYamahaASIODriever()
{

	hModuleDll = NULL;
	init = NULL;

	ASIOError ret = -1;
	ASIOBool bok = -1;
	typedef HRESULT(__stdcall *PDllGetClassObject)(REFCLSID, REFIID, void**);
	typedef HRESULT(_stdcall *PDllRegisterServer)();
	hModuleDll = LoadLibrary(_T("ysusb_asio64.dll"));
	PDllGetClassObject DllGetClassObject = NULL;
	DllGetClassObject = (PDllGetClassObject)GetProcAddress(hModuleDll, "DllGetClassObject");

	PDllRegisterServer DllRegisterServer = NULL;
	DllRegisterServer = (PDllRegisterServer)GetProcAddress(hModuleDll, "DllRegisterServer");
	if (DllRegisterServer)
	{
		DllRegisterServer();
	}

	IClassFactory* pcf = NULL;
	HRESULT hr = (DllGetClassObject)(YM, IID_IClassFactory, (void**)&samp);
	hr = 0;
	hr = (DllGetClassObject)(YM, IID_IClassFactory, (void**)&pcf);


	hr = pcf->CreateInstance(NULL, YM, (void**)&samp);



	bok = samp->init(NULL);

	long in = 0;
	long out = 0;
	samp->getChannels(&in, &out);



	char drivename[512] = { 0 };
	samp->getDriverName(drivename);
	



}


CString TestASIUO::GetLastErrMsg()
{
	return m_sLastErrMsg;
}

/******************ASIO驱动初始化***************************/
//这里读取不同厂家的驱动，开发阶段写死在程序中，后面发布版本改到配置文件方式
// 参数 ： type
//   0  ：瑞森驱动
//   1  ： 雅马哈驱动
//   2  ： LISSIG (黎声科技)
//   3  ： RME 驱动
//设备管理中每一条对应一个驱动，因此这里可以持有不同设备驱动
void TestASIUO::LoadRSASIO(vector<wstring>& driver, vector<string>& ch, bool binput, int type)
{
	
	driver.clear();
	ch.clear();
	m_sLastErrMsg = _T("");

	hModuleDll = NULL;
	init = NULL;

	ASIOError ret = -1;
	ASIOBool bok = -1;





	typedef HRESULT(__stdcall *PDllGetClassObject)(REFCLSID, REFIID, void**);
	typedef HRESULT (_stdcall *PDllRegisterServer)();

	typedef HRESULT(*PDllInitClasses)(BOOL);
	typedef ASIOError (*PASIOInit)(ASIODriverInfo *info);
	//RstechUsbAudioasio_x64.dll
	CLSID driverID;

	//if (initcardtimes1 == 1||
	//	initcardtimes2 == 1 ||
	//	initcardtimes3 == 1 ||
	//	initcardtimes4 == 1)
	//{
	//	long in = 0;
	//	long out = 0;
	//	samp->getChannels(&in, &out);
	//	
	//	/**************************加载asio通道信息********************************/
	//	if (binput)
	//	{
	//		ASIOChannelInfo* channelinfo = new ASIOChannelInfo[in];
	//		for (size_t i = 0; i < in; i++)
	//		{
	//			channelinfo[i].channel = i;
	//			channelinfo[i].isInput = ASIOTrue;
	//			samp->getChannelInfo(&channelinfo[i]);

	//			ch.push_back(channelinfo[i].name);
	//		}
	//		delete[] channelinfo;

	//	}
	//	else
	//	{
	//		ASIOChannelInfo* channelinfo = new ASIOChannelInfo[out];
	//		for (size_t i = 0; i < out; i++)
	//		{
	//			channelinfo[i].channel = i;
	//			channelinfo[i].isInput = ASIOFalse;
	//			samp->getChannelInfo(&channelinfo[i]);
	//			ch.push_back(channelinfo[i].name);
	//		}
	//		delete[] channelinfo;

	//	}

	//	return;
	//}
	if (type==0)
	{
		hModuleDll = LoadLibrary(_T("RstechUsbAudioasio_x64.dll"));
		driverID = RS;
		initcardtimes1++;
		initcardtimes2 = 0;
		initcardtimes3 = 0;
		initcardtimes4 = 0;
		
	}
	else if (type == 1)
	{
		hModuleDll = LoadLibrary(_T("ysusb_asio64.dll"));
		driverID = YM;
		initcardtimes2++;
		initcardtimes1 = 0;
		initcardtimes3 = 0;
		initcardtimes4 = 0;
	}
	else if (type == 2)
	{
		hModuleDll = LoadLibrary(_T("C:\\Program Files\\LISSIG\\USB Audio Device Driver\\x64\\LISSIGUsbAudioasio_x64.dll"));
		driverID = LI;
		initcardtimes3++;
		initcardtimes1 = 0;
		initcardtimes2 = 0;
		
		initcardtimes4 = 0;
	}
	else if (type == 3)
	{
		hModuleDll = LoadLibrary(_T("fireface_usb_asio_64.dll"));
		driverID = RME;
		initcardtimes4++;
		initcardtimes1 = 0;
		initcardtimes2 = 0;
		initcardtimes3 = 0;
		
	}
	
	//hModuleDll = LoadLibrary(_T("LISSIGUsbAudioasio_x64.dll"));

	PDllGetClassObject DllGetClassObject = NULL;
	PDllInitClasses    DllInitClasses = NULL;
	/*ploadAsioDriver loadAsioDriver=NULL;
	loadAsioDriver = (ploadAsioDriver)GetProcAddress(hModuleDll, "loadAsioDriver");*/
	DllInitClasses = (PDllInitClasses)GetProcAddress(hModuleDll, "DllInitClasses");
	if (DllInitClasses!=NULL)
	    DllInitClasses(1);

	PASIOInit ASIOInit = NULL;
	ASIOInit = (PASIOInit)GetProcAddress(hModuleDll, "ASIOInit");
	DllGetClassObject = (PDllGetClassObject)GetProcAddress(hModuleDll, "DllGetClassObject");

	PDllRegisterServer DllRegisterServer = NULL;
	DllRegisterServer = (PDllRegisterServer)GetProcAddress(hModuleDll, "DllRegisterServer");
	if (DllRegisterServer)
	{
		DllRegisterServer();
	}
	IClassFactory* pcf = NULL;
	if (samp)
	{
		samp->disposeBuffers();
		samp->Release();
		//delete samp;
		samp = NULL;
	}
	HRESULT hr = (DllGetClassObject)(driverID, IID_IClassFactory, (void**)&samp);
	hr = 0;
	hr = (DllGetClassObject)(driverID, IID_IClassFactory, (void**)&pcf);

	if (pcf)
	    hr = pcf->CreateInstance(NULL, driverID, (void**)&samp);

	if (samp == NULL)
	{
		AfxMessageBox(_T("声卡驱动初始化失败！"));
		mainf = (CMainFrame*)theApp.GetMainWnd();
		mainf->setStatusStr(CString("声卡驱动初始化失败！"));
		m_sLastErrMsg = _T("声卡驱动初始化失败！");
		return;
	}
	
	bok = samp->init(NULL);

	long in = 0;
	long out = 0;
	samp->getChannels(&in, &out);





	char drivename[512] = { 0 };
	samp->getDriverName(drivename);
	//driver.push_back(drivename);


	long  minsize, maxsize, preferredSize, granularity;

	ret = samp->getBufferSize(&minsize, &maxsize, &preferredSize, &granularity);

	if (preferredSize < 0)
	{
		//AfxMessageBox(_T("ASIO声卡设备不存在!!!"));
		m_sLastErrMsg = CString(drivename) + _T(" ASIO声卡设备不存在, 请重新配置!");
		return;
	}
	
	//ret = samp->setSampleRate(44100.0);



	ASIODriverInfo* info;



	/*for (size_t i = 0; i < in; i++)
	{
		buf[i].isInput = ASIOTrue;
		buf[i].channelNum = i;
		buf[i].buffers[0] = buf[i].buffers[1] = 0;
	}
	for (size_t i = 0; i < out; i++)
	{
		buf[in + i].isInput = ASIOFalse;
		buf[in + i].channelNum = i;
		buf[in + i].buffers[0] = buf[i].buffers[1] = 0;
	}*/

	/**************************加载asio通道信息********************************/
	if (binput)
	{
		ASIOChannelInfo* channelinfo = new ASIOChannelInfo[in];
		for (size_t i = 0; i < in; i++)
		{
			channelinfo[i].channel = i;
			channelinfo[i].isInput = ASIOTrue;
			samp->getChannelInfo(&channelinfo[i]);

			ch.push_back(channelinfo[i].name);
		}
		delete[] channelinfo;

	}
	else
	{
		ASIOChannelInfo* channelinfo = new ASIOChannelInfo[out];
		for (size_t i = 0; i < out; i++)
		{
			channelinfo[i].channel = i;
			channelinfo[i].isInput = ASIOFalse;
			samp->getChannelInfo(&channelinfo[i]);
			ch.push_back(channelinfo[i].name);
		}
		delete[] channelinfo;

	}
	
	ASIOCallbacks callback;
	callback.asioMessage = asioMessage;
	callback.bufferSwitch = bufferSwitch;
	callback.sampleRateDidChange = sampleRateDidChange;


	//ret = samp->createBuffers(buf, in + out, preferredSize, &callback);

	//if (ret != ASE_OK)
	//{
	//	initRME = false;
	//	//AfxMessageBox(_T("asio driver init failed!"));
	//}
	//else
	//{
	//	initRME = true;
	//	//AfxMessageBox(_T("init rme car success!"));
	//}


	/*  录音 播放以下代码放开
	//ret = samp->start();
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	GetThreadId(hThreadId);

	while (initRME)
	{
		if (len == 0 && !bstop&&!bfirstrun)
		{
			calldaw();
			bstop = true;
			bfirstrun = true;
			StopRME();
		}



		//Sleep(500);
	}

	*/

}

void TestASIUO::LoadRSASIO2(vector<wstring>& driver, vector<string>& ch, bool binput, int type)
{

	driver.clear();
	ch.clear();

	hModuleDll = NULL;
	init = NULL;

	ASIOError ret = -1;
	ASIOBool bok = -1;





	typedef HRESULT(__stdcall *PDllGetClassObject)(REFCLSID, REFIID, void**);
	typedef HRESULT(_stdcall *PDllRegisterServer)();

	typedef HRESULT(*PDllInitClasses)(BOOL);
	typedef ASIOError(*PASIOInit)(ASIODriverInfo *info);
	//RstechUsbAudioasio_x64.dll
	CLSID driverID;

	
	if (type == 0)
	{
		hModuleDll = LoadLibrary(_T("RstechUsbAudioasio_x64.dll"));
		driverID = RS;
		initcardtimes1++;
		initcardtimes2 = 0;
		initcardtimes3 = 0;
		initcardtimes4 = 0;

	}
	else if (type == 1)
	{
		hModuleDll = LoadLibrary(_T("ysusb_asio64.dll"));
		driverID = YM;
		initcardtimes2++;
		initcardtimes1 = 0;
		initcardtimes3 = 0;
		initcardtimes4 = 0;
	}
	else if (type == 2)
	{
		hModuleDll = LoadLibrary(_T("C:\\Program Files\\LISSIG\\USB Audio Device Driver\\x64\\LISSIGUsbAudioasio_x64.dll"));
		driverID = LI;
		initcardtimes3++;
		initcardtimes1 = 0;
		initcardtimes2 = 0;

		initcardtimes4 = 0;
	}
	else if (type == 3)
	{
		hModuleDll = LoadLibrary(_T("fireface_usb_asio_64.dll"));
		driverID = RME;
		initcardtimes4++;
		initcardtimes1 = 0;
		initcardtimes2 = 0;
		initcardtimes3 = 0;

	}

	//hModuleDll = LoadLibrary(_T("LISSIGUsbAudioasio_x64.dll"));

	PDllGetClassObject DllGetClassObject = NULL;
	PDllInitClasses    DllInitClasses = NULL;
	/*ploadAsioDriver loadAsioDriver=NULL;
	loadAsioDriver = (ploadAsioDriver)GetProcAddress(hModuleDll, "loadAsioDriver");*/
	DllInitClasses = (PDllInitClasses)GetProcAddress(hModuleDll, "DllInitClasses");
	if (DllInitClasses != NULL)
		DllInitClasses(1);

	PASIOInit ASIOInit = NULL;
	ASIOInit = (PASIOInit)GetProcAddress(hModuleDll, "ASIOInit");
	DllGetClassObject = (PDllGetClassObject)GetProcAddress(hModuleDll, "DllGetClassObject");

	PDllRegisterServer DllRegisterServer = NULL;
	DllRegisterServer = (PDllRegisterServer)GetProcAddress(hModuleDll, "DllRegisterServer");
	if (DllRegisterServer)
	{
		DllRegisterServer();
	}
	IClassFactory* pcf = NULL;
	//if (samp)
	//{
	//	samp->disposeBuffers();
	//	samp->Release();
	//	//delete samp;
	//	samp = NULL;
	//}
	HRESULT hr = (DllGetClassObject)(driverID, IID_IClassFactory, (void**)&samp);
	hr = 0;
	hr = (DllGetClassObject)(driverID, IID_IClassFactory, (void**)&pcf);

	if (pcf)
		hr = pcf->CreateInstance(NULL, driverID, (void**)&samp);

	if (samp == NULL)
	{
		//AfxMessageBox(_T("声卡驱动初始化失败！"));
		mainf = (CMainFrame*)theApp.GetMainWnd();
		mainf->setStatusStr(CString("声卡驱动初始化失败！"));
		return;
	}

	bok = samp->init(NULL);


	m_ASIODriverList[type] = samp;


}



int TestASIUO::createASIOBuffer(double device_fs)
{
	
	ASIOError ret = true;

	ASIOCallbacks callback;
	callback.asioMessage = asioMessage;
	callback.bufferSwitch = bufferSwitch;
	callback.sampleRateDidChange = sampleRateDidChange;

	long  minsize, maxsize, preferredSize, granularity;
	preferredSize = -1;
	if (!samp)
	{
		AfxMessageBox(_T("声卡未初始化"));
		return -1;
	}
	//ASIOBool berr = samp->init(NULL);
	//long inlat, outlat;
	//samp->getLatencies(&inlat, &outlat);
	ret = samp->getBufferSize(&minsize, &maxsize, &preferredSize, &granularity);
	if (preferredSize < 0)
	{
		AfxMessageBox(_T("检查声卡设备是否存在！"));
		return -2;
	}
	buffsize = preferredSize;
	long in = 0;
	long out = 0;
	samp->getChannels(&in, &out);
	device_in = in;
	//globle_out = out;
	buf = new ASIOBufferInfo[in + out];

	pld = new int[buffsize];

	//先是输入 再定输出
	for (size_t i = 0; i < in; i++)
	{
		buf[i].isInput = ASIOTrue;
		buf[i].channelNum = i;
		buf[i].buffers[0] = buf[i].buffers[1] = 0;

	}
	for (size_t i = 0; i < out; i++)
	{
		buf[in + i].isInput = ASIOFalse;
		buf[in + i].channelNum = i;
		buf[in + i].buffers[0] = buf[i].buffers[1] = 0;
	}

	ret=samp->setSampleRate(device_fs);
	if (ret != ASE_OK)
	{
		initRME = false;
	}
	
	
	   ret = samp->createBuffers(buf, in + out, preferredSize, &callback);

	if (ret != ASE_OK)
	{
		initRME = false;
		//AfxMessageBox(_T("asio driver init failed!"));
	}
	else
	{
		initRME = true;
		//AfxMessageBox(_T("init rme car success!"));
	}

	
	  //录音 播放以下代码放开
	//ret = samp->start();
	
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	GetThreadId(hThreadId);

	

	ret = samp->start();


	//ret = samp->outputReady();


	if (ret != 0 || !initRME)
	{
		//AfxMessageBox(_T("StartRME err"));
	}

	

	
	
	return ret;
}

int TestASIUO::createASIOBuffer2(double device_fs)
{

	ASIOError ret = true;

	ASIOCallbacks callback;
	callback.asioMessage = asioMessage;
	callback.bufferSwitch = bufferSwitch2;
	callback.sampleRateDidChange = sampleRateDidChange;

	long  minsize, maxsize, preferredSize, granularity;
	preferredSize = -1;
	if (!samp)
	{
		AfxMessageBox(_T("声卡未初始化"));
		return false;
	}
	//ASIOBool berr = samp->init(NULL);
	//long inlat, outlat;
	//samp->getLatencies(&inlat, &outlat);
	ret = samp->getBufferSize(&minsize, &maxsize, &preferredSize, &granularity);
	if (preferredSize < 0)
	{
		AfxMessageBox(_T("检查声卡设备是否存在！"));
		return false;
	}
	buffsize = preferredSize;
	long in = 0;
	long out = 0;
	samp->getChannels(&in, &out);
	device_in = in;
	//globle_out = out;
	buf = new ASIOBufferInfo[in + out];

	pld = new int[buffsize];
	bufferRecord = new int[buffsize];
	record_data.push_back(bufferRecord);

	//先是输入 再定输出
	for (size_t i = 0; i < in; i++)
	{
		buf[i].isInput = ASIOTrue;
		buf[i].channelNum = i;
		buf[i].buffers[0] = buf[i].buffers[1] = 0;

	}
	for (size_t i = 0; i < out; i++)
	{
		buf[in + i].isInput = ASIOFalse;
		buf[in + i].channelNum = i;
		buf[in + i].buffers[0] = buf[i].buffers[1] = 0;
	}

	ret = samp->setSampleRate(device_fs);
	if (ret != ASE_OK)
	{
		initRME = false;
	}


	ret = samp->createBuffers(buf, in + out, preferredSize, &callback);

	if (ret != ASE_OK)
	{
		initRME = false;
		//AfxMessageBox(_T("asio driver init failed!"));
	}
	else
	{
		initRME = true;
		//AfxMessageBox(_T("init rme car success!"));
	}


	//录音 播放以下代码放开
	//ret = samp->start();

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	GetThreadId(hThreadId);



	ret = samp->start();


	//ret = samp->outputReady();


	if (ret != 0 || !initRME)
	{
		//AfxMessageBox(_T("StartRME err"));
	}





	return ret;
}


/*********RME ASIO驱动初始化******************/
void TestASIUO::LoadDll()
{

	

	hModuleDll = NULL;
	init = NULL;

	ASIOError ret = -1;
	ASIOBool bok = -1;

	

	

	typedef HRESULT(__stdcall *pfnHello)(REFCLSID, REFIID, void**);
	typedef HRESULT(_stdcall *PDllRegisterServer)();
	//RstechUsbAudioasio_x64.dll
	hModuleDll = LoadLibrary(_T("fireface_usb_asio_64.dll"));
	//hModuleDll = LoadLibrary(_T("LISSIGUsbAudioasio_x64.dll"));
	//hModuleDll = LoadLibrary(_T("RstechUsbAudioasio_x64.dll"));

	pfnHello fHello = NULL;
	
	fHello = (pfnHello)GetProcAddress(hModuleDll, "DllGetClassObject");
	
	/********************这里注册其他驱动************************/
	PDllRegisterServer DllRegisterServer = NULL;
	DllRegisterServer = (PDllRegisterServer)GetProcAddress(hModuleDll, "DllRegisterServer");
	if (DllRegisterServer)
	{
		DllRegisterServer();
	}


	IClassFactory* pcf = NULL;
	HRESULT hr = (fHello)(RME, IID_IClassFactory, (void**)&pcf);

	
	hr = pcf->CreateInstance(NULL, RME, (void**)&samp);

	bok = samp->init(NULL);

	long in=0;
	long out=0;
	samp->getChannels(&in, &out);

	
	buf = new ASIOBufferInfo[in + out];
	


	char drivename[512] = { 0 };
	samp->getDriverName(drivename);
	
	long  minsize, maxsize, preferredSize, granularity;

	ret=samp->getBufferSize(&minsize, &maxsize, &preferredSize, &granularity);
	

	ret=samp->setSampleRate(44100.0);
	
	
	
	ASIODriverInfo* info;

	
	
	for (size_t i = 0; i < in; i++)
	{
		buf[i].isInput = ASIOTrue;
		buf[i].channelNum = i;
		buf[i].buffers[0] = buf[i].buffers[1] = 0;
	}
	for (size_t i = 0; i < out; i++)
	{
		buf[in + i].isInput = ASIOFalse;
		buf[in + i].channelNum = i;
		buf[in + i].buffers[0] = buf[i].buffers[1] = 0;
	}

	ASIOChannelInfo channelinfo[30];
	for (size_t i = 0; i < out; i++)
	{
		channelinfo[i].channel = i;
		channelinfo[i].isInput = 0;
		samp->getChannelInfo(&channelinfo[i]);
	}
	
	ASIOCallbacks callback;
	callback.asioMessage = asioMessage;
	callback.bufferSwitch = bufferSwitch;
	callback.sampleRateDidChange = sampleRateDidChange;
	
	
	ret = samp->createBuffers(buf, in+out, preferredSize, &callback);
	
	if (ret != ASE_OK)
	{
		initRME = false;
		AfxMessageBox(_T("createBuffers err"));
	}
	else
	{
		initRME = true;
		//AfxMessageBox(_T("init rme car success!"));
	}

	

	//ret = samp->start();
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);

	GetThreadId(hThreadId);

	while (initRME)
	{
		if (len == 0 && !bstop&&!bfirstrun)
		{
			calldaw();
			bstop = true;
			bfirstrun = true;
			StopRME();
			delete[] buf;
		}

		
		    
		//Sleep(500);
	}

	

}

bool TestASIUO::initRMECard()
{
	TerminateThread(hThreadId, 0);
	if (initRME)
	{
		std::thread testrme(&TestASIUO::LoadDll, this);
		testrme.detach();
	}
else
	AfxMessageBox(_T("init card failed"));


	return initRME;
}

bool TestASIUO::StartRME()
{
	if (samp == NULL)
	{
		AfxMessageBox(_T("检查硬件连接！"));
		return false;
	}
		

	ASIOError ret = -1;

	ASIOChannelInfo channelinfo;
	channelinfo.channel = 0;
	channelinfo.isInput = 0;
	if (samp->getChannelInfo(&channelinfo) != 0)
	{
		AfxMessageBox(_T("hardware err"));
		return false;
	}

	//if (jiliset.data.size() == 0)
	//{
	//	
	//	/*TerminateThread(hThreadId, 0);
	//	initRME = false;*/
	//	return false;
	//}
		
	//playdata = new int[jiliset.data.size()]{0};
	//for (size_t i = 0; i < jiliset.data.size(); i++)
	//{
	//	//playdata[i] = valote*1000000000*sin(2.0*3.1415926 * f * i / 44100.0);
	//	//playdata[i] = 10*sin(2.0*3.1415926 * f * i / 44100.0);
	//	//playdata[i] = jiliset.data[i];

	//}
	

	 len = 44100 * 4;//播放字节数
	 step = 0;
	 bstop = false;
	 bfirstrun = false;
	
	ret=samp->start();

	
	ret=samp->outputReady();
	

	if (ret != 0 || !initRME)
	{
		AfxMessageBox(_T("StartRME err"));
		return false;
	}

	return true;
}

void TestASIUO::StopRME()
{
	if (samp == NULL)
		return ;
	ASIOError ret = -1;

	
	ret=samp->disposeBuffers();
	ret = samp->stop();
	
	if (ret != 0)
	{
		//AfxMessageBox(_T("StopRME err"));
	}
}

void TestASIUO::SetCallBackFunc(CallBackDraw call)
{

	calldaw = call;

}

bool TestASIUO::LoadWDM(vector<wstring>& driver, vector<int>& ch, bool isinput)
{
	driver.clear();
	ch.clear();
	bool b_load = true;


	IMMDeviceEnumerator *pEnum = NULL;
	LPWSTR setstr = NULL;
	IMMDeviceCollection *pCollection = NULL;
	IPropertyStore *pStore = NULL;

	HRESULT hr2 = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)&pEnum);

	PCWSTR tempstr;

	if (isinput)
	{
		UINT ndev = waveInGetNumDevs();
		//UINT ndev = waveOutGetNumDevs();
		WAVEINCAPS pwic;
		//WAVEOUTCAPS pwic;

		int ret = -1;
		for (size_t i = 0; i < ndev; i++)
		{
			//MMSYSERR_BADDEVICEID
			ret = waveInGetDevCaps(i, &pwic, sizeof(WAVEINCAPS));
			//ret = waveOutGetDevCaps(i, &pwic, sizeof(WAVEOUTCAPS));

			//driver.push_back(pwic.szPname);

			ch.push_back(pwic.wChannels);


		}


		if (SUCCEEDED(hr2))
		{
			IMMDevice* pDevice = NULL;

			//hr2 = pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);

			pEnum->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pCollection);


			if (FAILED(hr2))
			{
				if (pEnum)
				{
					pEnum->Release();
					return false;
				}
			}

			UINT count;

			pCollection->GetCount(&count);
			if (FAILED(hr2))
			{
				if (pCollection)
				{
					pCollection->Release();
				}
				if (pEnum)
				{
					pEnum->Release();

				}
				return false;
			}

			for (UINT i = 0; i < count; i++)
			{
				pCollection->Item(i, &pDevice);
				//CString ss;
				
				if (SUCCEEDED(hr2))
				{


					hr2 = pDevice->OpenPropertyStore(STGM_READ, &pStore);
					LPWSTR* ss = NULL;
					pDevice->GetId(ss);
					//CPolicyConfigClient* pcobfg;
					//hr2 = pDevice->Activate(__uuidof(MMDeviceEnumerator), CLSCTX_ALL, NULL, (void**)&pcobfg);
					if (SUCCEEDED(hr2))
					{
						PROPVARIANT friendlyName;

						PropVariantInit(&friendlyName);

						pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
						if (SUCCEEDED(hr2))
						{
							driver.push_back(friendlyName.pwszVal);

						}

					}
				}


			}
		}


		

	}
	else
	{
		UINT ndev = waveOutGetNumDevs();
		WAVEOUTCAPS pwic;

		int ret = -1;
		for (size_t i = 0; i < ndev; i++)
		{
			ret = waveOutGetDevCaps(i, &pwic, sizeof(WAVEOUTCAPS));

			//driver.push_back(getcharp(pwic.szPname));

			ch.push_back(pwic.wChannels);


		}

		if (SUCCEEDED(hr2))
		{
			IMMDevice* pDevice = NULL;

			//hr2 = pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);

			pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);


			if (FAILED(hr2))
			{
				if (pEnum)
				{
					pEnum->Release();
					return false;
				}
			}

			UINT count;

			pCollection->GetCount(&count);
			if (FAILED(hr2))
			{
				if (pCollection)
				{
					pCollection->Release();
				}
				if (pEnum)
				{
					pEnum->Release();

				}
				return false;
			}

			for (UINT i = 0; i < count; i++)
			{
				pCollection->Item(i, &pDevice);
				if (SUCCEEDED(hr2))
				{


					hr2 = pDevice->OpenPropertyStore(STGM_READ, &pStore);
					//CPolicyConfigClient* pcobfg;
					//hr2 = pDevice->Activate(__uuidof(MMDeviceEnumerator), CLSCTX_ALL, NULL, (void**)&pcobfg);
					if (SUCCEEDED(hr2))
					{
						PROPVARIANT friendlyName;

						PropVariantInit(&friendlyName);

						pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
						if (SUCCEEDED(hr2))
						{
							driver.push_back(friendlyName.pwszVal);

						}

					}
				}


			}
		}


	}

	


	return b_load;
}

bool LoadASIO(vector<string>& driver, vector<int>& ch, bool isinput)
{
	driver.clear();
	ch.clear();
	bool b_load = true;

	if (isinput)
	{


	}
	else
	{

	}

	return b_load;
}
//[2,3]  [d1,d1]
//int ddd = 0;
//count 实际需要播放的通道数
void TestASIUO::playASIO(int* index, int**data, int count,double device_fs)
{
	if (data == NULL || *data == NULL)
		return;

	globle_out = count;
	playdata = new int*[count];
	playindex = new int[count];
	for (size_t i = 0; i < count; i++)
	{

		playdata[i] = data[i];
		playindex[i] = index[i];

	}

	len = _msize(data[0]);//播放字节数
	step = 0;
	bstop = false;
	bfirstrun = false;
	//if (ddd==0)
	if (createASIOBuffer(device_fs) < 0)
		return;
		
	
	
	//ddd++;

	while (initRME)
	{
		if (len == 0 && !bstop&&!bfirstrun)
		{
			//calldaw();
			bstop = true;
			bfirstrun = true;
			StopRME();
			initRME = false;
			//delete[] buf;
			//delete[] pld;

		}



		//Sleep(500);
	}
	/*std::thread thread_draw_bitmap(&TestASIUO::createASIOBuffer, this);
	thread_draw_bitmap.detach();*/
	
	delete[] playdata;
	delete[] playindex;


}

void TestASIUO::setplaydata(int**pldata, int count, int* playchs)
{
	
	playdata = pldata;
	globle_out = count;
	playindex = new int[count]{0};
	for (size_t i = 0; i < count; i++)
	{
		playindex[i] = playchs[i];
	}

}

void TestASIUO::setIsNoPlay(bool bplay)
{
	m_IsNoPlay = bplay;
}


void TestASIUO::recordASIO(int* index, int***data, int count, double device_fs, double recordtime)
{

	asioBufswitch = false;
	bstopASIO = true;
	rd = new int*[count]{0};
	recordindex = index;
	globle_in = count;

	

	double prerecordtime = 0.05;// 0.05;
	if (recordtime != -1)
	{
		size_t rdlen = device_fs*recordtime;

		len = rdlen * 4;//播放字节数
		if (m_IsNoPlay)
		{
			len = 0;//针对奋斗项目，不播放
			globle_out = 0;
		}
		recordlen = device_fs*(recordtime + prerecordtime) * 4;//录音字节

		for (size_t i = 0; i < count; i++)
		{
			if (recordlen % 4 == 0)
				rd[i] = new int[recordlen / 4];
			else
			{
				rd[i] = new int[recordlen / 4 + 1 * 4];
			}
		}
	}
	
	
	
	/*rd[1] = new int[recordlen];
	rd[2] = new int[recordlen];
	rd[3] = new int[recordlen];
	rd[4] = new int[recordlen];
	rd[5] = new int[recordlen];
	rd[6] = new int[recordlen];
	rd[7] = new int[recordlen];*/
		
	
	step = 0;
	recordstep = 0;
	bstop = false;
	bfirstrun = false;
	//if (ddd==0)
	
	if (recordtime == -1)
	{
		m_StopASIO_Record = false;
		recordlen = device_fs*(3 + prerecordtime) * 4;//录音字节
		createASIOBuffer2(device_fs);
	}
	else
	{
		createASIOBuffer(device_fs);


	}

	//ddd++;
	//

	

	while (initRME)
	{
		//if (!asioBufswitch)
		//	Sleep(10);

		//if (!asioBufswitch)
		//{
		//	AfxMessageBox(_T("请检查声卡是否存在"));
		//	bstop = true;
		//	bfirstrun = true;
		//	StopRME();
		//	initRME = false;

		//	//BOOL bfree=FreeLibrary(hModuleDll);
		//	break;
		//}
		if ((len == 0 && !bstop&&!bfirstrun&&recordlen == 0) || !bstopASIO)
		{
			//calldaw();
			bstop = true;
			bfirstrun = true;
			StopRME();
			initRME = false;
			//delete[] buf;
			//delete[] pld;

		}


		/*if (recordtime == -1 && m_StopASIO_Record)
		{
			bstop = true;
			bfirstrun = true;
			StopRME();
			initRME = false;
		}*/

	    

		//Sleep(500);
	}

	*data = rd;
	delete[] pld;
	pld = NULL;
	delete[] buf;
	buf = NULL;
	
	
	

}

void TestASIUO::setASIOStop(bool bstop)
{
	m_StopASIO_Record = bstop;

}

///暂时不使用
void TestASIUO::micCalibrationRecordASIO(int* index, int***data, int count, double device_fs, int recordtime)
{

}

void TestASIUO::setwdmindex_out(CString name)
{
	wdmName_out = name;
}
void TestASIUO::setwdmName_in(CString name)
{
	wdmName_in = name;
}

void TestASIUO::setMainf(CMainFrame* mf)
{
	mainf = mf;
}



TestASIUO::~TestASIUO()
{
	/*if (samp != NULL)
		delete samp;*/
}
