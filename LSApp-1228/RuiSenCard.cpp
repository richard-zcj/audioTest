#include "stdafx.h"
#include "RuiSenCard.h"


CRuiSenCard::CRuiSenCard()
{

}


CRuiSenCard::~CRuiSenCard()
{
	//FreeLibrary(hModuleDll);
}

bool CRuiSenCard::initCard()
{

#if RUISENCARD
	
	
	return true;

#endif

	bool ret;
	int nRet;

	ret = IsRscardInsert();
	if (ret)
	{
		;//MessageBox(L"Found Keyconnect!");
	}
	else
	{
		AfxMessageBox(L"Keyconnect Not Found!");
		return false;;
	}

	ret = loadAsioDriver("Rstech Rscard");
	if (ret)
	{
		;//MessageBox(L"Load Driver success!");
	}
	else
	{
		AfxMessageBox(L"Load Driver failed!");
		return false;;
	}

	if (Initialize() == 0)
	{
		;//MessageBox(L"Device Initialize success!");
	}
	else
	{
		AfxMessageBox(L"Device Initialize Failed!");
		return false;;
	}

	nRet = SetSampleRate(GetCurrentSampleRate());
	if (nRet == 0)
	{
		;//MessageBox(L"Set Sample Rate success!");
	}
	else
	{
		AfxMessageBox(L"Set Sample Rate Failed!");
		return false;;
	}

	nRet = Prepare();
	if (nRet == 0)
	{
		;//MessageBox(L"Device Prepare success!");
	}
	else
	{
		
		AfxMessageBox(_T("Device Prepare Failed!"));
		return false;
		
	}

	//AfxMessageBox(L"Initial Succed");


	return true;
}

bool CRuiSenCard::isCardInsert()
{

	bool ret = IsRscardInsert();
	if (ret)
	{
		;//MessageBox(L"Found Keyconnect!");
	}
	else
	{
		AfxMessageBox(L"Keyconnect Not Found!");
		return false;;
	}
	return true;
}

void CRuiSenCard::LoadAsioLib()
{
	hModuleDll = NULL;

	loadAsioDriver = NULL;
	releaseAsioDriver = NULL;
	getAsioDriverCount = NULL;
	getAsioDriverName = NULL;
	Initialize = NULL;
	GetBufferSizeMin = NULL;
	GetBufferSizeMax = NULL;
	GetBufferSizePreferred = NULL;
	SetSampleRate = NULL;
	Prepare = NULL;
	GetInputChannelCount = NULL;
	GetOutputChannelCount = NULL;
	GetCurrentSampleRate = NULL;
	PlayBuff = NULL;
	CaptureBuff = NULL;
	StopPlayback = NULL;
	StopCapture = NULL;
	Finish = NULL;
	GetInputLatency = NULL;
	GetOutputLatency = NULL;
	IsCaptureRunning = NULL;
	IsRscardInsert = NULL;
	SetPlayBuffer = NULL;
	SetCaptureBuffer = NULL;
	StartPlayback = NULL;
	StartCapture = NULL;
	IsPortCaptureRunning = NULL;
	IsPortPlaybackRunning = NULL;

	hModuleDll = LoadLibrary(_T("ASIODriverLib.dll"));//

	loadAsioDriver = (ploadAsioDriver)GetProcAddress(hModuleDll, "loadAsioDriver");
	releaseAsioDriver = (preleaseAsioDriver)GetProcAddress(hModuleDll, "releaseAsioDriver");
	getAsioDriverCount = (pgetAsioDriverCount)GetProcAddress(hModuleDll, "getAsioDriverCount");
	getAsioDriverName = (pgetAsioDriverName)GetProcAddress(hModuleDll, "getAsioDriverName");
	Initialize = (pInitialize)GetProcAddress(hModuleDll, "Initialize");
	GetBufferSizeMin = (pGetBufferSizeMin)GetProcAddress(hModuleDll, "GetBufferSizeMin");
	GetBufferSizeMax = (pGetBufferSizeMax)GetProcAddress(hModuleDll, "GetBufferSizeMax");
	GetBufferSizePreferred = (pGetBufferSizePreferred)GetProcAddress(hModuleDll, "GetBufferSizePreferred");
	SetSampleRate = (pSetSampleRate)GetProcAddress(hModuleDll, "SetSampleRate");
	Prepare = (pPrepare)GetProcAddress(hModuleDll, "Prepare");
	GetInputChannelCount = (pGetInputChannelCount)GetProcAddress(hModuleDll, "GetInputChannelCount");
	GetOutputChannelCount = (pGetOutputChannelCount)GetProcAddress(hModuleDll, "GetOutputChannelCount");
	GetCurrentSampleRate = (pGetCurrentSampleRate)GetProcAddress(hModuleDll, "GetCurrentSampleRate");
	PlayBuff = (pPlayBuff)GetProcAddress(hModuleDll, "PlayBuff");
	CaptureBuff = (pCaptureBuff)GetProcAddress(hModuleDll, "CaptureBuff");
	StopPlayback = (pStopPlayback)GetProcAddress(hModuleDll, "StopPlayback");
	StopCapture = (pStopCapture)GetProcAddress(hModuleDll, "StopCapture");
	Finish = (pFinish)GetProcAddress(hModuleDll, "Finish");
	GetInputLatency = (pGetInputLatency)GetProcAddress(hModuleDll, "GetInputLatency");
	GetOutputLatency = (pGetOutputLatency)GetProcAddress(hModuleDll, "GetOutputLatency");
	IsCaptureRunning = (pIsCaptureRunning)GetProcAddress(hModuleDll, "IsCaptureRunning");
	IsRscardInsert = (pIsRscardInsert)GetProcAddress(hModuleDll, "IsRscardInsert");
	SetPlayBuffer = (pSetPlayBuffer)GetProcAddress(hModuleDll, "SetPlayBuffer");
	SetCaptureBuffer = (pSetCaptureBuffer)GetProcAddress(hModuleDll, "SetCaptureBuffer");
	StartPlayback = (pStartPlayback)GetProcAddress(hModuleDll, "StartPlayback");
	StartCapture = (pStartCapture)GetProcAddress(hModuleDll, "StartCapture");
	IsPortCaptureRunning = (pIsPortCaptureRunning)GetProcAddress(hModuleDll, "IsPortCaptureRunning");
	IsPortPlaybackRunning = (pIsPortPlaybackRunning)GetProcAddress(hModuleDll, "IsPortPlaybackRunning");


}


