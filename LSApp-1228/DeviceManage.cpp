// DeviceManage.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "DeviceManage.h"
#include "afxdialogex.h"
#include "resource.h"
#include "wavHandler.h"
#include <fstream>
#include "wav.h"
#include "MyButton.h"
#include "InputDialg.h"

extern int globle_out;
// CDeviceManage 对话框

IMPLEMENT_DYNAMIC(CDeviceManage, CDialogEx)

CDeviceManage::CDeviceManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceManage::IDD, pParent)
{
	//bool b_ok = wavInitialize();
	m_bInitFinish = false;
	m_bInitDialog = true;

	/*if (!b_ok)
	{
		AfxMessageBox(_T("wavInitialize err"));

	}*/
}

CDeviceManage::~CDeviceManage()
{
}

void CDeviceManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON1, m_addinBtn);
}

BOOL CDeviceManage::OnInitDialog()
{
	UpdateData(FALSE);
	mainf = (CMainFrame*)theApp.GetMainWnd();
	//m_btn01Brush.CreateSolidBrush(RGB(0, 255, 0));
	/*CMyButton* mybtn=NULL;
	mybtn = new CMyButton;
	CRect rc(100,500,280,540);
	mybtn->Create(CString("删除输入通道"), 0, rc, this, ID_MYBTN_01);
	mybtn->UpdateWindow();
	mybtn->ShowWindow(SW_SHOW);*/

	/*CMyButton* mybtn = new CMyButton;
	mybtn->Attach(IDC_BUTTON1,this);*/
//	mybtn.Attach(ID_MYBTN_01, this);

	HFONT hFont = CreateFont(30, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,_T("宋体"));

	CWnd* pgroupbox = GetDlgItem(IDC_STATIC);
	pgroupbox->SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	pgroupbox = GetDlgItem(IDC_STATIC2);
	pgroupbox->SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);

	CFont f;
	f.CreateFontW(60, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	/*GetDlgItem(IDC_BUTTON1)->SetFont(&f);
	GetDlgItem(IDC_BUTTON2)->SetFont(&f);
	GetDlgItem(IDC_BUTTON3)->SetFont(&f);
	GetDlgItem(IDC_BUTTON4)->SetFont(&f);
	GetDlgItem(IDC_devicesavein)->SetFont(&f);
	GetDlgItem(IDC_devicemangerecord)->SetFont(&f);
	GetDlgItem(IDC_devicesaveout)->SetFont(&f);
	GetDlgItem(IDC_devicemangeplay)->SetFont(&f);*/

	//m_addinBtn.SetButtonStyle(BS_DEFPUSHBUTTON);

	
	//this->SetBackgroundColor(RGB(0, 197, 205));
	brush1.DeleteObject();
	brush1.CreateSolidBrush(RGB(248, 248, 255));


	//删除数据，新创建
	/*for (size_t i = 0; i < data.size(); i++)
	{
		delete data[i];
	}
	for (size_t i = 0; i < data2.size(); i++)
	{
		delete data2[i];
	}*/

	//if (m_bInitDialog)
	    //initDialogData();


	///m_bInitDialog = true;
    
	return TRUE;
}

bool CDeviceManage::isInitFinish()
{
	return m_bInitFinish;
}

BOOL CDeviceManage::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	
	
	for (size_t i = 0; i < data.size(); i++)
	{
	  delete data[i];
	  
	}

	data.clear();
	for (size_t i = 0; i < data2.size(); i++)
	{
		delete data2[i];
		
	}
	data2.clear();

	devParam* pDev = NULL;
	m_bInitDialog = false;
	
	CDialogEx::Create(nIDTemplate, pParentWnd);
	
	

	BOOL b = -1;
	MYcontrol2* cont = NULL;
	MyControldata* myctroldata = NULL;
	bool bInput = true;

	for (int i = 0; i < GetDevCount(bInput); i++)
	{
		cont = new MYcontrol2(bInput, i);

		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
		itemdatawnd = cont->m_hWnd;
		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
		CRect rect, rect1;
		GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//IDC_mycontrolbits
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		cont->MoveWindow(5, 80 + i*rect1.Height(), rect.Width() - 6, rect1.Height());
		
		pDev = GetDevParam(i, bInput);
		//cont->setInnerChsName(pDev->sName);
		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowText(pDev->sName);
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
		{
			cont->SetDriverSelected(0);
		}
		else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
		{
			cont->SetDriverSelected(1);
		}
		else
		{
			cont->SetDriverSelected(-1);
		}
		cont->OnCbnSelchangemycontroldrivername();
		cont->SetDeviceSelected(pDev->sDevice);
		cont->OnCbnSelchangemycontroldevicename2();
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
		{
			cont->SetChannelSelected(pDev->nChannel - 1);
		}
		else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
		{
			cont->SetChannelSelected(pDev->nChannel);
		}
		else
		{
			cont->SetChannelSelected(-1);
		}
		cont->setVpVal(pDev->nVp);
		cont->SetSamplingRateVal(pDev->nSamplingRate);
		cont->SetBitDepthVal(pDev->nBitDepth);
		cont->SetSensitivityOrGainVal(pDev->nSensitivity);

		myctroldata = new MyControldata(CString(pDev->sName), CString(pDev->sDriver),
			CString(pDev->sDevice));
		inlist.push_back(myctroldata);
		data.push_back(cont);
		cont->setChecked();
		cont->ShowWindow(SW_SHOW);
	}

	curindex = 0;//当前的开始位置0
	testi = GetDevCount(bInput);
	
	//if (data.size() == 0)
	//{
	//	for (int i = 0; i < 3; i++)
	//	{
	//		cont = new MYcontrol2(true);
	//		
	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//		itemdatawnd = cont->m_hWnd;
	//		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		//IDC_mycontrolbits
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		cont->MoveWindow(5, 80 + i*rect1.Height(), rect.Width()-6, rect1.Height());
	//		char buf[1024];
	//		sprintf(buf, "%d", i);
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString(buf));

	//		myctroldata = new MyControldata(CString(buf), CString(""), CString(""));
	//		inlist.push_back(myctroldata);
	//		data.push_back(cont);
	//		cont->setChecked();
	//		cont->ShowWindow(SW_SHOW);
	//	}
	//	curindex = 0;//当前的开始位置0
	//	testi = 3;
	//}
	//else
	//{
	//	CRect rect, rect1;
	//	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//	for (int i = 0; i < data.size(); i++)
	//	{
	//		delete data[i];
	//	}
	//	int lastcount = data.size();
	//	data.clear();
	//	//for (size_t i = 0; i < lastcount; i++)
	//	//{
	//	//	cont = new MYcontrol2(true);

	//	//	b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//	//	CRect rect, rect1;
	//	//	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//	//	cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	cont->MoveWindow(5, 80 + i*rect1.Height(), rect.Width(), rect1.Height());

	//	//	//设置数据
	//	//	myctroldata = new MyControldata(CString("0"), CString(""), CString(""));
	//	//	inlist.push_back(myctroldata);
	//	//	data.push_back(cont);
	//	//	cont->ShowWindow(SW_SHOW);
	//	//}

	//	/*for (int i = 0; i < inlist.size(); i++)
	//	{
	//		delete inlist[i];
	//	}*/

	//	int deli = 0;
	//	for (list<MyControldata*>::iterator it = inlist.begin(); it != inlist.end(); it = inlist.begin())
	//	{
	//		if (lastcount == deli)
	//			break;

	//		cont = new MYcontrol2(true);

	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		cont->MoveWindow(5, 80 + deli*rect1.Height(), rect.Width()-6, rect1.Height());

	//		//设置数据
	//		CString s1, s2, s3;
	//		(*it)->getdata(s1, s2, s2);
	//		(*it)->setdata(s1, s2, s3);
	//		myctroldata = new MyControldata(CString(s1), CString(s2), CString(s3));
	//		inlist.push_back(myctroldata);
	//		
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(s1);
	//		cont->GetDlgItem(IDC_mycontroldrivername)->SetWindowTextW(s2);
	//		cont->GetDlgItem(IDC_mycontroldevicename2)->SetWindowTextW(s3);

	//		data.push_back(cont);
	//		cont->ShowWindow(SW_SHOW);

	//		delete *it;
	//		deli++;
	//		inlist.pop_front();
	//	}


	//}

	/************************输出通道创建*******************************/
	bInput = false;
	for (int i = 0; i < GetDevCount(bInput); i++)
	{
		cont = new MYcontrol2(bInput, i);
		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
		CRect rect, rect1;
		GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//cont->MoveWindow(110, 280 + 8 * rect1.Height() + i * rect1.Height(), rect.Width(), rect1.Height());
		cont->MoveWindow(5, 80 + (i % 8) * rect1.Height(), rect.Width()-7, rect1.Height());

		pDev = GetDevParam(i, bInput);
		//cont->setInnerChsName(pDev->sName);
		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowText(pDev->sName);
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
		{
			cont->SetDriverSelected(0);
		}
		else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
		{
			cont->SetDriverSelected(1);
		}
		else
		{
			cont->SetDriverSelected(-1);
		}
		cont->OnCbnSelchangemycontroldrivername();
		cont->SetDeviceSelected(pDev->sDevice);
		cont->OnCbnSelchangemycontroldevicename2();
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
		{
			cont->SetChannelSelected(pDev->nChannel - 1);
		}
		else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
		{
			cont->SetChannelSelected(pDev->nChannel);
		}
		else
		{
			cont->SetChannelSelected(-1);
		}
		cont->setVpVal(pDev->nVp);
		cont->SetSamplingRateVal(pDev->nSamplingRate);
		cont->SetBitDepthVal(pDev->nBitDepth);
		cont->SetSensitivityOrGainVal(pDev->nGain);

		data2.push_back(cont);
		cont->setChecked();
		cont->ShowWindow(SW_SHOW);
	}
	curindex2 = 0;//当前的开始位置0
	curindexout = 0;//当前的开始位置0
	testi2 = GetDevCount(bInput);

	//if (data2.size() == 0)
	//{
	//	char buf[1024];
	//	for (int i = 0; i < 2; i++)
	//	{
	//		cont = new MYcontrol2(false);
	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
	//		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		//cont->MoveWindow(110, 280 + 8 * rect1.Height() + i * rect1.Height(), rect.Width(), rect1.Height());
	//		cont->MoveWindow(5, 80 + (i % 8) * rect1.Height(), rect.Width()-7, rect1.Height());

	//		
	//		sprintf(buf, "%d", i);
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString(buf));
	//		data2.push_back(cont);
	//		cont->setChecked();
	//		cont->ShowWindow(SW_SHOW);
	//	}
	//	curindex2 = 0;//当前的开始位置0
	//	curindexout = 0;//当前的开始位置0
	//	testi2 = 2;
	//}
	//else
	//{
	//	CRect rect, rect1;
	//	GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	//	for (int i = curindexout; i < data2.size(); i++)
	//	{
	//		delete data2[i];
	//	}
	//	int lastcount = data2.size();
	//	data2.clear();
	//	/************二次创建********************/
	//	char buf[1024];
	//	for (int i = 0; i < lastcount; i++)
	//	{
	//		cont = new MYcontrol2(false);
	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
	//		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		//cont->MoveWindow(110, 280 + 8 * rect1.Height() + i * rect1.Height(), rect.Width(), rect1.Height());
	//		cont->MoveWindow(5, 80 + (i % 8) * rect1.Height(), rect.Width()-7, rect1.Height());

	//		//设置数据

	//		sprintf(buf, "%d", i);
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString(buf));
	//		data2.push_back(cont);
	//		cont->setChecked();
	//		cont->ShowWindow(SW_SHOW);
	//	}
	//	curindex2 = 0;//当前的开始位置0
	//	curindexout = 0;//当前的开始位置0
	//	testi2 = 2;

	//}


	
	m_bInitFinish = true;
	return  TRUE;
}

void  CDeviceManage::setInitFinish(bool binit)
{
	m_bInitFinish = binit;
}


void CDeviceManage::initDialogData()
{
	devParam* pDev = NULL;
	BOOL b = -1;
	MYcontrol2* cont = NULL;
	MyControldata* myctroldata = NULL;
	bool bInput = true;

	for (int i = 0; i < GetDevCount(bInput); i++)
	{
		cont = data[i];
		//cont = new MYcontrol2(bInput, i);

		//b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
		//itemdatawnd = cont->m_hWnd;
		////CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
		//CRect rect, rect1;
		//GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
		//cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		////IDC_mycontrolbits
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//cont->MoveWindow(5, 80 + i*rect1.Height(), rect.Width() - 6, rect1.Height());

		pDev = GetDevParam(i, bInput);
		//cont->setInnerChsName(pDev->sName);
		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowText(pDev->sName);
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
			cont->SetDriverSelected(0);
		else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
			cont->SetDriverSelected(1);
		else
			cont->SetDriverSelected(-1);
		cont->OnCbnSelchangemycontroldrivername();
		cont->SetDeviceSelected(pDev->sDevice);
		cont->SetChannelSelected(pDev->nChannel - 1);
		cont->setVpVal(pDev->nVp);
		cont->SetSamplingRateVal(pDev->nSamplingRate);
		cont->SetBitDepthVal(pDev->nBitDepth);
		cont->SetSensitivityOrGainVal(pDev->nSensitivity);

		myctroldata = new MyControldata(CString(pDev->sName), CString(pDev->sDriver),
			CString(pDev->sDevice));
		inlist.push_back(myctroldata);
		//data.push_back(cont);
		cont->setChecked();
		cont->ShowWindow(SW_SHOW);
	}

	curindex = 0;//当前的开始位置0
	testi = GetDevCount(bInput);

	//if (data.size() == 0)
	//{
	//	for (int i = 0; i < 3; i++)
	//	{
	//		cont = new MYcontrol2(true);
	//		
	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//		itemdatawnd = cont->m_hWnd;
	//		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		//IDC_mycontrolbits
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		cont->MoveWindow(5, 80 + i*rect1.Height(), rect.Width()-6, rect1.Height());
	//		char buf[1024];
	//		sprintf(buf, "%d", i);
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString(buf));

	//		myctroldata = new MyControldata(CString(buf), CString(""), CString(""));
	//		inlist.push_back(myctroldata);
	//		data.push_back(cont);
	//		cont->setChecked();
	//		cont->ShowWindow(SW_SHOW);
	//	}
	//	curindex = 0;//当前的开始位置0
	//	testi = 3;
	//}
	//else
	//{
	//	CRect rect, rect1;
	//	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//	for (int i = 0; i < data.size(); i++)
	//	{
	//		delete data[i];
	//	}
	//	int lastcount = data.size();
	//	data.clear();
	//	//for (size_t i = 0; i < lastcount; i++)
	//	//{
	//	//	cont = new MYcontrol2(true);

	//	//	b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//	//	CRect rect, rect1;
	//	//	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//	//	cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//	//	cont->MoveWindow(5, 80 + i*rect1.Height(), rect.Width(), rect1.Height());

	//	//	//设置数据
	//	//	myctroldata = new MyControldata(CString("0"), CString(""), CString(""));
	//	//	inlist.push_back(myctroldata);
	//	//	data.push_back(cont);
	//	//	cont->ShowWindow(SW_SHOW);
	//	//}

	//	/*for (int i = 0; i < inlist.size(); i++)
	//	{
	//		delete inlist[i];
	//	}*/

	//	int deli = 0;
	//	for (list<MyControldata*>::iterator it = inlist.begin(); it != inlist.end(); it = inlist.begin())
	//	{
	//		if (lastcount == deli)
	//			break;

	//		cont = new MYcontrol2(true);

	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		cont->MoveWindow(5, 80 + deli*rect1.Height(), rect.Width()-6, rect1.Height());

	//		//设置数据
	//		CString s1, s2, s3;
	//		(*it)->getdata(s1, s2, s2);
	//		(*it)->setdata(s1, s2, s3);
	//		myctroldata = new MyControldata(CString(s1), CString(s2), CString(s3));
	//		inlist.push_back(myctroldata);
	//		
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(s1);
	//		cont->GetDlgItem(IDC_mycontroldrivername)->SetWindowTextW(s2);
	//		cont->GetDlgItem(IDC_mycontroldevicename2)->SetWindowTextW(s3);

	//		data.push_back(cont);
	//		cont->ShowWindow(SW_SHOW);

	//		delete *it;
	//		deli++;
	//		inlist.pop_front();
	//	}


	//}

	/************************输出通道创建*******************************/
	bInput = false;
	for (int i = 0; i < GetDevCount(bInput); i++)
	{
		cont = data2[i];
		//cont = new MYcontrol2(bInput, i);
		//b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
		////CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
		//CRect rect, rect1;
		//GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
		//cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		//(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
		////cont->MoveWindow(110, 280 + 8 * rect1.Height() + i * rect1.Height(), rect.Width(), rect1.Height());
		//cont->MoveWindow(5, 80 + (i % 8) * rect1.Height(), rect.Width() - 7, rect1.Height());

		pDev = GetDevParam(i, bInput);
		//cont->setInnerChsName(pDev->sName);
		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowText(pDev->sName);
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
			cont->SetDriverSelected(0);
		else if (StrCmpI(pDev->sDriver, _T("ASIO")) == 0)
			cont->SetDriverSelected(1);
		else
			cont->SetDriverSelected(-1);
		cont->OnCbnSelchangemycontroldrivername();
		cont->SetDeviceSelected(pDev->sDevice);
		cont->SetChannelSelected(pDev->nChannel - 1);
		cont->setVpVal(pDev->nVp);
		cont->SetSamplingRateVal(pDev->nSamplingRate);
		cont->SetBitDepthVal(pDev->nBitDepth);
		cont->SetSensitivityOrGainVal(pDev->nGain);

		//data2.push_back(cont);
		cont->setChecked();
		cont->ShowWindow(SW_SHOW);
	}
	curindex2 = 0;//当前的开始位置0
	curindexout = 0;//当前的开始位置0
	testi2 = GetDevCount(bInput);

	//if (data2.size() == 0)
	//{
	//	char buf[1024];
	//	for (int i = 0; i < 2; i++)
	//	{
	//		cont = new MYcontrol2(false);
	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
	//		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		//cont->MoveWindow(110, 280 + 8 * rect1.Height() + i * rect1.Height(), rect.Width(), rect1.Height());
	//		cont->MoveWindow(5, 80 + (i % 8) * rect1.Height(), rect.Width()-7, rect1.Height());

	//		
	//		sprintf(buf, "%d", i);
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString(buf));
	//		data2.push_back(cont);
	//		cont->setChecked();
	//		cont->ShowWindow(SW_SHOW);
	//	}
	//	curindex2 = 0;//当前的开始位置0
	//	curindexout = 0;//当前的开始位置0
	//	testi2 = 2;
	//}
	//else
	//{
	//	CRect rect, rect1;
	//	GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	//	for (int i = curindexout; i < data2.size(); i++)
	//	{
	//		delete data2[i];
	//	}
	//	int lastcount = data2.size();
	//	data2.clear();
	//	/************二次创建********************/
	//	char buf[1024];
	//	for (int i = 0; i < lastcount; i++)
	//	{
	//		cont = new MYcontrol2(false);
	//		b = cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
	//		//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	//		CRect rect, rect1;
	//		GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	//		cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	//		//cont->MoveWindow(110, 280 + 8 * rect1.Height() + i * rect1.Height(), rect.Width(), rect1.Height());
	//		cont->MoveWindow(5, 80 + (i % 8) * rect1.Height(), rect.Width()-7, rect1.Height());

	//		//设置数据

	//		sprintf(buf, "%d", i);
	//		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString(buf));
	//		data2.push_back(cont);
	//		cont->setChecked();
	//		cont->ShowWindow(SW_SHOW);
	//	}
	//	curindex2 = 0;//当前的开始位置0
	//	curindexout = 0;//当前的开始位置0
	//	testi2 = 2;

	//}



	m_bInitFinish = true;
}



