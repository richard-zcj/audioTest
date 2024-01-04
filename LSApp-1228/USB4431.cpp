
#include "stdafx.h"
#include "USB4431.h"
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

/*
回调函数静态使用，程序架构不能使用多线程去扩展，应采用队列调用模式。
*/
int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

CUSB4431 * CUSB4431::m_pThis = NULL;

CUSB4431::CUSB4431(CCallBackBase* pCommEvents){
	m_pCommEvents = pCommEvents;
	m_pThis = this;
}

CUSB4431::~CUSB4431(){
	ClearTask();
}
void CUSB4431::AddCallBackPtr(CCallBackBase* pCommEvents)
{
	m_pCommEvents = pCommEvents;
}

void CUSB4431::ClearTask(){
	map<string, TaskHandle>::iterator it;
	it = m_AiHandles.begin();
	while (it != m_AiHandles.end())
	{
		DAQmxStopTask(it->second);
		DAQmxClearTask(it->second);
		it++;
	}
	it = m_AoHandles.begin();
	while (it != m_AoHandles.end())
	{
		DAQmxStopTask(it->second);
		DAQmxClearTask(it->second);
		it++;
	}
	m_AiHandles.clear();
	m_AoHandles.clear();
	m_AiTasks.clear();
	m_AoTasks.clear();
}

bool CUSB4431::GetAiTaskParam(const char taskName[], DAQmxAiTaskParam *param){
	if (m_AiTasks.find(taskName) != m_AiTasks.end())
	{
		*param = m_AiTasks[taskName];
		return true;
	}
	return false;
}
bool CUSB4431::GetAiTaskParam(TaskHandle handle, DAQmxAiTaskParam *param){
	map<string, TaskHandle>::iterator it;
	it = m_AiHandles.begin();
	while (it != m_AiHandles.end())
	{
		if (it->second == handle)
		{
			*param = m_AiTasks[it->first];
			return true;
		}
		it++;
	}
	return false;
}
bool CUSB4431::GetAoTaskParam(const char taskName[], DAQmxAoTaskParam *param){
	if (m_AoTasks.find(taskName) != m_AoTasks.end())
	{
		*param = m_AoTasks[taskName];
		return true;
	}
	return false;
}
bool CUSB4431::LoadAiTask(const char name[], DAQmxAiTaskParam param, DAQmxEveryNSamplesEventCallbackPtr callbackFunction)
{
	char taskName[100] = { '\0' };
	strcpy_s(taskName, name);
	char        errBuff[2048] = { '\0' };
	int32       error = 0;
	map<string, DAQmxAiTaskParam>::iterator iter1;
	map<string, TaskHandle>::iterator iter2;
	iter1 = m_AiTasks.find(taskName);
	iter2 = m_AiHandles.find(taskName);
	if (iter1 != m_AiTasks.end())
	{
		m_AiTasks.erase(iter1);
	}
	if(iter2!=m_AiHandles.end() )
	{	
		DAQmxStopTask(m_AiHandles[taskName]);
		DAQmxClearTask(m_AiHandles[taskName]);
		m_AiHandles.erase(iter2);
	}
#if FALSE
	/*加载外部配置好的任务*/
	//DAQmxErrChk(DAQmxLoadTask(taskName, &AiTaskHandle));
	DAQmxErrChk(DAQmxLoadTask("SoundTaskA0_Test", &AiTaskHandle));
	char val[100] ={'\0'};
	int nVal[10] = { 0 };
	DAQmxErrChk(DAQmxGetTrigAttribute(AiTaskHandle, DAQmx_StartTrig_Type, nVal));
	DAQmxErrChk(DAQmxGetTrigAttribute(AiTaskHandle,DAQmx_AnlgEdge_StartTrig_Src,val));
	DAQmxErrChk(DAQmxGetTrigAttribute(AiTaskHandle, DAQmx_AnlgEdge_StartTrig_Slope, nVal));
#else
	/*创建配置自己的任务*/
	TaskHandle taskHandle = 0;
	DAQmxErrChk(DAQmxCreateTask(taskName, &taskHandle));
	param.taskName = taskName;
	param.channelName = taskName;
	//bool32 bEnabled =0;
	m_AiTasks.insert(pair<string, DAQmxAiTaskParam>(string(taskName), param));
	m_AiHandles.insert(pair<string, TaskHandle>(string(taskName), taskHandle));
	/* 句柄，硬件通道地址,通道类型命名，接线端配置（默认 伪差分模式）,声压单位,灵敏度,最大电平，激励电流源，激励电流值，*/
	DAQmxErrChk(DAQmxCreateAIMicrophoneChan(taskHandle, param.physicalChannel, param.channelName, param.terminalConfig, param.units, param.sensitivity,
		param.maxSndPressLevel, param.currentExcitSource, param.currentExcitVal, NULL));
	/*句柄，信号源，采样率，触发边缘,采样模式（连续）,采样数*/
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", param.sampleRate, DAQmx_Val_Rising, param.sampleMode, param.totalSamples));
	if (param.sampleMode == DAQmx_Val_FiniteSamps)
	{
		//DAQmxErrChk(DAQmxCfgAnlgEdgeStartTrig(taskHandle, param.channelName, DAQmx_Val_Rising, param.trigLvl));
		//DAQmxErrChk(DAQmxSetAnlgEdgeStartTrigHyst(taskHandle, 1.0));
	}
	//set coupling DAQmx_Val_AC
	DAQmxErrChk(DAQmxSetAICoupling(taskHandle, param.channelName, DAQmx_Val_AC));
	int32 nCoupling = 0;
	DAQmxErrChk(DAQmxGetAICoupling(taskHandle, param.channelName, &nCoupling));


	//DAQmxErrChk(DAQmxGetAIFilterEnable(taskHandle, param.physicalChannel, &bEnabled));
#endif
	//没有回调就调用自身
	DAQmxErrChk(DAQmxRegisterEveryNSamplesEvent(taskHandle, DAQmx_Val_Transferred_From_Buffer, param.samples, 0,
		callbackFunction == NULL ? EveryNCallback:callbackFunction, NULL));
	DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle, 0, DoneCallback, NULL));
