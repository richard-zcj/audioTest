// BlueEreTest.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "BlueEreTest.h"
#include "afxdialogex.h"
#include "wavHandler.h"
#include <fstream>
#include <thread>


#include "JiLiSetNewVersion.h"
#include "AlogriSettins.h"
#include "TestProcess.h"

#include <Imm.h>

#pragma comment(lib, "imm32.lib")










extern CDeviceManage devicemanage;
extern JiLiSetNewVersion jilisetnew;//激励设置界面
extern CTestBorderLineSettings testborderline;
extern CPlayRecordSettings playrecordset;
extern CAlogriSettins  alogriSettins;


extern vector<char*> recordDatas;
extern int MMECALLBACKCOUNT;

UINT refreshTimes(LPVOID pParam);
UINT TestEreThread(LPVOID pParam);

// CBlueEreTest 对话框

IMPLEMENT_DYNAMIC(CBlueEreTest, CDialogEx)


CBlueEreTest::CBlueEreTest(CWnd* pParent /*=NULL*/)
: CDialogEx(CBlueEreTest::IDD, pParent), m_time(5000), m_slectall(false), m_bfr(1),
m_testID("LKTestID"), testCycleTimes(1000), currentTestTimes(0), m_color(RGB(0, 0, 255))
{
	//构造函数中不能获取窗口实例，此时窗口没有初始化完成

	m_isrunCycles = false;
	cyclesPeriod = 10;
	testCycleTimes = 10;
	focus_num = false;
	m_btestSPK = true;
	bIsSucess = true;
	m_bloadDriver = false;
	m_IsOffline = false;


	
	InitASIODriver(_T("LISSIG USB Audio Device"), true);
	
	 
}

CBlueEreTest::~CBlueEreTest()
{
	if (datashow)
		delete datashow;
}

void CBlueEreTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_eretime, m_time);
	DDX_Control(pDX, IDC_cmbtestItem, m_cmbTestItem);
	DDX_Check(pDX, IDC_CHECKFR, m_bfr);
	DDX_Check(pDX, IDC_CHECKTHD, m_bthd);
	DDX_Check(pDX, IDC_CHECKTHD_N, m_bthdn);
	DDX_Check(pDX, IDC_CHECKRub_Buzz, m_bRub_Buzz);
	DDX_Check(pDX, IDC_CHECKPhase, m_bPhase);
	DDX_Check(pDX, IDC_CHECKSensitivity, m_bSensitivity_spk);


	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_eretestIncom, mInchsnames);
	DDX_Control(pDX, IDC_eretestOutcom, mOutchsnames);

	DDX_Text(pDX, IDC_eretestTestTimes, playtimes);
	DDX_Control(pDX, IDC_eretestTestTimes, m_staticPlayTimes);

	DDX_Text(pDX, IDC_EDITtestId, m_testID);


	DDX_Control(pDX, IDC_EDITtestId, m_EditTextNum);
	DDX_Control(pDX, IDC_BUTTON4, m_startTest);
	DDX_Control(pDX, IDC_BUTTON9, m_stopTest);
}


BEGIN_MESSAGE_MAP(CBlueEreTest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBlueEreTest::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CBlueEreTest::OnBnClickedButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON3, &CBlueEreTest::OnBnClickedButtonRecordOnly)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, &CBlueEreTest::OnBnClickedRadio1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBlueEreTest::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON4, &CBlueEreTest::OnBnClickedButtonTest4)
	ON_BN_CLICKED(IDC_BUTTON5, &CBlueEreTest::OnBnClickedButtonRecordTest)
	ON_BN_CLICKED(IDC_BUTTON6, &CBlueEreTest::OnBnClickedButtonAutoConnEre)
	ON_BN_CLICKED(IDC_BUTTON7, &CBlueEreTest::OnBnClickedButtonDisconnectEre)
	ON_CBN_DROPDOWN(IDC_eretestIncom, &CBlueEreTest::OnCbnDropdowneretestincom)
	ON_CBN_DROPDOWN(IDC_eretestOutcom, &CBlueEreTest::OnCbnDropdowneretestoutcom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &CBlueEreTest::OnBnClickedButtonTestCycles)
	ON_BN_CLICKED(IDC_BUTTON9, &CBlueEreTest::OnBnClickedButtonStopTest)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON10, &CBlueEreTest::OnBnClickedButtonSwitchBlueMode)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDH_OK, &CBlueEreTest::OnOK)
	ON_EN_SETFOCUS(IDC_EDITtestId, &CBlueEreTest::OnEnSetfocusEdittestid)
	ON_EN_KILLFOCUS(IDC_EDITtestId, &CBlueEreTest::OnEnKillfocusEdittestid)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_MESSAGE(RESITEMSSHOW, &CBlueEreTest::OnResItemShow)
END_MESSAGE_MAP()


// CBlueEreTest 消息处理程序

//播放蓝牙耳机
void CBlueEreTest::OnBnClickedButtonPlay()
{
	// TODO:  在此添加控件通知处理程序代码
	//test.play(_T("play.wav"), 1000);

}

CTestResManagement* CBlueEreTest::GetResManagement(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		return &testResManage;
	case 1:
		return &testResManage2;
	case 2:
		return &testResManage3;
	case 3:
		return &testResManage4;
	default:
		break;
	}

	return NULL;
}

BOOL CBlueEreTest::OnInitDialog()
{
	/*CDialog::OnInitDialog();*/

	UpdateData(false);
	
	m_connBuleIsFial = true;

	/*
	CDataShow* testshow = new CDataShow;
	testshow->Create(CDataShow::IDD);

	vector<double> test1;
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);

	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);

	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);

	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);
	test1.push_back(1);

	test1.push_back(1);
	test1.push_back(1);

	vector<double> test2;
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);

	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);

	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);

	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);
	test2.push_back(2);

	test2.push_back(2);
	test2.push_back(2);

	vector<double> test3;
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);

	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);

	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);

	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);
	test3.push_back(3);

	test3.push_back(3);
	test3.push_back(3);

	vector<double> test4;
	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);

	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);

	test4.push_back(4);
	test4.push_back(40);
	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);

	test4.push_back(40);
	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);
	test4.push_back(4);

	test4.push_back(40);
	test4.push_back(40);

	testshow->setdata1(test1, 1);
	testshow->setdata1(test2, 2);
	testshow->setdata1(test3, 3);
	testshow->setdata1(test4, 4);
	

	vector<double> x;
	
	x.push_back(100);
	x.push_back(110);
	x.push_back(120);
	x.push_back(200);
	x.push_back(210);
	x.push_back(300);
	x.push_back(350);
	x.push_back(500);
	x.push_back(510);
	x.push_back(1000);
	x.push_back(1100);
	x.push_back(2000);
	x.push_back(2050);
	x.push_back(3000);
	x.push_back(4100);
	x.push_back(4500);
	x.push_back(5000);
	x.push_back(6000);
	x.push_back(7000);
	x.push_back(8000);

	testshow->setDataX(&x, x.size());
	testshow->setYscale(1000000);*/
	
	
	//testshow->ShowWindow(SW_SHOW);


	//mthdn.ShowWindow(TRUE);

	CRect rc;
	
	CEdit* pwnd = (CEdit*)GetDlgItem(IDC_EDITtestId);
	pwnd->GetClientRect(&rc);
	
	CFont * pFont = GetDlgItem(IDC_EDITtestId)->GetFont();
	//LOGFONT logFont;
	//pFont->GetLogFont(&logFont);
	//rc.top = rc.bottom / 2 - logFont.lfHeight;
	//rc.OffsetRect(5, 30);
	pwnd->SetRect(rc);
	CFont font;
	font.CreatePointFont(180, _T("宋体"));
	pwnd->SetFont(&font);
	//pwnd->SetWindowTextW(_T("\r\n123"));
	//rc.OffsetRect(5, 30);
	//::SendMessage(pwnd->m_hWnd, EM_SETRECT, 0,(LPARAM)&rc);


	HFONT hFont = CreateFont(30, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	CWnd* pgroupbox = GetDlgItem(IDC_STATICtest);
	pgroupbox->SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	pgroupbox = GetDlgItem(IDC_STATICerectl);
	pgroupbox->SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);

	hFont = CreateFont(20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	pgroupbox = GetDlgItem(IDC_BUTTON4);
	CFont * f = new CFont;
	f->CreateFontW(20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	CFont ff;
	ff.CreatePointFont(500, _T("宋体"));

	CButton * pbtn = (CButton*)GetDlgItem(IDC_BUTTON4);
	pbtn->GetClientRect(&rc);
	
	//GetDlgItem(IDC_BUTTON4)->SetFont(&ff);

	brush1.DeleteObject();
	brush1.CreateSolidBrush(RGB(248, 248, 255));


	CRect ResManageRc,winRc;
	testResManage.GetWindowRect(ResManageRc);
	ScreenToClient(ResManageRc);
	GetDlgItem(IDC_ereTestResnew)->GetWindowRect(winRc);
	ScreenToClient(winRc);
	
	testResManage.SetWindowPos(NULL, winRc.right+100, winRc.bottom+200, ResManageRc.Width(), ResManageRc.Height(), 0);
	testResManage.ShowWindow(SW_HIDE);
	testResManage.setHwnd(this->m_hWnd, waveShow.m_hWnd);
	testResManage.setWindowText(_T("1#"));

	testResManage2.GetWindowRect(ResManageRc);
	testResManage2.SetWindowPos(NULL, winRc.right + 100, winRc.bottom + 200, ResManageRc.Width(), ResManageRc.Height(), 0);
	testResManage2.ShowWindow(SW_HIDE);
	testResManage2.setHwnd(this->m_hWnd, waveShow.m_hWnd);
	testResManage2.setWindowText(_T("2#"));
	
	testResManage3.GetWindowRect(ResManageRc);
	testResManage3.SetWindowPos(NULL, winRc.right + 100, winRc.bottom + 200, ResManageRc.Width(), ResManageRc.Height(), 0);
	testResManage3.ShowWindow(SW_HIDE);
	testResManage3.setHwnd(this->m_hWnd, waveShow.m_hWnd);
	testResManage3.setWindowText(_T("3#"));
	
	testResManage4.GetWindowRect(ResManageRc);
	testResManage4.SetWindowPos(NULL, winRc.right + 100, winRc.bottom + 200, ResManageRc.Width(), ResManageRc.Height(), 0);
	testResManage4.ShowWindow(SW_HIDE);
	testResManage4.setHwnd(this->m_hWnd, waveShow.m_hWnd);
	testResManage4.setWindowText(_T("4#"));

	/*m_EditTextNum.GetClientRect(&rc);
	pFont=m_EditTextNum.GetFont();
	pFont->GetLogFont(&logFont);
	rc.top = rc.bottom / 2 - logFont.lfHeight;
	m_EditTextNum.SetRect(rc);*/

	HBITMAP hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 64, 64, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_startTest.SetBitmap(hbmp);

	hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 69, 69, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_stopTest.SetBitmap(hbmp);

	m_stopTest.ShowWindow(SW_HIDE);

	mainf = (CMainFrame*)theApp.GetMainWnd();

	if (m_IsOffline)
	{
		GetDlgItem(IDC_ereTestResnew)->SetWindowTextW(_T("OnLine"));
	}
	else
	{
		GetDlgItem(IDC_ereTestResnew)->SetWindowTextW(_T("OffLine"));
	}

	/*for (int i = 0; i < GetSlotCount(); i++)
	{
		CalcStimulusData(GetSlotParam(i), true);
		CalcStimulusData(GetSlotParam(i), false);
	}*/

	//加载测试项
	vector<CString> sTestItems;
	int id = 0;
	char sn[10]{0};
	for (size_t j = 0; j < 2; j++)////添加流程应与主流程一致
	{
		for (int i = 0; i < GetSlotCount(); i++)
		{
			slotParam* pSlot = GetSlotParam(i);
			if (j == 0)
			{
				sprintf(sn, "%d#", i+1);
				CString s1(sn);
				s1.Append(pSlot->vSpkItems[0].sAliasName);
				sTestItems.push_back(s1);
				memset(sn, 0, 10);
				sprintf(sn, "%d#", i + 1);
				CString s2(sn);
				s2.Append(pSlot->vSpkItems[1].sAliasName);
				sTestItems.push_back(s2);
				memset(sn, 0, 10);
				sprintf(sn, "%d#", i + 1);
				CString s3(sn);
				s3.Append(pSlot->vSpkItems[2].sAliasName);
				sTestItems.push_back(s3);
				for (int k = 0; k < pSlot->vSpkItems.size(); k++)
				{
					pSlot->vSpkItems[k].nWaveFormID = id;
					id++;
				}
			}
			else
			{
				memset(sn, 0, 10);
				sprintf(sn, "%d#", i + 1);
				CString s1(sn);
				s1.Append(pSlot->vMicItems[0].sAliasName);
				sTestItems.push_back(s1);
				memset(sn, 0, 10);
				sprintf(sn, "%d#", i + 1);
				CString s2(sn);
				s2.Append(pSlot->vMicItems[1].sAliasName);
				sTestItems.push_back(s2);
				memset(sn, 0, 10);
				sprintf(sn, "%d#", i + 1);
				CString s3(sn);
				s3.Append(pSlot->vMicItems[2].sAliasName);
				sTestItems.push_back(s3);
				for (int k = 0; k < pSlot->vMicItems.size(); k++)
				{
					pSlot->vMicItems[k].nWaveFormID = id;
					id++;
				}
			}

			//GetResManagement(i)->setID(i);

			GetResManagement(i)->setItem(sTestItems);
			sTestItems.clear();

		}
	}

	return TRUE;
}

void CBlueEreTest::destroyRes()
{

	testResManage.DestroyWindow();
	testResManage2.DestroyWindow();
	testResManage3.DestroyWindow();
	testResManage4.DestroyWindow();
	waveShow.DestroyWindow();
}

//录音蓝牙耳机
void CBlueEreTest::OnBnClickedButtonRecord()
{
	// TODO:  在此添加控件通知处理程序代码
	//test.playAndRecord(1000, _T("play.wav"));
}


void CBlueEreTest::OnBnClickedButtonRecordOnly()
{
	// TODO:  在此添加控件通知处理程序代码
	CString ss;
	GetDlgItem(IDC_eretime)->GetWindowTextW(ss);
	USES_CONVERSION;
	int time = atoi(T2A(ss));
	//test.openRecordThread(time);
}


HBRUSH CBlueEreTest::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if (pWnd->GetDlgCtrlID() == IDC_ereTestResnew)
	{
		CFont font ;
		font.CreatePointFont(390, _T("宋体"));
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(230, 10, 30));
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC||
		pWnd->GetDlgCtrlID() == IDC_EreID)
	{

		/*pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pDC->FillSolidRect(rc, RGB(248, 248, 255));*/

		pDC->SetBkColor(RGB(248, 248, 255));
		return brush1;
	}

	if (pWnd->GetDlgCtrlID() == IDC_ereTestResnew)
	{
		if (bIsSucess)
		{
			//pDC->SetBkColor(RGB(0, 255, 0));
		}
		else
		{
			//pDC->SetBkColor(RGB(255, 0, 0));
		}
		
		//return brush1;
	}


	return hbr;
}


void CBlueEreTest::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	
}


void CBlueEreTest::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int index = m_tab.GetCurSel();
	switch (index)
	{

	case 0:
		mfr.ShowWindow(TRUE);
		mthd.ShowWindow(FALSE);
		mthdn.ShowWindow(FALSE);
		mrubz.ShowWindow(FALSE);
		mphase.ShowWindow(FALSE);
		msen.ShowWindow(FALSE);
		break;
	case 1:
		mfr.ShowWindow(FALSE);
		mthd.ShowWindow(TRUE);
		mthdn.ShowWindow(FALSE);
		mrubz.ShowWindow(FALSE);
		mphase.ShowWindow(FALSE);
		msen.ShowWindow(FALSE);
		break;
	case 2:
		mfr.ShowWindow(FALSE);
		mthd.ShowWindow(FALSE);
		mthdn.ShowWindow(TRUE);
		mrubz.ShowWindow(FALSE);
		mphase.ShowWindow(FALSE);
		msen.ShowWindow(FALSE);
		break;

	case 3:
		mfr.ShowWindow(FALSE);
		mthd.ShowWindow(FALSE);
		mthdn.ShowWindow(FALSE);
		mrubz.ShowWindow(TRUE);
		mphase.ShowWindow(FALSE);
		msen.ShowWindow(FALSE);
		break;

	case 4:
		mfr.ShowWindow(FALSE);
		mthd.ShowWindow(FALSE);
		mthdn.ShowWindow(FALSE);
		mrubz.ShowWindow(FALSE);
		mphase.ShowWindow(TRUE);
		msen.ShowWindow(FALSE);
		break;

	case 5:
		mfr.ShowWindow(FALSE);
		mthd.ShowWindow(FALSE);
		mthdn.ShowWindow(FALSE);
		mrubz.ShowWindow(FALSE);
		mphase.ShowWindow(FALSE);
		msen.ShowWindow(TRUE);
		break;

	default:
		break;
	}
	*pResult = 0;
}

