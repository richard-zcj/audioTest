#ifndef USB4431_H_    
#define USB4431_H_  
#include <stdio.h>
#include <stdlib.h>
//#include <NIDAQmx.h>
#include <map>
#include "CallBackBase.h"
struct DAQmxAiTaskParam
{
	/*任务名称*/
	const char *taskName;
	/*物理通道名称*/
	const char *physicalChannel;
	/*通道名称*/
	const char *channelName;
	/*采样模式:DAQmx_Val_ContSamps-持续采样/DAQmx_Val_FiniteSamps-单次采样*/
	int sampleMode;
	/*缓冲区长度*/
	int samples;
	/*总采样长度*/
	long totalSamples;
	/*采样率*/
	int sampleRate;
	/*默认值*/
	int terminalConfig;
	/*使用默认值*/
	int units;
	/*使用默认值*/
	double sensitivity;
	/*使用默认值*/
	double maxSndPressLevel;
	/*条件触发模式*/
	int currentExcitSource;
	/*条件触发起始值*/
	double currentExcitVal;
	/*使用默认值*/
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