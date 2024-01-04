// Calibration.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "Calibration.h"
#include "afxdialogex.h"


// CCalibration 对话框

IMPLEMENT_DYNAMIC(CCalibration, CDialogEx)

CCalibration::CCalibration(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibration::IDD, pParent)
{

}

CCalibration::~CCalibration()
{
}

void CCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BOOL CCalibration::OnInitDialog()
{
	UpdateData(false);
	m_tab.InsertItem(0, _T("输入通道"));
	m_tab.InsertItem(1, _T("输出通道"));

	calibrinput.Create(IDD_calibrationinput, &m_tab);
	calibroutput.Create(IDD_calibrationoutput, &m_tab);


	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 35;
	rc.bottom -= 5;
	rc.left += 5;
	rc.right -= 5;
	calibrinput.MoveWindow(&rc);
	calibroutput.MoveWindow(&rc);

	calibrinput.SetParent(&m_tab);
	calibroutput.SetParent(&m_tab);

	//calibrinput.SetBackgroundColor(RGB(255, 255, 255));
	//calibroutput.SetBackgroundColor(RGB(255, 255, 255));
	calibrinput.ShowWindow(TRUE);

	

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCalibration, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CCalibration::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_KEYDOWN, IDC_TAB1, &CCalibration::OnTcnKeydownTab1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CCalibration 消息处理程序


void CCalibration::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码

	int index = m_tab.GetCurSel();

	switch (index)
	{
	case 0:
		calibrinput.ShowWindow(TRUE);
		calibroutput.ShowWindow(FALSE);
		break;
	case 1:
		calibrinput.ShowWindow(FALSE);
		calibroutput.ShowWindow(TRUE);
		break;
	default:
		break;
	}

	*pResult = 0;
}

double CCalibration::getinputSens()
{
	return calibrinput.getInDeviceSensitivity();

}


void CCalibration::OnTcnKeydownTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTCKEYDOWN* pTCKeyDown = reinterpret_cast<NMTCKEYDOWN*>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CCalibration::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	/*CWnd* pwnd = GetDlgItem(IDC_TAB1);
	CRect rc;
	pwnd->GetWindowRect(rc);
	ScreenToClient(rc);

	dc.FillSolidRect(rc, RGB(248, 248, 255));*/
}
