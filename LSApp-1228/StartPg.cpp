// StartPg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "StartPg.h"
#include "afxdialogex.h"


// CStartPg �Ի���

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

	m_shwstr = CString("���������ڼ�����,��ȴ�....(1%)");
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


// CStartPg ��Ϣ�������


void CStartPg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(rc);

	CFont f;
	f.CreatePointFont(200,_T("����"));
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

	// TODO:  �ڴ˸��� DC ���κ�����

	/*if (pWnd == this)
		return brush;*/

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
