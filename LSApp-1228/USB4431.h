#ifndef USB4431_H_    
#define USB4431_H_  
#include <stdio.h>
#include <stdlib.h>
//#include <NIDAQmx.h>
#include <map>
#include "CallBackBase.h"
struct DAQmxAiTaskParam
{
	/*��������*/
	const char *taskName;
	/*����ͨ������*/
	const char *physicalChannel;
	/*ͨ������*/
	const char *channelName;
	/*����ģʽ:DAQmx_Val_ContSamps-��������/DAQmx_Val_FiniteSamps-���β���*/
	int sampleMode;
	/*����������*/
	int samples;
	/*�ܲ�������*/
	long totalSamples;
	/*������*/
	int sampleRate;
	/*Ĭ��ֵ*/
	int terminalConfig;
	/*ʹ��Ĭ��ֵ*/
	int units;
	/*ʹ��Ĭ��ֵ*/
	double sensitivity;
	/*ʹ��Ĭ��ֵ*/
	double maxSndPressLevel;
	/*��������ģʽ*/
	int currentExcitSource;
	/*����������ʼֵ*/
	double currentExcitVal;
	/*ʹ��Ĭ��ֵ*/
	double trigLvl;
	DAQmxAiTaskParam()
	{
		physicalChannel = "Dev1/ai0";
//		sampleMode = DAQmx_Val_ContSamps;
		samples = 2000;
		totalSamples = 2000;
		sampleRate = 48000;
	//	terminalConfig = DAQmx_Val_PseudoDiff;
	//	units = DAQmx_Val_Pascals;
		sensitivity = 10;
		maxSndPressLevel = 100.0;
	//	currentExcitSource = DAQmx_Val_Internal;
		currentExcitVal = 0.0021;
		trigLvl = 0;
	}
};
struct DAQmxAoTaskParam
{
	const char *taskName;
	const char *physicalChannel;
	const char *channelName;
	int sampleMode;
	int samples;
	int sampleRate;
	double *data;
	double maxVoltage;
	double minVoltage;
	DAQmxAoTaskParam()
	{
		physicalChannel = "Dev1/ao0";
//		sampleMode = DAQmx_Val_ContSamps;
		samples = 100;
		sampleRate = 51200;
		maxVoltage = 1.0;
		minVoltage = -1.0;
	}
};
class CUSB4431
{
public:
	static CUSB4431* m_pThis;
	CUSB4431(CCallBackBase* pCommEvents = NULL);
	~CUSB4431();
private:
	map<string, DAQmxAiTaskParam> m_AiTasks;
	map<string, DAQmxAoTaskParam> m_AoTasks;
//	map<string, TaskHandle> m_AiHandles;
	//map<string, TaskHandle> m_AoHandles;
	    
protected:
public:
	CCallBackBase* m_pCommEvents;
	void ClearTask();
	void AddCallBackPtr(CCallBackBase* pCommEvents);
	//bool LoadAiTask(const char name[], DAQmxAiTaskParam param, DAQmxEveryNSamplesEventCallbackPtr callbackFunction);
//	bool LoadAoTask(const char name[], DAQmxAoTaskParam param, DAQmxDoneEventCallbackPtr callbackFunction);
	bool StartAiTask(const char taskName[]);
	bool StopAiTask(const char taskName[]);
	bool StartAoTask(const char taskName[]);
	bool StopAoTask(const char taskName[]);
	bool GetAiTaskParam(const char taskName[], DAQmxAiTaskParam* param);
//	bool GetAiTaskParam(TaskHandle handle, DAQmxAiTaskParam* param);
	bool GetAoTaskParam(const char taskName[], DAQmxAoTaskParam* param);
};
#endif    