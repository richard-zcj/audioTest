// InputDialg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "InputDialg.h"
#include "afxdialogex.h"
#include "DeviceManage.h"
#include "resource.h"



extern CDeviceManage devicemanage;
// CInputDialg �Ի���

IMPLEMENT_DYNAMIC(CInputDialg, CDialogEx)

//CInputDialg::CInputDialg(CWnd* pParent /*=NULL*/)
//: CDialogEx(CInputDialg::IDD, pParent), v(1)
//{
//	
//}

CInputDialg::CInputDialg(bool isStr, CWnd* pParent/* = NULL*/)
: CDialogEx(CInputDialg::IDD, pParent), v(1)
{
	m_isStr = isStr;
}

CInputDialg::~CInputDialg()
{
}

void CInputDialg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	if (m_isStr)
	{
		DDX_Text(pDX, IDC_InputDlgEdit, v2);
		DDX_Text(pDX, IDC_cmb_inputdialog, v2);

	}
	else
	{
		DDX_Text(pDX, IDC_InputDlgEdit, v);
	}


	DDX_Text(pDX, IDC_InputDlgCaption, m_caption);
	DDX_Text(pDX, IDC_InputDlgCaption2, m_uit);
	DDX_Control(pDX, IDC_cmb_inputdialog, m_cmb);


	//����ִ�м�������
	if (m_isStr)
	{
		if (m_caption == "����ͨ����")
		{
			LoadCmb(true);
			
		}
		else
		{
			LoadCmb(false);
		}
	}
	else
	{
		GetDlgItem(IDC_cmb_inputdialog)->ShowWindow(SW_HIDE);
	}

}


BEGIN_MESSAGE_MAP(CInputDialg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CInputDialg ��Ϣ�������

void CInputDialg::setunit(CString unit)
{
	m_uit = unit;
	//UpdateData(FALSE);
}
void CInputDialg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CloseWindow();

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CInputDialg::OnInitDialog()
{
	UpdateData(FALSE);
	
	//SetTimer(1, 10000, NULL);
	
	return TRUE;
}

double CInputDialg::getV()
{
	return v;
}

CString CInputDialg::getV2()
{
	return v2;
}

void CInputDialg::setCaption(CString cap)
{
	m_caption = cap;
}

void CInputDialg::LoadCmb(bool isinput)
{
	
	if (isinput)
	{
		for (size_t i = 0; i < devicemanage.data.size(); i++)
		{
			m_cmb.InsertString(i, devicemanage.data[i]->getInnerName());
		}
	}
	else
	{
		for (size_t i = 0; i < devicemanage.data2.size(); i++)
		{
			m_cmb.InsertString(i, devicemanage.data2[i]->getInnerName());
		}
	}

	GetDlgItem(IDC_cmb_inputdialog)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_InputDlgEdit)->ShowWindow(SW_HIDE);

}