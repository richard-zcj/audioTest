// GetInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "GetInput.h"
#include "afxdialogex.h"


// CGetInput �Ի���

IMPLEMENT_DYNAMIC(CGetInput, CDialogEx)

CGetInput::CGetInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetInput::IDD, pParent)
{

}

CGetInput::~CGetInput()
{
}

void CGetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGetInput, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGetInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGetInput::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGetInput ��Ϣ�������


void CGetInput::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_EDITgetinput)->GetWindowTextW(m_jiliname);

	CDialogEx::OnOK();
}


void CGetInput::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������



	CDialogEx::OnCancel();
}

void CGetInput::getjiliName(CString& name)
{
	name = m_jiliname;
}