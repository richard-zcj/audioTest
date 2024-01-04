// StartPg.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "StartPg.h"
#include "afxdialogex.h"


// CStartPg 对话框

IMPLEMENT_DYNAMIC(CStartPg, CDialogEx)

CStartPg::CStartPg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartPg::IDD, pParent)
{

}

CStartPg::~CStartPg()
{
}

void CStartPg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStartPg::OnInitDialog()
{
	UpdateData(FALSE);

	m_shwstr = CString("主程序正在加载中,请等待....(1%)");
	CImage image;
	HRESULT res = -1;
	res = image.Load(_T("./res/bg.jpg"));

	CBitmap bitmap;
	bitmap.Attach(image.Detach());
	
	brush.CreatePatternBrush(&bitmap);
	//SetBackgroundColor(RGB(21, 255, 23));
	return TRUE;
}


BEGIN_MESSAGE_MAP(CStartPg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CStartPg 消息处理程序


void CStartPg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(rc);

	CFont f;
	f.CreatePointFont(200,_T("宋体"));
	dc.SelectObject(&f);

	CBrush b;
	b.CreateSolidBrush(RGB(3, 133, 145));
	dc.SelectObject(&b);
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(143, 133, 45));
	dc.TextOutW(rc.Width() / 2-450, rc.Height() / 2-30, m_shwstr);
	Invalidate();
	
}

void CStartPg::SetStartInfo(CString info)
{
	m_shwstr = info;
	::SendMessage(m_hWnd, WM_PAINT, 0, 0);
}


HBRUSH CStartPg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	/*if (pWnd == this)
		return brush;*/

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