void CRuiSenCard::FreeAsioLib()
{
	FreeLibrary(hModuleDll);
}

DWORD dwRecordThreadID;
#define TYPE int


DWORD WINAPI ThreadFuncMyPlay(LPVOID lparam)
{
	RecordPartmeter* pMy = (RecordPartmeter*)lparam;
	CRuiSenCard *dlg = (CRuiSenCard *)pMy->pThis;

	
	if (dlg->SetPlayBuffer(pMy->data, sizeof(TYPE)*pMy->len, pMy->port) != 0)
		AfxMessageBox(_T("err setbuff"));

	dlg->StartPlayback();

	while (true == dlg->IsPortPlaybackRunning(0))
	{
		Sleep(100);
	}

	char* fileName1 = "D:\\TestRecord\\3.wav";
	wavwrite(fileName1, 1, 44100, 32, (char*)pMy->data, sizeof(TYPE)*pMy->len);
	
	return 0;
}

void CRuiSenCard::playwithRecord(int* data, int len, int port, int* res, int* res2, bool isrecord)
{
#if RUISENCARD

	AfxMessageBox(_T("当前声卡运行于模拟模式！"));
	return ;

#endif
	/*m_RecordParameter.pThis = this;
	m_RecordParameter.port = port;
	m_RecordParameter.len = len;
	m_RecordParameter.data = data;*/

	//CreateThread(
	//	NULL,
	//	0,
	//	ThreadFuncMyPlay,
	//	&m_RecordParameter,
	//	0,
	//	&dwRecordThreadID);


	/*if (isrecord)
	   record(res, len, 0);

	if (isrecord&&res2)
		record(res2, len, 1);*/

	Testrecord(res, len, 0, res2, len, 1);//


	if (SetPlayBuffer(data, sizeof(TYPE)*len, port) != 0)
		AfxMessageBox(_T("err setbuff"));

	StartPlayback();

	while (true == IsPortPlaybackRunning(port))
	{
		Sleep(100);
	}

	char* fileName1 = "D:\\TestRecord\\play.wav";
	wavwrite(fileName1, 1, 44100, 32, (char*)data, sizeof(TYPE)*len);



}

CMutex mutex;
DWORD WINAPI ThreadFuncMyRecord(LPVOID lparam)
{
	
	
	CSingleLock siglock(&mutex);
	siglock.Lock();
	RecordPartmeter* pMy = (RecordPartmeter*)lparam;
	CRuiSenCard *dlg = (CRuiSenCard *)pMy->pThis;
	//char data[44100*16];
	dlg->isruning = true;
	int len = pMy->len;
	//if (pMy->data==NULL)
	int* data = pMy->data;

	if (data==NULL)
		data=new int[len];


	int port = pMy->port;
	if (dlg->SetCaptureBuffer(pMy->data == NULL ? data : pMy->data, sizeof(TYPE)*len, port) != 0)
		AfxMessageBox(_T("set buff fail"));

	dlg->StartCapture();
	while (dlg->IsPortCaptureRunning(port))
	{
		;
	}

	if (!dlg->IsPortCaptureRunning(port))
	{
		
		char* fileName = "D:\\TestRecord\\record.wav";
		wavwrite(fileName, 1, 44100, 32, (char*)(pMy->data == NULL ? data : pMy->data), sizeof(TYPE)*len);
	}

	
	dlg->isruning = false;
	siglock.Unlock();
	return 0;
}

