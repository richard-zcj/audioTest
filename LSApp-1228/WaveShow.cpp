// WaveShow.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "WaveShow.h"
#include "afxdialogex.h"
#include "TestResManagement.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <stdlib.h>

// CWaveShow 对话框

IMPLEMENT_DYNAMIC(CWaveShow, CDialogEx)

CWaveShow::CWaveShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaveShow::IDD, pParent)
{
	/**********测试使用**************/
	//LoadTxt(&m_data);
}

CWaveShow::~CWaveShow()
{
}

void CWaveShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CWaveShow::LoadTxt(vector<double>* data)
{
	
	CString path1("D:\\TestRecord\\response.txt");
	ifstream outfile1;
	outfile1.open(path1, std::ios::in);
	
	double val = 0;
	
	while (outfile1.peek()!=EOF)
	{
		outfile1 >> val;
		if (val > maxv)
		{
			maxv = val;
		}

		m_data.push_back(val);
	}
	

	outfile1.close();
}

LRESULT CWaveShow::OnResItemShow(WPARAM wParam, LPARAM lParam)
{

	if (wParam == 7)
	{
		CPaintDC dc(this);
		dc.TextOutW(100, 100, _T("更新中...."));
		Invalidate();
		ShowWindow(SW_SHOW);
		//加载txt数据
		m_data.clear();
		LoadTxt(&m_data);
		
		//根据txt 刷新显示wave
		/*thread thread_draw_bitmap(&CWaveShow::DrawThreadBitMap, this);
		thread_draw_bitmap.detach();*/
		

		
		/*if (IsWindowVisible())
			ShowWindow(SW_SHOW);
		else
		{
			ShowWindow(SW_HIDE);
		}*/

	}

	return 0;
}


BEGIN_MESSAGE_MAP(CWaveShow, CDialogEx)
	ON_MESSAGE(RESITEMSSHOW, &CWaveShow::OnResItemShow)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWaveShow 消息处理程序


void CWaveShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	DrawThreadBitMap();
	dc.TextOutW(100, 100, _T(""));
}

BOOL CWaveShow::OnInitDialog()
{
	UpdateData(FALSE);

	GetDlgItem(IDC_waveshowPic)->GetClientRect(m_rc);


	return TRUE;
}

void CWaveShow::setData(const vector<double>* data)
{
	m_data.clear();
	for (size_t i = 0; i < data->size(); i++)
	{
		m_data.push_back(data->at(i));
	}
}

void CWaveShow::setData(const double* data, size_t len)
{
	m_data.clear();
	for (size_t i = 0; i < len; i++)
	{



		m_data.push_back(data[i]);
	}
}


void CWaveShow::DrawThreadBitMap()
{

	CDC* dc = GetDlgItem(IDC_waveshowPic)->GetDC();

	CPen pen;
	pen.CreatePen(0, 1, RGB(0, 0, 255));
	CPen* oldpen = (CPen*)dc->SelectObject(pen);

	CBrush bm;
	bm.CreateSolidBrush(RGB(255, 255, 255));

	//x 根据显示的宽度计算   需要显示的信号长度
	double perpointlen = 1.0*m_rc.Width() / m_data.size();//每个信号的点占用绘图实际的长度
	double jiange = perpointlen;//每个信号的显示间隔长度

	
	double perpointhight = 1.0*m_rc.Height() / 2.0 / maxv;
	int ymove = m_rc.Height() / 2.0;


	vector< int> tempdata;
	tempdata.resize(m_data.size());
	if (m_data.size() == 0)
		return;

	for (size_t i = 0; i < m_data.size() - 1; i++)
	{
		tempdata[i] = m_data[i] * perpointhight;

	}


	BOOL b = 0;
	CBitmap membitmap;
	CDC memDC;

	b = memDC.CreateCompatibleDC(dc);
	b = membitmap.CreateCompatibleBitmap(dc, m_rc.Width(), m_rc.Height());
	memDC.SelectObject(membitmap);

	pen.DeleteObject();
	//pen.CreatePen(0, 1, RGB(56, 94, 15));
	pen.CreatePen(0, 1, RGB(255, 255, 255));
	memDC.SelectObject(pen);
	for (size_t i = 0; i < tempdata.size() - 1; i++)
	{
		MoveToEx(memDC, i*jiange, tempdata[i] + ymove, NULL);
		LineTo(memDC, (i + 1)*jiange, tempdata[i + 1] + ymove);
	}

	dc->BitBlt(0, 0, m_rc.Width(), m_rc.Height(), &memDC, 0, 0, SRCCOPY);

	ReleaseDC(&memDC);

	

}
