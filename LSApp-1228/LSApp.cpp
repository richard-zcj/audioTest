
// LSApp.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "LSApp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "LSAppDoc.h"
#include "LSAppView.h"
#include "StartPage.h"
#include "SplashThread.h"
#include "WaveShow.h"
#include <io.h>
#include "SerialPort.h"
#include "LogIn.h"
#include "AuthorizationKey.h"
#include "wav.h"
#include "fftzcj.h"
#include "StartPg.h"


#pragma comment(lib, "Version.lib")

#ifdef _DEBUG
#pragma comment(lib,"AuthorizationKeyd-x64.lib")
#ifdef UNICODE
#pragma comment(lib, "LogUnitd_U_x64.lib")
#else
#pragma comment(lib, "LogUnitd_x64.lib")
#endif
#else
#pragma comment(lib,"AuthorizationKey-x64.lib")
#ifdef UNICODE
#pragma comment(lib, "LogUnit_U_x64.lib")
#else
#pragma comment(lib, "LogUnit_x64.lib")
#endif
#endif

extern int golble_dotNum;
extern CBlueEreTest blueEreTest;
extern int m_docId;
extern bool bstopASIO;
extern int globle_w;
extern int globle_h;

/***********�������в������뺯��*********************/

#include "audiodeals.h"

audiodeals audios;

//CWaveShow wave;

/**********************************/


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLSAppApp

BEGIN_MESSAGE_MAP(CLSAppApp, CWinAppEx)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_MIMA, &CLSAppApp::OnPassWd)
	ON_COMMAND(ID_FILE_OPEN, &CLSAppApp::OnOpenFile)
	ON_COMMAND(ID_AUTHORITY, &CLSAppApp::OnFileClose)

	ON_COMMAND(ID_APP_ABOUT, &CLSAppApp::OnAppAbout)

	ON_COMMAND(ID_MKPZ, &CLSAppApp::OnAppMoKuaiSheZhi)

	ON_COMMAND(ID_DOTSET, &CLSAppApp::OnDotSet)

	ON_COMMAND(ID_START_CYCLES, &CLSAppApp::OnStartCyclesTest)
	ON_COMMAND(ID_STOP_CYCLES, &CLSAppApp::OnStopCyclesTest)
	ON_COMMAND(ID_CYCLES_SET, &CLSAppApp::OnCyclesTestSet)

	ON_COMMAND(ID_VIEW_RES, &CLSAppApp::OnResShow)
	
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()



