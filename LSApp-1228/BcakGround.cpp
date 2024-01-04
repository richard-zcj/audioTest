// BcakGround.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "BcakGround.h"
#include "afxdialogex.h"


extern int globle_w;
extern int globle_h;

// CBcakGround 对话框

IMPLEMENT_DYNAMIC(CBcakGround, CDialogEx)

CBcakGround::CBcakGround(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBcakGround::IDD, pParent)
{

}

CBcakGround::~CBcakGround()
{
}

void CBcakGround::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INKPICTURE1, m_showbg);
}


BEGIN_MESSAGE_MAP(CBcakGround, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CBcakGround 消息处理程序


void CBcakGround::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CFont f1;
	f1.CreatePointFont(250, _T("宋体"), NULL);
	CFont* oldf = dc.SelectObject(&f1);
	dc.SetBkColor(RGB(171, 171, 171));
	dc.TextOutW(500 / (2880.0 / globle_w), 400 / (1800.0 / globle_h), _T("联康声学测试系统Version1.0"));
	dc.SelectObject(oldf);
}


void CBcakGround::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码



}

BOOL CBcakGround::OnInitDialog()
{
	UpdateData(FALSE);

	CRect curRect;
	GetWindowRect(curRect);


	double xx = 2880.0 / globle_w;
	double hh = 1800.0 / globle_h;

	//SetWindowPos(NULL, 500.0 * xx, 250.0* hh, 2000.0 * xx, 1000.0 * hh, 0);
	SetWindowPos(NULL, 500.0, 250.0, 2000.0, 1000.0, 0);

	

	CImage image;
	HRESULT res = -1;
	res = image.Load(_T("./res/bg.jpg"));

	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	//加载图片
	/*m_bg.LoadBitmapW(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC);
	p->SetBitmap(m_bg)*/;

	/*brush.DeleteObject();
	brush.CreatePatternBrush(&bitmap);*/

	

	return TRUE;
}


HBRUSH CBcakGround::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	/*if (pWnd == this)
		return brush;*/
	if (nCtlColor == CTLCOLOR_DLG)
	{
		HBRUSH br;
		
		br = CreateSolidBrush(RGB(171, 171, 171));
		
		hbr = br;


	}

	if (pWnd->GetDlgCtrlID() == IDC_INKPICTURE1)
	{
		pDC->SetBkColor(RGB(171, 171, 171));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
