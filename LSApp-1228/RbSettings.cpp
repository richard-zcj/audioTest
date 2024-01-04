// RbSettings.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "RbSettings.h"
#include "afxdialogex.h"


// CRbSettings �Ի���

IMPLEMENT_DYNAMIC(CRbSettings, CDialogEx)

CRbSettings::CRbSettings(CWnd* pParent /*=NULL*/)
: CDialogEx(CRbSettings::IDD, pParent), m_radioHamnicIEC(1), hamnicStart(10), hamnicEnd(35)
{
	m_bEndofHamnics = 1;
}

CRbSettings::~CRbSettings()
{
}

void CRbSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_editalogriendsRB, m_bEndofHamnicsRB);
	DDX_Check(pDX, IDC_CHECKendofhamnicsRB, m_bEndofHamnics);
	DDX_Text(pDX, IDC_EDIThamnicStartRB, hamnicStart);
	DDX_Text(pDX, IDC_editalogriendsRB, hamnicEnd);

	DDX_Check(pDX, IDC_IECRB, m_radioHamnicIEC);
	DDX_Check(pDX, IDC_IEEERB, m_radioHamnicIEEE);
}


BEGIN_MESSAGE_MAP(CRbSettings, CDialogEx)
	ON_BN_CLICKED(IDC_CHECKendofhamnicsRB, &CRbSettings::OnBnClickedCheckendofhamnics)
	ON_BN_CLICKED(IDC_BUTTON1, &CRbSettings::OnBnClickedButtonSave)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDH_OK, &CRbSettings::OnOK)
END_MESSAGE_MAP()


// CRbSettings ��Ϣ�������


void CRbSettings::OnBnClickedCheckendofhamnics()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if (m_bEndofHamnics)
		m_bEndofHamnicsRB.EnableWindow(TRUE);
	else
		m_bEndofHamnicsRB.EnableWindow(FALSE);
}

void  CRbSettings::OnOK()
{
	return;

}
BOOL CRbSettings::OnInitDialog()
{
	UpdateData(FALSE);
	mainf = (CMainFrame*)theApp.GetMainWnd();
	SetBackgroundColor(RGB(248, 248, 255));

	
	return TRUE;
}


void CRbSettings::OnBnClickedButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	rbData.cleardata();
	if (hamnicStart <= 2)
	{
		mainf->setStatusStr(_T("��ʼг������С��2��Rb���ݱ���ʧ��..."));
		return;
	}
	if (m_bEndofHamnics)
	{
		for (size_t i = hamnicStart; i <= hamnicEnd; i++)
		{
			if (i>0)
				rbData.setdHamnics(i-2);
		}
	}
	else
	{
		if (hamnicStart>0)
			rbData.setdHamnics(hamnicStart);
	}

	if (m_radioHamnicIEC == 1)
	{
		rbData.setIECorIEEE(true);
	}
	else
	{
		rbData.setIECorIEEE(false);
	}

	mainf->setStatusStr(CString("RB�㷨�����ѱ���ɹ���"));

}

RBdata CRbSettings::getdata()
{
	return rbData;
}


RBdata::RBdata()
{
}

RBdata::~RBdata()
{
}

void RBdata::setdHamnics(int hamnics)
{
	m_hamincs.push_back(hamnics);
}
void RBdata::cleardata()
{
	m_hamincs.clear();
}

void RBdata::setIECorIEEE(bool alogriType)
{
	m_isIECorIEEE = alogriType;
}

bool RBdata::getAlogrithm()
{
	return m_isIECorIEEE;
}

vector<int>* RBdata::getHamnics()
{
	return &m_hamincs;
}

void CRbSettings::drawGroupBox(CWnd* wnd, CPaintDC* dc)
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



void CRbSettings::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CWnd* pwnd = GetDlgItem(IDC_STATICrbaddcishu);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICrbbiozhun);
	drawGroupBox(pwnd, &dc);
}