Error:
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		m_pCommEvents->ErrorMessage(errBuff);
		return false;
	}
	
	return true;
}
bool CUSB4431::LoadAoTask(const char name[], DAQmxAoTaskParam param, DAQmxDoneEventCallbackPtr callbackFunction)
{
	char taskName[100] = { '\0' };
	strcpy_s(taskName, name);
	char        errBuff[2048] = { '\0' };
	int32       error = 0;
	int32      writenlen = 0;
	map<string, DAQmxAoTaskParam>::iterator iter1;
	map<string, TaskHandle>::iterator iter2;
	iter1 = m_AoTasks.find(taskName);
	iter2 = m_AoHandles.find(taskName);
	if (iter1 != m_AoTasks.end())
	{
		m_AoTasks.erase(iter1);
	}
	if (iter2 != m_AoHandles.end())
	{
		DAQmxStopTask(m_AoHandles[taskName]);
		DAQmxClearTask(m_AoHandles[taskName]);
		m_AoHandles.erase(iter2);
	}
	/*创建配置自己的任务*/
	TaskHandle taskHandle = 0;
	DAQmxErrChk(DAQmxCreateTask(taskName, &taskHandle));
	param.taskName = taskName;
	param.channelName = taskName;
	m_AoTasks.insert(pair<string, DAQmxAoTaskParam>(string(taskName), param));
	m_AoHandles.insert(pair<string, TaskHandle>(string(taskName), taskHandle));
	DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle, param.physicalChannel, param.channelName, param.minVoltage, param.maxVoltage, DAQmx_Val_Volts, NULL));
	/*句柄，信号源，采样率，触发边缘,采样模式（连续）,采样数*/
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", param.sampleRate, DAQmx_Val_Rising, param.sampleMode, param.samples));
	DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle, param.samples, 0, 10.0, DAQmx_Val_GroupByChannel, param.data, &writenlen, NULL));
	
	DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle, 0, callbackFunction == NULL ? DoneCallback : callbackFunction, NULL));


