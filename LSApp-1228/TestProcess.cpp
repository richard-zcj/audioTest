// TestProcess.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "TestProcess.h"
#include "afxdialogex.h"


// CTestProcess �Ի���

IMPLEMENT_DYNAMIC(CTestProcess, CDialogEx)

CTestProcess::CTestProcess(CWnd* pParent /*=NULL*/)
: CDialogEx(CTestProcess::IDD, pParent), l(100), t(0), r(150), b(50),
m_rcmove(50)
{

}

CTestProcess::~CTestProcess()
{
}

void CTestProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestProcess, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTestProcess ��Ϣ�������


void CTestProcess::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	
	GetDlgItem(IDC_processPic)->GetWindowRect(picrc);
	ScreenToClient(picrc);
	l = picrc.left;
	t = picrc.top;
	r = l + m_rcmove;
	b = t + picrc.Height();
	//dc.FillSolidRect(rc, RGB(0, 255, 0));
	CBrush br;
	br.CreateSolidBrush(RGB(0, 139, 0));
	rc.SetRect(l, t, r, b);
	dc.FillRect(rc, &br);
	//dc.FillPath();

}

void CTestProcess::setProcessMove(int x)
{
	/*CRect picrc;
	GetDlgItem(IDC_processPic)->GetWindowRect(picrc);
	ScreenToClient(picrc);*/
	if (x == 0)
	{
		m_rcmove = picrc.Width();
	}
	else
	{
		m_rcmove = x;
	}
	
	Invalidate();
}

BOOL CTestProcess::OnInitDialog()
{
	
	UpdateData(FALSE);


	HICON icon = AfxGetApp()->LoadIconW(IDI_LK);

	SetIcon(icon, 0);

	/*CRect rc;
	GetWindowRect(rc);
	ScreenToClient(rc);
	SetWindowPos(NULL, 600, 1500, rc.Width(), rc.Height(), 0);*/

	return TRUE;
}


void CTestProcess::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}