// CLSAppApp ����
SplashThread* startpagethread;
CLSAppApp::CLSAppApp()
{
	m_bdotset = false;
	m_bHiColorIcons = TRUE;

	InitTestData(_T(""));
	CString sLogPath(_T(""));
	sLogPath.Format(_T("%slog\\"), GetAppPath().GetBuffer(0));
	InitLogService(sLogPath, 60);
	WriteInfoLog(L"Start...");

	WriteInfoLog(_T("Config file: %s"), GetConfigIniFile().GetBuffer(0));
	WriteInfoLog(_T("Script file: %s"), GetScriptIniFile().GetBuffer(0));

	bool bSpk = true;

	for (int n = 0; n < 2; n++)
	{
		for (int i = 0; i < GetPlayRecordCount(bSpk); i++)
		{
			CalcStimData(i, bSpk);
		}

		bSpk = false;
	}

	WriteDebugLog(_T("Calc stimulus data and frequency points finish"));

	//��ȡ����Ļ�ֱ���
	 globle_w = GetSystemMetrics(SM_CXSCREEN);
	 globle_h = GetSystemMetrics(SM_CYSCREEN);

	 /*****************����������л���׼��*************************/
	 
	 CreateSavePath();

	//golble_dotNum = 1;
	
	//startpagethread = (SplashThread*)AfxBeginThread(RUNTIME_CLASS(SplashThread), 0, 0, CREATE_SUSPENDED);
	//startpagethread = new SplashThread;

	//ASSERT(startpagethread->IsKindOf(RUNTIME_CLASS(SplashThread)));

	//ϵͳ�������������ر�Ҫģ����Ϣ�����ĵȴ�....
	//satrtpage->setmsg(CString("ϵͳ�������������ر�Ҫģ����Ϣ�����ĵȴ�...."));
	//startpagethread->CreateThread();
	//startpagethread->ResumeThread();
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("LSApp.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

//void CLSAppApp::WriteDoubleData(CArchive& ar, double dbData, int nNum)
//{
//	char szBuf[16];
//	char szFormat[8];
//	memset(szBuf, 0, sizeof(szBuf));
//	memset(szFormat, 0, sizeof(szFormat));
//	sprintf_s(szFormat, sizeof(szFormat), "%%.%df,", nNum);
//	sprintf_s(szBuf, sizeof(szBuf), szFormat, dbData);
//	ar.Write(szBuf, (UINT)strlen(szBuf));
//	ar.Flush();
//}
//
//void CLSAppApp::WriteStringData(CArchive& ar, const char* pData)
//{
//	/*char szBuf[256];
//	memset(szBuf, 0, sizeof(szBuf));
//	strcpy(szBuf, ",");*/
//	ar.Write(pData, (UINT)strlen(pData));
//	ar.Flush();
//}
//
//void CLSAppApp::SaveLimitHead(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, prParam* pPR, bool bLower)
//{
//	if (bLower)
//	{
//		WriteStringData(ar, ",Lower Limit,");
//	}
//	else
//	{
//		WriteStringData(ar, ",Upper Limit,");
//	}
//
//	bool bLimit;
//	testItem* pItem = GetItemParam(lpItemName, bSpkMode);
//	if (pItem == nullptr)
//		return;
//
//	if (StrCmpI(lpItemName, _T("Sensitivity")) == 0)
//	{
//		if (pItem->vLimit.size() > 0)
//		{
//			if (bLower)
//				WriteDoubleData(ar, pItem->vLimit[0].nLower);
//			else
//				WriteDoubleData(ar, pItem->vLimit[0].nUpper);
//		}
//		else
//		{
//			WriteStringData(ar, ",");
//		}
//
//		return;
//	}
//
//	for (int i = 0; i < pPR->vPoints.size(); i++)
//	{
//		bLimit = false;
//		for (int j = 0; j < pItem->vLimit.size(); j++)
//		{
//			if (pPR->vPoints[i] == pItem->vLimit[j].nFrequency)
//			{
//				bLimit = true;
//				if (bLower)
//					WriteDoubleData(ar, pItem->vLimit[j].nLower);
//				else
//					WriteDoubleData(ar, pItem->vLimit[j].nUpper);
//			}
//		}
//
//		if (!bLimit)
//		{
//			WriteStringData(ar, ",");
//		}
//	}
//}
//
//void CLSAppApp::SaveDataToCsv()
//{
//	CString sFile = ReadStr(_T("FilePath"), _T("DataFilePath"), _T("d:\\data"), GetConfigIniFile());
//	sFile = GetFilePath(sFile);
//	ForceCreateDirectory(sFile);
//	if (sFile.Right(1) != _T("/") && sFile.Right(1) != _T("\\"))
//	{
//		sFile += _T("\\");
//	}
//
//	bool bWriteHead = false;
//	FILE *fSave;
//	SYSTEMTIME stSysTime;
//	::GetLocalTime(&stSysTime);
//	CString sDate(_T(""));
//	sDate.Format(_T("%04d%02d%02d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);
//	sFile += ReadStr(_T("Common"), _T("ExcelPrefix"), _T(""), GetConfigIniFile());
//	sFile += _T("_");
//	sFile += sDate;
//	sFile += _T(".csv");
//
//	if (!FileExists(sFile))
//	{
//		bWriteHead = true;
//	}
//
//	CFile mFile;
//	mFile.Open(sFile, CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
//	mFile.SeekToEnd();
//	CArchive ar(&mFile, CArchive::store);
//
//	if (bWriteHead)
//	{
//		WriteStringData(ar, "Time,Overall TestResult,Result Info:,,Result Info:,,Result Info:,Test Result:,");
//
//		slotParam* pSlot = GetSlotParam(0);
//		prParam* pPR = GetPlayRecord(pSlot->nSpkPlayRecord, true);
//		for (int i = 0; i < pPR->vPoints.size(); i++)
//		{
//			WriteDoubleData(ar, pPR->vPoints[i]);
//		}
//
//		for (int n = 0; n < 4; n++)
//		{
//			WriteStringData(ar, "Result Info:,Test Result:,");
//
//			for (int i = 0; i < pPR->vPoints.size(); i++)
//			{
//				WriteDoubleData(ar, pPR->vPoints[i]);
//			}
//		}
//
//		WriteStringData(ar, "Result Info:,Test Result:,,\n");
//
//		WriteStringData(ar, ",,,,,,");
//		SaveLimitHead(_T("FR"), true, ar, pPR);
//		SaveLimitHead(_T("THD"), true, ar, pPR);
//		SaveLimitHead(_T("RB"), true, ar, pPR);
//		SaveLimitHead(_T("FR"), false, ar, pPR);
//		SaveLimitHead(_T("THD"), false, ar, pPR);
//		SaveLimitHead(_T("Sensitivity"), false, ar, pPR);
//		WriteStringData(ar, "\n");
//
//		WriteStringData(ar, ",,,,,,");
//		SaveLimitHead(_T("FR"), true, ar, pPR, false);
//		SaveLimitHead(_T("THD"), true, ar, pPR, false);
//		SaveLimitHead(_T("RB"), true, ar, pPR, false);
//		SaveLimitHead(_T("FR"), false, ar, pPR, false);
//		SaveLimitHead(_T("THD"), false, ar, pPR, false);
//		SaveLimitHead(_T("Sensitivity"), false, ar, pPR, false);
//		WriteStringData(ar, "\n");
//
//		
//	}
//
//	/*for (int i = 0; i < GetSlotCount(); i++)
//	{
//	}*/
//
//	ar.Close();
//	mFile.Close();
//}

CLSAppApp::~CLSAppApp()
{
	for (int i = 0; i < GetSlotCount(); i++)
	{
		slotParam* pSlot = GetSlotParam(i);

		try
		{
			char szBuff[16] = "\0";
			strcpy(szBuff, ">DISC\r\n");
			pSlot->pSerial->WriteBuffer(szBuff, strlen(szBuff), 3000);
			pSlot->pSerial->Close();
		}
		catch (...)
		{
		}

		delete pSlot->pSerial;
	}

	WriteInfoLog(L"Exit.\n");
	DestroyLogService();
}

void CLSAppApp::CreateSavePath()
{
	/*char * path = "D:\\TestRecord";
	if (_access(path, 0) == -1)
	{
		HANDLE hFile= CreateFile(_T("D:\\TestRecord"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);

		if (hFile != INVALID_HANDLE_VALUE)
		{

		}
		else
		{
			AfxMessageBox(_T("��D�̴���TestRecordĿ¼ʧ�ܣ�"));
			return;
		}
	}*/


	if (!PathIsDirectory(_T("D:\\TestRecord")))
	{
		if (!CreateDirectory(_T("D:\\TestRecord"), NULL))
		{
			AfxMessageBox(_T("��D�̴���TestRecordĿ¼ʧ�ܣ�"));
			return;
		}
	}

	if (!PathIsDirectory(_T("D:\\Data")))
	{
		if (!CreateDirectory(_T("D:\\Data"), NULL))
		{
			AfxMessageBox(_T("��D�̴���TestRecordĿ¼ʧ�ܣ�"));
			return;
		}
	}

}

// Ψһ��һ�� CLSAppApp ����

CLSAppApp theApp;


// CLSAppApp ��ʼ��

BOOL CLSAppApp::InitInstance()
{
	WriteDebugLog(_T("LSApp InitInstance start"));
	//ϵͳ�ѳ�ʼ�����....
	//satrtpage->setmsg(CString("ϵͳ�ѳ�ʼ�����...."));
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	
	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	CMultiPaneFrameWnd();
	
	pDocTemplate = new CMultiDocTemplate(IDR_LSAppTYPE,
		RUNTIME_CLASS(CLSAppDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CLSAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	//������Ƿ����
	if (!InitPwdKey())
	{
		AfxMessageBox(_T("������������,����ϵ�豸����"));
	}

#if ONLINEMODE
	if (!CheckKeyExit())
	{
		AfxMessageBox(_T("������������,����ϵ�豸����"));
		return FALSE;
	}


	//��¼
	CLogIn login;
	login.DoModal();
	if (!login.GetDalgRes())
	{
		return FALSE;
	}
	blueEreTest.SetIsOffline(login.IsOffLine());
#endif

	CStartPg pg;
	pg.Create(CStartPg::IDD);
	pg.ShowWindow(SW_SHOW);
	::SendMessage(pg.m_hWnd, WM_PAINT, 0, 0);
	//pg.ShowWindow(SW_SHOW);


	/*********matlab������ʼ��**********/
#if MATLAB_COMPILE
	if (!mclInitializeApplication(nullptr, 0))
	{

		AfxMessageBox(_T("mclInitializeApplication err"));
	}

	pg.SetStartInfo(CString("���������ڼ�����,��ȴ�....(30%)"));

	/*bool b_ok = FrInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("FrInitialize err"));

	}*/


	bool b_ok = fftzcjInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("fftzcjInitialize err"));

	}

	b_ok = wavInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("wavInitialize err"));

	}

	pg.SetStartInfo(CString("���������ڼ�����,��ȴ�....(60%)"));

	b_ok = FrHarmInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("FrHarmInitialize err"));

	}
	pg.SetStartInfo(CString("���������ڼ�����,��ȴ�....(100%)"));
	/*bool b_ok = fftzcjInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("fftzcjInitialize err"));

	}*/