Error:
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		m_pCommEvents->ErrorMessage(errBuff);
		return false;
	}

	return true;
}
bool CUSB4431::StartAiTask(const char taskName[])
{
	int32       error = 0;
	char        errBuff[2048] = { '\0' };
	bool isStartOk = false;
	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	if (m_AiHandles.find(taskName) != m_AiHandles.end())
		DAQmxErrChk(DAQmxStartTask(m_AiHandles[taskName]));
	else
		return false;

Error:
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		m_pCommEvents->ErrorMessage(errBuff);
		if (m_AiHandles.find(taskName) != m_AiHandles.end()){
			/*********************************************/
			// DAQmx Stop Code
			/*********************************************/
			DAQmxStopTask(m_AiHandles[taskName]);
			//DAQmxClearTask(AiTaskHandle);
		}
	}
	else
	{
		isStartOk = true;
	}
	return isStartOk;
}
bool CUSB4431::StartAoTask(const char taskName[]){
	int32       error = 0;
	char        errBuff[2048] = { '\0' };
	bool isStartOk = false;

	if (m_AoHandles.find(taskName) != m_AoHandles.end())
		DAQmxErrChk(DAQmxStartTask(m_AoHandles[taskName]));
	else
		return false;

Error:
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		m_pCommEvents->ErrorMessage(errBuff);
		if (m_AoHandles.find(taskName) != m_AoHandles.end()){
			DAQmxStopTask(m_AoHandles[taskName]);
		}
	}
	else
	{
		isStartOk = true;
	}
	return isStartOk;
}

bool CUSB4431::StopAiTask(const char taskName[])
{
	if (m_AiHandles.find(taskName) != m_AiHandles.end())
	{
		DAQmxStopTask(m_AiHandles[taskName]);
		//DAQmxClearTask(m_AiHandles[taskName]);
		return true;
	}
	return false;
}
bool CUSB4431::StopAoTask(const char taskName[])
{
	if (m_AoHandles.find(taskName) != m_AoHandles.end())
	{
		DAQmxStopTask(m_AoHandles[taskName]);
		//DAQmxClearTask(m_AoHandles[taskName]);
		return true;
	}
	return false;
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	char        errBuff[2048] = { '\0' };
	int32       error = 0;
	static int  totalRead = 0;
	int32       read = 0;
	float64     *data = new float64[nSamples];
	bool32 isDone = 0;
	CCallBackBase * pCommn = CUSB4431::m_pThis->m_pCommEvents;
	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	//DAQmxAiTaskParam param;
	//CUSB4431::m_pThis->GetAiTaskParam(taskHandle, &param);
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, nSamples, 10, DAQmx_Val_GroupByScanNumber, data, nSamples, &read, NULL));
	if (read >= nSamples) {
		pCommn->RecieveUSB4431Data(data, read);
		DAQmxIsTaskDone(taskHandle, &isDone);
		if (!isDone)
		{
			//Sleep(5);
			//DAQmxStopTask(taskHandle);
			//if (CUSB4431::m_pThis->bContinus)DAQmxStartTask(taskHandle);
		}
	}

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		pCommn->ErrorMessage(errBuff);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		//DAQmxClearTask(taskHandle);
	}
	delete data;
	return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error = 0;
	char        errBuff[2048] = { '\0' };

		CCallBackBase * pCommn = CUSB4431::m_pThis->m_pCommEvents;
	// Check to see if an error stopped the task.
		DAQmxErrChk(status);
		DAQmxStopTask(taskHandle);
	pCommn->RunLog("End");

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		pCommn->ErrorMessage(errBuff);
		DAQmxClearTask(taskHandle);
	}
	return 0;
}