UINT TestEreThread(LPVOID pParam)
{
	/*
	CBlueEreTest* pBle = (CBlueEreTest*)pParam;

	pBle->playtimes = 0;
	m_bTesting = true;

	//std::thread testrefashtime(&CBlueEreTest::refreshTimes,this);
	CString s_time;
	s_time.Format(_T("%d 秒"), playtimes);
	m_staticPlayTimes.SetWindowTextW(s_time);
	AfxBeginThread(refreshTimes, this);

	// TODO:  在此添加控件通知处理程序代码
	//pBle->UpdateData();
	double* indata = NULL;
	int** recorddata = NULL;
	//CAudioAlgorithm audiotest;//这里加载matlab环境，慢


	CString playstr, recordstr;
	playrecordset.GetPlayAndRecord(playstr, recordstr);
	int fs = devicemanage.getfs(recordstr);
	double vp = devicemanage.getinputvp(recordstr);
	double vpout = devicemanage.getoutvp(playstr);
	//play(NULL);

	Sleep(1000);
	pBle->datafr.clear();
	pBle->datathd.clear();
	pBle->datathdn.clear();
	pBle->datarub.clear();;
	pBle->dataphase.clear();;
	pBle->datasen.clear();;
	vector<double> downY;
	vector<double> upY;
	vector<double> borderX;
	if (pBle->m_bfr)
	{
		char* driver = NULL;;
		devicemanage.getDriverType(recordstr, &driver);
		if (strcmp(driver, "ASIO") == 0)
		{
			//AfxMessageBox(_T("蓝牙测试不支持ASIO驱动！！！"));
			//return;'
			double playtimes = 0;
			devicemanage.record(recordstr, playstr, &recorddata, fs, playtimes);

			if (recorddata == NULL || *recorddata == NULL)
			{
				return -1;
			}
			char* fileName1 = "D:\\TestRecord\\fr01.wav";
			wavwrite(fileName1, 1, fs, 32, (char*)recorddata[0], _msize(recorddata[0]));
			if (_msize(recorddata[0]) / sizeof(int)>0)
				indata = new double[_msize(recorddata[0]) / sizeof(int)]{0};

			for (size_t i = 0; i < _msize(recorddata[0]) / sizeof(int); i++)
			{
				//if (recorddata[0][i]!=0)
				{
					pBle->datafr.push_back(recorddata[0][i]);
					indata[i] = recorddata[0][i];

				}


			}


			delete[] recorddata[0];
			pBle->audiotest.setDriverMode(0);
		}
		else
		{


			devicemanage.wdmPlayWithRecord(recordstr, playstr, 0, "D:\\TestRecord\\playjili.wav");//播放时间必须是文件的实际时长

			//读取约定文件，给indata赋值
			if (jilisetnew.getPlayTimes()>0)
				indata = new double[(int)(fs*jilisetnew.getPlayTimes())]{0};

			devicemanage.wav_readBymatlab(&indata, fs, jilisetnew.getPlayTimes());
			pBle->audiotest.setDriverMode(1);
		}

		delete driver;

		if (pBle->datashow != NULL)
		{
			delete pBle->datashow;
			pBle->datashow = NULL;
		}
		pBle->datashow = new CDataShow;
		pBle->datashow->Create(CDataShow::IDD);//执行此函数创建实体窗口
		pBle->datashow->setwindowtile(CString("频率响应测试结果"));
		pBle->datashow->setchartname(CString("FR"));
		pBle->datashow->rotalchartY();
		pBle->datashow->setYtitle(CString("SPLS/dB"));
		pBle->datashow->setXtitle(CString("Hz"));
		pBle->datashow->setYscale(100);

		pBle->audiotest.AudioAlgorithmInit();
		pBle->audiotest.setDeviceInputVpval(devicemanage.getinputvp(recordstr));
		pBle->audiotest.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		pBle->audiotest.setIsComputerNorse(true);

		pBle->audiotest.Alg_Fr(indata, fs);
		pBle->mainf->setStatusStr(CString("thd计算中..."));
		int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
		pBle->datafr.clear();
		for (size_t i = 0; i < pBle->audiotest.fr_data.size(); i++)
		{

			pBle->datafr.push_back(pBle->audiotest.fr_data[i]);

		}

		bool isSuccess = false;

		double hz, down, up;


		for (size_t i = 0; i < testborderline.fr_borderLine.size(); i++)
		{
			testborderline.fr_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.fr_borderLine.size(); i++)
		{

			testborderline.fr_borderLine[i]->getdata(hz, down, up);
			isSuccess = pBle->FrChecked(hz, down, up, pBle->audiotest.getpindiandata(), &pBle->datafr, testborderline.fr_borderLine);

			if (!isSuccess)
				break;


		}


		/****************************测试结果判定**************************/


		/*datafr.push_back(2.3);
		datafr.push_back(1.3);
		datafr.push_back(0.3);
		datafr.push_back(2.3);
		datafr.push_back(3.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(46.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);*/


		/*for (size_t i = 0; i < 100; i++)
		{
		datafr.push_back(abs(5.0*sin(2*3.1415926*1000*i/44100)));
		}*/
		/*datafr.push_back(3080.56778);
		datafr.push_back(1080.05078);
		datafr.push_back(1080.00000);
		datafr.push_back(1080.05078);
		datafr.push_back(380.056778);
		datafr.push_back(1800.05078);
		datafr.push_back(380.056778);*/

		/*for (size_t i = 0; i < audiotest.getpindiandata()->size(); i++)
		{
		datafr.push_back(2);
		}*/
		/*datafr.push_back(2);
		datafr.push_back(2);
		datafr.push_back(2);
		pBle->datashow->setTestResult(isSuccess);
		pBle->datashow->setData(&pBle->datafr, pBle->datafr.size());
		pBle->datashow->setDataX(pBle->audiotest.getpindiandata(), pBle->audiotest.getpindiandata()->size());

		//设置框线数据
		pBle->datashow->setDataBorderLine(borderX, borderX.size());
		pBle->datashow->setDataBorderLinedown(downY, downY.size());
		pBle->datashow->setDataBorderLineup(upY, upY.size());
		pBle->mfr.setDara(&pBle->datafr);

		if (pBle->datafr.size()>0)
			pBle->datashow->ShowWindow(SW_SHOW);

		if (indata != NULL)
			delete[] indata;

		pBle->audiotest.saveData("D:\\TestRecord\\frdata.txt", *pBle->audiotest.getpindiandata(), pBle->datafr);

	}

	if (pBle->m_bthd)
	{
		pBle->mainf->setStatusStr(CString("THD计算中..."));
		/*datathd.push_back(0.12234);
		datathd.push_back(0.3786);
		datathd.push_back(0.445);
		datathd.push_back(0.21231);
		datathd.push_back(0.253457);
		datathd.push_back(0.36124);
		datathd.push_back(0.92234);
		datathd.push_back(0.3);
		datathd.push_back(0.2545);
		datathd.push_back(0.36);
		datathd.push_back(1.0);
		datathd.push_back(0.21123);
		datathd.push_back(0.7);

		if (pBle->datashowthd != NULL)
		{
			delete pBle->datashowthd;
			pBle->datashowthd = NULL;
		}
		pBle->datashowthd = new CDataShow;
		pBle->datashowthd->Create(CDataShow::IDD);//执行此函数创建实体窗口
		pBle->datashowthd->setwindowtile(CString("频率失真测试结果"));
		pBle->datashowthd->setchartname(CString("THD"));
		pBle->datashowthd->rotalchartY();
		pBle->datashowthd->setYtitle(CString("THD(%)"));
		pBle->datashowthd->setXtitle(CString("Hz"));
		pBle->datashowthd->setYscale(10000);
		//audiotest.Alg_Fr(indata, fs);
		pBle->mainf->setStatusStr(CString("FR计算中..."));
		int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
		//datathd.clear();
		for (size_t i = 0; i < pBle->audiotest.thd_data.size(); i++)
		{

			pBle->datathd.push_back(pBle->audiotest.thd_data[i]);

		}

		bool isSuccess = false;



		borderX.clear();
		downY.clear();
		upY.clear();
		double hz, down, up;
		for (size_t i = 0; i < testborderline.thd_borderLine.size(); i++)
		{
			testborderline.thd_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.thd_borderLine.size(); i++)
		{

			testborderline.thd_borderLine[i]->getdata(hz, down, up);

			isSuccess = pBle->FrChecked(hz, down, up, pBle->audiotest.getpindiandata(), &pBle->datathd, testborderline.thd_borderLine);

			if (!isSuccess)
				break;


		}


		

		pBle->datashowthd->setData(&pBle->datathd, pBle->datathd.size());
		pBle->datashowthd->setDataX(pBle->audiotest.getpindiandata(), pBle->audiotest.getpindiandata()->size());



		pBle->datashowthd->setTestResult(isSuccess);
		if (pBle->datathd.size()>0)
			pBle->datashowthd->ShowWindow(SW_SHOW);

		//设置框线数据
		pBle->datashowthd->setDataBorderLine(borderX, borderX.size());
		pBle->datashowthd->setDataBorderLinedown(downY, downY.size());
		pBle->datashowthd->setDataBorderLineup(upY, upY.size());
		pBle->mthd.setDara(&pBle->datathd);

		pBle->audiotest.saveData("D:\\TestRecord\\thddata.txt", *pBle->audiotest.getpindiandata(), pBle->datathd);
	}


	if (pBle->m_bthdn)
	{
		pBle->mainf->setStatusStr(CString("THD+N计算中..."));
		/*datathdn.push_back(200.3);
		datathdn.push_back(100.3);
		datathdn.push_back(45.3);
		datathdn.push_back(123.3);
		datathdn.push_back(32.3);
		datathdn.push_back(26.3);
		datathdn.push_back(112.3);
		datathdn.push_back(48.3);
		datathdn.push_back(112.3);
		datathdn.push_back(32.3);
		datathdn.push_back(25.3);
		datathdn.push_back(18.3);
		datathdn.push_back(49.3);
		datathdn.push_back(112.3);
		datathdn.push_back(132.3);
		datathdn.push_back(112.3);
		datathdn.push_back(111.3);
		datathdn.push_back(148.3);
		datathdn.push_back(112.3);
		datathdn.push_back(32.3);
		datathdn.push_back(114.3);
		datathdn.push_back(112.3);
		datathdn.push_back(132.3);
		datathdn.push_back(112.3);
		datathdn.push_back(111.3);
		datathdn.push_back(114.3);
		datathdn.push_back(112.3);
		datathdn.push_back(132.3);


		//for (size_t i = 0; i < audiotest.getpindiandata()->size(); i++)
		//{
		//	datathdn.push_back(100+abs(132.3*sin(2*3.1415926*i*1000/3000)));
		//	//datathdn.push_back(132.3);
		//}



		if (pBle->datashowthd_N != NULL)
		{
			delete pBle->datashowthd_N;
			pBle->datashowthd_N = NULL;
		}
		pBle->datashowthd_N = new CDataShow;
		pBle->datashowthd_N->Create(CDataShow::IDD);//执行此函数创建实体窗口
		pBle->datashowthd_N->setwindowtile(CString("频率失真+噪音测试结果"));
		pBle->datashowthd_N->setchartname(CString("THD+N"));
		pBle->datashowthd_N->rotalchartY();
		pBle->datashowthd_N->setYtitle(CString("THD+N(%)"));
		pBle->datashowthd_N->setXtitle(CString("Hz"));
		pBle->datashowthd_N->setYscale(10000);

		pBle->datathdn.clear();
		for (size_t i = 0; i < pBle->audiotest.thdn_data.size(); i++)
		{

			pBle->datathdn.push_back(pBle->audiotest.thdn_data[i]);

		}


		pBle->datashowthd_N->setData(&pBle->datathdn, pBle->datathdn.size());
		pBle->datashowthd_N->setDataX(pBle->audiotest.getpindiandata(), pBle->audiotest.getpindiandata()->size());
		//if (datathdn.size()>0)


		pBle->datashowthd_N->setTestResult(false);
		pBle->datashowthd_N->ShowWindow(SW_SHOW);

		//设置框线数据
		pBle->datashowthd_N->setDataBorderLine(borderX, borderX.size());
		pBle->datashowthd_N->setDataBorderLinedown(downY, downY.size());
		pBle->datashowthd_N->setDataBorderLineup(upY, upY.size());
		pBle->mthdn.setDara(&pBle->datathdn);

		pBle->audiotest.saveData("D:\\TestRecord\\thdndata.txt", *pBle->audiotest.getpindiandata(), pBle->datathdn);

	}
	if (pBle->m_bRub_Buzz)
	{
		pBle->mainf->setStatusStr(CString("Rub&Buzz计算中..."));
		/*datarub.push_back(2.3);
		datarub.push_back(1.3);
		datarub.push_back(4.3);
		datarub.push_back(12.3);
		datarub.push_back(32.3);*/
		/*datathd.push_back(0.12234);
		datathd.push_back(0.3786);
		datathd.push_back(0.445);
		datathd.push_back(0.21231);
		datathd.push_back(0.253457);
		datathd.push_back(0.36124);
		datathd.push_back(0.92234);
		datathd.push_back(0.3);
		datathd.push_back(0.2545);
		datathd.push_back(0.36);
		datathd.push_back(1.0);
		datathd.push_back(0.21123);
		datathd.push_back(0.7);

		if (pBle->datashowRB != NULL)
		{
			delete pBle->datashowRB;
			pBle->datashowRB = NULL;
		}
		pBle->datashowRB = new CDataShow;
		pBle->datashowRB->Create(CDataShow::IDD);//执行此函数创建实体窗口
		pBle->datashowRB->setwindowtile(CString("RB测试结果"));
		pBle->datashowRB->setchartname(CString("RB"));
		pBle->datashowRB->rotalchartY();
		pBle->datashowRB->setYtitle(CString("RB(%)"));
		pBle->datashowRB->setXtitle(CString("Hz"));
		pBle->datashowRB->setYscale(10000);
		//audiotest.Alg_Fr(indata, fs);
		pBle->mainf->setStatusStr(CString("RB计算中..."));
		int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
		//datathd.clear();
		for (size_t i = 0; i < pBle->audiotest.rb_data.size(); i++)
		{

			pBle->datarub.push_back(pBle->audiotest.rb_data[i]);

		}

		bool isSuccess = false;
		


		borderX.clear();
		downY.clear();
		upY.clear();
		double hz, down, up;
		for (size_t i = 0; i < testborderline.thd_borderLine.size(); i++)
		{
			testborderline.thd_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.thd_borderLine.size(); i++)
		{

			testborderline.thd_borderLine[i]->getdata(hz, down, up);

			isSuccess = pBle->FrChecked(hz, down, up, pBle->audiotest.getpindiandata(), &pBle->datarub, testborderline.thd_borderLine);

			if (!isSuccess)
				break;


		}


		

		pBle->datashowRB->setData(&pBle->datarub, pBle->datarub.size());
		pBle->datashowRB->setDataX(pBle->audiotest.getpindiandata(), pBle->audiotest.getpindiandata()->size());



		pBle->datashowRB->setTestResult(isSuccess);
		if (pBle->datarub.size()>0)
			pBle->datashowRB->ShowWindow(SW_SHOW);

		//设置框线数据
		pBle->datashowRB->setDataBorderLine(borderX, borderX.size());
		pBle->datashowRB->setDataBorderLinedown(downY, downY.size());
		pBle->datashowRB->setDataBorderLineup(upY, upY.size());

		//mrubz.setDara(&datarub);

		pBle->audiotest.saveData("D:\\TestRecord\\rbdata.txt", *pBle->audiotest.getpindiandata(), pBle->datarub);
	}
	if (pBle->m_bPhase)
	{
		pBle->mainf->setStatusStr(CString("Phase计算中..."));
		pBle->dataphase.push_back(2.3);
		pBle->dataphase.push_back(1.3);
		pBle->dataphase.push_back(4.3);
		pBle->dataphase.push_back(12.3);
		pBle->dataphase.push_back(32.3);
		pBle->mphase.setDara(&pBle->dataphase);
	}
	if (pBle->m_bSensitivity)
	{
		pBle->mainf->setStatusStr(CString("Sensitivity计算中..."));
		pBle->datasen.push_back(2.3);
		pBle->datasen.push_back(1.3);
		pBle->datasen.push_back(4.3);
		pBle->datasen.push_back(12.3);
		pBle->datasen.push_back(32.3);
		pBle->msen.setDara(&pBle->datasen);
	}

	pBle->mainf->setStatusStr(CString("计算中完成..."));
	pBle->mfr.updateshow();
	pBle->mthd.updateshow();
	pBle->mthdn.updateshow();
	pBle->mrubz.updateshow();
	pBle->mphase.updateshow();
	pBle->msen.updateshow();
	m_bTesting = false;
*/

return 0;
}
//cycle 片号
void CBlueEreTest::updateanalysisStatus_spk(const vector<int>* analysisdata, int cycle, char* name)
{
	m_bfr = false;
	m_bthd = false;
	m_bthdn = false;
	m_bRub_Buzz = false;
	m_bSensitivity_spk = false;
	M_Polarity = false;
	m_Balance = false;
	m_snr = false;
	if (cycle==0)
	   testResManage.clearItem();
	else if (cycle == 1)
	{
		testResManage2.clearItem();
	}
	else if (cycle == 2)
	{
		testResManage3.clearItem();
	}
	else if (cycle == 3)
	{
		testResManage4.clearItem();
	}
	vector<CString> items;
	for (size_t i = 0; i < analysisdata->size(); i++)
	{
		if (analysisdata->at(i) == 1)
		{
			m_bfr = true;
			items.push_back(_T("SPK_FR"));
		}
		else if (analysisdata->at(i) == 2)
		{
			m_bthd = true;
			items.push_back(_T("THD"));
		}
		else if (analysisdata->at(i) == 7)
		{
			m_bthdn = true;
			items.push_back(_T("THDN"));
		}
		else if (analysisdata->at(i) == 8)
		{
			m_bRub_Buzz = true;
			items.push_back(_T("RB"));
		}
		else if (analysisdata->at(i) == 5)
		{
			m_bPhase = true;
			items.push_back(_T("Phase"));
		}
		else if (analysisdata->at(i) == 3)
		{
			m_bSensitivity_spk = true;
			items.push_back(_T("Sensitivity_spk"));
		}
		else if (analysisdata->at(i) == 6)
		{
			M_Polarity = true;
			items.push_back(_T("Polarity"));
		}
		else if (analysisdata->at(i) == 9)
		{
			m_Balance = true;
			items.push_back(_T("Balance"));
		}
		else if (analysisdata->at(i) == 4)
		{
			m_snr = true;
			items.push_back(_T("SNR"));
		}
			
	}

	items.clear();
	items.push_back(CString(name));

	//items.push_back(_T("响应信号"));
	if (cycle == 0)
	{
		testResManage.setItem(items);
	}
	else if (cycle == 1)
	{
		testResManage2.setItem(items);
	}
	else if (cycle == 2)
	{
		testResManage3.setItem(items);
	}
	else if (cycle == 3)
	{
		testResManage4.setItem(items);
	}
	
}

void CBlueEreTest::updateanalysisStatus_mic(const vector<int>* analysisdata, int cycle,char* name)
{
	m_bfr = false;
	m_bthd = false;
	m_bthdn = false;
	m_bRub_Buzz = false;
	m_bSensitivity_mic = false;
	m_NoiseReduction = false;
	m_snr = false;
	//testResManage.clearItem();
	if (cycle == 0)
		testResManage.clearItem();
	else if (cycle == 1)
	{
		testResManage2.clearItem();
	}
	else if (cycle == 2)
	{
		testResManage3.clearItem();
	}
	else if (cycle == 3)
	{
		testResManage4.clearItem();
	}
	vector<CString> items;
	for (size_t i = 0; i < analysisdata->size(); i++)
	{
		if (analysisdata->at(i) == 1)
		{
			m_bfr = true;
			items.push_back(_T("MIC_FR"));
		}
		else if (analysisdata->at(i) == 2)
		{
			m_bthd = true;
			items.push_back(_T("THD"));
		}
		else if (analysisdata->at(i) == 6)
		{
			m_bthdn = true;
			items.push_back(_T("THDN"));
		}
		else if (analysisdata->at(i) == 7)
		{
			m_bRub_Buzz = true;
			items.push_back(_T("RB"));
		}
		else if (analysisdata->at(i) == 5)
		{
			m_bPhase = true;
			items.push_back(_T("Phase"));
		}
		else if (analysisdata->at(i) == 3)
		{
			m_bSensitivity_mic = true;
			items.push_back(_T("Sensitivity_mic"));
		}
		else if (analysisdata->at(i) == 8)
		{
			m_NoiseReduction = true;
			items.push_back(_T("NoiseReduction"));
		}
		else if (analysisdata->at(i) == 4)
		{
			m_snr = true;
			items.push_back(_T("SNR"));
		}
		

	}

	items.clear();
	items.push_back(CString(name));

	items.push_back(_T("响应信号"));

	//testResManage.setItem(items);
	if (cycle == 0)
	{
		testResManage.setItem(items);
	}
	else if (cycle == 1)
	{
		testResManage2.setItem(items);
	}
	else if (cycle == 2)
	{
		testResManage3.setItem(items);
	}
	else if (cycle == 3)
	{
		testResManage4.setItem(items);
	}
}

CTestProcess* process=NULL;
CTestProcess* processClose = NULL;
DWORD WINAPI runProcess(LPVOID lParam)
{
	/*CBlueEreTest* bel = (CBlueEreTest*)lParam;
	process = new CTestProcess;
	process->Create(IDD_DIgTestProcess, bel);
	process->ShowWindow(SW_SHOW);

	while (1)
		;*/
	if (process != NULL)
	{
		process = NULL;	
	}
	CTestProcess tp;
	CBlueEreTest* bel = (CBlueEreTest*)lParam;
	process = &tp;
	processClose = process;
	tp.DoModal();
	
	return 0;
}

void CBlueEreTest::HideProcess(bool bhide)
{
	if (process != NULL)
	{
		if (bhide)
		  process->ShowWindow(SW_HIDE);
		else
		{
			process->ShowWindow(SW_SHOW);
		}
	}
}


struct PlayRecordData
{
	int palystr;
	CString jili;
	int recordstr;
	CString path;
	int record_index;
	int play_index;
	bool isASIO_in;
	bool isASIO_out;
	double fs;
	double recordtime;
	CString playName;
	CString recordName;


};

struct TestBlueData
{
	int play;
	int play_index;
	CString jili;
	int record;
	int record_index;
	CString path;
	int data_index;
	int data2_index;
	bool isASIO_in;
	bool isASIO_out;
	double fs;
	double recordtime;
	CString playName;//WDN驱动对应的设备名称
	CString recordName;

	int num_thread;

};

HANDLE mainthread;
vector<HANDLE> mainthread_list;
vector<TestBlueData*> testdataParam;
int testThreadCount;

DWORD WINAPI runPlayThread(LPVOID pParam)
{
	TestBlueData* pldata = testdataParam[testThreadCount-1];
	if (pldata->isASIO_out)
	{
		//asio

		devicemanage.play(pldata->play, pldata->jili, pldata->isASIO_out, pldata->fs, pldata->play_index, "",
			pldata->playName);
	}
	else
	{
		devicemanage.play(pldata->play, pldata->jili, pldata->isASIO_out, pldata->fs, pldata->play_index, "D:\\TestRecord\\playjili.wav",
			pldata->playName);
	}
	
	
	return 0;
}

DWORD WINAPI runRecordThread(LPVOID pParam)
{
	TestBlueData* pldata = testdataParam[testThreadCount-1];

	if (pldata->isASIO_in)
	{


		int** recorddata = NULL;
		devicemanage.record2(pldata->record, pldata->path, &recorddata, pldata->record_index, pldata->isASIO_in, pldata->fs
			, pldata->recordtime);

		char fileName1[1024]{0};
		sprintf(fileName1, "D:\\TestRecord\\fr_%d.wav", testThreadCount);


		//把录音到的数据，输出

		std::ofstream outfile1;
		CString path1("D:\\TestRecord\\record.txt");
		outfile1.open(path1, std::ios::out | std::ios::trunc);
		for (size_t i = 0; i < _msize(recorddata[0]) / sizeof(int); i++)
		{
			outfile1 << recorddata[0][i] << endl;
		}
		outfile1.close();

		wavwrite(fileName1, 1, pldata->fs, 32, (char*)(recorddata[0]), pldata->fs * pldata->recordtime * 32 / 8);

	}
	else
	{
		//wdm
		double* indata = NULL;
		
		devicemanage.record2(pldata->record, pldata->path,NULL, pldata->record_index, pldata->isASIO_in, pldata->fs
			, pldata->recordtime);

		devicemanage.wav_readBymatlab(&indata, pldata->fs, pldata->recordtime,"");

		//创建wdm录音线程



	}
	return 0;
}

DWORD WINAPI runPlayRecordThread(LPVOID pParam)
{
	TestBlueData* pldata = testdataParam[testThreadCount - 1];
	
	int** recorddata = NULL;
	if (pldata->isASIO_in)
	{
		devicemanage.record_new(pldata->record, pldata->play, &recorddata, pldata->fs, pldata->recordtime, pldata->record_index
			,pldata->play_index);

		char fileName1[1024]{0};
		sprintf(fileName1, "D:\\TestRecord\\fr_%d.wav", testThreadCount);


		//把录音到的数据，输出

		std::ofstream outfile1;
		CString path1("D:\\TestRecord\\record.txt");
		outfile1.open(path1, std::ios::out | std::ios::trunc);
		for (size_t i = 0; i < _msize(recorddata[0]) / sizeof(int); i++)
		{
			outfile1 << recorddata[0][i] << endl;
		}
		outfile1.close();

		wavwrite(fileName1, 1, pldata->fs, 32, (char*)(recorddata[0]), pldata->fs * pldata->recordtime * 32 / 8);
	}
	else
	{
		double* indata = NULL;
		devicemanage.wdmPlayWithRecord_new(pldata->record, pldata->play, 0, "D:\\TestRecord\\playjili.wav",
			pldata->playName, pldata->recordName, pldata->fs);//播放时间必须是文件的实际时长
	

		devicemanage.wav_readBymatlab(&indata, pldata->fs, pldata->recordtime,"");
	}


	return 0;
}





DWORD WINAPI runTestBlue(LPVOID pParam)
{
	DWORD times1 = 0;
	DWORD times2 = 0;
	
	
	//测试新功能，根据播放采集模块设置的顺序执行测试流程
		TestBlueData* testParam = testdataParam[testThreadCount++];
		times2 = (testParam->recordtime+2) * 1000;
		//执行播放或录音 ;这里的录音和播放将使用线程，也就是同时进行

		//1.播放与录音不同设备

		  //1.2驱动不同
		 //播放
		/*PlayRecordData pldata;
		pldata.palystr = testParam->play;
		pldata.jili = testParam->jili;
		pldata.recordstr = testParam->record;
		pldata.path = testParam->path;
		pldata.play_index = testParam->play_index;
		pldata.record_index = testParam->record_index;
		pldata.isASIO_in = testParam->isASIO_in;
		pldata.isASIO_out = testParam->isASIO_out;
		pldata.fs = testParam->fs;
		pldata.recordtime = testParam->recordtime;


		PlayRecordData pldata2;
		pldata.palystr = testParam->play;
		pldata.jili = testParam->jili;
		pldata.recordstr = testParam->record;
		pldata.path = testParam->path;
		pldata.play_index = testParam->play_index;
		pldata.record_index = testParam->record_index;
		pldata.isASIO_in = testParam->isASIO_in;
		pldata.isASIO_out = testParam->isASIO_out;
		pldata.fs = testParam->fs;
		pldata.recordtime = testParam->recordtime;*/

		if (testParam->isASIO_in != testParam->isASIO_out)
		{


			//times1 = 4 * 1000;//这里实际为激励时长



			HANDLE subthread1 = CreateThread(NULL, 0, runPlayThread, 0, 0, NULL);

			//录音
			//times2 = 4 * 1000;
			HANDLE subthread2 = CreateThread(NULL, 0, runRecordThread, 0, 0, NULL);

			//线程等待
			WaitForSingleObject(subthread1, times2);
			WaitForSingleObject(subthread2, times2);
			//WaitForSingleObject(mainthread_list[0], times2>times1 ? times2 : times1);
			CloseHandle(subthread1);
			CloseHandle(subthread2);
			//

			

			
		}
		else
		{
			//2.播放与录音相同设备

			//2.1驱动必须一致	

			//这里只创建一个线程
			HANDLE subthread2 = CreateThread(NULL, 0, runPlayRecordThread, 0, 0, NULL);
			WaitForSingleObject(subthread2, times2);
			CloseHandle(subthread2);	
			
		}

		if (testThreadCount < mainthread_list.size())
			ResumeThread(mainthread_list[testThreadCount]);

		//关闭所有主线程
		if (testThreadCount >= mainthread_list.size())
		{
			for (size_t i = 0; i < mainthread_list.size(); i++)
			{
				CloseHandle(mainthread_list[i]);
				delete testdataParam[i];
			}

			mainthread_list.clear();
			testdataParam.clear();
		}
	return 0;
}

//处理线程同步问题
void CBlueEreTest::OnBnClickedButtonTest2()
{

	//在线程中执行测试流程

	vector<PlayRecordDialogData*>* prdata = playrecordset.getPlayRecordData();
	int row = prdata->size();// playrecordset.mlistPlayRecordItems.GetItemCount() - 1;
	CString play;
	CString jili;
	CString record;
	CString path;
	testThreadCount = 0;
	int chs = 0;
	int bits = 0;
	int mmeindex = 0;
	for (int i = 0; i < row; i++)
	{
		TestBlueData* testParam = new TestBlueData;
		//playrecordset.GetPlayRecordInfo(i, play, jili, record, path);
		play=(*prdata)[i]->play;
		jili = (*prdata)[i]->jili;
		record = (*prdata)[i]->record;
		testParam->jili = jili;
		
		testParam->path = path;
		testParam->record_index = devicemanage.getASIOindex(record, true, &testParam->data_index, testParam->isASIO_in, testParam->fs, testParam->recordName, chs, bits, mmeindex);
		testParam->play_index = devicemanage.getASIOindex(play, false, &testParam->data2_index, testParam->isASIO_out, testParam->fs, testParam->playName, chs, bits, mmeindex);
		testParam->play = testParam->data2_index;
		testParam->record = testParam->data_index;
		testParam->num_thread = row;
		testParam->recordtime = 1.0*jilisetnew.data.size() / testParam->fs;

		if (i > 0)
		{
			mainthread = CreateThread(NULL, 0, runTestBlue, 0, CREATE_SUSPENDED, NULL);
		}
		else
		{
			mainthread = CreateThread(NULL, 0, runTestBlue,0, CREATE_SUSPENDED, NULL);
		}
		 
		 mainthread_list.push_back(mainthread);
		 testdataParam.push_back(testParam);
		

	}

	ResumeThread(mainthread_list[0]);
  
}

DWORD WINAPI runMMEPlayThread(LPVOID param)
{
	/*testASIUO.playMME(pPlayData, pDev->nSamplingRate, pDev->nChannel,
		pDev->nBitDepth, pDev->nDevIndex, pDev->nVp);*/

	TestBlueData* pldata = testdataParam[0];
	if (pldata->isASIO_out)
	{
		//asio

		devicemanage.play(pldata->play, pldata->jili, pldata->isASIO_out, pldata->fs, pldata->play_index, "",
			pldata->playName);
	}
	else
	{
		devicemanage.play(pldata->play, pldata->jili, pldata->isASIO_out, pldata->fs, pldata->play_index, "D:\\TestRecord\\playjili.wav",
			pldata->playName);
	}

	delete pldata;
	
	return 0;

}