#endif
	Sleep(1000);
	pg.ShowWindow(SW_HIDE);


	////��¼
	//CLogIn login;
	//login.DoModal();
	//if (!login.GetDalgRes())
	//{
	//	return FALSE;
	//}

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))//IDR_MAINFRAME
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);





	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	m_docId = 9;
	AfxGetApp()->m_pDocManager->OnFileNew();

	//startpagethread->hidewindow();
	//delete startpagethread;
	//startpagethread = NULL;

	bool bStatus = true;

	for (int i = 0; i < GetSlotCount(); i++)
	{
		slotParam* pSlot = GetSlotParam(i);
		if (!pSlot->bEnableBT)
			continue;

		try
		{
			pSlot->pSerial->Open();
			WriteInfoLog(_T("��Slot%d�˿�(COM%d, Baud=%d)�ɹ�"), pSlot->nSlotIndex,
				pSlot->pSerial->Port, pSlot->pSerial->Baud);
			char szBuff[16] = "\0";
			strcpy(szBuff, ">DISC\r\n");
			pSlot->pSerial->WriteBuffer(szBuff, strlen(szBuff), 3000);
		}
		catch (EDeviceError& e)
		{
			WriteErrorLog(_T("��Slot%d�˿�(COM%d, Baud=%d)�쳣: %s"),
				pSlot->nSlotIndex, pSlot->pSerial->Port, pSlot->pSerial->Baud, e.Message);
			bStatus = false;
		}
		catch (...)
		{
			WriteErrorLog(_T("��Slot%d�˿�(COM%d, Baud=%d)�쳣: δ֪����"),
				pSlot->nSlotIndex, pSlot->pSerial->Port, pSlot->pSerial->Baud);
			bStatus = false;
		}

		blueEreTest.setOffline(bStatus);
		
	}

	WriteDebugLog(_T("LSApp InitInstance finish"));

	return TRUE;
}