BEGIN_MESSAGE_MAP(CDeviceManage, CDialogEx)
	
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON1, &CDeviceManage::OnBnClickedButtonaddInCh)
	ON_BN_CLICKED(IDC_BUTTON2, &CDeviceManage::OnBnClickedButtonDelInCh)
	ON_BN_CLICKED(IDC_BUTTON3, &CDeviceManage::OnBnClickedButtonaddOutChs)
	ON_BN_CLICKED(IDC_BUTTON4, &CDeviceManage::OnBnClickedButtonDelOutChs)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_devicemangeplay, &CDeviceManage::OnBnClickeddevicemangeplay)
	ON_BN_CLICKED(IDC_devicemangerecord, &CDeviceManage::OnBnClickeddevicemangerecord)
	ON_BN_CLICKED(IDC_devicesaveout, &CDeviceManage::OnBnClickeddevicesaveout)
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_PAINT()

	ON_COMMAND(ID_32780, &CDeviceManage::openASIOPanle)
	

	ON_BN_CLICKED(IDC_BUTTON5, &CDeviceManage::OnBnClickedButton5)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDeviceManage 消息处理程序



void CDeviceManage::OnRButtonDown(UINT nFlags, CPoint point)
{
	
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*CMenu menu;
	menu.LoadMenuW(IDR_MYCONTROL);
	ClientToScreen(&point);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);


	GetParent();
	CDialogEx::OnRButtonDown(nFlags, point);*/
	
}


void CDeviceManage::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CDeviceManage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	int oldindex = curindex;
	int oldindexout = curindexout;;
	int inOrOut = -1;//判定当前用户的鼠标滚动区域
		
	int tempindex=0;
	CRect rect, rect1, rectscreen;
	CRect rectout, rect1out, rectoutscreen;
	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rectscreen);
	
	//ClientToScreen(&rect);
	if (pt.y >= rectscreen.top && pt.y <= rectscreen.bottom)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			data[i]->ShowWindow(SW_HIDE);
		}
		inOrOut = 0;
	}
	
	GetDlgItem(IDC_STATIC2)->GetClientRect(&rectout);
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&rectoutscreen);
	//ClientToScreen(&rectout);
	if (pt.y >= rectoutscreen.top && pt.y <= rectoutscreen.bottom)
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			data2[i]->ShowWindow(SW_HIDE);
		}
		inOrOut = 1;
		
	}
	char buf[512];
	switch (zDelta)
	{

	case -120:
		if (inOrOut==0)
		{

			if (data.size() > (tempindex = curindex + 8))
				curindex += 8;
			//显示下一页

			for (size_t i = curindex; i < curindex + 8; i++)
			{
				if (i < data.size())
				{
					data[i]->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
					data[i]->MoveWindow(5, 80 + (i % 8)*rect1.Height(), rect.Width(), rect1.Height());
					data[i]->setChecked();
					data[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					break;
				}
			}

			if (curindex < 8)
			{
				GetDlgItem(IDC_devicemanagecurpage)->SetWindowTextW(CString("1"));
			}
			else
			{
				sprintf(buf, "%d", curindex / 8 + 1);
				GetDlgItem(IDC_devicemanagecurpage)->SetWindowTextW(CString(buf));

			}
		}
		else if(inOrOut == 1)
		{
			if (data2.size() > (tempindex = curindexout + 8))
				curindexout += 8;
			//输出通道显示下一页

			for (size_t i = curindexout; i < curindexout + 8; i++)
			{
				if (i < data2.size())
				{
					data2[i]->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1out);
					//110, 280 + 8 * rect1.Height() + i * rect1.Height()
					data2[i]->MoveWindow(5, 80+(i % 8) * rect1out.Height(), rectout.Width(), rect1out.Height());
					data2[i]->setChecked();
					data2[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					break;
				}
			}

			if (curindexout < 8)
			{
				GetDlgItem(IDC_devicemanagecurpage2)->SetWindowTextW(CString("1"));
			}
			else
			{
				sprintf(buf, "%d", curindexout / 8 + 1);
				GetDlgItem(IDC_devicemanagecurpage2)->SetWindowTextW(CString(buf));

			}
		}
		break;
	case 120:
		if (inOrOut==0)
		{

			tempindex = curindex;
			if ((tempindex -= 8) >= 0)
				curindex -= 8;
			//显示上一页
			for (size_t i = curindex; i < curindex + 8; i++)
			{
				if (i < data.size())
				{
					data[i]->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
					data[i]->MoveWindow(5, 80+(i % 8)*rect1.Height(), rect.Width(), rect1.Height());
					data[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					break;
				}
			}
			if (curindex < 8)
			{
				GetDlgItem(IDC_devicemanagecurpage)->SetWindowTextW(CString("1"));
			}
			else
			{
				sprintf(buf, "%d", curindex / 8 + 1);
				GetDlgItem(IDC_devicemanagecurpage)->SetWindowTextW(CString(buf));

			}
		}
		else if (inOrOut == 1)
		{
			tempindex = curindexout;
			if ((tempindex -= 8) >= 0)
				curindexout -= 8;
			//输出通道显示上一页
			for (size_t i = curindexout; i < curindexout + 8; i++)
			{
				if (i < data2.size())
				{
					data2[i]->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1out);
					data2[i]->MoveWindow(5, 80 + (i%8) * rect1out.Height(), rectout.Width(), rect1out.Height());
					data2[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					break;
				}
			}
			if (curindexout < 8)
			{
				GetDlgItem(IDC_devicemanagecurpage2)->SetWindowTextW(CString("1"));
			}
			else
			{
				sprintf(buf, "%d", curindexout / 8 + 1);
				GetDlgItem(IDC_devicemanagecurpage2)->SetWindowTextW(CString(buf));

			}

		}
		break;
	default:
		break;
	}
	if (oldindex != curindex)
	{
		rect.bottom += 45;
		InvalidateRect(rect);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	}

	if (oldindexout != curindexout)
	{
		//rectou.bottom += 45;

		rectoutscreen.bottom += 100;
		rectoutscreen.left -= 300;
		InvalidateRect(rectoutscreen);
		
		//GetDlgItem(IDC_STATIC2)->InvalidateRect(rectout);
		//Invalidate();
		
	}
		
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDeviceManage::OnBnClickedButtonaddInCh()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bInitFinish = false;
	CString sName(_T(""));
	MYcontrol2*cont = NULL;
	cont = new MYcontrol2(true, testi);
	cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC));
	//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	CRect rect, rect1;
	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	
	sName.Format(_T("Input%d"), 1 + testi++);
	cont->GetDlgItem(IDC_mycontrolchname)->SetWindowText(sName);

	if (data.size() < 8)
	{
		cont->MoveWindow(5, 80 + data.size()*rect1.Height(), rect.Width()-6, rect1.Height());
	}
	else
	{
		if (data.size() - curindex <= 8)
		{
			cont->MoveWindow(5, 80 + (data.size() - curindex) * rect1.Height(), rect.Width()-6, rect1.Height());
		}
		else
		{
			//加到数据尾部，不直接显示,需要用户翻页
		}
	}
	

	data.push_back(cont);
	cont->updatePlayAndRecordCmb();
	AddChannelDev(sName, true);

	//如果不够显示需要翻动一条数据
	if (data.size() <= 8)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			data[i]->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (data.size() - curindex < 8)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				data[i]->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			
		}
	}
	
	sName = _T("");
	if (data.size() % 8 == 0)
		sName.Format(_T("%d"), data.size() / 8);
	else
		sName.Format(_T("%d"), data.size() / 8 + 1);
	GetDlgItem(IDC_devicemanagetotalpage)->SetWindowText(sName);
	m_bInitFinish = true;
}


