// CaculateSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "CaculateSet.h"
#include "afxdialogex.h"

extern int golble_dotNum;
// CCaculateSet �Ի���

IMPLEMENT_DYNAMIC(CCaculateSet, CDialogEx)

CCaculateSet::CCaculateSet(CWnd* pParent /*=NULL*/)
: CDialogEx(CCaculateSet::IDD, pParent), m_dotNum(1)
{
	
}

CCaculateSet::~CCaculateSet()
{
}

void CCaculateSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_caculatesetDotNum, m_dotNum);
}

int CCaculateSet::getDotNum()
{
	return m_dotNum;
}

void CCaculateSet::setDotNum(int num)
{
	m_dotNum = num;
	
}

BOOL CCaculateSet::OnInitDialog()
{
	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCaculateSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCaculateSet::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CCaculateSet ��Ϣ�������


void CCaculateSet::OnBnClickedButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	golble_dotNum = m_dotNum;
	AfxMessageBox(_T("����ɹ�"));
	this->ShowWindow(SW_HIDE);

}
