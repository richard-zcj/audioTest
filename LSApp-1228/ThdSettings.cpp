// ThdSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "ThdSettings.h"
#include "afxdialogex.h"



//对话框数据



// CThdSettings 对话框

IMPLEMENT_DYNAMIC(CThdSettings, CDialogEx)

CThdSettings::CThdSettings(CWnd* pParent /*=NULL*/)
: CDialogEx(CThdSettings::IDD, pParent), m_checkHamnic1(1),m_radioHamnicIEC(1)
{
	m_checkHamnic2 = 1;
	m_checkHamnic3 = 1;
	m_checkHamnic4 = 1;
	m_checkHamnic5 = 1;
}

CThdSettings::~CThdSettings()
{
}

void CThdSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECKendofhamnics, m_bEndofHamnics);
	DDX_Control(pDX, IDC_editalogriends, m_btnEndofHamics);
	DDX_Control(pDX, IDC_SPIN1, m_alogriTHDSetMinCycles);
	DDX_Control(pDX, IDC_EDIThamnicStart, m_startxiebo);

	DDX_Check(pDX, IDC_CHECK1, m_checkHamnic1);
	DDX_Check(pDX, IDC_CHECK2, m_checkHamnic2);
	DDX_Check(pDX, IDC_CHECK3, m_checkHamnic3);
	DDX_Check(pDX, IDC_CHECK4, m_checkHamnic4);
	DDX_Check(pDX, IDC_CHECK5, m_checkHamnic5);
	DDX_Check(pDX, IDC_CHECK6, m_checkHamnic6);
	DDX_Check(pDX, IDC_CHECK7, m_checkHamnic7);
	DDX_Check(pDX, IDC_CHECK8, m_checkHamnic8);
	DDX_Check(pDX, IDC_CHECK9, m_checkHamnic9);
	DDX_Check(pDX, IDC_CHECK10, m_checkHamnic10);
	DDX_Check(pDX, IDC_CHECK11, m_checkHamnic11);
	DDX_Check(pDX, IDC_CHECK12, m_checkHamnic12);
	DDX_Check(pDX, IDC_CHECK13, m_checkHamnic13);
	DDX_Check(pDX, IDC_CHECK14, m_checkHamnic14);
	DDX_Check(pDX, IDC_CHECK15, m_checkHamnic15);
	DDX_Check(pDX, IDC_CHECK16, m_checkHamnic16);

	DDX_Check(pDX, IDC_RADIO2, m_radioHamnicIEC);
	DDX_Check(pDX, IDC_RADIO3, m_radioHamnicIEEE);

	DDX_Text(pDX, IDC_EDIThamnicStart, hamnicStart);
	DDX_Text(pDX, IDC_editalogriends, hamnicEnd);

}


BEGIN_MESSAGE_MAP(CThdSettings, CDialogEx)
	ON_BN_CLICKED(IDC_CHECKendofhamnics, &CThdSettings::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_BUTTON1, &CThdSettings::OnBnClickedButtonSaveAlogrithmThdData)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDH_OK, &CThdSettings::OnOK)
END_MESSAGE_MAP()


// CThdSettings 消息处理程序


void CThdSettings::OnBnClickedCheck17()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	if (m_bEndofHamnics)
		m_btnEndofHamics.EnableWindow(TRUE);
	else
		m_btnEndofHamics.EnableWindow(FALSE);
}

void  CThdSettings::OnOK()
{
	return;

}

BOOL CThdSettings::OnInitDialog()
{
	UpdateData(FALSE);

	m_alogriTHDSetMinCycles.SetRange(0, 1000);
	m_alogriTHDSetMinCycles.SetBase(10);
	m_alogriTHDSetMinCycles.SetBuddy(GetDlgItem(IDC_alogrithsetthdmincycles));
	m_alogriTHDSetMinCycles.SetPos(0);

	mainf = (CMainFrame*)theApp.GetMainWnd();

	SetBackgroundColor(RGB(248, 248, 255));
	
	
	return TRUE;
}

void CThdSettings::setMincycles()
{
	
}

ThdData CThdSettings::getdata()
{
	return thdData;
}