void CDeviceManage::OnBnClickedButtonDelInCh()
{
	CRect rect, rect1;
	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	rect.bottom += 45;
	InvalidateRect(rect);
	
	char buf[512];
	// TODO:  在此添加控件通知处理程序代码
	
	std::vector<MYcontrol2*>::iterator it;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->isCheck())
		{
			data[i]->ShowWindow(SW_HIDE);
			delete data[i];
			it=data.erase(data.begin() + i);
			break;
		}



	}

	if (data.size() == 1)
	{
		
		cont = *it;
		
		
	}
	else if (data.size() == 0 && cont!=NULL)
	{
		testi = 0;
		/*cont->ShowWindow(SW_HIDE);
		CString s;
		cont->GetDlgItem(IDC_mycontrolchname)->GetWindowTextW(s);
		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString("2222"));
		delete cont;
		cont = NULL;*/
	}

	for (size_t i = curindex; i < curindex + 8; i++)
	{
		if (i < data.size())
		{
			data[i]->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
			data[i]->MoveWindow(5, 80 + (i % 8)*rect1.Height(), rect.Width()-6, rect1.Height());
			data[i]->setChecked();
			data[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			break;
		}
	}
	
	if (data.size() % 8 == 0)
		sprintf(buf, "%d", data.size() / 8);
	else
		sprintf(buf, "%d", data.size() / 8 + 1);
	GetDlgItem(IDC_devicemanagetotalpage)->SetWindowTextW(CString(buf));

	for (size_t i = 0; i < data.size(); i++)
	{
		data[i]->updatePlayAndRecordCmb();
		break;
	}
	
}

void CDeviceManage::play(CString innerDeviceName,double v)
{

	if (data2.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输出通道未配置"));
		return;
	}

	//把播放的准备数据集中放到这个类管理
	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getDriverType() == "ASIO")
		{
			if (data2[i]->getInnerName() == innerDeviceName)
			{
				data2[i]->playASIO(data2[i]->getFs(),v);
			}
		}
		else if(data2[i]->getDriverType() == "WDM")
		{
			
			if (data2[i]->getInnerName() == innerDeviceName)
			{
				//data2[i]->playWDM(1000, "D:\\TestRecord\\playjili.wav",data2[i]->getChsName_Show());
				data2[i]->cal_vpByMME(data2[i]->getFs(), data2[i]->getVpValues());
			}
		}

	}

	

}

bool CDeviceManage::calbrationplay(CString innerDeviceName, double v)
{
	if (data2.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输出通道未配置"));
		return false;
	}

	//把播放的准备数据集中放到这个类管理
	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getDriverType() == "ASIO")
		{
			//产生v电压大小的信号
			if (data2[i]->getInnerName() == innerDeviceName)
			{
				//data2[i]->generateV(v, data2[i]->getFs());
				data2[i]->playASIO(data2[i]->getFs(), v);

				break;
			}
		}
		else if (data2[i]->getDriverType() == "WDM")
		{
			//产生v电压大小的信号
			if (data2[i]->getInnerName() == innerDeviceName)
			{
				//data2[i]->generateV(v, data2[i]->getFs());
				//data2[i]->playWDM(1000, "D:\\TestRecord\\playjili.wav", data2[i]->getChsName_Show());
				
				if(data2[i]->cal_vpByMME(data2[i]->getFs()))
				      Sleep(10 * 1000);
				else
				{
					return false;
				}

				break;

			}
		}

	}

	return true;
}

void CDeviceManage::setASIOStop(CString innername, bool isInput)
{
	if (isInput)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i]->getInnerName() == innername)
			{
				data[i]->setASIOStop(true);
				break;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			if (data2[i]->getInnerName() == innername)
			{
				data2[i]->setASIOStop(true);
				break;
			}
		}
	}
}

void CDeviceManage::play(int innerDeviceName, CString jili, bool isasio, double fs, int asioIndex, char* playWdmName, CString playChsName)
{
	if (data2.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输出通道未配置"));
		return;
	}

	//把播放的准备数据集中放到这个类管理
	
		if (isasio)
		{
			
			
			data2[innerDeviceName]->playASIO2(fs, asioIndex);
			
		}
		else 
		{

			
			data2[innerDeviceName]->playWDM(0, playWdmName, playChsName);
		}

	
}

void CDeviceManage::record2(int innerDeviceName, CString path, int***indata, int asioIndex, bool isasio, double fs, double recordtime)
{
	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输入通道未配置"));
		return;
	}

	int* recordchs = new int[1]{0};

	//for (size_t i = 0; i < data.size(); i++)
	{
		//if (data[i]->getInnerName() == innerDeviceName)
		{
			/*if (isasio)
			{
				data[innerDeviceName]->setMainf(mainf);
				data[innerDeviceName]->micCalibrationRecordASIO2(fs, indata, recordtime, asioIndex);

			}
			else*/
			{
				//if (data[i]->getInnerName() == innerDeviceName)
				{
					data[innerDeviceName]->recordWDM(recordtime, "");

					while (!data[innerDeviceName]->wdmisdone())
						;
					//AfxMessageBox(_T("wdm录音完成！"));
				}


			}
		}
	}



}

void CDeviceManage::record(CString innerDeviceName, CString path, int***indata)
{
	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输入通道未配置"));
		return;
	}
	int* recordchs = new int[1]{0};
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == innerDeviceName)
		{
			if (data[i]->getDriverType() == "ASIO")
			{
				


				recordchs[0] = data[i]->getASIOchIndex();
				data[i]->setMainf(mainf);
				data[i]->micCalibrationRecordASIO(data[i]->getFs(), indata, -1);

				/*delete[] playchs;
				delete[] recordchs;*/
			}
			else if (data[i]->getDriverType() == "WDM")
			{
				if (data[i]->getInnerName() == innerDeviceName)
				{
					data[i]->recordWDM(5000, "");

					while (!data[i]->wdmisdone())
						;
					AfxMessageBox(_T("wdm录音完成！"));
				}


			}

		}
	}

	delete[] recordchs;

}

int CDeviceManage::getASIOindex(CString innerDeviceName, bool isInput, int* dataindex,
	bool& isasio, double& fs, CString& wdmDeviceName, int& chs, int& bits, int& mmeindex)
{
	int index=-1;
	if (isInput)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i]->getInnerName() == innerDeviceName)
			{
				index = data[i]->getASIOchIndex();
				*dataindex = i;

				if (data[i]->getDriverType() == "ASIO")
				{
					isasio = true;
				}
				else if (data[i]->getDriverType() == "WDM")
				{
					isasio = false;
				}
				fs = data[i]->getFs();
				wdmDeviceName = data[i]->getChsName_Show();
				chs = data[i]->getMMEchs();
				bits = data[i]->getMMEbits();
				mmeindex=data[i]->getchsIndex();
				break;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			if (data2[i]->getInnerName() == innerDeviceName)
			{
				index = data2[i]->getASIOchIndex();
				*dataindex = i;
				if (data2[i]->getDriverType() == "ASIO")
				{
					isasio = true;
				}
				else if (data2[i]->getDriverType() == "WDM")
				{
					isasio = false;
				}
				fs=data2[i]->getFs();
				wdmDeviceName = data2[i]->getChsName_Show();
				chs = data[i]->getMMEchs();
				bits = data[i]->getMMEbits();
				mmeindex = data[i]->getchsIndex();
				break;
			}
		}
	}

	return index;
}

//indata 是录音返回数据
void CDeviceManage::record(CString innerDeviceName, CString innerDeviceNameplay, int***indata, int fs, double& recordtime)
{
	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输入通道未配置"));
		return;
	}
	int* playchs = new int[1]{0};
	int* recordchs = new int[1]{0};

	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getInnerName() == innerDeviceNameplay)
		{
			playchs[0] = data2[i]->getASIOchIndex();
		}
	}
	

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == innerDeviceName)
		{
			if (data[i]->getDriverType() == "ASIO")
			{
				//data[i]->micCalibrationRecordASIO(data[i]->getFs(), indata, recordtime);
				
				
				recordchs[0] = data[i]->getASIOchIndex();
				data[i]->setMainf(mainf);
				data[i]->recordASIO(data[i]->getFs(), indata, recordtime, playchs, recordchs);

				/*delete[] playchs;
				delete[] recordchs;*/
			}
			else if (data[i]->getDriverType() == "WDM")
			{
				if (data[i]->getInnerName() == innerDeviceName)
				{
					data[i]->recordWDM(5000, "");

					while (!data[i]->wdmisdone())
						;
					AfxMessageBox(_T("wdm录音完成！"));
				}
				
				
			}

		}
	}

	delete[] playchs;
	delete[] recordchs;

}

void CDeviceManage::calSpkASIO(CString outname, int*** recorddata)
{

	for (size_t i = 0; i < data2.size(); i++)
	{

		if (data2[i]->getInnerName() == outname)
		{
			data2[i]->setIsNoPlay(false);
			data2[i]->calSpkASIO(recorddata, data2[i]->getASIOchIndex(), data2[i]->getFs());
			
		}
	}

}

void CDeviceManage::record_new(int innerDeviceName, int innerDeviceNameplay, int***indata, int fs, double& recordtime, int asioindex_in, int asioindex_out)
{
	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输入通道未配置"));
		return;
	}
	int* playchs = new int[1]{0};
	int* recordchs = new int[1]{0};

	
	playchs[0] = asioindex_out;
	recordchs[0] = asioindex_in;
	data[innerDeviceName]->setMainf(mainf);
	data[innerDeviceName]->recordASIO(fs, indata, recordtime, playchs, recordchs);

				
			

		
	

	delete[] playchs;
	delete[] recordchs;

}

//wdm驱动的播放与录音，用于测试fr
void CDeviceManage::wdmPlayWithRecord(CString innerDeviceName_in, CString innerDeviceName_out, double playTimes, char* playFileName)
{

	int index_in = -1;
	int index_out = -1;
	double fs = 0;
	CString chsName_in, chsName_out;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == innerDeviceName_in)
		{
			/*data[i]->setwdmName_in(innerDeviceName_in);*/
			chsName_in = data[i]->getChsName_Show();
			index_in =  data[i]->getchsIndex();
			fs=data[i]->getFs();
			break;
		}
	}
	
	for (size_t i = 0; i < data2.size(); i++)
	{
		
		if (data2[i]->getInnerName() == innerDeviceName_out)
		{

			index_out =  data2[i]->getchsIndex();
			chsName_out = data2[i]->getChsName_Show();
			data2[i]->setwdmindex_out(chsName_out);
			data2[i]->setMainf(mainf);
			data2[i]->wdmplayWithRecord(playTimes, playFileName, index_out, index_in, chsName_in, fs);//"play.wav"
			
			while (!data2[i]->wdmisdone())
				;
			//AfxMessageBox(_T("wdm录音完成！"));
			mainf->setStatusStr(_T("wdm录音完成！"));
			break;
		}
		
	}
	

}

void CDeviceManage::wdmPlayWithRecord_new(int innerDeviceName_in, int innerDeviceName_out, double playTimes, char* playFileName, CString playName, CString recordName, double fs)
{

	int index_in = -1;
	int index_out = -1;
	CString chsName_in, chsName_out;
	//for (size_t i = 0; i < data.size(); i++)
	//{
		//if (data[i]->getInnerName() == innerDeviceName_in)
		//{
			/*data[i]->setwdmName_in(innerDeviceName_in);*/
			chsName_in = recordName;// data[innerDeviceName_in]->getChsName_Show();
			//index_in = data[innerDeviceName_in]->getchsIndex();
			//fs = data[innerDeviceName_in]->getFs();
			//break;
		//}
	//}

	//for (size_t i = 0; i < data2.size(); i++)
	//{

		//if (data2[i]->getInnerName() == innerDeviceName_out)
		//{

			//index_out = data2[i]->getchsIndex();
			chsName_out = playName;// data2[i]->getChsName_Show();
			data2[innerDeviceName_out]->setwdmindex_out(chsName_out);
			data2[innerDeviceName_out]->setMainf(mainf);
			data2[innerDeviceName_out]->wdmplayWithRecord(playTimes, playFileName, index_out, index_in, chsName_in, fs);//"play.wav"

			while (!data2[innerDeviceName_out]->wdmisdone())
				;
			//AfxMessageBox(_T("wdm录音完成！"));
			//mainf->setStatusStr(_T("wdm录音完成！"));
			//break;
		//}

	//}


}

void CDeviceManage::calSpkMME(CString recordChs, int* indata, int playChs, double** outdata, double recordtime)
{
	int recordchs = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == recordChs)
		{
			recordchs = i;
		}
	}



	//录音

	int ndev=waveInGetNumDevs();

	data[recordchs]->createMMEbuffer(ndev);
	data[recordchs]->recordMME(recordtime, NULL, data[recordchs]->getMMEchs(), data[recordchs]->getMMEbits(), data[recordchs]->getFs(), data[recordchs]->getchsIndex());
	data[recordchs]->startMM();
	//播放
	data[recordchs]->playMME(indata, playChs);

	Sleep(10 * 1000);
	data[recordchs]->saveMMEFile(recordtime);

	wav_readBymatlab(outdata, data[recordchs]->getFs(), recordtime, "");
}

//当该函数内调用函数（data[i]->recordASIO(fs, indata, recordtime, playchs, recordchs);）
//实际的录音时间（recordtime）有下层函数内部通过激励信号计算得出
//因此上层调用函数可不用传递

//若该函数内调用函数(data[i]->micCalibrationRecordASIO(fs, indata, recordtime);//麦克风录音 纯录音 ，声卡不会播放)
//则需要外部调用函数传递具体的录音时间
void CDeviceManage::calibrationMic(CString innerDeviceName, int***indata, int fs, double recordtime, double**indata2, int   outchs, int inchs, double outvp)
{
	//AfxMessageBox(_T("请确保麦克风已接入输入1通道！"));

	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输入通道未配置"));
		return;
	}




	for (size_t i = 0; i < data.size(); i++)
	{
		
		if (data[i]->getInnerName() == innerDeviceName)
		{
			if (data[i]->getDriverType() == "ASIO")
			{
				int* playchs = new int[outchs+1]{0};
				for (size_t t = 0; t < outchs + 1; t++)
				{
					playchs[t] =t ;
				}
				
				
				int* recordchs = new int[1]{0};
				recordchs[0] = inchs;

				fs=data[i]->getFs();


				data[i]->setCalVp(outvp);


				data[i]->recordASIO(fs, indata, recordtime, playchs, recordchs);//录音同时播放,自发自采
				//data[i]->micCalibrationRecordASIO(fs, indata, recordtime);//麦克风录音 纯录音 ，声卡不会播放

				 delete[] playchs ;
				 delete[] recordchs;
				
			}
			else if (data[i]->getDriverType() == "WDM")
			{


				CString chsname = data[i]->getChsName_Show();
				data[i]->setwdmName_in(chsname);
				//data[i]->recordWDM(recordtime,""); 只录音没有播放
				data[i]->setwdmindex_out(data2[0]->getChsName_Show());
				//data[i]->wdmplayWithRecord(0, "D:\\TestRecord\\playjili.wav", 0, 0, chsname, fs);//录音时间由播放文件时长计算出
				UINT ndev = waveInGetNumDevs();

				data[i]->setCalVp(outvp);
				data[i]->createMMEbuffer(ndev);
				data[i]->mmeplayWithRecord(outchs);
				
				/*while (!data[i]->wdmisdone())
					;*/

				Sleep(10 * 1000);
				
				data[i]->saveMMEFile(recordtime);
				data[i]->delMMEbuffer();
				//AfxMessageBox(_T("wdm录音完成！"));


				//使用wdm驱动时，给indata赋值
				double fs = 44100;
				double times = recordtime;
				double * wavdata = new double[(int)(fs*times)];
				wav_readBymatlab(&wavdata, fs ,times,"");

				//读取约定文件，给indata赋值
				wave_t* wav = (wave_t*)malloc(sizeof(wave_t));
				wavread("D:\\TestRecord\\wdmRecord1.wav", wav);
				if (wav->fp!=NULL)
				    fclose(wav->fp);
				int** tempre = new int*[1]{};
				tempre[0] = new int[wav->length]{0};
				/*std::ofstream outfile3;
				CString path3("D:\\TestRecord\\wdmdata.txt");
				
				outfile3.open(path3, std::ios::out | std::ios::trunc);*/
				//16 位 立体声
				int tempint = 0;
				float temavach=0;
				for (size_t i = 0; i < 88200; i++)
				{
					/*tempint = wav->pData[i];

					tempre[0][i] = (tempint & 0xffff0000)>>16;
					temavach = (tempint & 0xffff0000)>>16;*/
					//outfile3 << wavdata[i] << endl;
					

				}
				//outfile3.close();
				*indata = tempre;
				*indata2 = wavdata;
				free(wav);

			}


			break;
		}
		
		
		
	}
	
	

}



