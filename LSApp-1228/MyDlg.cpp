// MyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "MyDlg.h"
#include "afxdialogex.h"


// MyDlg 对话框

IMPLEMENT_DYNAMIC(MyDlg, CDialogEx)

MyDlg::MyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MyDlg::IDD, pParent)
{
	
}

MyDlg::~MyDlg()
{
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_INPUT, m_cmbInput);
	DDX_Control(pDX, IDC_CMB_INPUTINSIDE, m_inputInside);

	DDX_Control(pDX, IDC_CMB_YINXIANG, m_cmbYinxiang);
	DDX_Control(pDX, IDC_CMB_YINXIANG2, m_cmbyinxiang2);
}

BOOL MyDlg::OnInitDialog()
{
	UpdateData(false);//设置数据传递方向

	m_cmbInput.InsertString(0,_T("INPUT1"));
	m_cmbInput.InsertString(1,_T("INPUT2"));
	m_cmbInput.InsertString(2,_T("INPUT3"));
	m_cmbInput.InsertString(3,_T("INPUT4"));
	m_cmbInput.InsertString(4,_T("INPUT5"));
	m_cmbInput.InsertString(5,_T("INPUT6"));
	m_cmbInput.InsertString(6,_T("INPUT7"));
	m_cmbInput.InsertString(7,_T("INPUT8"));

	m_cmbInput.SetCurSel(0);

	m_inputInside.InsertString(0,_T("INPUT1"));
	m_inputInside.InsertString(1,_T("INPUT2"));
	m_inputInside.InsertString(2,_T("INPUT3"));
	m_inputInside.InsertString(3,_T("INPUT4"));
	m_inputInside.InsertString(4,_T("INPUT5"));
	m_inputInside.InsertString(5,_T("INPUT6"));
	m_inputInside.InsertString(6,_T("INPUT7"));
	m_inputInside.InsertString(7,_T("INPUT8"));

	m_inputInside.SetCurSel(1);

	m_cmbYinxiang.InsertString(0,_T("OUTPUT1"));
	m_cmbYinxiang.InsertString(1,_T("OUTPUT2"));
	m_cmbYinxiang.InsertString(2,_T("OUTPUT3"));
	m_cmbYinxiang.InsertString(3,_T("OUTPUT4"));
	m_cmbYinxiang.SetCurSel(0);

	m_cmbyinxiang2.InsertString(0,_T("OUTPUT1"));
	m_cmbyinxiang2.InsertString(1,_T("OUTPUT2"));
	m_cmbyinxiang2.InsertString(2,_T("OUTPUT3"));
	m_cmbyinxiang2.InsertString(3,_T("OUTPUT4"));
	m_cmbyinxiang2.SetCurSel(1);

	return TRUE;

}


BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CMB_INPUTINSIDE, &MyDlg::OnCbnSelchangeCmbInputinside)
	ON_CBN_SELCHANGE(IDC_CMB_INPUT, &MyDlg::OnCbnSelchangeCmbInput)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// MyDlg 消息处理程序


void MyDlg::OnCbnSelchangeCmbInputinside()
{
	// TODO: 在此添加控件通知处理程序代码
	CString inputstr;
	CString inputstr2;
	int index=m_cmbInput.GetCurSel();
	if(index>=0)
	    m_cmbInput.GetLBText( index,inputstr);
	m_inputInside.GetLBText( m_inputInside.GetCurSel(),inputstr2);

	if(inputstr==inputstr2)
	{
		m_inputInside.SetCurSel(-1);
		AfxMessageBox(_T("通道已被选择！选择其他通道"));
	}



}


void MyDlg::OnCbnSelchangeCmbInput()
{
	// TODO: 在此添加控件通知处理程序代码
	CString inputstr;
	CString inputstr2;
	int index=m_inputInside.GetCurSel();
	if(index>=0)
	   m_inputInside.GetLBText( index,inputstr);
	m_cmbInput.GetLBText( m_cmbInput.GetCurSel(),inputstr2);

	if(inputstr==inputstr2)
	{
		m_cmbInput.SetCurSel(-1);
		AfxMessageBox(_T("通道已被选择！选择其他通道"));
	}

}


void MyDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	

	CDialogEx::OnClose();

	
}


void MyDlg::OnDestroy()
{
	
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	
}


void MyDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}


void MyDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	//int res=AfxMessageBox(_T("确认修改吗？"),MB_OKCANCEL);
}

bool MyDlg::SaveData()
{



	return true;
	
}
