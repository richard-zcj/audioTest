// Sensitivity.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "Sensitivity.h"
#include "afxdialogex.h"


// CSensitivity �Ի���

IMPLEMENT_DYNAMIC(CSensitivity, CDialogEx)

CSensitivity::CSensitivity(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSensitivity::IDD, pParent)
{

}

CSensitivity::~CSensitivity()
{
}

void CSensitivity::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSensitivity, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSensitivity ��Ϣ�������


void CSensitivity::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	return;
	CPen pen;
	pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
	dc.SelectObject(&pen);
	//����������x y
	dc.MoveTo(50, 600);    //ԭ�����꣨50��600��
	dc.LineTo(1550, 600);   //X��������꣨1550��600��
	dc.TextOutW(1500, 620, _T("Hz"));
	dc.MoveTo(50, 600);//ԭ��
	dc.LineTo(50, 10);//Y���Ϸ����꣨50��10��
	dc.TextOutW(25, 10, _T("db"));
	CFont font;
	font.CreatePointFont(80, _T("Arial"), NULL);
	dc.SelectObject(&font);
	dc.TextOutW(30, 595, _T("O"));

	pen.DeleteObject();
	pen.CreatePen(BS_SOLID, 2, RGB(255, 0, 0));
	dc.SelectObject(&pen);
	//����
	dc.MoveTo(50, 200);
	dc.LineTo(600, 200);
	pen.DeleteObject();
	pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 255));
	dc.SelectObject(&pen);
	if (m_data != NULL&&m_data->size()>0)
	{
		for (size_t i = 0; i < m_data->size() - 1; i++)
		{

			dc.MoveTo(i * 150 + 50, m_data->at(i) + 100);
			dc.LineTo((i + 1) * 150 + 50, m_data->at(i + 1) + 100);
		}
	}
}

void CSensitivity::setDara(vector<double>* data)
{
	m_data = data;

}

void CSensitivity::updateshow()
{
	Invalidate();
}