int CDeviceManage::getChsIndexByName(CString chsName, bool isInput)
{
	int index = 0;

	if (isInput)
	{

		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i]->getInnerName() == chsName)
			{
				index=data[i]->getASIOchIndex();
				break;
			}

		}

	}
	else
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			if (data2[i]->getInnerName() == chsName)
			{
				index = data2[i]->getASIOchIndex();
				break;
			}
		}
	}


	return index;
}

int CDeviceManage::getMMEChsIndexByName(CString chsName, bool isInput)
{
	int index = 0;

	if (isInput)
	{

		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i]->getInnerName() == chsName)
			{
				index = data[i]->getMMEdevID();
				break;
			}

		}

	}
	else
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			if (data2[i]->getInnerName() == chsName)
			{
				index = data2[i]->getMMEdevID();
				break;
			}
		}
	}


	return index;

}

void CDeviceManage::calibrationMic_NotPlay(CString innerDeviceName, int***indata, int fs, double recordtime, double**indata2)
{
	//AfxMessageBox(_T("请确保麦克风已接入输入1通道！"));

	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块，输入通道未配置"));
		return;
	}




	for (size_t i = 0; i < data.size(); i++)
	{

		if (data[i]->getInnerName() == innerDeviceName)
		{
			if (data[i]->getDriverType() == "ASIO")
			{
				/*int* playchs = new int[data2[0]->getASIOchIndex() + 1]{0};
				for (size_t t = 0; t < data2[0]->getASIOchIndex() + 1; t++)
				{
					playchs[t] = t;
				}*/

				globle_out = 0;//这里设置asio播放通道数为0
				int* recordchs = new int[1]{0};
				recordchs[0] = data[i]->getASIOchIndex();

				//data[i]->recordASIO(fs, indata, recordtime, playchs, recordchs);//录音同时播放,自发自采
				data[i]->micCalibrationRecordASIO(fs, indata, recordtime);//麦克风录音 纯录音 ，声卡不会播放

				//delete[] playchs;
				delete[] recordchs;

			}
			else if (data[i]->getDriverType() == "WDM")
			{


				CString chsname = data[i]->getChsName_Show();
				data[i]->setwdmName_in(chsname);
				//data[i]->recordWDM(recordtime,""); //只录音没有播放
				//data[i]->setwdmindex_out(data2[0]->getChsName_Show());
				//data[i]->wdmplayWithRecord(0, "D:\\TestRecord\\playjili.wav", 0, 0, chsname, fs);//录音时间由播放文件时长计算出
				/*while (!data[i]->wdmisdone())
					;*/

				//AfxMessageBox(_T("wdm录音完成！"));
				UINT ndev = waveInGetNumDevs();

				data[i]->createMMEbuffer(ndev);
				data[i]->mmeplayWithRecord(-1);

				Sleep(10 * 1000);

				data[i]->saveMMEFile(3);
				data[i]->delMMEbuffer();
				//使用wdm驱动时，给indata赋值
				//double fs = 44100;
				double times = 5;
				double * wavdata = new double[(int)(fs*times)];
				wav_readBymatlab(&wavdata, fs, times,"");

				//读取约定文件，给indata赋值
				wave_t* wav = (wave_t*)malloc(sizeof(wave_t));
				wavread("D:\\TestRecord\\wdmRecord1.wav", wav);
				fclose(wav->fp);
				int** tempre = new int*[1]{};
				tempre[0] = new int[wav->length]{0};
				/*std::ofstream outfile3;
				CString path3("D:\\TestRecord\\wdmdata.txt");

				outfile3.open(path3, std::ios::out | std::ios::trunc);*/
				//16 位 立体声
				int tempint = 0;
				float temavach = 0;
				for (size_t i = 0; i < 88200; i++)
				{
					/*tempint = wav->pData[i];

					tempre[0][i] = (tempint & 0xffff0000)>>16;
					temavach = (tempint & 0xffff0000)>>16;*/
					//outfile3 << wavdata[i] << endl;


				}
				//outfile3.close();
				*indata = tempre;
				*indata2 = wavdata;
				free(wav);

			}
		}



	}



}
void CDeviceManage::OnBnClickedButtonaddOutChs()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bInitFinish = false;
	CString sName(_T(""));
	MYcontrol2*cont = NULL;
	cont = new MYcontrol2(false, testi2);
	cont->Create(MYcontrol2::IDD, GetDlgItem(IDC_STATIC2));
	//CListBox* ls = (CListBox*)GetDlgItem(IDC_LIST2);
	CRect rect, rect1;
	GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	cont->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontroldrivername)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolfs)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontroldeviceName)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolchnames)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontroldevicename2)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolbits)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);
	(CComboBox*)cont->GetDlgItem(IDC_mycontrolsenOrzengyi)->SendMessage(CB_SETITEMHEIGHT, -1, rect1.Height() - 1);

	sName.Format(_T("Output%d"), 1 + testi2++);
	//cont->setInnerChsName(sName);
	cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(sName);
	if (data2.size() < 8)
	{
		cont->MoveWindow(5, 80 + data2.size()*rect1.Height(), rect.Width()-7, rect1.Height());

	}
	else
	{
		if (data2.size() - curindexout <= 8)
		{
			cont->MoveWindow(5, 80 + (data2.size() - curindexout) * rect1.Height(), rect.Width()-7, rect1.Height());
		}
		else
		{
			//加到数据尾部，不直接显示,需要用户翻页
		}
	}

	data2.push_back(cont);
	cont->updatePlayAndRecordCmb();
	AddChannelDev(sName, false);

	//如果不够显示需要翻动一条数据
	if (data2.size() <= 8)
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			data2[i]->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (data2.size() - curindexout < 8)
		{
			for (size_t i = 0; i < data2.size(); i++)
			{
				data2[i]->ShowWindow(SW_SHOW);
			}
		}
		else
		{

		}



	}
	
	if (data2.size() % 8 == 0)
		sName.Format(_T("%d"), data2.size() / 8);
	else
		sName.Format(_T("%d"), data2.size() / 8 + 1);
	GetDlgItem(IDC_devicemanagetotalpage2)->SetWindowText(sName);
	m_bInitFinish = true;
}


void CDeviceManage::OnBnClickedButtonDelOutChs()
{
	// TODO:  在此添加控件通知处理程序代码
	CRect rect, rect1, rectscreen;
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&rectscreen);
	GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	rect.bottom += 45;
	rectscreen.bottom += 100;
	rectscreen.left -= 300;
	rectscreen.top -= 100;
	InvalidateRect(rectscreen);

	char buf[512];
	// TODO:  在此添加控件通知处理程序代码

	std::vector<MYcontrol2*>::iterator it;
	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->isCheck())
		{
			data2[i]->ShowWindow(SW_HIDE);
			delete data2[i];
			it = data2.erase(data2.begin() + i);
			break;
		}



	}

	if (data2.size() == 1)
	{

		cont = *it;


	}
	else if (data2.size() == 0 && cont != NULL)
	{
		testi2 = 0;
		/*cont->ShowWindow(SW_HIDE);
		CString s;
		cont->GetDlgItem(IDC_mycontrolchname)->GetWindowTextW(s);
		cont->GetDlgItem(IDC_mycontrolchname)->SetWindowTextW(CString("2222"));
		delete cont;
		cont = NULL;*/
	}

	for (size_t i = curindexout; i < curindexout + 8; i++)
	{
		if (i < data2.size())
		{
			data2[i]->GetDlgItem(IDC_mycontrolchname)->GetClientRect(&rect1);
			data2[i]->MoveWindow(5, 80 + (i % 8)*rect1.Height(), rect.Width()-7, rect1.Height());
			data2[i]->setChecked();
			data2[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			break;
		}
	}

	if (data2.size() % 8 == 0)
		sprintf(buf, "%d", data2.size() / 8);
	else
		sprintf(buf, "%d", data2.size() / 8 + 1);
	GetDlgItem(IDC_devicemanagetotalpage2)->SetWindowTextW(CString(buf));

	for (size_t i = 0; i < data2.size(); i++)
	{
		data2[i]->updatePlayAndRecordCmb();
		break;
	}
}