char* WCharToCharEx(LPCTSTR lpData, char* pBuff, int nSize)
{
#ifdef UNICODE
	int nLen = WideCharToMultiByte(CP_ACP, 0, lpData, -1, NULL, 0, NULL, NULL);
	if (nLen >= nSize)
	{
		nLen = nSize - 1;
	}

	memset(pBuff, 0, nSize);
	WideCharToMultiByte(CP_ACP, 0, lpData, -1, pBuff, nLen, NULL, NULL);
#else
	memset(pBuff, 0, nSize;);
	if (strlen(lpData) < nSize)
	{
		nSize = nLen;
	}
	else
	{
		nSize -= 1;
	}

	memcpy(pBuff, lpData, nSize);
#endif

	return pBuff;
}

bool CLSAppApp::CheckKeyExit()
{
	CString m_sCheckCode("");
	char szCheckCode[40] = "\0";
	char* pCheckCode = nullptr;
	if (!m_sCheckCode.IsEmpty())
	{
		pCheckCode = WCharToCharEx(m_sCheckCode.GetBuffer(0), szCheckCode, sizeof(szCheckCode));
	}

	if (CheckKeyExists(pCheckCode)!=0)
	{
		WriteErrorLog(_T("��ȨKey�����ڣ�"));
		//MessageBox(_T("��ȨKey�����ڣ�"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

int CLSAppApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CLSAppApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAuthroty();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CLSAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CLSAppApp::OnAppMoKuaiSheZhi()
{
	AfxMessageBox(_T("ģ�����ã�δ������"));
}
void CLSAppApp::OnOpenFile()
{
	// TODO: �ڴ˴���������������

	AfxMessageBox(_T("��ģ��δ����"));
}
CCaculateSet* caulateset;
void CLSAppApp::OnDotSet()
{
	int numdot = 0;
	if (m_bdotset)
	{
		numdot = caulateset->getDotNum();
		delete caulateset;
		caulateset = NULL;
	}

	caulateset = new CCaculateSet;
	if (m_bdotset)
	   caulateset->setDotNum(numdot);
	caulateset->Create(CCaculateSet::IDD);
	caulateset->ShowWindow(SW_SHOW);
	
	m_bdotset = true;
	
}

void CLSAppApp::OnStartCyclesTest()
{
	/*PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON4, BN_CLICKED), NULL);*/

	blueEreTest.OnBnClickedButtonTestCycles();
	
}
void CLSAppApp::OnStopCyclesTest()
{
	bstopASIO = false;
	blueEreTest.setRunCycles(false);
}
void CLSAppApp::OnCyclesTestSet()
{

	//if (cyclesSet != NULL)
	//{
	//	delete cyclesSet;
	//	cyclesSet = NULL;
	//}
	//if (cyclesSet == NULL)
	//{
	//	cyclesSet = new CCyclesTestSet;
	//	cyclesSet->Create(CCyclesTestSet::IDD);
	//	//cyclesSet->ShowWindow(SW_SHOW);
	//	cyclesSet->DoModal();
	//}
	
	
	CCyclesTestSet cyclesSet;

	CyclesSetData data;
	//����ʹ�ô���
	data.setCyclesTimes(blueEreTest.getTestCycleTimes());
	data.setCyclesPeriod(blueEreTest.getTestCyclePeriod());
	data.setCyclesColr(blueEreTest.getTestCyclecolor());
	//data.setmainTestColr(blueEreTest.getmainTestColr());
	cyclesSet.setData(data);
	cyclesSet.DoModal();

	//���ø��º����
	data = cyclesSet.getData();
	blueEreTest.setTestCycleTimes(data.getCyclesTimes());
	blueEreTest.setTestCyclePeriod(data.getCyclesPeriod());
	blueEreTest.setTestCyclecolor(data.getCyclesColr());
	//blueEreTest.setmainTestColr(data.getmainTestColr());
	
	


}

void CLSAppApp::OnResShow()
{
	blueEreTest.ShowRes();
}


// CLSAppApp �Զ������/���淽��

void CLSAppApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_MYCONTROL);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CLSAppApp::LoadCustomState()
{
}

void CLSAppApp::SaveCustomState()
{
}

void CLSAppApp::OnFileClose()
{
	AfxMessageBox(_T("Ȩ�޹���δ������"));
}

void CLSAppApp::OnPassWd()
{
	AfxMessageBox(_T("�������δ������"));
}

// CLSAppApp ��Ϣ�������





