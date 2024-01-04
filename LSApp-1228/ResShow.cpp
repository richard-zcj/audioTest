// ResShow.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "ResShow.h"
#include "afxdialogex.h"


// CResShow 对话框

IMPLEMENT_DYNAMIC(CResShow, CDialogEx)

CResShow::CResShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResShow::IDD, pParent)
{
	m_truncolor = true;
}

CResShow::~CResShow()
{
}

void CResShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_resshowitemname,m_itemName);
	DDX_Text(pDX, IDC_uintresshow,m_uint);
	
	DDX_Text(pDX, IDC_EDITresshow,m_val);
}

void CResShow::setitemName(CString s)
{
	m_itemName = s;
	UpdateData(FALSE);
}
void CResShow::setuint(CString s)
{
	m_uint = s;
	UpdateData(FALSE);
}
void CResShow::setval(double value)
{
	int tem = value * 10000;
	m_val = tem / 10000.0;
	GetDlgItem(IDC_resshowPolarity)->ShowWindow(SW_HIDE);
	UpdateData(FALSE);
}

void CResShow::setval(CString value)
{
	((CEdit*)GetDlgItem(IDC_EDITresshow))->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_resshowPolarity)->SetWindowTextW(value);
	UpdateData(FALSE);
}

void CResShow::setTitle(CString title)
{
	SetWindowTextW(title);
}


BEGIN_MESSAGE_MAP(CResShow, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDH_OK, &CResShow::OnOK)
END_MESSAGE_MAP()


// CResShow 消息处理程序


void CResShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	//SetBackgroundColor(RGB(120, 123, 0));

}


HBRUSH CResShow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);


	if (nCtlColor == CTLCOLOR_DLG)
	{
		HBRUSH br;
		if (m_truncolor)
			br = CreateSolidBrush(RGB(84, 139, 84));
		else
		{
			br = CreateSolidBrush(RGB(255, 0, 0));
		}
		hbr = br;


	}

	if (pWnd->GetDlgCtrlID() == IDC_resshowitemname ||
		pWnd->GetDlgCtrlID() == IDC_uintresshow||
		pWnd->GetDlgCtrlID() == IDC_resshowPolarity)
	{

		/*pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pDC->FillSolidRect(rc, RGB(248, 248, 255));*/

		HBRUSH br;
		if (m_truncolor)
			br = CreateSolidBrush(RGB(84, 139, 84));
		else
		{
			br = CreateSolidBrush(RGB(255, 0, 0));
		}
		hbr = br;
		pDC->SetBkColor(RGB(84, 139, 84));
		
	}

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CResShow::IsSucess(bool bsucess)
{
	m_truncolor = bsucess;
	Invalidate();
}

void CResShow::OnOK()
{
	int a = 100;
	return;
}
