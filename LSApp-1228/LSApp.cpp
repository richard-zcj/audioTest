
// LSApp.cpp : 定义应用程序的类行为。
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

/***********这里运行测试类与函数*********************/

#include "audiodeals.h"

audiodeals audios;

//CWaveShow wave;

/**********************************/


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLSAppApp

BEGIN_MESSAGE_MAP(CLSAppApp, CWinAppEx)
	// 基于文件的标准文档命令
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
	
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()



// CLSAppApp 构造
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

	//获取点屏幕分辨率
	 globle_w = GetSystemMetrics(SM_CXSCREEN);
	 globle_h = GetSystemMetrics(SM_CYSCREEN);

	 /*****************软件所需运行环境准备*************************/
	 
	 CreateSavePath();

	//golble_dotNum = 1;
	
	//startpagethread = (SplashThread*)AfxBeginThread(RUNTIME_CLASS(SplashThread), 0, 0, CREATE_SUSPENDED);
	//startpagethread = new SplashThread;

	//ASSERT(startpagethread->IsKindOf(RUNTIME_CLASS(SplashThread)));

	//系统正在启动，加载必要模块信息，耐心等待....
	//satrtpage->setmsg(CString("系统正在启动，加载必要模块信息，耐心等待...."));
	//startpagethread->CreateThread();
	//startpagethread->ResumeThread();
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("LSApp.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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
			AfxMessageBox(_T("在D盘创建TestRecord目录失败！"));
			return;
		}
	}*/


	if (!PathIsDirectory(_T("D:\\TestRecord")))
	{
		if (!CreateDirectory(_T("D:\\TestRecord"), NULL))
		{
			AfxMessageBox(_T("在D盘创建TestRecord目录失败！"));
			return;
		}
	}

	if (!PathIsDirectory(_T("D:\\Data")))
	{
		if (!CreateDirectory(_T("D:\\Data"), NULL))
		{
			AfxMessageBox(_T("在D盘创建TestRecord目录失败！"));
			return;
		}
	}

}

// 唯一的一个 CLSAppApp 对象

CLSAppApp theApp;


// CLSAppApp 初始化

BOOL CLSAppApp::InitInstance()
{
	WriteDebugLog(_T("LSApp InitInstance start"));
	//系统已初始化完成....
	//satrtpage->setmsg(CString("系统已初始化完成...."));
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	
	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	CMultiPaneFrameWnd();
	
	pDocTemplate = new CMultiDocTemplate(IDR_LSAppTYPE,
		RUNTIME_CLASS(CLSAppDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CLSAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	//检查锁是否存在
	if (!InitPwdKey())
	{
		AfxMessageBox(_T("加密锁不存在,请联系设备厂商"));
	}

#if ONLINEMODE
	if (!CheckKeyExit())
	{
		AfxMessageBox(_T("加密锁不存在,请联系设备厂商"));
		return FALSE;
	}


	//登录
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


	/*********matlab环境初始化**********/
#if MATLAB_COMPILE
	if (!mclInitializeApplication(nullptr, 0))
	{

		AfxMessageBox(_T("mclInitializeApplication err"));
	}

	pg.SetStartInfo(CString("主程序正在加载中,请等待....(30%)"));

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

	pg.SetStartInfo(CString("主程序正在加载中,请等待....(60%)"));

	b_ok = FrHarmInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("FrHarmInitialize err"));

	}
	pg.SetStartInfo(CString("主程序正在加载中,请等待....(100%)"));
	/*bool b_ok = fftzcjInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("fftzcjInitialize err"));

	}*/



#endif
	Sleep(1000);
	pg.ShowWindow(SW_HIDE);


	////登录
	//CLogIn login;
	//login.DoModal();
	//if (!login.GetDalgRes())
	//{
	//	return FALSE;
	//}

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))//IDR_MAINFRAME
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);





	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
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
			WriteInfoLog(_T("打开Slot%d端口(COM%d, Baud=%d)成功"), pSlot->nSlotIndex,
				pSlot->pSerial->Port, pSlot->pSerial->Baud);
			char szBuff[16] = "\0";
			strcpy(szBuff, ">DISC\r\n");
			pSlot->pSerial->WriteBuffer(szBuff, strlen(szBuff), 3000);
		}
		catch (EDeviceError& e)
		{
			WriteErrorLog(_T("打开Slot%d端口(COM%d, Baud=%d)异常: %s"),
				pSlot->nSlotIndex, pSlot->pSerial->Port, pSlot->pSerial->Baud, e.Message);
			bStatus = false;
		}
		catch (...)
		{
			WriteErrorLog(_T("打开Slot%d端口(COM%d, Baud=%d)异常: 未知错误"),
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
		WriteErrorLog(_T("授权Key不存在！"));
		//MessageBox(_T("授权Key不存在！"), _T("错误"), MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

int CLSAppApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CLSAppApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CLSAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CLSAppApp::OnAppMoKuaiSheZhi()
{
	AfxMessageBox(_T("模块设置，未开发！"));
}
void CLSAppApp::OnOpenFile()
{
	// TODO: 在此处添加命令处理程序代码

	AfxMessageBox(_T("该模块未开发"));
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
	//回显使用代码
	data.setCyclesTimes(blueEreTest.getTestCycleTimes());
	data.setCyclesPeriod(blueEreTest.getTestCyclePeriod());
	data.setCyclesColr(blueEreTest.getTestCyclecolor());
	//data.setmainTestColr(blueEreTest.getmainTestColr());
	cyclesSet.setData(data);
	cyclesSet.DoModal();

	//设置更新后代码
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


// CLSAppApp 自定义加载/保存方法

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
	AfxMessageBox(_T("权限管理，未开发！"));
}

void CLSAppApp::OnPassWd()
{
	AfxMessageBox(_T("密码管理，未开发！"));
}

// CLSAppApp 消息处理程序