bool CDeviceManage::checkhasinputname(CString name)
{
	bool ret = false;

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getChsName() == name)
			ret = true;
	}

	return ret;

}
bool CDeviceManage::checkhasoutputname(CString name)
{
	bool ret = false;
	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getChsName() == name)
			ret = true;

	}

	return ret;
}

double CDeviceManage::getfs(CString index)
{
	double fs = 0;

	if (m_hWnd == NULL)
		return fs;

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == index)
		{
			fs = data[i]->getFs();
			break;
		}
	}

	return fs;
}

void CDeviceManage::getDriverType(CString index, char** drivertype)
{
	if (m_hWnd == NULL)
		return;
	bool bserch = false;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == index)
		{
			*drivertype = getcharp(data[i]->getDriverType());
			bserch = true;
			break;
		}
	}

	if (bserch)
		return;


	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getInnerName() == index)
		{
			*drivertype = getcharp(data2[i]->getDriverType());
			break;
		}
	}

	


}

void CDeviceManage::getOutDriverType(int index, char** drivertype)
{
	if (m_hWnd == NULL)
		return;

	*drivertype = getcharp(data2[index]->getDriverType());
}

char* CDeviceManage::getcharp(CString str)
{

	int len1 = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char* ch = new char[len1 + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
	return ch;
}

//输入参数代表第几个设备或通道
double CDeviceManage::getinputvp(CString innerName)//获取输入VP值
{
	double vp = 0;
	if (m_hWnd == NULL)
		return vp;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == innerName)
		{
			vp = data[i]->getVpValues();
			break;
		}
	}
	
	return vp;
}

double CDeviceManage::getoutvp(CString innerName)
{
	double vp = 0;
	if (m_hWnd == NULL)
		return vp;
	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getInnerName() == innerName)
		{
			vp = data2[i]->getVpValues();
			break;
		}
	}
	
	return vp;
}

MyControldata::MyControldata(CString innername, CString drivername, CString chsname)
{
	m_innername = innername;
	m_drivername = drivername;
	m_chsname = chsname;
}

void MyControldata::getdata(CString& innername, CString& drivername, CString& chsname)
{
	innername = m_innername;
	drivername = m_drivername;
	chsname = m_chsname;
}

void MyControldata::setdata(CString innername, CString drivername, CString chsname)
{
	m_innername = innername;
	m_drivername = drivername;
	m_chsname = chsname;
}



MyControldata::~MyControldata()
{
}


void CDeviceManage::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CDeviceManage::OnBnClickeddevicemangeplay()
{
	// TODO:  在此添加控件通知处理程序代码

	if (data2.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块数据条目！"));
		return;
	}

	if (data2[0]->getDriverType() == "")
	{
		AfxMessageBox(_T("设备管理模块数据,驱动未选择！"));
		return;
	}

	if (data2[0]->getDriverType() == "ASIO")
	{
		
			data2[0]->playASIO(data2[0]->getFs(), 0);
		
	}
	else if (data2[0]->getDriverType() == "WDM")
	{
		
		data2[0]->playWDM(0, "D:\\TestRecord\\playjili.wav", data2[0]->getChsName_Show());
		//data2[0]->playMME();
	}


}


void CDeviceManage::OnBnClickeddevicemangerecord()
{
	// TODO:  在此添加控件通知处理程序代码
	if (data.size() == 0)
	{
		AfxMessageBox(_T("设备管理模块数据条目！"));
		return;
	}

	if (data[0]->getDriverType() == "")
	{
		AfxMessageBox(_T("设备管理模块数据,驱动未选择！"));
		return;
	}



	if (data[0]->getDriverType() == "ASIO")
	{

		//data2[0]->recordASIO(data2[0]->getFs(), 0);
		AfxMessageBox(_T("没有ASIO方式！"));
		return;

	}
	else if (data[0]->getDriverType() == "WDM")
	{
		CInputDialg inputdlg(false);
		inputdlg.setCaption(CString("输入录音时长(s)："));
		inputdlg.DoModal();
		int time = inputdlg.getV();

		int chs = data[0]->getMMEchs();
		int bit = data[0]->getMMEbits();
		DWORD fs = data[0]->getFs();
		int index = data[0]->getchsIndex();

		WAVEINCAPS pwic;
		UINT ndev = waveInGetNumDevs();

		data[0]->createMMEbuffer(ndev);

		data[0]->recordMME(time, NULL, chs, bit, fs, index);
		data[0]->startMM();
		/*while (!data[0]->checkMM(0))
		;
		data[0]->stopRe();*/
		Sleep((time + 1) * 1000);
		this->MMEinit(time, chs, bit, fs);
		AfxMessageBox(_T("wdm录音完成！"));
		/*CInputDialg inputdlg;
		inputdlg.setCaption(CString("输入电压值："));
		inputdlg.DoModal();
		int invrms = inputdlg.getV();

		data[0]->recordWDM(invrms, "");
		
		while (!data[0]->wdmisdone())
			;
		AfxMessageBox(_T("wdm录音完成！"));*/
		/*WAVEINCAPS pwic;
		UINT ndev = waveInGetNumDevs();
		
		vector<MMERecordParams> p;
		data[0]->MMEinit(ndev,0,p);
		for (size_t i = 0; i < ndev; i++)
		{
			waveInGetDevCaps(i, &pwic, sizeof(WAVEINCAPS));
			if (i == 1)
			{
				data[0]->recordMME(10, NULL, 2, 16, 48000, i);
			}
			else if (i==3)
			{
				data[0]->recordMME(10, NULL, 1, 24, 44100, i);
			}
		}
		data[0]->startMM();*/
		
	}

}

void CDeviceManage::MMERecord(const vector<MMERecordParams>& mmeParams)
{

	WAVEINCAPS pwic;
	UINT ndev = waveInGetNumDevs();

	data[0]->createMMEbuffer(ndev);
	for (size_t i = 0; i < mmeParams.size(); i++)
	{
		waveInGetDevCaps(i, &pwic, sizeof(WAVEINCAPS));
		
		data[0]->recordMME(mmeParams[i].time, NULL, mmeParams[i].chs, mmeParams[i].bit, mmeParams[i].fs, mmeParams[i].index);
		
		
	}
	OutputDebugString(_T("CDeviceManage::MMERecord\n"));
	data[0]->startMM();
	//data[0]->MMEinit(ndev);

}

void CDeviceManage::MMERecord(const MMERecordParams& mmeParams)
{
	WAVEINCAPS pwic;
	UINT ndev = waveInGetNumDevs();

	data[0]->createMMEbuffer(ndev);

	waveInGetDevCaps(mmeParams.index, &pwic, sizeof(WAVEINCAPS));
	data[0]->recordMME(mmeParams.time, NULL, mmeParams.chs, mmeParams.bit, mmeParams.fs, mmeParams.index);

	OutputDebugString(_T("CDeviceManage::MMERecord\n"));
	WriteInfoLog(_T("%s[%d:%s]"), _FL_, __FUNCTIONW__);
	data[0]->startMM();
	//data[0]->MMEinit(ndev);

}

void CDeviceManage::MMEinit(double times, const vector<MMERecordParams>& mmesparam, int nIndex)
{
	UINT ndev = waveInGetNumDevs();
	data[0]->MMEinit(ndev, times, mmesparam, nIndex);
}

void CDeviceManage::MMEinit(double times, int chs, int bit, DWORD fs)
{
	vector<MMERecordParams> mmesparam;
	MMERecordParams param;
	param.chs = chs;
	param.bit = bit;
	param.fs = fs;
	mmesparam.push_back(param);
	UINT ndev = waveInGetNumDevs();
	data[0]->MMEinit(ndev, times, mmesparam);
}