//double CBlueEreTest::FindPos(double nVal, int& nPos)
//{
//	double nRet = 0;
//	double nMin = 0;
//	nMin = abs(nVal - GetIndexOfAllFrPoint(0));
//	nRet = GetIndexOfAllFrPoint(0);
//	nPos = 0;
//
//	for (size_t i = 1; i < GetAllFrPointCount(); i++)
//	{
//		if (abs(nVal - GetIndexOfAllFrPoint(i))< nMin)
//		{
//			nRet = GetIndexOfAllFrPoint(i);
//			nMin = abs(nVal - GetIndexOfAllFrPoint(i));
//			nPos = i;
//
//		}
//	}
//
//	return nRet;
//}
//
//void CBlueEreTest::GetFrPoints(int nStimIndex)
//{
//	int nStep = 0;
//	stimParam* pStim = GetStimParam(nStimIndex);
//	switch (pStim->nOctave)
//	{
//	case 0:
//		//1/3
//		nStep = 24 / 3;
//		break;
//	case 1:
//		//1/6
//		nStep = 24 / 6;
//		break;
//	case 2:
//		//1/12
//		nStep = 24 / 12;
//		break;
//	case 3:
//		//1/24
//		nStep = 24 / 24;
//		break;
//	default:
//		break;
//	}
//
//	//1.确定开始频点
//	int nPos1 = 0;
//	int nPos2 = 0;
//	double nVal = FindPos(pStim->nFrequencyStart, nPos1);
//	nVal = FindPos(pStim->nFrequencyStop, nPos2);
//
//	//2.根据步长确定取频点
//	for (size_t i = nPos1; i < GetAllFrPointCount(); i += nStep)
//	{
//		if (nPos2 <= i)
//		{
//			break;
//		}
//			
//		AddStimFrPoint(nStimIndex, GetIndexOfAllFrPoint(i));
//	}
//
//	AddStimFrPoint(nStimIndex, nVal);
//}
//
//void CBlueEreTest::CalcStimulusData(slotParam* pSlot, bool bSpk)
//{
//	//1.生成激励信号
//	//1.1 根据倍频程确定频点
//	int nPlayRecord = pSlot->nMicPlayRecord;
//	if (bSpk)
//	{
//		nPlayRecord = pSlot->nSpkPlayRecord;
//	}
//
//	prParam* pPlayRecord = GetPlayRecord(nPlayRecord, bSpk);
//	stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);
//	devParam* pDev = GetDevParam(pPlayRecord->nPlayIndex, false);
//
//	ClearStimData(pPlayRecord->nStimIndex);
//	ClearStimFrPoint(pPlayRecord->nStimIndex);
//	ClearFrPointPos(pPlayRecord->nStimIndex);
//	GetFrPoints(pPlayRecord->nStimIndex);
//
//	/*if (pStim->bEQEnable)
//	{
//		if (m_listEQ.size() != pinduan.size())
//		{
//			AfxMessageBox(_T("EQ文件，频点数错误！"), MB_ICONERROR);
//			return;
//		}
//	}*/
//
//
//	//1.2 根据频点集合创建激励信号
//	int count = 0;
//	double v = pStim->nVoltage * pow(2, 31) * sqrt(2) / pDev->nVp;//pow(2, 31)
//
//	if (pDev->sDriver.IsEmpty())
//	{
//		AfxMessageBox(_T("设备管理，输出通道未设置！"));
//		return;
//	}
//
//	int f1 = pStim->nCycle / (pStim->nDuration * 0.001);
//	double f;
//	
//
//	//信号前补0
//	double offsetTime = 0.05;
//	int buling = pStim->nSamplingRate * offsetTime;
//	for (size_t i = 0; i < buling; i++)
//	{
//		AddStimData(pPlayRecord->nStimIndex, 0);
//	}
//
//	for (size_t i = 0; i < GetAllFrPointCount(); i++)
//	{
//		f = GetIndexOfAllFrPoint(i);
//		int cur_samples = 0;
//
//		if (f <= f1)
//		{
//			cur_samples = pStim->nCycle * pStim->nSamplingRate / f;//cycle/f cycle个周期 乘以1/f 一个周期的时间 等于 cycle个周期的时间 每秒的点数fs  总点数 fs*总时间
//
//		}
//		else
//		{
//
//			cur_samples = round(pStim->nDuration * 0.001 * f) * pStim->nSamplingRate / f;
//		}
//
//		/*if (m_eq)
//		{
//			v = m_listEQ[i] * pow(2, 31)*sqrt(2) / vp;
//		}*/
//
//		//pos[f] = cur_samples;
//		AddFrPointPos(pPlayRecord->nStimIndex, cur_samples);
//
//		for (size_t j = 0; j <cur_samples; j++)
//		{
//			AddStimData(pPlayRecord->nStimIndex, 1.0 * v * sin(2.0 * 3.1415926 * f * j / pStim->nSamplingRate));
//			//data.push_back(1.0*v*sin(2.0 * 3.1415926*f*j / fs));
//		}
//	}
//
//	buling = pStim->nSamplingRate * 0.05;
//	for (size_t i = 0; i < buling; i++)
//	{
//		AddStimData(pPlayRecord->nStimIndex, 0);
//		//data.push_back(0);
//	}
//
//	double nTime = 1.0 * GetStimDataCount(pPlayRecord->nStimIndex) / pStim->nSamplingRate;
//	CString sIime(_T(""));
//	sIime.Format(_T("%.3f"), nTime);
//	EditStimParam(_T("time"), sIime, pPlayRecord->nStimIndex);
//}

double CBlueEreTest::GetTestParamData(vector<PlayRecordDialogData*>* prdata, vector<MMERecordParams> &mmeparams)
{
	double mmetimes = 0;
	CString play;
	CString jili;
	CString record;
	CString path;
	testThreadCount = 0;
	int chs = 0;
	int bits = 0;
	int mmeindex = 0;
	int chs1 = 0;//输出通道
	int bits1 = 0;
	int mmeindex1 = 0;
	double fs_play = 0;
	int row = prdata->size();
	//vector<MMERecordParams> mmeparams;
	TestBlueData* testParam = new TestBlueData;
	for (int i = 0; i < row; i++)
	{
		MMERecordParams param;

		//playrecordset.GetPlayRecordInfo(i, play, jili, record, path);
		play = (*prdata)[i]->play;
		jili = (*prdata)[i]->jili;
		record = (*prdata)[i]->record;


		testParam->jili = jili;
		testParam->path = path;
		testParam->record_index = devicemanage.getASIOindex(record, true, &testParam->data_index, testParam->isASIO_in, testParam->fs, testParam->recordName, chs, bits, mmeindex);
		testParam->play_index = devicemanage.getASIOindex(play, false, &testParam->data2_index, testParam->isASIO_out, fs_play, testParam->playName, chs1, bits1, mmeindex1);
		testParam->play = testParam->data2_index;
		testParam->record = testParam->data_index;
		testParam->num_thread = row;
		testParam->recordtime = 1.0*jilisetnew.data.size() / testParam->fs;
		mmetimes = testParam->recordtime + 1;

		param.fs = testParam->fs;
		param.time = testParam->recordtime;
		param.index = mmeindex;
		param.chs = chs;
		param.bit = bits;
		param.mode = (*prdata)[i]->mode;
		mmeparams.push_back(param);
		testdataParam.push_back(testParam);

	}

	return mmetimes;
}



DWORD WINAPI PlayMMEThread(LPVOID param)
{
	TestASIUO* pTestDriver = (TestASIUO*)param;
	slotParam* pSlot = nullptr;
	for (int i = 0; i < GetSlotCount(); i++)
	{
		pSlot = GetSlotParam(i);
		if (pSlot->bEnable)
		{
			break;
		}
	}

	if (pSlot == nullptr)
	{
		return 0;
	}

	prParam* pPlayRecord = GetPlayRecord(pSlot->nMicPlayRecord, false);
	devParam* pOutDev = GetDevParam(pPlayRecord->nPlayIndex, false);
	int* pPlayData = new int[pPlayRecord->vDatas.size()];
	for (size_t j = 0; j < pPlayRecord->vDatas.size(); j++)
	{
		pPlayData[j] = pPlayRecord->vDatas[j];
	}
	
	/*pTestDriver->playMME(pPlayData, pOutDev->nSamplingRate, pOutDev->nChannel,
		pOutDev->nBitDepth, pOutDev->nDevIndex, pOutDev->nVp);*/

	pTestDriver->playASIO(&pOutDev->nChannel, &pPlayData, 1, pOutDev->nSamplingRate);

	return 0;

}

bool GetBlueToothID(int nSlot, char* pBtId, CScanlot* scanlot)
{
	/*CString sSection = ReadStr(_T("Slot"), IntToStr(nSlot), _T(""), GetScriptIniFile());
	if (sSection.IsEmpty())
		return false;

	sSection = ReadStr(sSection, _T("BlueToothID"), _T(""), GetScriptIniFile());
	if (sSection.IsEmpty())
		return false;

	WCharToChar(sSection, pBtId, nSize);*/

     CString sid=scanlot->getBuleID(nSlot);

	 int len1 = WideCharToMultiByte(CP_ACP, 0, sid, -1, NULL, 0, NULL, NULL);
	// char* ch = new char[len1 + 1];
	 WideCharToMultiByte(CP_ACP, 0, sid, -1, pBtId, len1, NULL, NULL);



	return true;
}

bool CBlueEreTest::InitASIODriver(const CString& sDevName, bool bInput)
{


	/*int count=GetDevCount(true);

	devParam* devparam;
	for (size_t i = 0; i < count; i++)
	{
		devparam=GetDevParam(i, true);

	}

	count = GetDevCount(false);

	for (size_t i = 0; i < count; i++)
	{

	}*/



	vector<wstring> wdmDeviceName;
	vector<string> asioChName;

	if (sDevName == "Rstech Rscard" ||
		sDevName == "Rstech Rscarddio Device")
	{
		testASIUO.LoadRSASIO(wdmDeviceName, asioChName, bInput, 0);
	}
	else if (sDevName == "Yamaha Steinberg USB ASIO")
	{
		

		testASIUO.LoadRSASIO(wdmDeviceName, asioChName, bInput, 1);
		
	}
	else if (sDevName == "LISSIG USB Audio Device")
	{
		if (m_bloadDriver)
			return true;
		testASIUO.LoadRSASIO(wdmDeviceName, asioChName, bInput, 2);
		m_bloadDriver = true;
	}
	else if (sDevName == "ASIO Fireface USB")
	{
		testASIUO.LoadRSASIO(wdmDeviceName, asioChName, bInput, 3);
	}

	if (!testASIUO.GetLastErrMsg().IsEmpty())
	{
		AfxMessageBox(testASIUO.GetLastErrMsg());
		return false;
	}

	return true;
}

bool CBlueEreTest::SingleRecord(double nTime, devParam* pDev, int*** pData)
{
	if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
	{
		//WAVEINCAPS pwic;
		testASIUO.createMMEbuffer();
		//waveInGetDevCaps(nIndex, &pwic, sizeof(WAVEINCAPS));
		testASIUO.recordMME(nTime, NULL, pDev->nChannel, pDev->nBitDepth,
			pDev->nSamplingRate, pDev->nDevIndex);
		
		WriteInfoLog(_T("%s[%d:%s]"), _FL_, __FUNCTIONW__);
		testASIUO.startMM();
	}
	else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
	{
		//InitASIODriver(pDev->sDevice, true);
		int nIndex[] = { pDev->nChannel };
		testASIUO.recordASIO(nIndex, pData, 1, pDev->nSamplingRate, nTime);
	}
	else
	{
		WriteErrorLog(_T("SPK，驱动(%s)错误，只能是WDM或ASIO"), pDev->sDriver);
		return false;
	}

	return true;
}

double CBlueEreTest::MultiRecord()
{
	//WAVEINCAPS pwic;
	double nRet = 0;
	testASIUO.createMMEbuffer();

	for (int i = 0; i < GetSlotCount(); i++)
	{
		slotParam* pSlot = GetSlotParam(i);
		if (!pSlot->bEnable)
		{
			continue;
		}

		if (!GetPlayRecord(pSlot->nMicPlayRecord, false)->bEnable)
		{
			WriteInfoLog(_T("Slot%d MIC模式已禁用测试"), pSlot->nSlotIndex);
			continue;
		}

		if (pSlot->bEnableBT)
		{
			/*char szBtId[32];
			if (!GetBlueToothID(i, szBtId, sizeof(szBtId)))
			{
				WriteErrorLog(_T("Slot%d蓝牙ID不存在！"), pSlot->nSlotIndex);
				continue;
			}

			///DisonnectBT(i);
			ConnectBTSwitch(i, szBtId, false);*/
		}
		//waveInGetDevCaps(i, &pwic, sizeof(WAVEINCAPS));

		prParam* pPlayRecord = GetPlayRecord(pSlot->nMicPlayRecord, false);
		devParam* pDev = GetDevParam(pPlayRecord->nRecordIndex, true);
		//stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);

		pDev->nDevIndex = -1;
		int ndev = waveInGetNumDevs();
		WAVEINCAPSW pic;
		int dev_id;
		for (size_t j = 0; j < ndev; j++)
		{
			waveInGetDevCaps(j, &pic, sizeof(WAVEINCAPSW));
			if (CString(pic.szPname) == pDev->sDevice)
			{

				pDev->nDevIndex = j;
				break;

			}
		}
		if (pDev->nDevIndex == -1)
		{
			return false;
		}

		testASIUO.recordMME(pPlayRecord->nTime + 1, NULL, pDev->nChannel, pDev->nBitDepth,
			pDev->nSamplingRate, pDev->nDevIndex);

		if (nRet < pPlayRecord->nTime)
		{
			nRet = pPlayRecord->nTime;
		}
	}
	
	testASIUO.startMM();
	return nRet;
}

bool CBlueEreTest::ConnectBTSwitch(int nSlot, const char* pBtID, bool bSpkMode)
{


	bool bRet = false;
	char szBuff[80] = "\0";
	DWORD dwPrev;
	slotParam* pSlot = GetSlotParam(nSlot);

	if (bSpkMode)
	{
		sprintf(szBuff, ">CONN=%s", pBtID);
		WriteDebugLog(_T("Connect command:%s"), CString(szBuff));
		pSlot->pSerial->WriteBuffer(szBuff, strlen(szBuff), 3000);
		dwPrev = GetTickCount();
		while ((GetTickCount() - dwPrev) < 10 * 1000)
		{
			memset(szBuff, 0, sizeof(szBuff));
			pSlot->pSerial->Read(szBuff, sizeof(szBuff), 2000);
			WriteDebugLog(_T("Connect recv：%s"), CString(szBuff));

			if (strstr(szBuff, "_success") != NULL)
			{
				bRet = true;
				WriteInfoLog(_T("Slot%d蓝牙连接成功，蓝牙ID：%s"), pSlot->nSlotIndex, CString(pBtID));
				break;
			}
		}

		if (!bRet)
		{
			WriteErrorLog(_T("Slot%d蓝牙连接超时，蓝牙ID：%s"), pSlot->nSlotIndex, CString(pBtID));
		}
	}
	else
	{
		char szPass[16] = "\0";
		CString sMode(_T("SPK"));
		memset(szBuff, 0, sizeof(szBuff));
		if (bSpkMode)
		{
			strcpy(szBuff, ">OPEN A2DP");
			strcpy(szPass, "PAIREDA2DP");
		}
		else
		{
			strcpy(szBuff, ">OPEN HFP");
			strcpy(szPass, "PAIREDHFP");
			sMode = (_T("MIC"));
		}

		bRet = false;
		WriteDebugLog(_T("Switch command:%s"), CString(szBuff));
		pSlot->pSerial->WriteBuffer(szBuff, strlen(szBuff), 3000);
		dwPrev = GetTickCount();
		while ((GetTickCount() - dwPrev) < 10 * 1000)
		{
			memset(szBuff, 0, sizeof(szBuff));
			pSlot->pSerial->Read(szBuff, sizeof(szBuff), 2000);
			WriteDebugLog(_T("Switch recv：%s"), CString(szBuff));

			if (strstr(szBuff, szPass) != NULL)
			{
				bRet = true;
				WriteInfoLog(_T("Slot%d切换 %s 模式成功"), pSlot->nSlotIndex, sMode);
				break;
			}
		}

		if (!bRet)
		{
			WriteErrorLog(_T("Slot%d切换 %s 模式超时"), pSlot->nSlotIndex, sMode);
		}
	}


	return bRet;
}

void CBlueEreTest::DisonnectBT(int nSlot)
{
	char szBuff[80] = "\0";
	slotParam* pSlot = GetSlotParam(nSlot);
	strcpy(szBuff, ">DISC\r\n");
	WriteDebugLog(_T("Slot%d Disconnect command:%s"), pSlot->nSlotIndex, CString(szBuff));
	pSlot->pSerial->WriteBuffer(szBuff, strlen(szBuff), 3000);
}