ThdData::ThdData()
{
	/*slotParam* pSlot = GetSlotParam(0);
	vector<testItem>* pItems;
	vector<testItem>::iterator it;

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			pItems = &pSlot->vSpkItems;
		}
		else
		{
			pItems = &pSlot->vMicItems;
		}

		it = pItems->begin();
		while (it != pItems->end())
		{
			if (StrCmpI(it->sItemName, _T("FR")) == 0)
			{
				for (int j = 0; j < it->vLimit.size(); j++)
				{
					itemLimit* pLimit = &it->vLimit[j];
					Mutidata data(pLimit->nFrequency, pLimit->nLower, pLimit->nUpper);
					if (i == 0)
						spk_fr_borderLine.push_back(&data);
					else
						mic_fr_borderLine.push_back(&data);
				}
			}
			else if (StrCmpI(it->sItemName, _T("THD")) == 0)
			{
				for (int j = 0; j < it->vLimit.size(); j++)
				{
					itemLimit* pLimit = &it->vLimit[j];
					Mutidata data(pLimit->nFrequency, pLimit->nLower, pLimit->nUpper);
					if (i == 0)
						spk_thd_borderLine.push_back(&data);
					else
						mic_thd_borderLine.push_back(&data);
				}
			}
			else if (StrCmpI(it->sItemName, _T("RB")) == 0)
			{
				for (int j = 0; j < it->vLimit.size(); j++)
				{
					itemLimit* pLimit = &it->vLimit[j];
					Mutidata data(pLimit->nFrequency, pLimit->nLower, pLimit->nUpper);
					if (i == 0)
						rb_borderLine.push_back(&data);
				}
			}


			it++;
		}
	}*/
}

ThdData::~ThdData()
{
}

void ThdData::operator=(const ThdData& data)
{
	m_hamincs = data.m_hamincs;
}

void ThdData::setdHamnics(int hamnics)
{
	m_hamincs.push_back(hamnics);
}

void ThdData::cleardata()
{
	m_hamincs.clear();
	m_isIECorIEEE = false;
}

void ThdData::setIECorIEEE(bool alogriType)
{
	m_isIECorIEEE = alogriType;
}

vector<int>* ThdData::getHamnics() 
{
	return &m_hamincs;
}

bool ThdData::getAlogrithm() 
{
	return m_isIECorIEEE;
}



void CThdSettings::OnBnClickedButtonSaveAlogrithmThdData()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	thdData.cleardata();

	if (m_checkHamnic1==1)
	{
		//thdData.setdHamnics(1);
	}
	if (m_checkHamnic2 == 1)
	{
		thdData.setdHamnics(0);
	}
	if (m_checkHamnic3 == 1)
	{
		thdData.setdHamnics(1);
	}
	if (m_checkHamnic4 == 1)
	{
		thdData.setdHamnics(2);
	}
	if (m_checkHamnic5 == 1)
	{
		thdData.setdHamnics(3);
	}
	if (m_checkHamnic6 == 1)
	{
		thdData.setdHamnics(4);
	}
	if (m_checkHamnic7 == 1)
	{
		thdData.setdHamnics(5);
	}
	if (m_checkHamnic8 == 1)
	{
		thdData.setdHamnics(6);
	}
	if (m_checkHamnic9 == 1)
	{
		thdData.setdHamnics(7);
	}
	if (m_checkHamnic10 == 1)
	{
		thdData.setdHamnics(8);
	}
	if (m_checkHamnic11 == 1)
	{
		thdData.setdHamnics(9);
	}
	if (m_checkHamnic12 == 1)
	{
		thdData.setdHamnics(10);
	}
	if (m_checkHamnic13 == 1)
	{
		thdData.setdHamnics(11);
	}
	if (m_checkHamnic14 == 1)
	{
		thdData.setdHamnics(12);
	}
	if (m_checkHamnic15 == 1)
	{
		thdData.setdHamnics(13);
	}
	if (m_checkHamnic16 == 1)
	{
		thdData.setdHamnics(14);
	}

	if (m_bEndofHamnics)
	{
		for (size_t i = hamnicStart; i <= hamnicEnd; i++)
		{
			if (i>0)
				thdData.setdHamnics(i);
		}
	}
	else
	{
		if (hamnicStart>0)
		    thdData.setdHamnics(hamnicStart);
	}

	if (m_radioHamnicIEC == 1)
	{
		thdData.setIECorIEEE(true);
	}
	else
	{
		thdData.setIECorIEEE(false);
	}


	//MessageBox(_T("THD算法参数已保存成功！"), _T("THD算法参数设置"), MB_ICONINFORMATION);

	mainf->setStatusStr(CString("THD算法参数已保存成功！"));

}

void CThdSettings::drawGroupBox(CWnd* wnd, CPaintDC* dc)
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



void CThdSettings::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICbiaozhun);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICdiscardtime);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICaddcishu);
	drawGroupBox(pwnd, &dc);
}