void CDeviceManage::wav_readBymatlab(double**data, double fs, double times, char* pathwav)
{
	//matlab 环境初始化,全局matlab环境初始化完成
	/*bool b_ok = wavInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("wavInitialize err"));

	}*/

	if (strcmp(pathwav, "") == 0)
	{
		pathwav = "D:\\TestRecord\\wdmRecord1.wav";
	}

	//调用具体函数
	wave_t* wav1 = (wave_t*)malloc(sizeof(wave_t));
	wavread(pathwav, wav1);
	if (wav1->fp == NULL)
		return;

	fclose(wav1->fp);

	
	
	
	
	//const wchar_t* path = _T("D:\TestRecord\wdmRecord1.wav");
	char* temppath = "D:\\TestRecord\\wdmRecord_0.wav";
	int len = strlen(pathwav);

	mxChar* path = new mxChar[len];
	mwArray in(len, 1, mxCHAR_CLASS);
	mwArray in2(len, 1, mxCHAR_CLASS);
	for (size_t i = 0; i < len; i++)
	{
		path[i] = temppath[i];
	}
	path[len] = 0;
	//in.SetCharData(path, len+1);
	//in.GetCharData(in2,len);

	
	mwArray out1(wav1->length, 1, mxDOUBLE_CLASS);
	if (wav1->length == 0)
	{
		
		mainf->setStatusStr(_T("读取wav文件信息失败！"));
		return;
	}
	//wchar_t* ml = _T("D://TestRecord//wdmRecord_0.wav");

	CString path_wav(pathwav);
	mxChar output[100]{0};

	for (size_t i = 0; i < path_wav.GetLength(); i++)
	{
		output[i] = path_wav.GetAt(i);
	}



	mwArray matlabpath(1, path_wav.GetLength(), mxCHAR_CLASS);
	 
	matlabpath.SetCharData(path_wav, path_wav.GetLength());
	wchar_t* ml2 = new wchar_t[path_wav.GetLength()];
	matlabpath.GetCharData(ml2, path_wav.GetLength());
	mwArray col(1, 1, mxDOUBLE_CLASS);
	//
	//double* vc_col=new double[1];
	//vc_col[0] = 1;
	//col.SetData(vc_col, 1);
	col(1, 1) = 1;

	wav(1, out1, matlabpath, col);
	len = out1.NumberOfElements();

	*data = new double[len];
	double* wavdata = new double[len]{0};
	out1.GetData(wavdata, len);

	/*int k=(times*fs);
	if (k > len)
		k = len;*/


	std::ofstream outfile1;
	CString path1("D:\\TestRecord\\wavread.txt");
	outfile1.open(path1, std::ios::out | std::ios::trunc);
	for (size_t i = 0; i <len ; i++)
	{
		data[0][i] = wavdata[i];
		outfile1 << wavdata[i] << endl;
	}
	outfile1.close();
	//wavTerminate(); //后面全局清理
	delete[] wavdata;
}

void CDeviceManage::setDeviceVpVal(CString innerName, double vp, bool isinput)
{

	if (isinput)
	{

		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i]->getInnerName() == innerName)
			{
				data[i]->setVpVal(vp);
				break;
			}
		}

	}
	else
	{
		for (size_t i = 0; i < data2.size(); i++)
		{
			if (data2[i]->getInnerName() == innerName)
			{
				data2[i]->setVpVal(vp);
				break;
			}
		}
	}
}

void CDeviceManage::setApval(double apval)
{
	ap = apval;
}
double CDeviceManage::getApval()
{
	return ap;
}

vector<CString> CDeviceManage::getInnerNames_InCHS()
{
	vector<CString> temp;

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName()!="")
		  temp.push_back(data[i]->getInnerName());
	}

	return temp;
}

vector<CString> CDeviceManage::getInnerNames_OutCHS()
{
	vector<CString> temp;
	for (size_t i = 0; i < data2.size(); i++)
	{
		if (data2[i]->getInnerName() != "")
		   temp.push_back(data2[i]->getInnerName());
	}

	return temp;
}


void CDeviceManage::OnBnClickeddevicesaveout()
{
	// TODO:  在此添加控件通知处理程序代码
	
	//data2[0]->setWDMChs(0);
}


void CDeviceManage::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO:  在此处添加消息处理程序代码

	

	//MoveWindow(x, y, w, h);
}


HBRUSH CDeviceManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	CFont font;

	CRect rc;
	// TODO:  在此更改 DC 的任何特性


	if (pWnd->GetDlgCtrlID() == IDC_BUTTON1 || pWnd->GetDlgCtrlID() == IDC_BUTTON2||
		pWnd->GetDlgCtrlID() == IDC_devicesavein || pWnd->GetDlgCtrlID() == IDC_devicemangerecord||
		pWnd->GetDlgCtrlID() == IDC_BUTTON3 || pWnd->GetDlgCtrlID() == IDC_BUTTON4 ||
		pWnd->GetDlgCtrlID() == IDC_devicesaveout || pWnd->GetDlgCtrlID() == IDC_devicemangeplay)
	{


		//pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(255, 60, 255));
		//pDC->SetTextColor(RGB(255, 0, 0));
	}

	if (nCtlColor == CTLCOLOR_BTN)
	{
		//pDC->SetTextColor(RGB(255, 0, 0));

		//return m_btn01Brush;

	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC||
		pWnd->GetDlgCtrlID() == IDC_STATIC2)
	{
	
		/*pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pDC->FillSolidRect(rc, RGB(248, 248, 255));*/
		
		pDC->SetBkColor(RGB(248, 248, 255));
		return brush1;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDeviceManage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值


	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	CBrush brush;
	CRect rc;
	CString cap;
	CFont font;
	switch (lpDrawItemStruct->CtlID)
	{

	case IDC_BUTTON1:
		GetDlgItem(IDC_BUTTON1)->GetClientRect(rc);
		
		//cap = "新增输入通道";
		//dc->DrawText(cap, &rc, DT_RIGHT | DT_CENTER);
		//dc->SetBkMode(TRANSPARENT);
		//dc->SetTextColor(RGB(0, 255, 0));

		
		break;
	default:
		break;
	}

	
	//dc->SetBkColor(RGB(255, 60, 255));
	
	
	//brush.CreateSolidBrush(RGB(0, 60, 0));
	/*dc->FillRect(rc, &brush);

	dc->SetBkMode(TRANSPARENT);
	dc->SetTextColor(RGB(0, 255, 0));

	dc->DrawText(cap, &rc, DT_RIGHT | DT_CENTER);*/
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CDeviceManage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	
	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	CRect rc;
	pwnd->GetWindowRect(rc);
	rc.top += 12;

	ScreenToClient(rc);

	dc.FillSolidRect(rc, RGB(248, 248, 255));
	
	drawGroupBox(pwnd, &dc);
	/*rc.top += 14;
	MoveToEx(dc, rc.left + 100, rc.top, NULL);
	LineTo(dc, rc.right, rc.top);

	MoveToEx(dc, rc.right, rc.top, NULL);
	LineTo(dc, rc.right, rc.bottom);

	MoveToEx(dc, rc.right, rc.bottom, NULL);
	LineTo(dc, rc.left-1, rc.bottom);

	MoveToEx(dc, rc.left-1, rc.bottom, NULL);
	LineTo(dc, rc.left-1, rc.top);

	MoveToEx(dc, rc.left - 1, rc.top, NULL);
	LineTo(dc, rc.left + 19, rc.top);*/

	pwnd = GetDlgItem(IDC_STATIC2);
	
	drawGroupBox(pwnd, &dc);

	pwnd->GetWindowRect(rc);
	rc.top += 15;
	ScreenToClient(rc);

	dc.FillSolidRect(rc, RGB(248, 248, 255));
	

	/*CBrush brush(RGB(255, 0, 0));
	dc.FillRect(rc, &brush);*/

}

void CDeviceManage::drawGroupBox(CWnd* wnd,CPaintDC* dc)
{
	CRect rc;

	int dc_id=dc->SaveDC();
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

BOOL CDeviceManage::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_RBUTTONUP&&pMsg->hwnd != data[0]->m_hWnd)
	{
		return FALSE;
	}

	if (pMsg->hwnd == data[0]->m_hWnd)
	{

		if (pMsg->message == WM_LBUTTONDBLCLK)
		{
			//onDbClicklist();
			//AfxMessageBox(_T("ok"));
		}
		else if (pMsg->message == WM_RBUTTONUP)
		{

			//onRClicklist();
			openMenu();

		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDeviceManage::openMenu()
{
	CMenu menu;
	POINT  pt{ 0 };
	GetCursorPos(&pt);
	menu.LoadMenuW(IDR_MYCONTROL);

	menu.GetSubMenu(0)->TrackPopupMenu(0, pt.x, pt.y, this);
}

void CDeviceManage::openASIOPanle()
{

	//打开ASIO控制面板
	//C:\Program Files\LISSIG\USB Audio Device Driver\x64\LISSIGUsbAudioCpl.exe

	//system("C:\\Program Files\\LISSIG\\USB Audio Device Driver\\x64\\LISSIGUsbAudioCpl.exe");

	ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\LISSIG\\USB Audio Device Driver\\x64\\LISSIGUsbAudioCpl.exe"),
		NULL,NULL,SW_SHOW);


}

void CDeviceManage::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	//data[0]->stopRe();
	double * indata = NULL;
	wav_readBymatlab(&indata, 44100, 2.63, "");
}


void CDeviceManage::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	
	return;
	char str[300];

	sprintf(str, "x=%d,y=%d\n", point.x, point.y);

	//if (abs(point.x - 700) <= 2 && abs(point.y - 1500) <= 10)
	{
		HCURSOR hCur = LoadCursor(NULL, IDC_SIZEWE);

		::SetCursor(hCur);
	}

	OutputDebugString(CString(str));


	CDialogEx::OnMouseMove(nFlags, point);
}

double CDeviceManage::getSensitivity(CString innerName)
{
	double sensitivity = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i]->getInnerName() == innerName)
		{
			sensitivity=data[i]->getSen();
			break;
		}
	}

	return sensitivity;
}