DWORD WINAPI ThreadFuncTestRecord(LPVOID lparam)
{



	RecordPartmeter* pMy = (RecordPartmeter*)lparam;
	CRuiSenCard *dlg = (CRuiSenCard *)pMy->pThis;
	//char data[44100*16];
	dlg->isruning = true;
	int len = pMy->len;
	//if (pMy->data==NULL)
	int* data = pMy->data;

	if (data == NULL)
		data = new int[len];


	int port = pMy->port;
	if (dlg->SetCaptureBuffer(pMy->data == NULL ? data : pMy->data, sizeof(TYPE)*len, port) != 0)
		AfxMessageBox(_T("set buff fail"));

	/*int port2 = pMy->port2;
	int len2 = pMy->len2;
	if (dlg->SetCaptureBuffer(pMy->data2 == NULL ? data : pMy->data2, sizeof(TYPE)*len2, port2) != 0)
		AfxMessageBox(_T("set buff fail"));*/

	dlg->StartCapture();
	while (dlg->IsPortCaptureRunning(port) /*|| dlg->IsPortCaptureRunning(port2)*/)
	{
		;
	}

	if (!dlg->IsPortCaptureRunning(port))
	{

		char* fileName = "D:\\TestRecord\\record.wav";
		wavwrite(fileName, 1, 44100, 32, (char*)(pMy->data == NULL ? data : pMy->data), sizeof(TYPE)*len);
	}


	dlg->isruning = false;
	return 0;
}

DWORD WINAPI ThreadFuncTestRecord2(LPVOID lparam)
{



	RecordPartmeter* pMy = (RecordPartmeter*)lparam;
	CRuiSenCard *dlg = (CRuiSenCard *)pMy->pThis;
	//char data[44100*16];
	dlg->isruning = true;
	int len = pMy->len2;
	//if (pMy->data==NULL)
	int* data = pMy->data;

	if (data == NULL)
		data = new int[len];


	/*int port = pMy->port;
	if (dlg->SetCaptureBuffer(pMy->data == NULL ? data : pMy->data, sizeof(TYPE)*len, port) != 0)
		AfxMessageBox(_T("set buff fail"));*/

	int port2 = pMy->port2;
	int len2 = pMy->len2;
	if (dlg->SetCaptureBuffer(pMy->data2 == NULL ? data : pMy->data2, sizeof(TYPE)*len2, port2) != 0)
	AfxMessageBox(_T("set buff fail"));

	dlg->StartCapture();
	while (dlg->IsPortCaptureRunning(port2) /*|| dlg->IsPortCaptureRunning(port2)*/)
	{
		;
	}

	if (!dlg->IsPortCaptureRunning(port2))
	{

		char* fileName = "D:\\TestRecord\\record.wav";
		wavwrite(fileName, 1, 44100, 32, (char*)(pMy->data2 == NULL ? data : pMy->data2), sizeof(TYPE)*len2);
	}


	dlg->isruning = false;
	return 0;
}



void CRuiSenCard::Testrecord(int* data, int len, int port, int* data2, int len2, int port2)
{
#if RUISENCARD

	AfxMessageBox(_T("当前声卡运行于模拟模式！"));
	return;

#endif
	m_RecordParameter.pThis = this;
	m_RecordParameter.port = port;
	m_RecordParameter.len = len;
	m_RecordParameter.data = data;
	m_RecordParameter.port2 = port2;
	m_RecordParameter.len2 = len2;
	m_RecordParameter.data2 = data2;

	CreateThread(
		NULL,
		0,
		ThreadFuncTestRecord,
		&m_RecordParameter,
		0,
		&dwRecordThreadID);

	if (data2)
	{
		CreateThread(
			NULL,
			0,
			ThreadFuncTestRecord2,
			&m_RecordParameter,
			0,
			&dwRecordThreadID);
	}

}



void CRuiSenCard::record(int* data, int len, int port)
{
#if RUISENCARD

	AfxMessageBox(_T("当前声卡运行于模拟模式！"));
	return ;

#endif
	m_RecordParameter.pThis = this;
	m_RecordParameter.port = port;
	m_RecordParameter.len = len;
	m_RecordParameter.data = data;
	
	CreateThread(
		NULL,
		0,
		ThreadFuncMyRecord,
		&m_RecordParameter,
		0,
		&dwRecordThreadID);

}