bool CBlueEreTest::ConnectAllBT(CScanlot* scallot)
{
	bool bRet = true;
	CString sMsg(_T(""));
	WriteDebugLog(_T("开始连接所有蓝牙..."));

	for (int i = 0; i < GetSlotCount(); i++)
	{
		slotParam* pSlot = GetSlotParam(i);
		if (!pSlot->bEnable)
		{
			WriteDebugLog(_T("Slot%d未启用"), pSlot->nSlotIndex);
			continue;
		}

		prParam* pPlayRecord = GetPlayRecord(pSlot->nSpkPlayRecord, true);
		devParam* pDev = GetDevParam(pPlayRecord->nRecordIndex, true);
		stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);

		if (pDev == NULL)
		{
			WriteErrorLog(_T("Slot%d设备对象为空！"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		//AfxMessageBox(_T("确认测试slot") + IntToStr(pSlot->nSlotIndex));

		if (pSlot->bEnableBT)
		{
			WriteDebugLog(_T("Slot%d开始连接蓝牙..."), pSlot->nSlotIndex);
			char szBtId[32];
			if (!GetBlueToothID(i, szBtId, scallot))
			{
				WriteErrorLog(_T("Slot%d蓝牙ID不存在！"), pSlot->nSlotIndex);
				SetSlotItem(_T("failmsg"), _T("蓝牙ID不存在"), i);
				SetSlotItem(_T("result"), _T("1"), i);
				bRet = false;
				continue;
			}

			WriteInfoLog(_T("DUT%d蓝牙ID: %s"), pSlot->nSlotIndex, CString(szBtId));

			if (!pSlot->pSerial->IsOpen())
			{
				WriteErrorLog(_T("Slot%d蓝牙适配器端口未打开！"), pSlot->nSlotIndex);
				SetSlotItem(_T("failmsg"), _T("蓝牙适配器端口未打开"), i);
				SetSlotItem(_T("result"), _T("1"), i);
				bRet = false;
				continue;
			}

			//DisonnectBT(i);
			//WriteDebugLog(_T("Slot%d断开蓝牙连接成功"), pSlot->nSlotIndex);

			if (!ConnFenDaWatch( szBtId,i))
			{
				sMsg = _T("");
				sMsg.Format(_T("Slot%d连接蓝牙失败!"), pSlot->nSlotIndex);
				mainf->setStatusStr(sMsg);
				SetSlotItem(_T("failmsg"), _T("蓝牙连接失败"), i);
				SetSlotItem(_T("result"), _T("1"), i);
				bRet = false;
				continue;
			}

			SetSlotBTConnect(true, i);
			sMsg = _T("");
			sMsg.Format(_T("Slot%d连接蓝牙成功"), pSlot->nSlotIndex);
			WriteInfoLog(sMsg);
			mainf->setStatusStr(sMsg);
		}
	}

	WriteDebugLog(_T("所有蓝牙连接完成"));
	return bRet;
}

void CBlueEreTest::DisconnectAllBT()
{
	WriteDebugLog(_T("开始断开所有蓝牙..."));

	for (int i = 0; i < GetSlotCount(); i++)
	{
		slotParam* pSlot = GetSlotParam(i);
		if (!pSlot->bEnable)
		{
			WriteDebugLog(_T("Slot%d未启用"), pSlot->nSlotIndex);
			continue;
		}

		if (!pSlot->pSerial->IsOpen())
		{
			WriteErrorLog(_T("Slot%d蓝牙适配器端口未打开！"), pSlot->nSlotIndex);
			/*SetSlotItem(_T("failmsg"), _T("蓝牙适配器端口未打开"), i);
			SetSlotItem(_T("result"), _T("1"), i);*/
			continue;
		}

		DisonnectBT(i);
	}

	WriteDebugLog(_T("所有蓝牙断开完成"));
}

void CBlueEreTest::PlaySharkJili()
{
	

	// SPK模式
	WriteInfoLog(_T("抖动激励"));
	double palytime = 0;
	double fs = 0;
	for (int i = 0; i < GetSlotCount(); i++)
	{

		slotParam* pSlot = GetSlotParam(i);
		if (!pSlot->bEnable)
		{
			WriteDebugLog(_T("Slot%d未启用"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		prParam* pPlayRecord = GetPlayRecord(pSlot->nSpkPlayRecord, true);
		devParam* pDevRecord = GetDevParam(pPlayRecord->nRecordIndex, true);
		devParam* pDevPlay = GetDevParam(pPlayRecord->nPlayIndex, false);
		stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);

		palytime = pPlayRecord->nTime;
		fs = pDevPlay->nSamplingRate;
		if (pDevRecord == NULL)
		{
			WriteErrorLog(_T("Slot%d采集设备对象为空！"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		if (pDevPlay == NULL)
		{
			WriteErrorLog(_T("Slot%播放设备对象为空！"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		if (!pPlayRecord->bEnable)
		{
			WriteInfoLog(_T("Spk模式禁用测试"));
			
			return ;
		}

		if (pSlot->bEnableBT && !pSlot->bBTConnected)
		{
			WriteErrorLog(_T("Slot%d蓝牙适配器未连接成功！"), pSlot->nSlotIndex);
			/*if (ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
				AfxMessageBox(_T("Slot") + IntToStr(pSlot->nSlotIndex) + _T("蓝牙未连接成功"));*/
			continue;
		}

		/*if (ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
		AfxMessageBox(_T("确认播放slot") + IntToStr(pSlot->nSlotIndex));*/
		int* pPlayData = new int[pPlayRecord->vDatas.size()];
		for (size_t j = 0; j < pPlayRecord->vDatas.size(); j++)
		{
			pPlayData[j] = pPlayRecord->vDatas[j];
		}

		

		int ndev = waveOutGetNumDevs();
		WAVEOUTCAPSW pic;
		int dev_id;
		for (size_t j = 0; j < ndev; j++)
		{
			waveOutGetDevCaps(j, &pic, sizeof(WAVEOUTCAPSW));
			if (CString(pic.szPname) == pDevPlay->sDevice)
			{

				pDevPlay->nDevIndex = j;
				break;

			}
		}
		testASIUO.playMME(pPlayData, pDevPlay->nSamplingRate, pDevPlay->nChannel,
			pDevPlay->nBitDepth, pDevPlay->nDevIndex, pDevPlay->nVp);
		

		WriteDebugLog(_T("Slot%d启动抖动成功"), pSlot->nSlotIndex);

		


	}

	int** indta = NULL;


	//InitASIODriver(pDev->sDevice, true);
	int nIndex[] = { 0, 1, 2, 3 };
	testASIUO.recordASIO(nIndex, &indta, 4, fs, palytime);



	delete[] indta[0];
	delete[] indta[1];
	delete[] indta[2];
	delete[] indta[3];

	

}

bool CBlueEreTest::SpkModeTest(bool& bEnable, bool bInfo)
{
	bool bRet = true;
	bool bWdm = false;
	double nTimeOffset = ReadDouble(_T("RecordData"), _T("TimeOffset"), 0, GetScriptIniFile());
	double nDelayTime = ReadDouble(_T("RecordData"), _T("DelayTime"), 0, GetScriptIniFile());
	if (bInfo)
	{
		mainf->setStatusStr(_T("Spk模式测试开始..."));
	}
	WriteInfoLog(_T("Spk模式测试开始..."));
	bEnable = true;

	// SPK模式
	WriteInfoLog(_T("Spk Mode"));

	for (int i = 0; i < GetSlotCount(); i++)
	{
		if (!bInfo&&i==1)
		{
			break;
		}
		slotParam* pSlot = GetSlotParam(i);
		if (!pSlot->bEnable)
		{
			WriteDebugLog(_T("Slot%d未启用"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		prParam* pPlayRecord = GetPlayRecord(pSlot->nSpkPlayRecord, true);
		devParam* pDevRecord = GetDevParam(pPlayRecord->nRecordIndex, true);
		devParam* pDevPlay = GetDevParam(pPlayRecord->nPlayIndex, false);
		stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);

		if (pDevRecord == NULL)
		{
			WriteErrorLog(_T("Slot%d采集设备对象为空！"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		if (pDevPlay == NULL)
		{
			WriteErrorLog(_T("Slot%播放设备对象为空！"), pSlot->nSlotIndex);
			asioRecordData.push_back(NULL);
			continue;
		}

		if (!pPlayRecord->bEnable)
		{
			WriteInfoLog(_T("Spk模式禁用测试"));
			bEnable = false;
			return bRet;
		}

		if (pSlot->bEnableBT && !pSlot->bBTConnected)
		{
			WriteErrorLog(_T("Slot%d蓝牙适配器未连接成功！"), pSlot->nSlotIndex);
			/*if (ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
				AfxMessageBox(_T("Slot") + IntToStr(pSlot->nSlotIndex) + _T("蓝牙未连接成功"));*/
			continue;
		}

		//AfxMessageBox(_T("确认测试slot") + IntToStr(pSlot->nSlotIndex));

		/*if (pSlot->bEnableBT)
		{
			WriteDebugLog(_T("Slot%d开始连接蓝牙..."), pSlot->nSlotIndex);
			char szBtId[32];
			if (!GetBlueToothID(i, szBtId, sizeof(szBtId)))
			{
				WriteErrorLog(_T("Slot%d蓝牙ID不存在！"), pSlot->nSlotIndex);
				SetSlotItem(_T("failmsg"), _T("蓝牙ID不存在"), i);
				SetSlotItem(_T("result"), _T("1"), i);
				bRet = false;
				continue;
			}

			WriteInfoLog(_T("DUT%d蓝牙ID: %s"), pSlot->nSlotIndex, CString(szBtId));

			if (!pSlot->pSerial->IsOpen())
			{
				WriteErrorLog(_T("Slot%d蓝牙适配器端口未打开！"), pSlot->nSlotIndex);
				SetSlotItem(_T("failmsg"), _T("蓝牙适配器端口未打开"), i);
				SetSlotItem(_T("result"), _T("1"), i);
				bRet = false;
				continue;
			}

			DisonnectBT(i);
			WriteDebugLog(_T("Slot%d断开蓝牙连接成功"), pSlot->nSlotIndex);

			if (!ConnectBTSwitch(i, szBtId))
			{
				SetSlotItem(_T("failmsg"), _T("蓝牙连接失败"), i);
				SetSlotItem(_T("result"), _T("1"), i);
				bRet = false;
				continue;
			}
			WriteInfoLog(_T("Slot%d连接蓝牙成功"), pSlot->nSlotIndex);
		}*/

		/*if (ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
			AfxMessageBox(_T("确认播放slot") + IntToStr(pSlot->nSlotIndex));*/
		int* pPlayData = new int[pPlayRecord->vDatas.size()];
		for (size_t j = 0; j < pPlayRecord->vDatas.size(); j++)
		{
			pPlayData[j] = pPlayRecord->vDatas[j];
		}

		if (StrCmpI(pDevRecord->sDriver, _T("WDM")) == 0)
		{
			bWdm = true;
			WriteDebugLog(_T("Slot%d采集驱动：WDM"), pSlot->nSlotIndex);
		}
		else
		{
			WriteDebugLog(_T("Slot%d采集驱动：ASIO"), pSlot->nSlotIndex);
		}

		WriteDebugLog(_T("Slot%d启动播放..."), pSlot->nSlotIndex);

		int ndev = waveOutGetNumDevs();
		WAVEOUTCAPSW pic;
		int dev_id;
		for (size_t j = 0; j < ndev; j++)
		{
			waveOutGetDevCaps(j, &pic, sizeof(WAVEOUTCAPSW));
			if (CString(pic.szPname) == pDevPlay->sDevice)
			{

				pDevPlay->nDevIndex = j;
				break;

			}
		}
		testASIUO.playMME(pPlayData, pDevPlay->nSamplingRate, pDevPlay->nChannel,
			pDevPlay->nBitDepth, pDevPlay->nDevIndex, pDevPlay->nVp);
		WriteDebugLog(_T("Slot%d启动播放成功"), pSlot->nSlotIndex);

		WriteDebugLog(_T("Slot%d启动录音..."), pSlot->nSlotIndex);
		int** indta = NULL;

		if (!SingleRecord(pPlayRecord->nTime + 1, pDevRecord, &indta))
		{
			SetSlotItem(_T("failmsg"), _T("启动录音失败"), i);
			SetSlotItem(_T("result"), _T("1"), i);
			bRet = false;
			continue;
		}

		WriteDebugLog(_T("Slot%d启动录音成功"), pSlot->nSlotIndex);

		/*testASIUO.playMME(pPlayData, pDevPlay->nSamplingRate, pDevPlay->nChannel,
			pDevPlay->nBitDepth, pDevPlay->nDevIndex, pDevPlay->nVp);
		WriteDebugLog(_T("Slot%d启动播放成功"), pSlot->nSlotIndex);*/

		//Sleep((pPlayRecord->nTime + nDelayTime) * 1000);
		Sleep((pPlayRecord->nTime) * 1000);

		if (bWdm)
		{
			testASIUO.MMEFree(pPlayRecord->nTime + nTimeOffset, true, pSlot->nSlotIndex);
			WriteDebugLog(_T("Slot%d, WDM采集完成"), pSlot->nSlotIndex);
		}
		else
		{

			
			


      
			

			int* asioRecordDataSave = new int[_msize(indta[0]) / 4];
			for (size_t ih = 0; ih < _msize(indta[0]) / 4; ih++)
			{
				asioRecordDataSave[ih] = indta[0][ih];
				
			}

			if (ReadBool(_T("Common"), _T("SaveFile"), false, GetConfigIniFile()))
			{


				std::ofstream outfile;
				char sss1[512];
				sprintf(sss1, "D:\\TestRecord\\spk_%d.txt", pSlot->nSlotIndex);
				CString path(sss1);
				outfile.open(path, std::ios::out | std::ios::trunc);

				for (size_t ih = 0; ih < _msize(indta[0]) / 4; ih++)
				{
					
					outfile << indta[0][ih] << endl;
				}
				outfile.close();
			}

			delete[] indta[0];
			
			asioRecordData.push_back(asioRecordDataSave);
			WriteDebugLog(_T("Slot%d, ASIO采集完成"), pSlot->nSlotIndex);
		}
		char info[512];

	     sprintf(info, "SPK模式Slot%d, 数据采集完成", pSlot->nSlotIndex);
		 if (bInfo)
		 {
			 mainf->setStatusStr(CString(info));
		 }

		if (i == GetSlotCount() - 1)
		{
			Sleep(1000);
		}

	}

	if (bInfo)
	{
		mainf->setStatusStr(_T("Spk模式测试完成"));
	}
	WriteInfoLog(_T("Spk模式测试完成"));
	return bRet;
}

bool CBlueEreTest::MicModeTest(bool& bEnable)
{
	bool bRet = true;
	mainf->setStatusStr(_T("Mic模式测试开始..."));
	WriteInfoLog(_T("Mic模式测试开始..."));
	bEnable = true;

	slotParam* pSlot = GetSlotParam(0);
	prParam* pPlayRecord = GetPlayRecord(pSlot->nMicPlayRecord, false);

	if (!pPlayRecord->bEnable)
	{
		WriteInfoLog(_T("Mic模式禁用测试"));
		bEnable = false;
		return bRet;
	}

	double nTimeOffset = ReadDouble(_T("RecordData"), _T("TimeOffset"), 0, GetScriptIniFile());
	double nDelayTime = ReadDouble(_T("RecordData"), _T("DelayTime"), 0, GetScriptIniFile());
	HANDLE hPlayThread = NULL;
	DWORD dwThreadId = 0;
	double nTime = 0;

	try
	{
		nTime = MultiRecord();
		WriteDebugLog(_T("Mic模式启动录音完成"));
		hPlayThread = CreateThread(NULL, 0, PlayMMEThread, &testASIUO, 0, &dwThreadId);
		WriteDebugLog(_T("Mic模式启动播放完成"));

		Sleep((nTime + nDelayTime) * 1000);
		testASIUO.MMEFree(nTime + nTimeOffset, false);
		WriteDebugLog(_T("Mic模式采集完成"));
	}
	catch (...)
	{
		bRet = false;
	}

	mainf->setStatusStr(_T("Mic模式测试完成"));
	WriteInfoLog(_T("Mic模式测试完成"));
	return bRet;
}

void CBlueEreTest::setOffline(bool bstatus)
{
	m_boffline = bstatus;
}

void CBlueEreTest::DelDir(CString path)
{
	CFileFind tempFind;

	TCHAR sTempFileFind[MAX_PATH] = { 0 };
	wsprintf(sTempFileFind, _T("%s\\*.*"), path);

	BOOL Isfind = tempFind.FindFile(sTempFileFind);

	while (Isfind)
	{
		Isfind = tempFind.FindNextFileW();

		if (!tempFind.IsDots())
		{
			TCHAR sFounfFileName[200] = { 0 };
			_tcscpy(sFounfFileName, tempFind.GetFileName().GetBuffer(200));

			if (tempFind.IsDirectory())
			{
				TCHAR sTempDir[200] = { 0 };
				wsprintf(sTempDir, _T("%s\\%s"), path, sFounfFileName);
				_trmdir(sTempDir);
				

			}
			else
			{
				TCHAR sTempDir[200] = { 0 };
				wsprintf(sTempDir, _T("%s\\%s"), path, sFounfFileName);

				CFile file;
				file.Remove(sTempDir);


			}
		}
		else
		{

		}

	}

	tempFind.Close();

}

void CBlueEreTest::OnBnClickedButtonTest4()
{
	/*thread thread_draw_bitmap(&CBlueEreTest::OnBnClickedButtonTest3, this);
	thread_draw_bitmap.detach();*/

	OnBnClickedButtonTest3();

}

//基于奋达项目,序列同步操作,第一种测试模式，一个播放，四个录音
void CBlueEreTest::OnBnClickedButtonTest3()
{
	//保存文件检查
	CString sFile = GetSaveFileName();

	if (FileExists(sFile))
	{
		HANDLE Handle;

		Handle = CreateFile(sFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == Handle)
		{
			AfxMessageBox(_T("文件被占用"));
			return;

		}
		CloseHandle(Handle);
	}

	
	CRect rc, rc1;
	GetWindowRect(rc);
	


	bool slot1_res[6]{false};//spk fr thd  rb  mic  fr thd sen
	bool slot2_res[6]{false};
	bool slot3_res[6]{false};
	bool slot4_res[6]{false};
	
	/***********************/
	/*slot1_res[0] = false;
	slot1_res[1] = false;
	slot2_res[1] = false;
	slot2_res[3] = false;
	slot3_res[2] = false;
	slot3_res[5] = false;
	slot4_res[3] = false;
	slot4_res[2] = false;*/
	/***********************/

	scanlot.ClearLots(m_connBuleIsFial);
	scanlot.DoModal();

	/*HKL hInput = LoadKeyboardLayout(_T("00000804"), KLF_ACTIVATE);
	PostMessage(WM_INPUTLANGCHANGEREQUEST, (WPARAM)TRUE, (LPARAM)hInput);*/

	if (!scanlot.GetDialogRes())
	{
		return;
	}

	testResManage.ShowWindow(SW_HIDE);
	testResManage2.ShowWindow(SW_HIDE);
	testResManage3.ShowWindow(SW_HIDE);
	testResManage4.ShowWindow(SW_HIDE);

	//@for test
	
	/*for (size_t i = 0; i < 12; i++)
	{
		 datashow = new CDataShow;
		 spkres[i] = datashow;
		 datashow->Create(CDataShow::IDD);


		
	}
	for (size_t i = 12; i < 24; i++)
	{
		 datashow = new CDataShow;
		 micres[i] = datashow;
		 datashow->Create(CDataShow::IDD);
	}*/

	Sleep(50);
	AfxMessageBox(_T("确认关闭屏蔽箱门!"));

	int nLoop = ReadInt(_T("Common"), _T("LoopTimes"), 1, GetConfigIniFile());
	for (int x = 0; x < nLoop; x++)
	{
		/*
		删除临时文件
		*/
		DelDir(_T("D:\\TestRecord"));

		ClearAllSlotItemData();


		if (x > 1)
		{
			WriteInfoLog(_T("第%d次测试开始..."), x + 1);
		}
		else
		{

			WriteInfoLog(_T("测试开始..."));
		}
		mainf->setStatusStr(_T("开始测试..."));

		for (size_t i = 12; i <24; i++)
		{
			if (micres[i] != NULL)
			{
				//micres[i]->ShowWindow(SW_HIDE);
				if (micres[i]->m_hWnd)
				    delete micres[i];
			}
		}

		for (size_t i = 0; i <12; i++)
		{
			if (spkres[i] != NULL)
			{
				//spkres[i]->ShowWindow(SW_HIDE);
				if (spkres[i]->m_hWnd)
				    delete spkres[i];
			}

		}

		bool bSpk = true;
		bool bRet = false;
		bool bSpkEnable = true;
		bool bMicEnable = true;

		spkres.clear();
		micres.clear();
		asioRecordData.clear();

		for (int n = 0; n < 2; n++)//模式
		{
			m_show_min = 0;
			m_show_min_thd = 0;
			m_show_min_rb = 0;
			if (n == 0)
			{
				if (!scanlot.isRetestMode())
				{
					/*if (ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
						AfxMessageBox(_T("确认连接所有蓝牙..."));*/

					mainf->setStatusStr(_T("DUT蓝牙连接中..."));
					DisconnectAllBT();
					if (!ConnectAllBT(&scanlot))
					{
						AfxMessageBox(_T("DUT蓝牙连接失败..."));
						mainf->setStatusStr(_T("DUT蓝牙连接失败..."));
						m_connBuleIsFial = false;
						return;
					}
					m_connBuleIsFial = true;
					mainf->setStatusStr(_T("DUT蓝牙连接完成"));
				}

				/*********播放抖动激励********/
				//PlaySharkJili();
				//Sleep(2000);
				/*******************处理系统第一次测试不过问题***********************/
				bool aaaa;
				SpkModeTest(aaaa,false);

				for (size_t i = 0; i < asioRecordData.size(); i++)
				{
					delete[] asioRecordData[i];
				}
				asioRecordData.clear();

				//MicModeTest(aaaa);
				/*********************************************************/

				bRet = SpkModeTest(bSpkEnable);
			}
			else
			{
				
				/*if (ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
					AfxMessageBox(_T("确认测试MIC模式..."));*/

				mainf->setStatusStr(_T("Mci模式开始测试..."));
				bSpk = false;
				bRet = MicModeTest(bMicEnable);

				//DisconnectAllBT();
			}

			char savePtha[512];
			WriteInfoLog(_T("开始计算数据..."));

			for (int i = 0; i < GetSlotCount(); i++)
			{
				slotParam* pSlot = GetSlotParam(i);
				if (!pSlot->bEnable)
				{
					WriteInfoLog(_T("计算，Slot%d未启用测试"), pSlot->nSlotIndex);

					for (size_t d = 0; d < 3; d++)
					{
						datashow = new CDataShow;
						datashow->Create(CDataShow::IDD);
						if (n==0)
						{
							spkres[pSlot->vSpkItems[d].nWaveFormID] = datashow;
						}
						else
						{
							micres[pSlot->vMicItems[d].nWaveFormID] = datashow;
						}
					}

					continue;
				}

				/*******检查每个SLOT路由***********/
				m_sn = scanlot.getBarCodeSn(pSlot->nSlotIndex);

				if (0&&!m_pHttp->MesCheckRoute(m_station, m_sn))
				{
					WriteInfoLog(_T("Slot%d路由检查失败..."), pSlot->nSlotIndex);
					AfxMessageBox(m_pHttp->GetLastErrMsg());
					for (size_t d = 0; d < 6; d++)
					{
						datashow = new CDataShow;
						datashow->Create(CDataShow::IDD);
						if (i < 3)
						{
							spkres[pSlot->vSpkItems[(pSlot->nSlotIndex - 1) * 6 + d].nWaveFormID] = datashow;
						}
						else
						{
							micres[pSlot->vMicItems[(pSlot->nSlotIndex - 1) * 6 + d].nWaveFormID] = datashow;
						}
					}
					continue;
				}

				int nPlayRecordIndex = 0;
				if (bSpk)
				{
					if (!bSpkEnable)
					{
						WriteInfoLog(_T("计算，Slot%d Spk模式已禁用测试"), pSlot->nSlotIndex);
						break;
					}

					nPlayRecordIndex = pSlot->nSpkPlayRecord;
				}
				else
				{
					if (!bMicEnable)
					{
						WriteInfoLog(_T("计算，Slot%d Mic模式已禁用测试"), pSlot->nSlotIndex);
						break;
					}

					nPlayRecordIndex = pSlot->nMicPlayRecord;
				}

				WriteDebugLog(_T("计算数据初始化..."));
				prParam* pPlayRecord = GetPlayRecord(nPlayRecordIndex, bSpk);
				devParam* pDevRecord = GetDevParam(pPlayRecord->nRecordIndex, true);
				devParam* pDevPlay = GetDevParam(pPlayRecord->nPlayIndex, false);
				
				vector<double> downY;
				vector<double> upY;
				vector<double> borderX;

				audiotest.AudioAlgorithmInit();
				audiotest.setSen(pDevRecord->nSensitivity);
				WriteDebugLog(_T("算法初始化完成"));
				/*if (datashow != NULL)
				{
				delete datashow;
				datashow = NULL;
				}*/


				//主动发送消息
				::SendMessage(alogriSettins.GettreeH(), WM_LBUTTONDOWN, 0, 0);

				vector<int>* analysisdat = alogriSettins.getAnalysisData_spk();
				if (bSpk)
				{
					for (size_t j = 0; j < 3; j++)
					{

					}
					//updateanalysisStatus_spk(analysisdat, i, "");
				}
				else
				{
					analysisdat = alogriSettins.getAnalysisData_mic();

					//fr thd sen
					for (size_t j = 0; j < 3; j++)
					{
						//fr thd sen
						//updateanalysisStatus_mic(analysisdat, j, "");
					}

				}

				WriteDebugLog(_T("获取分析数据完成"));

				//if (datashow == NULL)
				datashow = new CDataShow;
				if (n == 0)
				{
					spkres[pSlot->vSpkItems[0].nWaveFormID] = datashow;
				}
				else
				{
					micres[pSlot->vMicItems[0].nWaveFormID] = datashow;
				}
				datashow->Create(CDataShow::IDD);//执行此函数创建实体窗口

				CString sTitle(_T(""));
				sTitle.Format(_T("%d# MIC - FR"), i+1);
				if (bSpk)
				{
					sTitle.Format(_T("%d# SPK - FR"), i + 1);
				}

				WriteDebugLogPos(_FL_, _T("%s"), sTitle.GetBuffer(0));
				datashow->setwindowtile(sTitle);
				datashow->initPos();
				datashow->setchartname(sTitle);

				datashow->rotalchartY();
				datashow->setYtitle(CString("[SPLS/dB]"));
				datashow->setXtitle(CString("[Hz]"));
				datashow->setYscale(10000);
				datashow->setColor(m_color);
				WriteDebugLog(_T("FR窗口初始化完成"));

				audiotest.setDeviceInputVpval(pDevRecord->nVp);
				audiotest.setDeviceOutputVpval(pDevPlay->nVp);
				audiotest.setIsComputerNorse(true);

				//int samlen = _msize(recordDatas[i]) / 4;
				int samlen = pDevRecord->nSamplingRate * pPlayRecord->nTime;
				double* indata = NULL; // new double[samlen];

				memset(savePtha, 0, 512);
				if (bSpk)
				{
					sprintf(savePtha, "D:\\TestRecord\\spk_wdmRecord_DUT%d.wav", pSlot->nSlotIndex);
				}
				else
				{
					sprintf(savePtha, "D:\\TestRecord\\mic_wdmRecord_DUT%d.wav", pSlot->nSlotIndex);
				}

				WriteDebugLogPos(_FL_, _T("%s"), CString(savePtha));
				if (!bSpk)
				{
					audiotest.wav_readBymatlab(&indata, 0, 0, savePtha);
				}
				WriteDebugLog(_T("读取音频文件数据完成"));

				vector<int>* pHoni;
				if (bSpk)
					pHoni = &pSlot->vSpkItems[1].vHarmonic;
				else
					pHoni = &pSlot->vMicItems[1].vHarmonic;

				audiotest.setDriverMode(1);
				audiotest.setHmani(pHoni);
				if (bSpk)
					pHoni = &pSlot->vSpkItems[2].vHarmonic;
				else
					pHoni = &pSlot->vMicItems[2].vHarmonic;

				audiotest.setHmaniRB(pHoni);
				
				if (bSpk)
				{
				
					if (pSlot->vSpkItems[1].nMethod != pSlot->vSpkItems[2].nMethod)
					{
						mainf->setStatusStr(_T("spk mode thd 与rb算法标准设置不同..."));
						WriteErrorLog(_T("spk mode thd 与rb算法标准设置不同..."));
						continue;
					}
				}
				
				

				audiotest.setIEEE(pSlot->vMicItems[2].nMethod == 1);
				WriteDebugLog(_T("设置算法参数完成"));


				if (n == 0)
				{

					

					if (asioRecordData.size()>pSlot->nSlotIndex - 1&&asioRecordData[pSlot->nSlotIndex - 1] != NULL&&_msize(asioRecordData[pSlot->nSlotIndex - 1]) > 0)
					{
						double * inasio = new double[_msize(asioRecordData[pSlot->nSlotIndex - 1]) / 4];
						for (size_t ii = 0; ii < _msize(asioRecordData[pSlot->nSlotIndex - 1]) / 4; ii++)
						{
							inasio[ii] = asioRecordData[pSlot->nSlotIndex - 1][ii];
						}
						//保存文件

						/*memset(savePtha, 0, 512);
						sprintf(savePtha, "D:\\TestRecord\\spk_wdmRecord_DUT%d.wav", pSlot->nSlotIndex);
						
						int* seedata = new int[_msize(asioRecordData[pSlot->nSlotIndex - 1])/4];
						for (size_t t = 0; t < _msize(asioRecordData[pSlot->nSlotIndex - 1])/4; t++)
						{
							seedata[t] = asioRecordData[pSlot->nSlotIndex - 1][t] / 10000;
						}

						wavwrite(savePtha, 1, 44100, 32, (char*)seedata, _msize(seedata));

						delete[] seedata;*/

						audiotest.setDriverMode(0);
						audiotest.Alg_Fr(inasio, pDevRecord->nSamplingRate, pPlayRecord,pSlot->nSlotIndex);

						delete[] asioRecordData[pSlot->nSlotIndex - 1];
#if ALGOFFSET
						/*************THD算法补偿修正 针对奋发************/
						for (size_t j = 0; j < audiotest.rb_data.size(); j++)
						{
							if (pPlayRecord->vPoints[j] >= 250 && pPlayRecord->vPoints[j] <= 600)
							{
								audiotest.rb_data[j] *= 0.5;
							}
							else if (pPlayRecord->vPoints[j] > 600 && pPlayRecord->vPoints[j] <= 700)
							{
								audiotest.rb_data[j] *= 0.3;
							}
						}

						

						/*************THD算法补偿修正 针对奋发************/
						for (size_t j = 0; j < audiotest.thd_data.size(); j++)
						{
							
								audiotest.thd_data[j] *= 0.8;
			
						}
#endif

						audiotest.Smooth(audiotest.rb_data, pPlayRecord);
						audiotest.Smooth(audiotest.thd_data, pPlayRecord);
					}

				}
				else
				{		
					//mic 模式
					audiotest.setDeviceInputVpval(1);

					/*memset(savePtha, 0, 512);
					sprintf(savePtha, "D:\\TestRecord\\spk_%d.txt", pSlot->nSlotIndex);
					std::ofstream outfile;
					CString path(savePtha);
					outfile.open(path, std::ios::out | std::ios::trunc);
		
					for (size_t t = 0; t < _msize(indata) / sizeof(double); t++)
					{
						
						outfile << indata[t] << endl;
					}
					outfile.close();*/


					audiotest.setIsComputerNorse(false);
					audiotest.Alg_Fr(indata, pDevRecord->nSamplingRate, pPlayRecord, pSlot->nSlotIndex);

					audiotest.Smooth(audiotest.fr_data, pPlayRecord);
					audiotest.SmoothMIC(audiotest.fr_data, pPlayRecord,1);


#if ALGOFFSET
					/*************THD算法补偿修正 针对奋发************/
					for (size_t j = 0; j < audiotest.thd_data.size(); j++)
					{
						if (pPlayRecord->vPoints[j] >= 200 && pPlayRecord->vPoints[j] <= 600)
						{
							audiotest.thd_data[j] *= 0.4;
						}

						if (pPlayRecord->vPoints[j] >= 600 && pPlayRecord->vPoints[j] <= 900)
						{
							audiotest.thd_data[j] *= 0.05;
						}
					}
#endif
					audiotest.Smooth(audiotest.thd_data, pPlayRecord);

				}

				WriteDebugLog(_T("数据计算完成，开始判断结果..."));
				delete[] indata;
				mainf->setStatusStr(CString("fr计算中..."));
				int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
				datafr.clear();
				
				if (m_bfr)
				{
					for (size_t j = 0; j < audiotest.fr_data.size(); j++)
					{
						//WriteDebugLogPos(_FL_, _T(""));
						datafr.push_back(audiotest.fr_data[j]);

					}
				}


				/****************************测试结果判定**************************/
				upY.clear();
				downY.clear();
				borderX.clear();

				if (bSpk)
					bIsSucess = CheckFRLimit(testborderline.spk_fr_borderLine, borderX, downY, upY, pPlayRecord, audiotest.fr_data);
				else
					bIsSucess = CheckFRLimit(testborderline.mic_fr_borderLine, borderX, downY, upY, pPlayRecord, audiotest.fr_data);

				if (audiotest.fr_data.size() == 0)
				{
					bIsSucess = false;
				}



				if (bSpk)
				{
					if (pSlot->nSlotIndex == 1)
					{
						slot1_res[3] = bIsSucess;
					}
					else if (pSlot->nSlotIndex == 2)
					{
						slot2_res[3] = bIsSucess;
					}
					else if (pSlot->nSlotIndex == 3)
					{
						slot3_res[3] = bIsSucess;
					}
					else if (pSlot->nSlotIndex == 4)
					{
						slot4_res[3] = bIsSucess;
					}
				}
				else
				{
					if (pSlot->nSlotIndex == 1)
					{
						slot1_res[0] = bIsSucess;
					}
					else if (pSlot->nSlotIndex == 2)
					{
						slot2_res[0] = bIsSucess;
					}
					else if (pSlot->nSlotIndex == 3)
					{
						slot3_res[0] = bIsSucess;
					}
					else if (pSlot->nSlotIndex == 4)
					{
						slot4_res[0] = bIsSucess;
					}

				}
				

				datashow->setTestResult(bIsSucess);
				bool bmove, bmove2;
				double phase_min = getPhaseMinVal(datafr, bmove);
				vector<double> tempdown;
				for (size_t j = 0; j < downY.size(); j++)
				{
					tempdown.push_back(downY[j]);
					
				}

				if (downY.size() == 0)
				{
					//没有下框线
					if (upY.size() == 0)
					{
						AfxMessageBox(_T("框线未设置..."));
					}
					for (size_t j = 0; j < upY.size(); j++)
					{
						tempdown.push_back(upY[j]);

					}

				}

				

				double boder_min = getPhaseMinVal(tempdown, bmove2);

				if ((bmove || bmove2) && boder_min > phase_min)
				{
					if (bmove)
					{
						for (size_t j = 0; j < datafr.size(); j++)
						{
							datafr[j] = datafr[j] - phase_min + boder_min;

						}
					}
					

					phase_min = boder_min;


				}
				
				if (m_show_min <phase_min)
				   m_show_min = phase_min;



				datashow->setOwnerDraw(true, phase_min);
				//datashow->setData(&datafr, datafr.size());
				datashow->setdata1(datafr, pSlot->nSlotIndex);
				datashow->setDataX(&pPlayRecord->vPoints, pPlayRecord->vPoints.size());

				//设置框线数据
				if (bmove || bmove2)
				{
					movephase(downY, phase_min);
					movephase(upY, phase_min);
				}
				
				datashow->setDataBorderLine(borderX, borderX.size());
				datashow->setDataBorderLinedown(downY, downY.size());
				datashow->setDataBorderLineup(upY, upY.size());
				mfr.setDara(&datafr);

				//datashow->MoveWindow()
				/*if (datafr.size() > 0 && m_bfr)
					datashow->ShowWindow(SW_SHOW);*/

				WriteDebugLog(_T("FR结果判断完成"));
				//AfxMessageBox(_T("ok"));

				/*if (indata != NULL&&!m_snr)
				{
				delete[] indata;
				indata = NULL;
				}*/

				char szDataFile[256] = "\0";
				if (n == 0)
				{
					sprintf(szDataFile, "D:\\Data\\spk_frdata.csv");
				}
				else
				{
					sprintf(szDataFile, "D:\\Data\\mic_frdata.csv");
				}

				memset(savePtha, 0, 512);
				sprintf(savePtha, scanlot.getBarCodeSn(pSlot->nSlotIndex) + "_%d", i);
				if (bmove)
					revermovephase(datafr, -phase_min);

				AddSlotItemData(bIsSucess, audiotest.fr_data, _T("FR"), bSpk, i);

				audiotest.saveData(CString(savePtha), szDataFile, pPlayRecord->vPoints, audiotest.fr_data);
				WriteDebugLogPos(_FL_, _T("FR保存CSV数据完成，开始计算THD..."));


				//if (m_bthd)
				{

					//thd
					mainf->setStatusStr(CString("THD计算中..."));
					/*if (datashowthd != NULL)
					{
					delete datashowthd;
					datashowthd = NULL;
					}*/

					datashowthd = new CDataShow;
					if (n == 0)
					{
						spkres[pSlot->vSpkItems[1].nWaveFormID] = datashowthd;
					}
					else
					{
						micres[pSlot->vMicItems[1].nWaveFormID] = datashowthd;
					}
					datashowthd->Create(CDataShow::IDD);//执行此函数创建实体窗口

					sTitle = CString(_T(""));
					sTitle.Format(_T("%d# MIC - THD"), i + 1);
					if (bSpk)
					{
						sTitle.Format(_T("%d# SPK - THD"), i + 1);
					}

					datashowthd->setwindowtile(sTitle);
					datashowthd->initPos();
					datashowthd->setchartname(sTitle);
					datashowthd->rotalchartY();
					datashowthd->setYtitle(CString("[THD(%)]"));
					datashowthd->setXtitle(CString("[Hz]"));
					datashowthd->setYscale(1000000);
					datashowthd->setColor(m_color);
					//audiotest.Alg_Fr(indata, fs);
					mainf->setStatusStr(CString("FR计算中..."));
					int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
					datathd.clear();
					for (size_t j = 0; j < audiotest.thd_data.size(); j++)
					{

						datathd.push_back(audiotest.thd_data[j]);

					}
					WriteDebugLog(_T("THD窗口初始化完成"));

					/****************************测试结果判定**************************/
					borderX.clear();
					downY.clear();
					upY.clear();
					bool isSuccess_thd = false;

					if (bSpk)
					{
						isSuccess_thd = CheckTHDLimit(testborderline.spk_thd_borderLine,
							borderX, downY, upY, pPlayRecord, audiotest.thd_data);
					}
					else
					{
						isSuccess_thd = CheckTHDLimit(testborderline.mic_thd_borderLine,
							borderX, downY, upY, pPlayRecord, audiotest.thd_data);
					}

					if (audiotest.thd_data.size() == 0)
					{
						isSuccess_thd = false;
					}

					if (bSpk)
					{
						if (pSlot->nSlotIndex == 1)
						{
							slot1_res[4] = isSuccess_thd;
						}
						else if (pSlot->nSlotIndex == 2)
						{
							slot2_res[4] = isSuccess_thd;
						}
						else if (pSlot->nSlotIndex == 3)
						{
							slot3_res[4] = isSuccess_thd;
						}
						else if (pSlot->nSlotIndex == 4)
						{
							slot4_res[4] = isSuccess_thd;
						}
					}
					else
					{
						if (pSlot->nSlotIndex == 1)
						{
							slot1_res[1] = isSuccess_thd;
						}
						else if (pSlot->nSlotIndex == 2)
						{
							slot2_res[1] = isSuccess_thd;
						}
						else if (pSlot->nSlotIndex == 3)
						{
							slot3_res[1] = isSuccess_thd;
						}
						else if (pSlot->nSlotIndex == 4)
						{
							slot4_res[1] = isSuccess_thd;
						}

					}

					WriteDebugLog(_T("THD Limit设置完成"));

					/****************************测试结果判定**************************/
					bool bmove, bmove2;
					double phase_min = getPhaseMinVal(datathd, bmove);

					vector<double> tempdown;
					for (size_t j = 0; j < downY.size(); j++)
					{
						tempdown.push_back(downY[j]);
					}

					if (downY.size() == 0)
					{
						//没有下框线
						if (upY.size() == 0)
						{
							AfxMessageBox(_T("框线未设置..."));
						}
						for (size_t j = 0; j < upY.size(); j++)
						{
							tempdown.push_back(upY[j]);

						}

					}

					double boder_min = getPhaseMinVal(tempdown, bmove2);

					if (bmove&&bmove2 && boder_min > phase_min)
					{
						phase_min = boder_min;
					}


					if (m_show_min_thd < phase_min)
					{
						m_show_min_thd = phase_min;
					}

					if (bmove || bmove2)
						datashowthd->setOwnerDraw(true, phase_min);
					//datashowthd->setData(&datathd, datathd.size());
					datashowthd->setdata1(datathd, pSlot->nSlotIndex);
					datashowthd->setDataX(&pPlayRecord->vPoints, pPlayRecord->vPoints.size());

					if (isSuccess_thd)
						bIsSucess = isSuccess_thd;
					datashowthd->setTestResult(isSuccess_thd);
					
					if (datathd.size() > 0 && pHoni->size() > 0)
						datashowthd->ShowWindow(SW_HIDE);
					/*else if (alogriSettins.getThdData().getHamnics()->size() == 0)
						AfxMessageBox(_T("THD算法参数未设置...\n请在算法设置模块设置..."));*/


					//设置框线数据
					if (bmove || bmove2)
					{
						movephase(downY, phase_min);
						movephase(upY, phase_min);
					}
					
					datashowthd->setDataBorderLine(borderX, borderX.size());
					datashowthd->setDataBorderLinedown(downY, downY.size());
					datashowthd->setDataBorderLineup(upY, upY.size());
					mthd.setDara(&datathd);
					WriteDebugLog(_T("THD结果判断完成"));

					memset(szDataFile, 0, sizeof(szDataFile));
					if (n == 0)
					{
						sprintf(szDataFile, "D:\\Data\\spk_thddata.csv");
					}
					else
					{
						sprintf(szDataFile, "D:\\Data\\mic_thddata.csv");
					}
					memset(savePtha, 0, 512);
					sprintf(savePtha, scanlot.getBarCodeSn(pSlot->nSlotIndex) + "_%d", i);
					if (bmove)
						revermovephase(datathd, -phase_min);

					AddSlotItemData(isSuccess_thd, audiotest.thd_data, _T("THD"), bSpk, i);

					audiotest.saveData(CString(savePtha), szDataFile, pPlayRecord->vPoints, audiotest.thd_data);
					WriteDebugLogPos(_FL_, _T("THD保存CSV数据完成"));
				}

				if (n == 0)
				{

					//RB结果显示
					//if (m_brb)
					{

						//thd
						mainf->setStatusStr(CString("RB计算中..."));
						/*if (datashowthd != NULL)
						{
						delete datashowthd;
						datashowthd = NULL;
						}*/

						datashowRB = new CDataShow;
					    spkres[pSlot->vSpkItems[2].nWaveFormID] = datashowRB;
						
						datashowRB->Create(CDataShow::IDD);//执行此函数创建实体窗口

						sTitle = CString(_T(""));
						sTitle.Format(_T("%d# MIC - RB"), i + 1);
						if (bSpk)
						{
							sTitle.Format(_T("%d# SPK - RB"), i + 1);
						}

						datashowRB->setwindowtile(sTitle);
						datashowRB->initPos();
						datashowRB->setchartname(sTitle);
						datashowRB->rotalchartY();
						datashowRB->setYtitle(CString("[RB(%)]"));
						datashowRB->setXtitle(CString("[Hz]"));
						datashowRB->setYscale(1000000);
						datashowRB->setColor(m_color);
						//audiotest.Alg_Fr(indata, fs);
						mainf->setStatusStr(CString("FR计算中..."));
						int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
						datarub.clear();
						for (size_t j = 0; j < audiotest.rb_data.size(); j++)
						{

							datarub.push_back(audiotest.rb_data[j]);

						}
						WriteDebugLog(_T("THD窗口初始化完成"));

						/****************************测试结果判定**************************/
						borderX.clear();
						downY.clear();
						upY.clear();
						bool isSuccess = false;

						if (bSpk)
						{
							isSuccess = CheckTHDLimit(testborderline.rb_borderLine,
								borderX, downY, upY, pPlayRecord, audiotest.rb_data);
						}

						if (audiotest.rb_data.size() == 0)
						{
							isSuccess = false;
						}
						
							if (pSlot->nSlotIndex == 1)
							{
								slot1_res[5] = isSuccess;
							}
							else if (pSlot->nSlotIndex == 2)
							{
								slot2_res[5] = isSuccess;
							}
							else if (pSlot->nSlotIndex == 3)
							{
								slot3_res[5] = isSuccess;
							}
							else if (pSlot->nSlotIndex == 4)
							{
								slot4_res[5] = isSuccess;
							}
						
						


						/*else
						{
						isSuccess = CheckTHDLimit(testborderline.mic_thd_borderLine,
						borderX, downY, upY, pPlayRecord);
						}*/
						WriteDebugLog(_T("THD Limit设置完成"));

						/****************************测试结果判定**************************/
						bool bmove;
						double phase_min = getPhaseMinVal(datarub, bmove);

						vector<double> tempdown;
						for (size_t j = 0; j < downY.size(); j++)
						{
							tempdown.push_back(downY[j]);
						}

						if (downY.size() == 0)
						{
							//没有下框线
							if (upY.size() == 0)
							{
								AfxMessageBox(_T("框线未设置..."));
							}
							for (size_t j = 0; j < upY.size(); j++)
							{
								tempdown.push_back(upY[j]);

							}

						}

						double boder_min = getPhaseMinVal(tempdown, bmove2);

						if (bmove&&bmove2 && boder_min > phase_min)
						{
							phase_min = boder_min;
						}

						if (m_show_min_rb < phase_min)
						{
							m_show_min_rb = phase_min;
						}

						if (bmove||bmove2)
							datashowRB->setOwnerDraw(true, phase_min);
						//datashowRB->setData(&datarub, datarub.size());
						datashowRB->setdata1(datarub, pSlot->nSlotIndex);
						datashowRB->setDataX(&pPlayRecord->vPoints, pPlayRecord->vPoints.size());

						if (bIsSucess)
							bIsSucess = isSuccess;
						datashowRB->setTestResult(isSuccess);

						if (datarub.size() > 0 && pHoni->size() > 0)
							datashowRB->ShowWindow(SW_HIDE);
						/*else if (alogriSettins.getThdData().getHamnics()->size() == 0)
						AfxMessageBox(_T("THD算法参数未设置...\n请在算法设置模块设置..."));*/


						//设置框线数据
						if (bmove || bmove2)
						{
							movephase(downY, phase_min);
							movephase(upY, phase_min);
						}
						
						datashowRB->setDataBorderLine(borderX, borderX.size());
						datashowRB->setDataBorderLinedown(downY, downY.size());
						datashowRB->setDataBorderLineup(upY, upY.size());
						mrubz.setDara(&datarub);
						WriteDebugLog(_T("THD结果判断完成"));

						memset(szDataFile, 0, sizeof(szDataFile));
						if (n == 1)
						{
							sprintf(szDataFile, "D:\\Data\\spk_rbdata.csv");
						}
						/*else
						{
						sprintf(szDataFile, "D:\\TestRecord\\mic_thddata.csv");
						}*/
						memset(savePtha, 0, 512);
						sprintf(savePtha, scanlot.getBarCodeSn(pSlot->nSlotIndex) + "_%d", i);
						if (bmove)
							revermovephase(datarub, -phase_min);

						AddSlotItemData(isSuccess, audiotest.rb_data, _T("RB"), true, i);

						audiotest.saveData(CString(savePtha), "D:\\Data\\rbdata.csv", pPlayRecord->vPoints, audiotest.rb_data);
						WriteDebugLogPos(_FL_, _T("THD保存CSV数据完成"));
					}


				}
				else if (n==1)
				{//mic

				//取出灵敏度值

					/********************************************/
					/*if (resshowSensitivity != NULL)
					{
						delete resshowSensitivity;
						resshowSensitivity = NULL;
					}*/

					resshowSensitivity = new CResShow;
					resshowSensitivity->Create(CResShow::IDD);
					//对激励信号做判断 需要定频
					
					resshowSensitivity->setTitle(_T("MIC_Sensitivity测试结果_#") + IntToStr(i+1));
						


					
					

					vector<double> dutsen;
					vector<double> dutsen_fs;
					switch (pSlot->nSlotIndex)
					{
					case 1:
						dut_sen1 = audiotest.m_micSensitivity;
						break;
					case 2:
						dut_sen2 = audiotest.m_micSensitivity;
						break;
					case 3:
						dut_sen3 = audiotest.m_micSensitivity;
						break;
					case 4:
						dut_sen4 = audiotest.m_micSensitivity;
						break;
					} 


					bool bsen = true;

					double sen_down = ReadInt(_T("AlgConf"), _T("Sen_Down"), 1, GetConfigIniFile());
					double sen_up = ReadInt(_T("AlgConf"), _T("Sen_Up"), 1, GetConfigIniFile());

					if (audiotest.m_micSensitivity > sen_up || audiotest.m_micSensitivity < sen_down)
					{
						bsen = false;
					}

					if (pSlot->nSlotIndex == 1)
					{
						slot1_res[2] = bsen;
					}
					else if (pSlot->nSlotIndex == 2)
					{
						slot2_res[2] = bsen;
					}
					else if (pSlot->nSlotIndex == 3)
					{
						slot3_res[2] = bsen;
					}
					else if (pSlot->nSlotIndex == 4)
					{
						slot4_res[2] = bsen;
					}


					sTitle = CString(_T(""));
					sTitle.Format(_T("%d# MIC - SEN"), i + 1);
					if (bSpk)
					{
						sTitle.Format(_T("%d# SPK - RB"), i + 1);
					}
					resshowSensitivity->SetWindowTextW(sTitle);
					resshowSensitivity->setitemName(_T("灵敏度:"));
					if (audiotest.fr_data.size()>0)
						resshowSensitivity->setval(GetSensitivity(audiotest.fr_data, pPlayRecord));
					resshowSensitivity->setuint(_T("dB/V"));

					/*************************************************/

					micres[pSlot->vMicItems[2].nWaveFormID] = (CDataShow*)resshowSensitivity;;

					dutsen.push_back(GetSensitivity(audiotest.fr_data,pPlayRecord));
					dutsen_fs.push_back(1000);
					memset(savePtha, 0, 512);
					sprintf(savePtha, scanlot.getBarCodeSn(pSlot->nSlotIndex) + "_%d", i);
					AddSlotItemData(bsen, dutsen, _T("Sensitivity"), false, i);
					audiotest.saveData(CString(savePtha), "D:\\Data\\sendata.csv", dutsen_fs, dutsen);

				}



				//WriteDebugLogPosEx(_T("Debug"), _FL_, _T(""));
			}
		}


		WriteDebugLogPos(_FL_, _T(""));

		//
		//delete testParam;



		
		mainf->setStatusStr(_T("测试完成."));

		if (x > 1)
		{
			WriteInfoLog(_T("第%d次测试完成\n"), x + 1);
			Sleep(5000);
		}
		else
		{
			WriteInfoLog(_T("测试完成\n"));
		}
	}

	/************保存CSV数据***************/
	WriteDebugLog(_T("开始保存CSV数据..."));
	SaveDataToCsv();
	WriteDebugLog(_T("保存CSV数据完成。"));


	/************上传mes数据***************/
	if (m_IsOffline)
	{

		for (size_t i = 0; i < GetSlotCount(); i++)
		{

			slotParam* pSlot = GetSlotParam(i);
			if (!pSlot->bEnable)
			{
				WriteInfoLog(_T("计算，Slot%d未启用测试"), pSlot->nSlotIndex);
				continue;
			}

			CString errCode;
			CString errItem;
			CString testspkItem;
			CString testmicItem;
			GetTestItems(testspkItem, _T("SPKTestItems"));
			GetTestItems(testmicItem, _T("MICTestItems"));
			testspkItem += ",";
			testspkItem += testmicItem;


			bool bfail = false;
			switch (pSlot->nSlotIndex)
			{
			case 1:
				bfail = GetErrCode(errCode, errItem, slot1_res, 1);
				break;
			case 2:
				bfail = GetErrCode(errCode, errItem, slot1_res, 2);
				break;
			case 3:
				bfail = GetErrCode(errCode, errItem, slot1_res, 3);
				break;
			case 4:
				bfail = GetErrCode(errCode, errItem, slot1_res, 4);
				break;
			}
			m_sn = scanlot.getBarCodeSn(pSlot->nSlotIndex);
			MesDataPost(testspkItem, !bfail, errCode, testmicItem, m_sn);

		}
	}

	testResManage.GetWindowRect(rc1);
	testResManage.setRes(slot1_res);
	testResManage.MoveWindow(rc.right - rc1.Width()-250, rc.top + 20, rc1.Width(), rc1.Height());
	testResManage.ShowWindow(SW_SHOW);

	testResManage2.setRes(slot2_res);
	testResManage2.MoveWindow(rc.right - rc1.Width()-250, rc.top + 20 + rc1.Height(), rc1.Width(), rc1.Height());
	testResManage2.ShowWindow(SW_SHOW);

	testResManage3.setRes(slot3_res);
	testResManage3.MoveWindow(rc.right - 2 * rc1.Width()-250, rc.top + 20, rc1.Width(), rc1.Height());
	testResManage3.ShowWindow(SW_SHOW);

	testResManage4.setRes(slot4_res);
	testResManage4.MoveWindow(rc.right - 2 * rc1.Width()-250, rc.top + 20 + rc1.Height(), rc1.Width(), rc1.Height());
	testResManage4.ShowWindow(SW_SHOW);

	SendMessage(WM_COMMAND,IDC_BUTTON4,0);
}

void CBlueEreTest::WriteDoubleData(CArchive& ar, double dbData, int nNum)
{
	char szBuf[16];
	char szFormat[8];
	memset(szBuf, 0, sizeof(szBuf));
	memset(szFormat, 0, sizeof(szFormat));
	sprintf_s(szFormat, sizeof(szFormat), "%%.%df,", nNum);
	sprintf_s(szBuf, sizeof(szBuf), szFormat, dbData);
	ar.Write(szBuf, (UINT)strlen(szBuf));
	ar.Flush();
}

void CBlueEreTest::WriteStringData(CArchive& ar, const char* pData)
{
	ar.Write(pData, (UINT)strlen(pData));
	ar.Flush();
}

void CBlueEreTest::SavePointHead(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, prParam* pPR)
{
	itemLimit* pLimit;
	WriteStringData(ar, "Result Info:,Test Result:,");
	
	for (int i = 0; i < pPR->vPoints.size(); i++)
	{
		pLimit = GetLimitParam(lpItemName, bSpkMode, pPR->vPoints[i]);
		if (pLimit == nullptr)
		{
			continue;
		}

		if (pLimit->bPoint)
		{
			WriteDoubleData(ar, pPR->vPoints[i]);
		}
		else if (pLimit->bLower || pLimit->bUpper || pLimit->bData)
		{
			WriteStringData(ar, ",");
		}
	}
}

void CBlueEreTest::SaveLimitHead(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, prParam* pPR, bool bLower)
{
	if (bLower)
	{
		WriteStringData(ar, ",Lower Limit,");
	}
	else
	{
		WriteStringData(ar, ",Upper Limit,");
	}

	//bool bLimit;
	itemLimit* pLimit;
	testItem* pItem = GetItemParam(lpItemName, bSpkMode);
	if (pItem == nullptr)
		return;

	if (StrCmpI(lpItemName, _T("Sensitivity")) == 0)
	{
		if (pItem->vLimit.size() > 0)
		{
			if (bLower)
				WriteDoubleData(ar, pItem->vLimit[0].nLower, 3);
			else
				WriteDoubleData(ar, pItem->vLimit[0].nUpper, 3);
		}
		else
		{
			WriteStringData(ar, ",");
		}

		return;
	}

	for (int i = 0; i < pPR->vPoints.size(); i++)
	{
		//bLimit = false;
		pLimit = GetLimitParam(lpItemName, bSpkMode, pPR->vPoints[i]);
		if (pLimit == nullptr)
		{
			WriteStringData(ar, ",");
		}
		else
		{
			if (bLower)
			{
				if (pLimit->bLower)
				{
					WriteDoubleData(ar, pLimit->nLower, 3);
				}
				else
				{
					if (pLimit->bPoint)
						WriteStringData(ar, ",");
				}
			}
			else
			{
				if (pLimit->bUpper)
				{
					WriteDoubleData(ar, pLimit->nUpper, 3);
				}
				else
				{
					if (pLimit->bPoint)
						WriteStringData(ar, ",");
				}
			}
		}
		
		/*for (int j = 0; j < pItem->vLimit.size(); j++)
		{
			if (pPR->vPoints[i] == pItem->vLimit[j].nFrequency)
			{
				bLimit = true;
				if (bLower)
					WriteDoubleData(ar, pItem->vLimit[j].nLower, 3);
				else
					WriteDoubleData(ar, pItem->vLimit[j].nUpper, 3);
			}
		}

		if (!bLimit)
		{
			WriteStringData(ar, ",");
		}*/
	}
}

void CBlueEreTest::WriteItemData(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, int nIndex, const char* pTestItem)
{
	char szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));
	sprintf_s(szBuf, sizeof(szBuf), "%d#%s,", nIndex + 1, pTestItem);
	WriteStringData(ar, szBuf);

	itemLimit* pLimit;
	slotParam* pSlot = GetSlotParam(0);
	prParam* pPR = GetPlayRecord(pSlot->nSpkPlayRecord, true);
	testItem* pItem = GetItemParam(lpItemName, bSpkMode, nIndex);
	if (pItem->nTestResult == 0)
	{
		WriteStringData(ar, "Pass,");
	}
	else if (pItem->nTestResult == 2)
	{
		WriteStringData(ar, "NO Test,");
	}
	else
	{
		WriteStringData(ar, "Fail,");
	}

	if (pItem->vDatas.size() == 0)
	{
		for (int i = 0; i < pPR->vPoints.size(); i++)
		{
			WriteStringData(ar, ",");
		}
	}
	else
	{
		if (StrCmpI(lpItemName, _T("Sensitivity")) == 0)
		{
			if (pItem->vLimit.size() > 0)
			{
				pLimit = GetLimitParam(lpItemName, bSpkMode, pItem->vLimit[0].nFrequency);
				if (pLimit == nullptr)
					return;
				
				WriteDoubleData(ar, pItem->vDatas[0], 3);
			}
			else
			{
				WriteStringData(ar, ",");
			}

			return;
		}

		for (int j = 0; j < pItem->vDatas.size(); j++)
		{
			pLimit = GetLimitParam(lpItemName, bSpkMode, pPR->vPoints[j]);
			if (pLimit != nullptr)
			{
				if (pLimit->bData)
				{
					WriteDoubleData(ar, pItem->vDatas[j], 3);
					continue;
				}
			}
			else
			{
				continue;
			}

			if (pLimit->bPoint)
			{
				WriteStringData(ar, ",");
			}
		}
	}
}

CString CBlueEreTest::GetSaveFileName()
{
	CString sFile = ReadStr(_T("FilePath"), _T("DataFilePath"), _T("d:\\data"), GetConfigIniFile());
	sFile = GetFilePath(sFile);
	ForceCreateDirectory(sFile);
	if (sFile.Right(1) != _T("/") && sFile.Right(1) != _T("\\"))
	{
		sFile += _T("\\");
	}

	SYSTEMTIME stSysTime;
	::GetLocalTime(&stSysTime);
	CString sDate(_T(""));
	sDate.Format(_T("%04d%02d%02d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);
	sFile += ReadStr(_T("Common"), _T("ExcelPrefix"), _T(""), GetConfigIniFile());
	sFile += _T("_");
	sFile += sDate;
	sFile += _T(".csv");

	return sFile;
}

void CBlueEreTest::SaveDataToCsv()
{
	bool bWriteHead = false;
	CString sFile = GetSaveFileName();

	FILE *fSave;
	SYSTEMTIME stSysTime;
	::GetLocalTime(&stSysTime);
	if (!FileExists(sFile))
	{
		bWriteHead = true;
	}
	else
	{
		HANDLE Handle;

		Handle = CreateFile(sFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == Handle)
		{
			AfxMessageBox(_T("文件被占用"));
			return;

		}
		CloseHandle(Handle);
	}

	try
	{
		CFile mFile;
		mFile.Open(sFile, CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		mFile.SeekToEnd();
		CArchive ar(&mFile, CArchive::store);

		if (bWriteHead)
		{
			WriteStringData(ar, "Time,Overall TestResult,Result Info:,,Result Info:,,");

			slotParam* pSlot = GetSlotParam(0);
			prParam* pPR = GetPlayRecord(pSlot->nSpkPlayRecord, true);

			SavePointHead(_T("FR"), true, ar, pPR);
			SavePointHead(_T("THD"), true, ar, pPR);
			SavePointHead(_T("RB"), true, ar, pPR);
			SavePointHead(_T("FR"), false, ar, pPR);
			SavePointHead(_T("THD"), false, ar, pPR);
			SavePointHead(_T("Sensitivity"), false, ar, pPR);
			WriteStringData(ar, "\n");

			/*for (int n = 0; n < 5; n++)
			{
				WriteStringData(ar, "Result Info:,Test Result:,");

				for (int i = 0; i < pPR->vPoints.size(); i++)
				{
					pLimit = GetLimitParam(lpItemName, bSpkMode, pPR->vPoints[j]);
					WriteDoubleData(ar, pPR->vPoints[i]);
				}
			}

			WriteStringData(ar, "Result Info:,Test Result:,,\n");*/

			WriteStringData(ar, ",,,,,,");
			SaveLimitHead(_T("FR"), true, ar, pPR);
			SaveLimitHead(_T("THD"), true, ar, pPR);
			SaveLimitHead(_T("RB"), true, ar, pPR);
			SaveLimitHead(_T("FR"), false, ar, pPR);
			SaveLimitHead(_T("THD"), false, ar, pPR);
			SaveLimitHead(_T("Sensitivity"), false, ar, pPR);
			WriteStringData(ar, "\n");

			WriteStringData(ar, ",,,,,,");
			SaveLimitHead(_T("FR"), true, ar, pPR, false);
			SaveLimitHead(_T("THD"), true, ar, pPR, false);
			SaveLimitHead(_T("RB"), true, ar, pPR, false);
			SaveLimitHead(_T("FR"), false, ar, pPR, false);
			SaveLimitHead(_T("THD"), false, ar, pPR, false);
			SaveLimitHead(_T("Sensitivity"), false, ar, pPR, false);
			WriteStringData(ar, "\n");
		}

		char szBuf[512];

		for (int i = 0; i < GetSlotCount(); i++)
		{
			slotParam* pSlot = GetSlotParam(i);
			if (!pSlot->bEnable)
			{
				continue;
			}

			memset(szBuf, 0, sizeof(szBuf));
			sprintf_s(szBuf, sizeof(szBuf), "%04d/%02d/%02d %02d:%02d:%02d,", stSysTime.wYear,
				stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
			WriteStringData(ar, szBuf);
			if (pSlot->nTestResult == 0)
			{
				WriteStringData(ar, "Pass,");
			}
			else
			{
				WriteStringData(ar, "Fail,");
			}

			memset(szBuf, 0, sizeof(szBuf));
			sprintf_s(szBuf, sizeof(szBuf), "%d#SN-Processed String,", pSlot->nSlotIndex);
			WriteStringData(ar, szBuf);

			memset(szBuf, 0, sizeof(szBuf));
			WCharToChar(pSlot->sBarcode, szBuf, sizeof(szBuf));
			//WriteInfoLog(_T("SN:%s"), szBuf);
			WriteStringData(ar, szBuf);
			WriteStringData(ar, "\t,");

			memset(szBuf, 0, sizeof(szBuf));
			sprintf_s(szBuf, sizeof(szBuf), "%d#BT-Processed String,", pSlot->nSlotIndex);
			WriteStringData(ar, szBuf);

			memset(szBuf, 0, sizeof(szBuf));
			WCharToChar(pSlot->sBuleId, szBuf, sizeof(szBuf));
			WriteStringData(ar, szBuf);
			WriteStringData(ar, ",");

			WriteItemData(_T("FR"), true, ar, i, "SPK测试-Frequency response (dBSPL)");
			WriteItemData(_T("THD"), true, ar, i, "SPK测试-THD (%)");
			WriteItemData(_T("RB"), true, ar, i, "SPK测试-Rub&Buzz (%)");
			WriteItemData(_T("FR"), false, ar, i, "MIC-套用MIC限值-Checked Curve (dBV)");
			WriteItemData(_T("THD"), false, ar, i, "MIC分析-THD (%)");
			WriteItemData(_T("Sensitivity"), false, ar, i, "MIC-灵敏度检查-Checked Single Value 1 (dBV)");
			WriteStringData(ar, "\n");
		}

		ar.Close();
		mFile.Close();
	}
	catch (...)
	{
		MessageBox(_T("保存CSV文件失败！"), _T("错误"), MB_OK|MB_ICONINFORMATION);
	}
}

double CBlueEreTest::GetSensitivity(const vector<double>& data, prParam* pPlayRecord)
{
	for (size_t i = 0; i < pPlayRecord->vPoints.size(); i++)
	{
		if (pPlayRecord->vPoints[i] == 1000)
		{
			if (data.size()>i)
			    return data[i];
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

bool CBlueEreTest::GetErrCode(CString& errcode, CString& erritem, bool* res, int nslot)
{
	bool bpass = true;
	slotParam* pSlot = GetSlotParam(nslot-1);
	char stemp[512];
	
	for (size_t i = 0; i < 6; i++)
	{
		if (!res[i])
		{
			memset(stemp, 0, 512);
			switch (i)
			{
			case 0:
				sprintf(stemp, "M%d_1_SPK1_FR,", nslot);
				errcode += stemp;
				memset(stemp, 0, 512);
				sprintf(stemp, "%d#", nslot);
				erritem += stemp;
				erritem += pSlot->vSpkItems[0].sAliasName;
				break;
			case 1:
				
				sprintf(stemp, "M%d_2_SPK1_THD,", nslot);
				errcode += stemp;

				memset(stemp, 0, 512);
				sprintf(stemp, "%d#", nslot);
				erritem += stemp;
				erritem += pSlot->vSpkItems[1].sAliasName;
				break;
			case 2:
				
				sprintf(stemp, "M%d_3_SPK1_RB,", nslot);
				errcode += stemp;

				memset(stemp, 0, 512);
				sprintf(stemp, "%d#", nslot);
				erritem += stemp;
				erritem += pSlot->vSpkItems[2].sAliasName;
				break;
			case 3:
		
				sprintf(stemp, "M%d_6_MIC1_FR,", nslot);
				errcode += stemp;
				memset(stemp, 0, 512);
				sprintf(stemp, "%d#", nslot);
				erritem += stemp;
				erritem += pSlot->vMicItems[0].sAliasName;
				break;
			case 4:
				
				sprintf(stemp, "M%d_7_MIC1_THD,", nslot);
				errcode += stemp;
				memset(stemp, 0, 512);
				sprintf(stemp, "%d#", nslot);
				erritem += stemp;
				erritem += pSlot->vMicItems[1].sAliasName;
				break;
			case 5:
				
				sprintf(stemp, "M%d_8_MIC1_SEN,", nslot);
				errcode += stemp;
				memset(stemp, 0, 512);
				sprintf(stemp, "%d#", nslot);
				erritem += stemp;
				erritem += pSlot->vMicItems[2].sAliasName;
				break;
			}

			bpass = false;
		}


	}


	return bpass;

}

void CBlueEreTest::GetTestItems(CString& testItems,CString itemName)
{

	int count=ReadInt(_T("SPKTestItems"), _T("Items"), 0, GetScriptIniFile());
	char s[10];

	for (size_t i = 0; i < count; i++)
	{
		memset(s, 0, 10);
		sprintf(s, "%d", i);
		testItems += ReadStr(itemName, CString(s), 0, GetScriptIniFile());
		if (i<count-1)
		    testItems += ",";
	}
	

}

void  CBlueEreTest::movephase(vector<double>& movedata, double moveVal)
{
	
	
	for (size_t i = 0; i < movedata.size(); i++)
	{
		movedata[i] += abs(moveVal);
	}
	
	
}

void CBlueEreTest::revermovephase(vector<double>& movedata, double moveVal)
{
	for (size_t i = 0; i < movedata.size(); i++)
	{
		movedata[i] += moveVal;
	}

}

bool CBlueEreTest::CheckFRLimit(vector<Mutidata*>& frLimit, vector<double>& borderX,
	vector<double>& downY, vector<double>& upY, prParam* pPlayRecord, vector<double>& checkData)
{
	bool isSuccess = false;
	double hz, down, up;

	WriteDebugLogPosEx(_T("Debug"), _FL_, _T(""));

	for (size_t j = 0; j < frLimit.size(); j++)
	{
		frLimit[j]->getdata(hz, down, up);

		borderX.push_back(hz);
		if (down != -1)
			downY.push_back(down);
		if (up != -1)
			upY.push_back(up);
	}
	hz = down = up = 0;
	for (size_t j = 0; j < frLimit.size(); j++)
	{
		frLimit[j]->getdata(hz, down, up);
		isSuccess = FrChecked(hz, down, up, &pPlayRecord->vPoints, &checkData, frLimit);

		if (!isSuccess)
		{
			//datashow->SetTimerStart();
			break;
		}
	}


	if (frLimit.size() > 0)
	{
		double x, x1, y, z;

		frLimit.back()->getdata(x, y, z);
		frLimit.front()->getdata(x1, y, z);
		if (pPlayRecord->vPoints.back() != x ||
			pPlayRecord->vPoints.front() != x1)
		{
			//::SendMessage(processClose->m_hWnd, WM_CLOSE, 0, 0);
			///*if (indata != NULL)
			//delete[] indata;*/
			//AfxMessageBox(_T("框线设置起止频点与激励信号起止频点不同！"), MB_ICONSTOP);
			//GetDlgItem(IDC_testResLable)->SetWindowTextW(CString("测试结束"));
			return false;
		}
	}

	return isSuccess;
}

bool CBlueEreTest::CheckTHDLimit(vector<Mutidata*>& thdLimit, vector<double>& borderX,
	vector<double>& downY, vector<double>& upY, prParam* pPlayRecord, vector<double>& checkData)
{
	bool isSuccess = false;
	double hz, down, up;

	for (size_t j = 0; j < thdLimit.size(); j++)
	{
		thdLimit[j]->getdata(hz, down, up);

		borderX.push_back(hz);
		if (down != -1)
			downY.push_back(down);
		if (up != -1)
			upY.push_back(up);
	}

	hz = down = up = 0;

	for (size_t j = 0; j < thdLimit.size(); j++)
	{

		thdLimit[j]->getdata(hz, down, up);
		isSuccess = FrChecked(hz, down, up, &pPlayRecord->vPoints/* audiotest.getpindiandata()*/, &checkData, thdLimit);

		if (!isSuccess)
		{
			//datashowthd->SetTimerStart();
			break;
		}
	}

	return isSuccess;
}


void CBlueEreTest::OnBnClickedButtonTest()
{
	testResManage.ShowWindow(SW_HIDE);
	bIsSucess = true;
	LPDWORD threadID=0;
	CreateThread(NULL, 0, runProcess, (LPVOID)this, 0, threadID);
	while (process == NULL)
		;
	Sleep(50);
	//process->setProcessMove(50);
	/*process->SetParent(this);*/
	
	m_bTesting = true;
	audiotest.AudioAlgorithmInit();
	CloseResWindow();
	GetDlgItem(IDC_testResLable)->SetWindowTextW(CString("测试中"));
	process->setProcessMove(50);
	playtimes = 0;
	//UINT_PTR TIMER_ID = SetTimer(1, 1000, NULL);

	/*playtimes = 0;
	m_bTesting = true;
	
	CString s_time;
	s_time.Format(_T("%d 秒"), playtimes);
	m_staticPlayTimes.SetWindowTextW(s_time);
	AfxBeginThread(refreshTimes, this);*/

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();



	double* indata = NULL;
	int** recorddata = NULL;
	//CAudioAlgorithm audiotest;//这里加载matlab环境，慢
	
	
	CString playstr, recordstr;
	playrecordset.GetPlayAndRecord(playstr, recordstr);
	if (recordstr == "")
	{
		::SendMessage(processClose->m_hWnd, WM_CLOSE, 0, 0);
		mainf->setStatusStr(CString("播放采集，模块未设置..."));
		GetDlgItem(IDC_testResLable)->SetWindowTextW(CString("测试结束"));
		return;
	}
	int fs = devicemanage.getfs(recordstr);
	double vp = devicemanage.getinputvp(recordstr);
	double vpout = devicemanage.getoutvp(playstr);
	//play(NULL);

	Sleep(1000);
	
	process->setProcessMove(100);
	datafr.clear();
	datathd.clear();
	datathdn.clear();
	datarub.clear();
	dataphase.clear();
	datasen.clear();
	vector<double> downY;
	vector<double> upY;
	vector<double> borderX;


	//主动发送消息
	::SendMessage(alogriSettins.GettreeH(), WM_LBUTTONDOWN, 0, 0);

	vector<int>* analysisdat = alogriSettins.getAnalysisData_spk();
	if (m_btestSPK)
	{
		updateanalysisStatus_spk(analysisdat,0,"");
	}
	else
	{
		analysisdat = alogriSettins.getAnalysisData_mic();
		updateanalysisStatus_mic(analysisdat,0,"");
	}
	

	
	process->setProcessMove(180);
	

	char* driver = NULL;;
	devicemanage.getDriverType(recordstr, &driver);
	if (strcmp(driver, "ASIO") == 0)
	{
		audiotest.setDriverMode(0);
	}
	else
	{
		audiotest.setDriverMode(1);
	}

	
		
		if (strcmp(driver,"ASIO")==0)
		{
			//AfxMessageBox(_T("蓝牙测试不支持ASIO驱动！！！"));
			//return;'
			double playtimes = 0;
			devicemanage.record(recordstr, playstr, &recorddata, fs, playtimes);
			process->setProcessMove(300);
			if (recorddata == NULL || *recorddata == NULL)
			{
				::SendMessage(processClose->m_hWnd, WM_CLOSE, 0, 0);
				return;
			}
			char* fileName1 = "D:\\TestRecord\\fr01.wav";

			//把录音到的数据，输出
			
			std::ofstream outfile1;
			CString path1("D:\\TestRecord\\record.txt");
			outfile1.open(path1, std::ios::out | std::ios::trunc);
			for (size_t i = 0; i < _msize(recorddata[0]) / sizeof(int); i++)
			{
				outfile1 << recorddata[0][i] << endl;
			}
			outfile1.close();

			wavwrite(fileName1, 1, fs, 32, (char*)(recorddata[0]), fs * jilisetnew.getPlayTimes() * 32 / 8/*_msize(recorddata[0])*/);

			size_t relen = _msize(recorddata[0]) / sizeof(int);

			if (relen>0)
				indata = new double[relen]{0};

			for (size_t i = 0; i < relen; i++)
			{
				//if (recorddata[0][i]!=0)
				{
					//datafr.push_back(recorddata[0][i]);
					indata[i] = recorddata[0][i];

				}


			}

			process->setProcessMove(350);
			delete[] recorddata[0];
			
		}
		else
		{
			
			/*if (m_btestSPK)
			{



			}
			else*/
			{

			
				devicemanage.wdmPlayWithRecord(recordstr, playstr, 0, "D:\\TestRecord\\playjili.wav");//播放时间必须是文件的实际时长
				process->setProcessMove(300);
				//读取约定文件，给indata赋值
				/*if (jilisetnew.getPlayTimes()>0)
					indata = new double[(int)(fs*jilisetnew.getPlayTimes())]{0};*/

				devicemanage.wav_readBymatlab(&indata, fs, jilisetnew.getPlayTimes(),"");
				process->setProcessMove(350);

			}
			

			
		}
		
	delete driver;
	//waveShow.setData(indata, _msize(indata)/sizeof(double));
	process->setProcessMove(400);
		
	if (m_bfr || m_Balance || m_NoiseReduction)
	{
		if (datashow != NULL)
		{
			delete datashow;
			datashow = NULL;
		}
		
			//if (datashow == NULL)
		
		datashow = new CDataShow;
		datashow->Create(CDataShow::IDD);//执行此函数创建实体窗口
		
		
		datashow->setwindowtile(CString("FR"));
		datashow->initPos();
		datashow->setchartname(CString("FR"));

		datashow->rotalchartY();
		datashow->setYtitle(CString("[SPLS/dB]"));
		datashow->setXtitle(CString("[Hz]"));
		datashow->setYscale(100);
		datashow->setColor(m_color);
		
		
		audiotest.setDeviceInputVpval(devicemanage.getinputvp(recordstr));
		audiotest.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		audiotest.setIsComputerNorse(true);
		
		audiotest.Alg_Fr(indata, fs);	
		mainf->setStatusStr(CString("fr计算中..."));
		int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
		datafr.clear();
		if (m_bfr)
		{
			for (size_t i = 0; i < audiotest.fr_data.size(); i++)
			{

				datafr.push_back(audiotest.fr_data[i]);

			}
		}
		

		bool isSuccess = false;
		/****************************测试结果判定**************************/

		double hz,down, up;

		
		for (size_t i = 0; i < testborderline.fr_borderLine.size(); i++)
		{
			testborderline.fr_borderLine[i]->getdata(hz, down, up);
			
			borderX.push_back(hz);
			if (down!=-1)
			   downY.push_back(down);
			if (up != -1)
			   upY.push_back(up);
		}
		hz=down=up = 0;
		for (size_t i = 0; i < testborderline.fr_borderLine.size(); i++)
		{

			testborderline.fr_borderLine[i]->getdata(hz, down, up);
			isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &datafr, testborderline.fr_borderLine);

			if (!isSuccess)
			{
				//datashow->SetTimerStart();
				break;
			}
			

		}


		if (testborderline.fr_borderLine.size() > 0)
		{
			double x,x1, y, z;
			
			testborderline.fr_borderLine.back()->getdata(x,y,z);
			testborderline.fr_borderLine.front()->getdata(x1, y, z);
			if (audiotest.getpindiandata()->back() != x||
				audiotest.getpindiandata()->front() != x1)
			{
				::SendMessage(processClose->m_hWnd, WM_CLOSE, 0, 0);
				if (indata != NULL)
					delete[] indata;
				AfxMessageBox(_T("框线设置起止频点与激励信号起止频点不同！"), MB_ICONSTOP);
				GetDlgItem(IDC_testResLable)->SetWindowTextW(CString("测试结束"));
				return;
			}
		}
		


		/****************************测试结果判定**************************/
		

		/*datafr.push_back(2.3);
		datafr.push_back(1.3);
		datafr.push_back(0.3);
		datafr.push_back(2.3);
		datafr.push_back(3.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(46.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(1.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);
		datafr.push_back(32.3);
		datafr.push_back(32.3);
		datafr.push_back(4.3);
		datafr.push_back(12.3);*/


		/*for (size_t i = 0; i < 100; i++)
		{
			datafr.push_back(abs(5.0*sin(2*3.1415926*1000*i/44100)));
		}*/
		/*datafr.push_back(3080.56778);
		datafr.push_back(1080.05078);
		datafr.push_back(1080.00000);
		datafr.push_back(1080.05078);
		datafr.push_back(380.056778);
		datafr.push_back(1800.05078);
		datafr.push_back(380.056778);*/

		/*for (size_t i = 0; i < audiotest.getpindiandata()->size(); i++)
		{
			datafr.push_back(2);
		}*/
		/*datafr.push_back(2);
		datafr.push_back(2);
		datafr.push_back(2);*/
		bIsSucess = isSuccess;
		datashow->setTestResult(isSuccess);
		datashow->setData(&datafr, datafr.size());
		datashow->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());

		//设置框线数据
		datashow->setDataBorderLine(borderX, borderX.size());
		datashow->setDataBorderLinedown(downY, downY.size());
		datashow->setDataBorderLineup(upY, upY.size());
		mfr.setDara(&datafr);

		//datashow->MoveWindow()
		if (datafr.size()>0 && m_bfr)
		   datashow->ShowWindow(SW_SHOW);
		
		if (indata != NULL&&!m_snr)
		{
			delete[] indata;
			indata = NULL;
		}
		
		audiotest.saveData(m_testID, "D:\\TestRecord\\frdata.csv", *audiotest.getpindiandata(), datafr);

	}
else
{
	if (m_bSensitivity_mic || m_bSensitivity_spk || M_Polarity || m_snr)
	{
		
	}
	else
	{
		::SendMessage(processClose->m_hWnd, WM_CLOSE, 0, 0);
		mainf->setStatusStr(CString("FR分析项不存在...."));
		GetDlgItem(IDC_testResLable)->SetWindowTextW(CString("测试结束"));
		AfxMessageBox(_T("FR分析项不存在！\n请在算法设置模块选择..."), MB_ICONSTOP);
		return;
	}

	
}


process->setProcessMove(450);

	if (m_bthd)
	{
		mainf->setStatusStr(CString("THD计算中..."));
		if (datashowthd != NULL)
		{
			delete datashowthd;
			datashowthd = NULL;
		}
		
		datashowthd = new CDataShow;
		datashowthd->Create(CDataShow::IDD);//执行此函数创建实体窗口
		
		
		datashowthd->setwindowtile(CString("THD"));
		datashowthd->initPos();
		datashowthd->setchartname(CString("THD"));
		datashowthd->rotalchartY();
		datashowthd->setYtitle(CString("[THD(%)]"));
		datashowthd->setXtitle(CString("[Hz]"));
		datashowthd->setYscale(1000000);
		datashowthd->setColor(m_color);
		//audiotest.Alg_Fr(indata, fs);
		mainf->setStatusStr(CString("FR计算中..."));
		int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
		//datathd.clear();
		for (size_t i = 0; i < audiotest.thd_data.size(); i++)
		{

			datathd.push_back(audiotest.thd_data[i]);

		}

		
		/****************************测试结果判定**************************/
		bool isSuccess = false;

		borderX.clear();
		downY.clear();
		upY.clear();
		double hz, down, up;
		for (size_t i = 0; i < testborderline.thd_borderLine.size(); i++)
		{
			testborderline.thd_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.thd_borderLine.size(); i++)
		{

			testborderline.thd_borderLine[i]->getdata(hz, down, up);

			isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &datathd, testborderline.thd_borderLine);

			if (!isSuccess)
			{
				//datashowthd->SetTimerStart();
				break;
			}


		}


		/****************************测试结果判定**************************/

		datashowthd->setData(&datathd, datathd.size());
		datashowthd->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());
		
		if (bIsSucess)
		    bIsSucess = isSuccess;
		datashowthd->setTestResult(isSuccess);
		if (datathd.size() > 0 && alogriSettins.getThdData().getHamnics()->size() > 0)
			datashowthd->ShowWindow(SW_HIDE);
		else if (alogriSettins.getThdData().getHamnics()->size()==0)
			AfxMessageBox(_T("THD算法参数未设置...\n请在算法设置模块设置..."));
		

		//设置框线数据
		datashowthd->setDataBorderLine(borderX, borderX.size());
		datashowthd->setDataBorderLinedown(downY, downY.size());
		datashowthd->setDataBorderLineup(upY, upY.size());
		mthd.setDara(&datathd);

		audiotest.saveData(m_testID,"D:\\TestRecord\\thddata.csv", *audiotest.getpindiandata(), datathd);
	}

	process->setProcessMove(500);
	if (m_bthdn)
	{
		mainf->setStatusStr(CString("THD+N计算中..."));
		/*datathdn.push_back(200.3);
		datathdn.push_back(100.3);
		datathdn.push_back(45.3);
		datathdn.push_back(123.3);
		datathdn.push_back(32.3);
		datathdn.push_back(26.3);
		datathdn.push_back(112.3);
		datathdn.push_back(48.3);
		datathdn.push_back(112.3);
		datathdn.push_back(32.3);
		datathdn.push_back(25.3);
		datathdn.push_back(18.3);
		datathdn.push_back(49.3);
		datathdn.push_back(112.3);
		datathdn.push_back(132.3);
		datathdn.push_back(112.3);
		datathdn.push_back(111.3);
		datathdn.push_back(148.3);
		datathdn.push_back(112.3);
		datathdn.push_back(32.3);
		datathdn.push_back(114.3);
		datathdn.push_back(112.3);
		datathdn.push_back(132.3);
		datathdn.push_back(112.3);
		datathdn.push_back(111.3);
		datathdn.push_back(114.3);
		datathdn.push_back(112.3);
		datathdn.push_back(132.3);*/


		//for (size_t i = 0; i < audiotest.getpindiandata()->size(); i++)
		//{
		//	datathdn.push_back(100+abs(132.3*sin(2*3.1415926*i*1000/3000)));
		//	//datathdn.push_back(132.3);
		//}



		if (datashowthd_N != NULL)
		{
			delete datashowthd_N;
			datashowthd_N = NULL;
		}
		datashowthd_N = new CDataShow;
		datashowthd_N->Create(CDataShow::IDD);//执行此函数创建实体窗口
		datashowthd_N->setwindowtile(CString("THD+N"));
		datashowthd_N->initPos();
		datashowthd_N->setchartname(CString("THD+N"));
		datashowthd_N->rotalchartY();
		datashowthd_N->setYtitle(CString("[THD+N(%)]"));
		datashowthd_N->setXtitle(CString("[Hz]"));
		datashowthd_N->setYscale(1000000);
		datashowthd_N->setColor(m_color);

		datathdn.clear();
		for (size_t i = 0; i < audiotest.thdn_data.size(); i++)
		{

			datathdn.push_back(audiotest.thdn_data[i]);

		}

		/****************************测试结果判定**************************/
		bool isSuccess = false;

		borderX.clear();
		downY.clear();
		upY.clear();
		double hz, down, up;
		for (size_t i = 0; i < testborderline.thdN_borderLine.size(); i++)
		{
			testborderline.thdN_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.thdN_borderLine.size(); i++)
		{

			testborderline.thdN_borderLine[i]->getdata(hz, down, up);

			isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &datathdn, testborderline.thdN_borderLine);

			if (!isSuccess)
			{
				//datashowthd_N->SetTimerStart();
				break;
			}


		}


		datashowthd_N->setData(&datathdn, datathdn.size());
		datashowthd_N->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());
		//if (datathdn.size()>0)

		if (bIsSucess)
			bIsSucess = isSuccess;
		datashowthd_N->setTestResult(isSuccess);
		if (datathdn.size() > 0)
		   datashowthd_N->ShowWindow(SW_HIDE);

		//设置框线数据
		datashowthd_N->setDataBorderLine(borderX, borderX.size());
		datashowthd_N->setDataBorderLinedown(downY, downY.size());
		datashowthd_N->setDataBorderLineup(upY, upY.size());
		mthdn.setDara(&datathdn);

		audiotest.saveData(m_testID,"D:\\TestRecord\\thdndata.csv", *audiotest.getpindiandata(), datathdn);

	}

	process->setProcessMove(550);
	if (m_bRub_Buzz)
	{
		mainf->setStatusStr(CString("Rub&Buzz计算中..."));
		
		/*datarub.push_back(2.3);
		datarub.push_back(1.3);
		datarub.push_back(4.3);
		datarub.push_back(12.3);
		datarub.push_back(32.3);*/
		/*datathd.push_back(0.12234);
		datathd.push_back(0.3786);
		datathd.push_back(0.445);
		datathd.push_back(0.21231);
		datathd.push_back(0.253457);
		datathd.push_back(0.36124);
		datathd.push_back(0.92234);
		datathd.push_back(0.3);
		datathd.push_back(0.2545);
		datathd.push_back(0.36);
		datathd.push_back(1.0);
		datathd.push_back(0.21123);
		datathd.push_back(0.7);*/

		if (datashowRB != NULL)
		{
			delete datashowRB;
			datashowRB = NULL;
		}
		datashowRB = new CDataShow;
		datashowRB->Create(CDataShow::IDD);//执行此函数创建实体窗口
		datashowRB->setwindowtile(CString("RB"));
		datashowRB->initPos();
		datashowRB->setchartname(CString("RB"));
		datashowRB->rotalchartY();
		datashowRB->setYtitle(CString("[RB(%)]"));
		datashowRB->setXtitle(CString("[Hz]"));
		datashowRB->setYscale(1000000000);
		datashowRB->setColor(m_color);
		//audiotest.Alg_Fr(indata, fs);
		mainf->setStatusStr(CString("RB计算中..."));
		int sh_data = 0;//把结果处理为屏幕显示数据，任何界面都可以随意调用显示
		datarub.clear();
		for (size_t i = 0; i < audiotest.rb_data.size(); i++)
		{

			datarub.push_back(audiotest.rb_data[i]);

		}

		bool isSuccess = false;
		/****************************测试结果判定**************************/


		borderX.clear();
		downY.clear();
		upY.clear();
		double hz, down, up;
		for (size_t i = 0; i < testborderline.rb_borderLine.size(); i++)
		{
			testborderline.rb_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.rb_borderLine.size(); i++)
		{

			testborderline.rb_borderLine[i]->getdata(hz, down, up);

			isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &datarub, testborderline.rb_borderLine);

			if (!isSuccess)
			{
				//datashowRB->SetTimerStart();
				break;
			}



		}


		/****************************测试结果判定**************************/

		datashowRB->setData(&datarub, datarub.size());
		datashowRB->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());


		if (bIsSucess)
			bIsSucess = isSuccess;
		datashowRB->setTestResult(isSuccess);
		if (datarub.size()>0 && alogriSettins.getRbData().getHamnics()->size()>0)
			datashowRB->ShowWindow(SW_HIDE);
		else if (alogriSettins.getRbData().getHamnics()->size()==0)
			AfxMessageBox(_T("RB算法参数未设置...\n请在算法设置模块设置..."));

		

		//设置框线数据
		datashowRB->setDataBorderLine(borderX, borderX.size());
		datashowRB->setDataBorderLinedown(downY, downY.size());
		datashowRB->setDataBorderLineup(upY, upY.size());
		
		//mrubz.setDara(&datarub);

		audiotest.saveData(m_testID,"D:\\TestRecord\\rbdata.csv", *audiotest.getpindiandata(), datarub);
	}
	process->setProcessMove(550);
	if (m_bPhase)
	{
		mainf->setStatusStr(CString("Phase计算中..."));
		/*dataphase.push_back(2.3);
		dataphase.push_back(1.3);
		dataphase.push_back(4.3);
		dataphase.push_back(12.3);
		dataphase.push_back(32.3);*/

		if (datashowPhase != NULL)
		{
			delete datashowPhase;
			datashowPhase = NULL;
		}
		datashowPhase = new CDataShow;
		datashowPhase->Create(CDataShow::IDD);//执行此函数创建实体窗口
		datashowPhase->setwindowtile(CString("Phase"));
		datashowPhase->initPos();
		datashowPhase->setchartname(CString("Phase"));
		datashowPhase->rotalchartY();
		datashowPhase->setYtitle(CString("[Phase(deg)]"));
		datashowPhase->setXtitle(CString("[Hz]"));
		datashowPhase->setYscale(1000000);
		datashowPhase->setColor(m_color);

		dataphase.clear();
		for (size_t i = 0; i < audiotest.phase_data.size(); i++)
		{

			dataphase.push_back(audiotest.phase_data[i]);

		}

		bool isSuccess = false;
		/****************************测试结果判定**************************/


		borderX.clear();
		downY.clear();
		upY.clear();
		double hz, down, up;
		for (size_t i = 0; i < testborderline.rb_borderLine.size(); i++)
		{
			testborderline.rb_borderLine[i]->getdata(hz, down, up);

			borderX.push_back(hz);
			if (down != -1)
				downY.push_back(down);
			if (up != -1)
				upY.push_back(up);
		}
		hz = down = up = 0;
		for (size_t i = 0; i < testborderline.rb_borderLine.size(); i++)
		{

			testborderline.rb_borderLine[i]->getdata(hz, down, up);

			isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &dataphase, testborderline.rb_borderLine);

			if (!isSuccess)
			{
				//datashowPhase->SetTimerStart();
				break;
			}


		}


		/****************************测试结果判定**************************/
		bool bmove;
		double phase_min = getPhaseMinVal(dataphase, bmove);
		datashowPhase->setOwnerDraw(true, phase_min);
		datashowPhase->setData(&dataphase, dataphase.size());
		datashowPhase->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());


		/*if (bIsSucess) 相位角不作为失败判别
			bIsSucess = isSuccess;*/
		
		datashowPhase->setTestResult(isSuccess);
		if (dataphase.size()>0 /*&& alogriSettins.getRbData().getHamnics()->size()>0*/)
			datashowPhase->ShowWindow(SW_HIDE);
		else if (alogriSettins.getRbData().getHamnics()->size() == 0)
			AfxMessageBox(_T("RB算法参数未设置...\n请在算法设置模块设置..."));



		//设置框线数据
		datashowPhase->setDataBorderLine(borderX, borderX.size());
		datashowPhase->setDataBorderLinedown(downY, downY.size());
		datashowPhase->setDataBorderLineup(upY, upY.size());

		//mrubz.setDara(&datarub);


		//因为相位数据按照最小值做了向上平移，因此向下平移复原原始数据保存
		for (size_t i = 0; i < dataphase.size(); i++)
		{

			dataphase[i] -= phase_min;
		}

		audiotest.saveData(m_testID, "D:\\TestRecord\\phasedata.csv", *audiotest.getpindiandata(), dataphase);



		//mphase.setDara(&dataphase);
	}
	process->setProcessMove(600);
	if (m_bSensitivity_spk || m_bSensitivity_mic)
	{
		mainf->setStatusStr(CString("Sensitivity计算中..."));
		/*datasen.push_back(2.3);
		datasen.push_back(1.3);
		datasen.push_back(4.3);
		datasen.push_back(12.3);
		datasen.push_back(32.3);
		msen.setDara(&datasen);*/


		//SPK与MIC测试方式有所不同

		audiotest.setDeviceInputVpval(devicemanage.getinputvp(recordstr));
		audiotest.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		audiotest.setIsComputerNorse(true);

		if (resshowSensitivity != NULL)
		{
			delete resshowSensitivity;
			resshowSensitivity = NULL;
		}

		resshowSensitivity = new CResShow;
		resshowSensitivity->Create(CResShow::IDD);
		//对激励信号做判断 需要定频
		if (m_bSensitivity_spk)
		{
			resshowSensitivity->setTitle(_T("SPK_Sensitivity"));
			
		}
		else if (m_bSensitivity_mic)
		{
			resshowSensitivity->setTitle(_T("MIC_Sensitivity测试结果"));
			//resshowSensitivity->setitemName(_T(""));
		}

		audiotest.Alg_Fr(indata, fs);

		resshowSensitivity->setitemName(_T("灵敏度:"));
		if (audiotest.fr_data.size()>0)
		   resshowSensitivity->setval(audiotest.fr_data.at(0));
		resshowSensitivity->setuint(_T("dB/V"));

		if (audiotest.fr_data.size()>0)
		   resshowSensitivity->ShowWindow(SW_SHOW);
		else
		{
			mainf->setStatusStr(_T("灵敏度测试失败..."));
		}


		





	}
	process->setProcessMove(650);
	if (M_Polarity)
	{
		mainf->setStatusStr(CString("Polarity计算中..."));

		audiotest.setDeviceInputVpval(devicemanage.getinputvp(recordstr));
		audiotest.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		audiotest.setIsComputerNorse(true);

		if (resshowPolarity != NULL)
		{
			delete resshowPolarity;
			resshowPolarity = NULL;
		}

		resshowPolarity = new CResShow;
		resshowPolarity->Create(CResShow::IDD);
		resshowPolarity->setTitle(_T("Polarity"));
		audiotest.Alg_Fr(indata, fs);
		process->setProcessMove(250);
		resshowPolarity->setitemName(_T("极性:"));
		if (audiotest.phase_data.size() > 0)
		{
			if (audiotest.phase_data.at(0) >= 0 &&
				audiotest.phase_data.at(0) <= 90)
			{
				resshowPolarity->setval(_T("正"));
			}
			else
			{
				resshowPolarity->setval(_T("负"));
			}
			
		}
			
		resshowPolarity->setuint(_T(""));

		process->setProcessMove(0);
		//tp.ShowWindow(SW_HIDE);
		//tp.CloseWindow();
		//::SendMessage(process->m_hWnd, WM_CLOSE, 0, 0);

		if (audiotest.phase_data.size()>0)
			resshowPolarity->ShowWindow(SW_HIDE);
		else
		{
			mainf->setStatusStr(_T("极性测试失败..."));
		}


	}
	process->setProcessMove(700);
	if (m_Balance)
	{
		//测试左耳的fr与右耳的fr差值,直接使用fr测试
		if (dataBalance1.size() == 0)
		{
			for (size_t i = 0; i < audiotest.fr_data.size(); i++)
			{

				dataBalance1.push_back(audiotest.fr_data[i]);

			}
		}
		else
		{
			if (dataBalance2.size() == 0)
			{
				for (size_t i = 0; i < audiotest.fr_data.size(); i++)
				{

					dataBalance2.push_back(audiotest.fr_data[i]);

				}
			}
		}

		

		if (dataBalance1.size() > 0 && dataBalance2.size() > 0)
		{

			for (size_t i = 0; i < dataBalance1.size(); i++)
			{
				dataBalance1[i] = dataBalance1[i] - dataBalance2[i];
			}


			if (resshowBalance != NULL)
			{

				delete resshowBalance;
				resshowBalance = NULL;
			}


			resshowBalance = new CDataShow;
			resshowBalance->Create(CDataShow::IDD);//执行此函数创建实体窗口
			resshowBalance->setwindowtile(CString("Balance"));
			resshowBalance->initPos();
			resshowBalance->setchartname(CString("Balance"));
			resshowBalance->rotalchartY();
			resshowBalance->setYtitle(CString("[Balance(dB)]"));
			resshowBalance->setXtitle(CString("[Hz]"));
			resshowBalance->setYscale(1000000);
			resshowBalance->setColor(m_color);

			/*dataphase.clear();
			for (size_t i = 0; i < audiotest.phase_data.size(); i++)
			{

				dataphase.push_back(audiotest.phase_data[i]);

			}*/

			bool isSuccess = false;
			/****************************测试结果判定**************************/


			borderX.clear();
			downY.clear();
			upY.clear();
			double hz, down, up;
			for (size_t i = 0; i < testborderline.Balance_borderLine.size(); i++)
			{
				testborderline.Balance_borderLine[i]->getdata(hz, down, up);

				borderX.push_back(hz);
				if (down != -1)
					downY.push_back(down);
				if (up != -1)
					upY.push_back(up);
			}
			hz = down = up = 0;
			for (size_t i = 0; i < testborderline.Balance_borderLine.size(); i++)
			{

				testborderline.Balance_borderLine[i]->getdata(hz, down, up);

				isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &dataBalance1, testborderline.Balance_borderLine);

				if (!isSuccess)
				{
					//datashowPhase->SetTimerStart();
					break;
				}


			}


			/****************************测试结果判定**************************/

			bool bmove;
			resshowBalance->setOwnerDraw(true, getPhaseMinVal(dataBalance1, bmove));
			resshowBalance->setData(&dataBalance1, dataBalance1.size());
			resshowBalance->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());


			if (bIsSucess)
				bIsSucess = isSuccess;
			resshowBalance->setTestResult(isSuccess);
			if (dataBalance1.size() > 0 /*&& alogriSettins.getRbData().getHamnics()->size()>0*/)
				resshowBalance->ShowWindow(SW_HIDE);



			//设置框线数据
			resshowBalance->setDataBorderLine(borderX, borderX.size());
			resshowBalance->setDataBorderLinedown(downY, downY.size());
			resshowBalance->setDataBorderLineup(upY, upY.size());

			//mrubz.setDara(&datarub);

			audiotest.saveData(m_testID, "D:\\TestRecord\\dataBalance.csv", *audiotest.getpindiandata(), dataBalance1);




			dataBalance2.clear();
			dataBalance1.clear();

		}
	}
	process->setProcessMove(750);
	if (m_NoiseReduction)
	{
		if (dataNoiseReduction1.size() == 0)
		{
			for (size_t i = 0; i < audiotest.fr_data.size(); i++)
			{

				dataNoiseReduction1.push_back(audiotest.fr_data[i]);

			}
		}
		else
		{
			if (dataNoiseReduction2.size() == 0)
			{
				for (size_t i = 0; i < audiotest.fr_data.size(); i++)
				{

					dataNoiseReduction2.push_back(audiotest.fr_data[i]);

				}
			}
		}



		if (dataNoiseReduction1.size() > 0 && dataNoiseReduction2.size() > 0)
		{

			for (size_t i = 0; i < dataNoiseReduction1.size(); i++)
			{
				dataNoiseReduction1[i] = dataNoiseReduction1[i] - dataNoiseReduction2[i];
			}


			if (dataNoiseReduction != NULL)
			{

				delete dataNoiseReduction;
				dataNoiseReduction = NULL;
			}


			dataNoiseReduction = new CDataShow;
			dataNoiseReduction->Create(CDataShow::IDD);//执行此函数创建实体窗口
			dataNoiseReduction->setwindowtile(CString("NoiseReduction"));
			dataNoiseReduction->initPos();
			dataNoiseReduction->setchartname(CString("NoiseReduction"));
			dataNoiseReduction->rotalchartY();
			dataNoiseReduction->setYtitle(CString("[NoiseReduction(dB)]"));
			dataNoiseReduction->setXtitle(CString("[Hz]"));
			dataNoiseReduction->setYscale(1000000);
			dataNoiseReduction->setColor(m_color);

			/*dataphase.clear();
			for (size_t i = 0; i < audiotest.phase_data.size(); i++)
			{

			dataphase.push_back(audiotest.phase_data[i]);

			}*/

			bool isSuccess = false;
			/****************************测试结果判定**************************/


			borderX.clear();
			downY.clear();
			upY.clear();
			double hz, down, up;
			for (size_t i = 0; i < testborderline.NoiseReduction_borderLine.size(); i++)
			{
				testborderline.NoiseReduction_borderLine[i]->getdata(hz, down, up);

				borderX.push_back(hz);
				if (down != -1)
					downY.push_back(down);
				if (up != -1)
					upY.push_back(up);
			}
			hz = down = up = 0;
			for (size_t i = 0; i < testborderline.NoiseReduction_borderLine.size(); i++)
			{

				testborderline.NoiseReduction_borderLine[i]->getdata(hz, down, up);

				isSuccess = FrChecked(hz, down, up, audiotest.getpindiandata(), &dataNoiseReduction1, testborderline.NoiseReduction_borderLine);

				if (!isSuccess)
				{
					//datashowPhase->SetTimerStart();
					break;
				}


			}


			/****************************测试结果判定**************************/

			bool bmove;
			dataNoiseReduction->setOwnerDraw(true, getPhaseMinVal(dataNoiseReduction1, bmove));
			dataNoiseReduction->setData(&dataNoiseReduction1, dataNoiseReduction1.size());
			dataNoiseReduction->setDataX(audiotest.getpindiandata(), audiotest.getpindiandata()->size());


			if (bIsSucess)
				bIsSucess = isSuccess;
			dataNoiseReduction->setTestResult(isSuccess);
			if (dataNoiseReduction1.size() > 0 /*&& alogriSettins.getRbData().getHamnics()->size()>0*/)
				dataNoiseReduction->ShowWindow(SW_HIDE);



			//设置框线数据
			dataNoiseReduction->setDataBorderLine(borderX, borderX.size());
			dataNoiseReduction->setDataBorderLinedown(downY, downY.size());
			dataNoiseReduction->setDataBorderLineup(upY, upY.size());

			//mrubz.setDara(&datarub);

			audiotest.saveData(m_testID, "D:\\TestRecord\\dataNoiseReduction.csv", *audiotest.getpindiandata(), dataNoiseReduction1);




			dataNoiseReduction1.clear();
			dataNoiseReduction2.clear();

		}
	}
	process->setProcessMove(800);
	if (m_snr)
	{

		mainf->setStatusStr(CString("SNR计算中..."));
		/*datasen.push_back(2.3);
		datasen.push_back(1.3);
		datasen.push_back(4.3);
		datasen.push_back(12.3);
		datasen.push_back(32.3);
		msen.setDara(&datasen);*/


		//SPK与MIC测试方式有所不同

		audiotest.setDeviceInputVpval(devicemanage.getinputvp(recordstr));
		audiotest.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		audiotest.setIsComputerNorse(true);

		if (resshowSNR != NULL)
		{
			delete resshowSNR;
			resshowSNR = NULL;
		}

		resshowSNR = new CResShow;
		resshowSNR->Create(CResShow::IDD);
		//对激励信号做判断 需要定频
		
		resshowSNR->setTitle(_T("SNR"));

		
		audiotest.setIsComputerSNR(true);

		if (!m_btestSnr)
		{
			for (size_t i = _msize(indata) / sizeof(double)-1000; i < _msize(indata) / sizeof(double); i++)
			{
				indata[i] = 0;
			}
		}

		audiotest.Alg_Fr(indata, fs);

		if (!m_btestSnr)
		{
			m_snr1 = audiotest.m_snr;
			m_btestSnr = true;
		}
		else
		{
			m_snr2 = audiotest.m_snr;
			resshowSNR->setitemName(_T("信噪比:"));
			if (audiotest.fr_data.size()>0)
				resshowSNR->setval(10 * log10(m_snr2 / m_snr1));
			
			resshowSNR->setuint(_T("dB"));

			if (audiotest.fr_data.size()>0)
				resshowSNR->ShowWindow(SW_HIDE);
			else
			{
				mainf->setStatusStr(_T("信噪比测试失败..."));
			}

			m_btestSnr = false;

			if (indata!=NULL)
			    delete[] indata;
		}

		
	}



	process->setProcessMove(0);
	Sleep(600);
	::SendMessage(processClose->m_hWnd, WM_CLOSE, 0, 0);
	mainf->setStatusStr(CString("计算中完成..."));
	/*mfr.updateshow();
	mthd.updateshow();
	mthdn.updateshow();
	mrubz.updateshow();
	mphase.updateshow();
	msen.updateshow();*/

	
	if (bIsSucess)
	{
		GetDlgItem(IDC_ereTestResnew)->SetWindowTextW(CString("PASS"));
	}
	else
	{
		GetDlgItem(IDC_ereTestResnew)->SetWindowTextW(CString("FAIL"));
	}
	GetDlgItem(IDC_testResLable)->SetWindowTextW(CString("测试结束"));
	//BOOL b = KillTimer(TIMER_ID);

	if (m_isrunCycles)
	{
		CString s_times("当前循环测试次数为：");

		char s_buf[512]{0};

		sprintf(s_buf, "%d", currentTestTimes);
		s_times.Append(CString(s_buf));
		s_times.Append(CString("次..."));
		mainf->setStatusStr(s_times);
	}
	

	m_bTesting = false;
	
	if (!m_btestSnr&&m_snr || !m_snr)
	    testResManage.ShowWindow(SW_SHOW);

	//设置wave的txt数据


	//waveShow.ShowWindow(SW_SHOW);


}

LRESULT CBlueEreTest::OnResItemShow(WPARAM wParam, LPARAM lParam)
{


	map<int, CDataShow*>::iterator it;

	bool bserch = false;
	int nslot = 0;
	int nitem = 0;
	for (it = micres.begin(); it != micres.end(); it++)
	{

		if (it->first == lParam)
		{
			//if (it->first == 2)
			//{
			//	//灵敏度特殊处理
			//	
			//}
			//else
			{
				if (!it->second->IsWindowVisible())
				{
					
					vector<double> mins;
						nslot = (lParam / 3)%4;
						nitem = lParam % 3; ////spk fr thd  rb  mic  fr thd sen
						if (nitem != 2)
						{

							//取其他三个slot数据,目前每个图一份数据因此要取其他三个
							vector<double> tempdata;
							
							if (nslot == 0)
							{
								//slot2  3  4
								micres[3 * 1 + 12 + nitem]->getData(tempdata, 2);
								it->second->setdata1(tempdata, 2);
								mins.push_back(micres[3 * 1 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 2 + 12 + nitem]->getData(tempdata, 3);
								it->second->setdata1(tempdata, 3);
								mins.push_back(micres[3 * 2 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 3 + 12 + nitem]->getData(tempdata, 4);
								it->second->setdata1(tempdata, 4);
								mins.push_back(micres[3 * 3 + 12 + nitem]->GetbownerDrwa_min());

							}
							else if (nslot == 1)
							{
								// 1  3  4
								micres[3 * 0 + 12 + nitem]->getData(tempdata, 1);
								it->second->setdata1(tempdata, 1);
								mins.push_back(micres[3 * 0 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 2 + 12 + nitem]->getData(tempdata, 3);
								it->second->setdata1(tempdata, 3);
								mins.push_back(micres[3 * 2 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 3 + 12 + nitem]->getData(tempdata, 4);
								it->second->setdata1(tempdata, 4);
								mins.push_back(micres[3 * 3 + 12 + nitem]->GetbownerDrwa_min());

							}
							else if (nslot == 2)
							{
								// 1 2 4
								micres[3 * 0 + 12 + nitem]->getData(tempdata, 1);
								it->second->setdata1(tempdata, 1);
								mins.push_back(micres[3 * 0 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 1 + 12 + nitem]->getData(tempdata, 2);
								it->second->setdata1(tempdata, 2);
								mins.push_back(micres[3 * 1 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 3 + 12 + nitem]->getData(tempdata, 4);
								it->second->setdata1(tempdata, 4);
								mins.push_back(micres[3 * 3 + 12 + nitem]->GetbownerDrwa_min());

							}
							else if (nslot == 3)
							{
								// 1 2 3
								micres[3 * 0 + 12 + nitem]->getData(tempdata, 1);
								it->second->setdata1(tempdata, 1);
								mins.push_back(micres[3 * 0 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 1 + 12 + nitem]->getData(tempdata, 2);
								it->second->setdata1(tempdata, 2);
								mins.push_back(micres[3 * 1 + 12 + nitem]->GetbownerDrwa_min());
								micres[3 * 2 + 12 + nitem]->getData(tempdata, 3);
								it->second->setdata1(tempdata, 3);
								mins.push_back(micres[3 * 2 + 12 + nitem]->GetbownerDrwa_min());

							}



						}
					
						if (nitem != 2)
						{

							double max;
							it->second->showDataMax(mins, max, false);
							it->second->setOwnerDraw(true, max);
						}

					   it->second->ShowWindow(SW_SHOW);
				}
				else
				{
					it->second->ShowWindow(SW_HIDE);
				}
			}
			
			bserch = true;
			break;
		}
	}
	if (bserch)
		return 0;
	for (it = spkres.begin(); it != spkres.end(); it++)
	{

		if (it->first == lParam)
		{
			vector<double> mins;
			nslot = (lParam / 3) % 4;
			nitem = lParam % 3; ////spk fr thd  rb  mic  fr thd sen
			if (!it->second->IsWindowVisible())
			{

				vector<double> tempdata;
				
				if (nslot == 0)
				{
					//slot2  3  4
					spkres[3 * 1 + nitem]->getData(tempdata, 2);
					it->second->setdata1(tempdata, 2);
					mins.push_back(spkres[3 * 1 + nitem]->GetbownerDrwa_min());
					spkres[3 * 2 + nitem]->getData(tempdata, 3);
					it->second->setdata1(tempdata, 3);
					mins.push_back(spkres[3 * 2 + nitem]->GetbownerDrwa_min());
					spkres[3 * 3 + nitem]->getData(tempdata, 4);
					it->second->setdata1(tempdata, 4);
					mins.push_back(spkres[3 * 3 + nitem]->GetbownerDrwa_min());

				}
				else if (nslot == 1)
				{
					// 1  3  4
					spkres[3 * 0 + nitem]->getData(tempdata, 1);
					it->second->setdata1(tempdata, 1);
					mins.push_back(spkres[3 * 0 + nitem]->GetbownerDrwa_min());
					spkres[3 * 2 + nitem]->getData(tempdata, 3);
					it->second->setdata1(tempdata, 3);
					mins.push_back(spkres[3 * 2 + nitem]->GetbownerDrwa_min());
					spkres[3 * 3 + nitem]->getData(tempdata, 4);
					it->second->setdata1(tempdata, 4);
					mins.push_back(spkres[3 * 3 + nitem]->GetbownerDrwa_min());

				}
				else if (nslot == 2)
				{
					// 1 2 4
					spkres[3 * 0 + nitem]->getData(tempdata, 1);
					it->second->setdata1(tempdata, 1);
					mins.push_back(spkres[3 * 0 + nitem]->GetbownerDrwa_min());
					spkres[3 * 1 + nitem]->getData(tempdata, 2);
					it->second->setdata1(tempdata, 2);
					mins.push_back(spkres[3 * 1 + nitem]->GetbownerDrwa_min());
					spkres[3 * 3 + nitem]->getData(tempdata, 4);
					it->second->setdata1(tempdata, 4);
					mins.push_back(spkres[3 * 3 + nitem]->GetbownerDrwa_min());

				}
				else if (nslot == 3)
				{
					// 1 2 3
					spkres[3 * 0 + nitem]->getData(tempdata, 1);
					it->second->setdata1(tempdata, 1);
					mins.push_back(spkres[3 * 0 + nitem]->GetbownerDrwa_min());
					spkres[3 * 1 + nitem]->getData(tempdata, 2);
					it->second->setdata1(tempdata, 2);
					mins.push_back(spkres[3 * 1 + nitem]->GetbownerDrwa_min());
					spkres[3 * 2 + nitem]->getData(tempdata, 3);
					it->second->setdata1(tempdata, 3);
					mins.push_back(spkres[3 * 2 + nitem]->GetbownerDrwa_min());

				}


				if (nitem != 2)
				{

					double max;
					it->second->showDataMax(mins, max, false);
					it->second->setOwnerDraw(true, max);
				}

				it->second->ShowWindow(SW_SHOW);
			}
			else
			{
				it->second->ShowWindow(SW_HIDE);
			}

			break;
		}
	}



	return 0;
	if (wParam == 0)
	{
		if (!datashow->IsWindowVisible())
		{
			if (lParam != -1)
			{

			}
			else
			{
				datashow->ShowWindow(SW_SHOW);

			}
			
		}
		else
		{
			datashow->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 1)
	{
		
		if (!datashowthd->IsWindowVisible())
			datashowthd->ShowWindow(SW_SHOW);
		else
		{
			datashowthd->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 2)
	{
		
		if (!datashowthd_N->IsWindowVisible())
			datashowthd_N->ShowWindow(SW_SHOW);
		else
		{
			datashowthd_N->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 3)
	{
		
		if (!datashowRB->IsWindowVisible())
			datashowRB->ShowWindow(SW_SHOW);
		else
		{
			datashowRB->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 4)
	{
		
		if (!datashowPhase->IsWindowVisible())
			datashowPhase->ShowWindow(SW_SHOW);
		else
		{
			datashowPhase->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 5)
	{
		
		if (!resshowSensitivity->IsWindowVisible())
			resshowSensitivity->ShowWindow(SW_SHOW);
		else
		{
			resshowSensitivity->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 6)
	{
		if (!resshowSensitivity->IsWindowVisible())
			resshowSensitivity->ShowWindow(SW_SHOW);
		else
		{
			resshowSensitivity->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 8)
	{
		if (!resshowSNR->IsWindowVisible())
			resshowSNR->ShowWindow(SW_SHOW);
		else
		{
			resshowSNR->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 9)
	{
		if (!resshowPolarity->IsWindowVisible())
			resshowPolarity->ShowWindow(SW_SHOW);
		else
		{
			resshowPolarity->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 10)
	{
		if (!resshowBalance->IsWindowVisible())
			resshowBalance->ShowWindow(SW_SHOW);
		else
		{
			resshowBalance->ShowWindow(SW_HIDE);
		}
	}
	else if (wParam == 11)
	{
		if (!dataNoiseReduction->IsWindowVisible())
			dataNoiseReduction->ShowWindow(SW_SHOW);
		else
		{
			dataNoiseReduction->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

void CBlueEreTest::CloseResWindow()
{
	if (datashow != NULL)
	{
		delete datashow;
		datashow = NULL;
	}

	if (datashowthd != NULL)
	{
		delete datashowthd;
		datashowthd = NULL;
	}

	if (datashowthd_N != NULL)
	{
		delete datashowthd_N;
		datashowthd_N = NULL;
	}

	if (datashowRB != NULL)
	{
		delete datashowRB;
		datashowRB = NULL;
	}
	if (datashowPhase != NULL)
	{
		delete datashowPhase;
		datashowPhase = NULL;
	}


}


double CBlueEreTest::getPhaseMinVal(vector<double>& phase_data, bool& bmove)
{

	double min = 0;
	bmove = false;
	for (size_t i = 0; i < phase_data.size(); i++)
	{
		if (phase_data[i] < min)
		{
			min = phase_data[i];
		}
	}
	if (min < 0)
	{
		for (size_t i = 0; i < phase_data.size(); i++)
		{
			phase_data[i] += abs(min);
		}

		min = abs(min);
		bmove = true;
	}
	
	return min;
}

 UINT refreshTimes(LPVOID pParam)
{
	// CBlueEreTest* pbl = (CBlueEreTest*)pParam;
	// CString s_time;
	// while (m_bTesting)
	//{
	//	 Sleep(1000);
	//	
	//	(pbl->playtimes)++;
	//	
	//	s_time.Format(_T("%d 秒"), pbl->playtimes);
	//	/*pbl->m_staticPlayTimes.SetWindowTextW(s_time);
	//	pbl->m_staticPlayTimes.UpdateData();*/
	//	pbl->GetDlgItem(IDC_eretestTestTimes)->SetWindowTextW(s_time);
	//}
	 return 0;
}



//up -1 表示不需要比较
bool CBlueEreTest::FrChecked(double hz, double down, double up, vector<double>* data, vector<double>* res, const vector<Mutidata*>& borderLine)
{
	bool ret = true;

	double ignorelow = ReadDouble(_T("AlgConf"), _T("IgnoreLow"), 300, GetConfigIniFile());
	double ignoreup = ReadDouble(_T("AlgConf"), _T("IgnoreUp"), 7100, GetConfigIniFile());

	if (res->size() == 0)
		return ret;

	double temp_dowen, temp_up,temp_fs;
	double x, y, z;
	double x1, y1, z1;
	for (size_t i = 0; i < data->size(); i++)
	{


		if (hz <= ignorelow || hz >= ignoreup)
		{
			return true;
		}

		if (data->at(i) == hz)
		{
			if (res->at(i) < down || (up != -1) && (res->at(i) > up))
			{
				ret = false;
				break;
				
			}

		
		}
		else
		{
			//计算出该频点出的框线值
			temp_fs = data->at(i);

			//1.确定频点在哪两个框线点之间
			
			for (size_t j = 1; j < borderLine.size(); j++)//框线需要从小到大顺序
			{
				borderLine[j-1]->getdata(x, y, z);
				borderLine[j]->getdata(x1, y1, z1);
				if (temp_fs<x1&&temp_fs>x)
				{

					if (y1 - y == 0)
					{
						temp_dowen = y;
						
					}
					else
					{
						temp_dowen = ((y1-y) / (x1 - x))*(temp_fs-x);
						temp_dowen += y;
						
					}

					if (z1 - z == 0)
					{
						
						temp_up = z;
					}
					else
					{
						
						temp_up = ((z1 - z) / (x1 - x))*(temp_fs-x);
						temp_up += z;
					}




					if (res->at(i) < temp_dowen || (temp_up != -1) && (res->at(i)>temp_up))
					{
						ret = false;
						

					}

					break;

				}

				if (j == borderLine.size())
				{
					//最后一个点
				}

				if (j == 0)
				{

				}
			}


		}
		
	}

	return ret;
}

void CBlueEreTest::recordTest()
{
	mainf->setStatusStr(CString("录音任务执行中..."));
	CString ss;
	GetDlgItem(IDC_eretime)->GetWindowTextW(ss);
	USES_CONVERSION;
	int time = atoi(T2A(ss));
	time = 10000;
	testRme.setwdmIsRecordDone(false);
	testRme.record(time);
	while (!testRme.wdmIsRecordDone())
		Sleep(1000);


	mainf->setStatusStr(CString("录音完成..."));

}

void CBlueEreTest::play(char* data)
{

	//测试数据
	int f = 1000;
	int fs = 44100;
	int time = 1;
	int len = fs * time;
	int* sindata = new int[len];
	
	int ap = 1000000000;
	for (size_t i = 0; i < len; i++)
	{

		sindata[i] = 0.5*ap*sin(2.0*3.1415926*f*i / fs);

	}

	//生成wav格式文件
	const char * filename = "testblueEre.wav";
	wavwrite(filename, 1, fs, 32, (char*)sindata, len * 4);


	//执行播放
	testRme.play(_T("testblueEre.wav"), time*1000);


}


void CBlueEreTest::OnBnClickedButtonRecordTest()
{
	// TODO:  在此添加控件通知处理程序代码
	//录音测试
	AfxMessageBox(_T("2s后开始录音,录音10s"));
	Sleep(2000);
	recordTest();
}


void CBlueEreTest::OnBnClickedButtonAutoConnEre()
{
	// TODO:  在此添加控件通知处理程序代码


	char szBtId[32];
	if (!GetBlueToothID(0, szBtId,NULL/* sizeof(szBtId)*/))
	{
		//WriteErrorLog(_T("Slot%d蓝牙ID不存在！"), pSlot->nSlotIndex);
		SetSlotItem(_T("failmsg"), _T("蓝牙ID不存在"), 0);
		SetSlotItem(_T("result"), _T("1"), 0);
		//continue;
	}

	DisonnectBT(0);

	if (!ConnectBTSwitch(0, szBtId))
	{
		SetSlotItem(_T("failmsg"), _T("蓝牙连接失败"), 0);
		SetSlotItem(_T("result"), _T("1"), 0);
	
	}

	return;


	//从配置文件中读取串口
	ifstream infile;
	
	
	infile.open("blueere.ini", ios_base::in);
	if (!infile.is_open())
	{
		ofstream outfile;
		outfile.open("blueere.ini", ios_base::out | ios_base::trunc);
		outfile << "PortName:COM1\n";
		outfile << "PortName:COM2\n";
		outfile << "PortName:COM3\n";
		outfile << "PortName:COM4\n";
		outfile.close();
		infile.open("blueere.ini", ios_base::in);

	}

	char* comname = new char[14]{0};
	vector<CString> comNames;
	char portname[5]{0};
	while (!infile.eof())
	{
		infile.read(comname, 13);
		
		memcpy(portname, comname + 9, 4);
		comNames.push_back(CString(portname));
		infile.read(comname, 1);
		memset(comname, 0, 14);
		memset(portname, 0, 5);
		
	}
	mainf->setStatusStr(CString("连接蓝牙耳机中..."));
	for (size_t i = 0; i < comNames.size(); i++)
	{


		if (!bluecomm.opencomm(comNames[i]))
		{
			mainf->setStatusStr(CString("串口打开失败..."));
			return;
		}
		
		char* id = bluecomm.searchEre();
		if (strlen(id) == 12)
		{
			
			GetDlgItem(IDC_EreID)->SetWindowTextW(CString(id));
			bluecomm.connectRre(id);
			GetDlgItem(IDC_EreID)->SetWindowTextW(CString(id));
			mainf->setStatusStr(CString("蓝牙连接成功..."));
		}
		else
		{
			//AfxMessageBox(_T("搜索失败!"));
			CString str = CString("搜索失败...");
			str.Append(comNames[i]);
			bluecomm.disconnect();
			mainf->setStatusStr(str);
		}

	}
	
}


void CBlueEreTest::OnBnClickedButtonDisconnectEre()
{
	// TODO:  在此添加控件通知处理程序代码
	bluecomm.disconnect();
	GetDlgItem(IDC_EreID)->SetWindowTextW(_T("..."));
	mainf->setStatusStr(CString("断开耳机连接..."));
}


void CBlueEreTest::OnCbnDropdowneretestincom()
{
	// TODO:  在此添加控件通知处理程序代码
	mInchsnames.ResetContent();
	vector<CString> innernames = devicemanage.getInnerNames_InCHS();

	for (size_t i = 0; i < innernames.size(); i++)
	{
		mInchsnames.InsertString(i, innernames[i]);
	}

}


void CBlueEreTest::OnCbnDropdowneretestoutcom()
{
	// TODO:  在此添加控件通知处理程序代码
	mOutchsnames.ResetContent();
	vector<CString> innernames = devicemanage.getInnerNames_OutCHS();

	for (size_t i = 0; i < innernames.size(); i++)
	{
		mOutchsnames.InsertString(i, innernames[i]);
	}
}


void CBlueEreTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	playtimes++;
	CString s_time;
	s_time.Format(_T("%d 秒"),playtimes);
	m_staticPlayTimes.SetWindowTextW(s_time);
	m_staticPlayTimes.UpdateData();

	CDialogEx::OnTimer(nIDEvent);
}


void CBlueEreTest::OnBnClickedButtonTestCycles()
{
	// TODO:  在此添加控件通知处理程序代码
	

	std::thread testcysle(&CBlueEreTest::cyclesTest,this);
	testcysle.detach();
	
}

void CBlueEreTest::cyclesTest()
{

	m_isrunCycles = true;
	currentTestTimes = 0;

	

	while (currentTestTimes<testCycleTimes )
	{
		if (cyclesPeriod < jilisetnew.getPlayTimes())
		{
			AfxMessageBox(_T("循环最大周期应大于激励信号播放时间..."),MB_ICONWARNING);
			break;
		}

		if (m_isrunCycles&&!m_bTesting)
		{
			
			//给按钮发送消息
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON4, BN_CLICKED), NULL);
			currentTestTimes++;

			Sleep(cyclesPeriod * 1000);
		}

		
		
	}
}

void  CBlueEreTest::setRunCycles(bool brun)
{
	m_isrunCycles = brun;
}

void CBlueEreTest::setTestCycleTimes(int CycleTimes)
{
	testCycleTimes = CycleTimes;
}

int CBlueEreTest::getTestCycleTimes()
{

	return testCycleTimes;
}

void CBlueEreTest::setTestCyclePeriod(int CycleTimes)
{
	cyclesPeriod = CycleTimes;
}
int CBlueEreTest::getTestCyclePeriod()
{
	return cyclesPeriod;
}

void CBlueEreTest::setTestCyclecolor(COLORREF color)
{
	m_color = color;
}
COLORREF CBlueEreTest::getTestCyclecolor()
{
	return m_color;
}


void CBlueEreTest::OnBnClickedButtonStopTest()
{
	// TODO:  在此添加控件通知处理程序代码
	m_isrunCycles = !m_isrunCycles;
	if (m_isrunCycles)
	{
		GetDlgItem(IDC_BUTTON9)->SetWindowTextW(CString("暂停测试"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON9)->SetWindowTextW(CString("继续测试"));
	}
	UpdateData(FALSE);
}

void CBlueEreTest::drawGroupBox(CWnd* wnd, CPaintDC* dc)
{
	CRect rc;

	int dc_id = dc->SaveDC();
	CBrush groupbox_brush(RGB(255, 0, 8));
	CPen pen;
	pen.CreatePen(0, 1, RGB(255, 0, 8));
	//dc->SelectObject(pen);

	wnd->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.top += 14;
	MoveToEx(*dc, rc.left + 100, rc.top, NULL);
	LineTo(*dc, rc.right, rc.top);

	MoveToEx(*dc, rc.right, rc.top, NULL);
	LineTo(*dc, rc.right, rc.bottom);

	MoveToEx(*dc, rc.right, rc.bottom, NULL);
	LineTo(*dc, rc.left - 1, rc.bottom);

	MoveToEx(*dc, rc.left - 1, rc.bottom, NULL);
	LineTo(*dc, rc.left - 1, rc.top);

	MoveToEx(*dc, rc.left - 1, rc.top, NULL);
	LineTo(*dc, rc.left + 19, rc.top);

	dc->RestoreDC(dc_id);
}



void CBlueEreTest::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CWnd* pwnd = GetDlgItem(IDC_STATICtest);
	
	CRect rc;
	pwnd->GetWindowRect(rc);
	rc.top += 15;
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(248, 248, 255));
	drawGroupBox(pwnd, &dc);
	
	/*pwnd = GetDlgItem(IDC_STATICerectl);
	pwnd->GetWindowRect(rc);
	rc.top += 15;
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(248, 248, 255));
	drawGroupBox(pwnd, &dc);*/


	/*pwnd = GetDlgItem(IDC_testResLable);
	pwnd->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(0, 139, 69));*/
}


void CBlueEreTest::OnBnClickedButtonSwitchBlueMode()
{
	// TODO:  在此添加控件通知处理程序代码



	CString ss;
	char buff[1024]{0};
	GetDlgItem(IDC_BUTTON10)->GetWindowTextW(ss);
	if (ss == "蓝牙(MIC)模式")
	{
		
		bluecomm.switchmode(CString("蓝牙(SPK)模式"));
		GetDlgItem(IDC_BUTTON10)->SetWindowTextW(_T("蓝牙(SPK)模式"));
		m_btestSPK = true;

	}
	else
	{
		bluecomm.switchmode(CString("蓝牙(MIC)模式"));
		GetDlgItem(IDC_BUTTON10)->SetWindowTextW(_T("蓝牙(MIC)模式"));
		m_btestSPK = false;

	}
	CString mode;
	bluecomm.getDeviceMode(mode);
	mainf->setStatusStr(mode);

}


void CBlueEreTest::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值


	if (nChar == VK_END)
	{
		AfxMessageBox(_T("enter"));
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void  CBlueEreTest::OnOK()
{
	if (focus_num)
		OnBnClickedButtonTest();

}


void CBlueEreTest::OnEnSetfocusEdittestid()
{
	// TODO:  在此添加控件通知处理程序代码

	//AfxMessageBox(_T("enter"));
	focus_num = true;

}

BOOL CBlueEreTest::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{

			//AfxMessageBox(_T("enter"));
		}
	}

	/*if (pMsg->hwnd == GetDlgItem(IDC_EDITtestId)->m_hWnd)
	{
		if (pMsg->message == WM_KEYDOWN&&
			pMsg->lParam == VK_RETURN)
		{
			return TRUE;
		}
	}*/

	return CDialog::PreTranslateMessage(pMsg);
}


void CBlueEreTest::OnEnKillfocusEdittestid()
{
	// TODO:  在此添加控件通知处理程序代码
	focus_num = false;
}

//在此函数中创建界面控件，this指针已完成，该主窗口类已加载完成
int CBlueEreTest::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	testResManage.Create(CTestResManagement::IDD, this);
	testResManage2.Create(CTestResManagement::IDD, this);
	testResManage3.Create(CTestResManagement::IDD, this);
	testResManage4.Create(CTestResManagement::IDD, this);
	waveShow.Create(CWaveShow::IDD, this);

	return 0;
}


void CBlueEreTest::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	testResManage.DestroyWindow();
	CDialogEx::OnClose();
}

void CBlueEreTest::ShowRes()
{
	if (testResManage.m_hWnd!=NULL)
	   testResManage.ShowWindow(SW_SHOW);
	if (testResManage2.m_hWnd != NULL)
		testResManage2.ShowWindow(SW_SHOW);
	if (testResManage3.m_hWnd != NULL)
		testResManage3.ShowWindow(SW_SHOW);
	if (testResManage4.m_hWnd != NULL)
		testResManage4.ShowWindow(SW_SHOW);
}

bool CBlueEreTest::SendBTDataAndCheck(const slotParam* pSlot, const char* pSend, const char* pRes)
{
	if (pSlot == NULL || pSend == NULL)
		return false;

	bool bRet = false;
	char szBuff[1024] = "\0";
	DWORD dwPrev;
	int nLen = 0;
	int nTimeout = ReadDouble(_T("BlueTooth"), _T("CmdTimeout"), 6, GetScriptIniFile());


	//memcpy(szBuff, ">PROFILE_SET=0&0", strlen(">PROFILE_SET=0&0"));
	WriteDebugLog(_T("Connect command:%s"), CString(pSend));
	pSlot->pSerial->WriteBuffer((char*)pSend, strlen(pSend), 3000);



	memset(szBuff, 0, sizeof(szBuff));
	dwPrev = GetTickCount();
	while ((GetTickCount() - dwPrev) < nTimeout * 1000)
	{
		pSlot->pSerial->Read(szBuff + nLen, sizeof(szBuff) - nLen, 1000);
		WriteDebugLog(_T("Connect recv：%s"), CString(szBuff));
		Sleep(10);
		nLen = strlen(szBuff);

		if (szBuff[nLen - 1] != 0x0D && szBuff[nLen - 1] != 0x0A)
		{
			continue;
		}

		if (strstr(szBuff, pRes) != NULL)
		{
			bRet = true;
			WriteInfoLog(_T("Slot%d执行蓝牙适配器命令(%s)成功"), pSlot->nSlotIndex, CString(pSend));
			break;
		}

		
	}

	if (!bRet)
	{
		WriteErrorLog(_T("Slot%d执行蓝牙适配器命令(%s)超时"), pSlot->nSlotIndex, CString(pSend));
	}

	return bRet;
}

bool CBlueEreTest::ConnFenDaWatch(const char* pBtID, int nSlot)
{

	//1.>PROFILE_SET=0&0
	//2.>CONN=ACB1EEDF217F
	//3.>SCO_DISC
	//4.>SCO_CONN
	char szBuff[80] = "\0";
	slotParam* pSlot = GetSlotParam(nSlot);

	int restTimeout = ReadDouble(_T("BlueTooth"), _T("RstTimeout"), 1000, GetScriptIniFile());
	int connTimeout = ReadDouble(_T("BlueTooth"), _T("ConnTimeout"), 1000, GetScriptIniFile());
	int scoTimeout = ReadDouble(_T("BlueTooth"), _T("ScoTimeout"), 1000, GetScriptIniFile());

	if (!SendBTDataAndCheck(pSlot, ">RST", "OK"))
	{
		WriteErrorLog(_T("Slot%d 复位失败！"), pSlot->nSlotIndex);
		return false;
	}


	Sleep(restTimeout);
	
	if (!SendBTDataAndCheck(pSlot, ">PROFILE_SET=0&0", "DISABLE A2DP"))
	{
		WriteErrorLog(_T("Slot%d 禁用A2DP失败！"), pSlot->nSlotIndex);
		return false;
	}

	bool bConn = false;
	//Sleep(500);
	memset(szBuff, 0, sizeof(szBuff));
	sprintf(szBuff, ">CONN=%s", pBtID);

	for (int i = 0; i < 3; i++)
	{
		if (!SendBTDataAndCheck(pSlot, szBuff, "_success"))
		{
			Sleep(500);
			continue;
		}

		bConn = true;
		break;
	}

	if (!bConn)
	{
		CString sMsg(_T(""));
		sMsg.Format(_T("Slot%d蓝牙连接失败！"), pSlot->nSlotIndex);
		MessageBox(sMsg, _T("错误"), MB_OK | MB_ICONERROR);
		return false;
	}

	Sleep(connTimeout);

	if (!SendBTDataAndCheck(pSlot, ">SCO_DISC", "hfp_connect_success"))
	{
		WriteErrorLog(_T("Slot%d SCO_DISC执行失败！"), pSlot->nSlotIndex);
		//return false;
	}

	Sleep(scoTimeout);

	if (!SendBTDataAndCheck(pSlot, ">SCO_CONN", "PAIREDHFP"))
	{
		WriteErrorLog(_T("Slot%d SCO_CONN执行失败！"), pSlot->nSlotIndex);
		//return false;
		return false;
	}

	return true;

}

void CBlueEreTest::setHTTPClinet(CHttpClient* pHttp, CString staton)
{
	m_pHttp = pHttp;
	m_station = staton;
}

void CBlueEreTest::MesDataPost(CString sTestItem, bool rdbFail, CString ErrCode, CString ErrItem, CString sSN)
{

	CString sStation=m_station;
	

	testData stData;
	stData.sErrCode = _T("");
	stData.sErrItem = _T("");
	stData.sResult = _T("PASS");
	//sStation=m_station;
	//sSN=m_sn;
	

	if (sStation.IsEmpty())
	{
		AfxMessageBox(_T("机台编号不能为空"));
		return;
	}

	if (sSN.IsEmpty())
	{
		AfxMessageBox(_T("SN不能为空"));
		return;
	}

	if (sTestItem.IsEmpty())
	{
		AfxMessageBox(_T("测试项目不能为空"));
		return;
	}

	if (rdbFail)
	{
		stData.sResult = _T("FAIL");
		stData.sErrCode = ErrCode;
		stData.sErrItem = ErrItem;

		if (stData.sErrCode.IsEmpty())
		{
			AfxMessageBox(_T("FAIL状态，不良代码不能为空"));
			return;
		}

		if (stData.sErrItem.IsEmpty())
		{
			AfxMessageBox(_T("FAIL状态，不良项目不能为空"));
			return;
		}
	}

	stData.vItems.clear();
	Split(sTestItem, _T(","), stData.vItems);

	bool bRet = m_pHttp->MesUploadData(sStation, sSN, &stData);
	if (!bRet)
	{
		mainf->setStatusStr(_T("上传mes数据失败！"));
		AfxMessageBox(m_pHttp->GetLastErrMsg());
		return;
	}

	mainf->setStatusStr(_T("上传mes数据成功！"));

}


void CBlueEreTest::SetIsOffline(bool onlineSta)
{
	m_IsOffline = onlineSta;
}