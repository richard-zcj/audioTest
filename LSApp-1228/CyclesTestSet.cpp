// CyclesTestSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "CyclesTestSet.h"
#include "afxdialogex.h"







// CCyclesTestSet �Ի���

IMPLEMENT_DYNAMIC(CCyclesTestSet, CDialogEx)

CCyclesTestSet::CCyclesTestSet(CWnd* pParent /*=NULL*/)
: CDialogEx(CCyclesTestSet::IDD, pParent), cyclesTimes(10), cyclesPeriod(25)
{

}

CCyclesTestSet::~CCyclesTestSet()
{
}

void CCyclesTestSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_cmbCyclesTestSetItems, m_cyclesItmes);

	DDX_Text(pDX, IDC_editCyclesTestSetTimes, cyclesTimes);
	DDX_Text(pDX, IDC_editCyclesTestSetperid, cyclesPeriod);
	data.setCyclesTimes(cyclesTimes);
	data.setCyclesPeriod(cyclesPeriod);
}

BOOL CCyclesTestSet::OnInitDialog()
{
	UpdateData(FALSE);

	m_cyclesItmes.InsertString(0, _T("��������"));
	m_cyclesItmes.InsertString(1, _T("----"));
	m_cyclesItmes.SetCurSel(0);

	return TRUE;
}

int CCyclesTestSet::getCurnetItemsIndex()
{
	return m_cyclesItmes.GetCurSel();

}

CyclesSetData CCyclesTestSet::getData()
{
	
	return data;
}

void CCyclesTestSet::setData( CyclesSetData& data)
{

	cyclesTimes = data.getCyclesTimes();
	cyclesPeriod = data.getCyclesPeriod();
	//UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CCyclesTestSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCyclesTestSet::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CCyclesTestSet::OnBnClickedButtonSetColor)
	ON_BN_CLICKED(IDC_BUTTON3, &CCyclesTestSet::OnBnClickedButtonMainTestFrmColorSet)
END_MESSAGE_MAP()


// CCyclesTestSet ��Ϣ�������


void CCyclesTestSet::OnBnClickedButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	data.setCyclesTimes(cyclesTimes);
	data.setCyclesPeriod(cyclesPeriod);

	//AfxMessageBox(_T("����ɹ�..."));
	//this->ShowWindow(SW_HIDE);

	::SendMessage(GetSafeHwnd(), WM_CLOSE, 0, 0);
}


CyclesSetData::CyclesSetData()
{
}

CyclesSetData::~CyclesSetData()
{
}

void CyclesSetData::setCyclesTimes(int Times)
{
	cyclesTimes = Times;
}
int CyclesSetData::getCyclesTimes()
{
	return cyclesTimes;
}

void CyclesSetData::setCyclesPeriod(int Times)
{
	cyclesPeriod = Times;
}
int CyclesSetData::getCyclesPeriod()
{
	return cyclesPeriod;
}

void CyclesSetData::setCyclesColr(COLORREF clo)
{
	clor = clo;
}
COLORREF CyclesSetData::getCyclesColr()
{
	return clor;
}

void CyclesSetData::setmainTestColr(COLORREF clo)
{
	clor_mainTest = clo;
}
COLORREF CyclesSetData::getmainTestColr()
{
	return clor_mainTest;
}

void CyclesSetData::operator = (const CyclesSetData& data)
{
	this->cyclesTimes = data.cyclesTimes;
	this->cyclesPeriod = data.cyclesPeriod;
	clor = data.clor;
	clor_mainTest = data.clor_mainTest;
}

void CCyclesTestSet::OnBnClickedButtonSetColor()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CColorDialog color;
	color.DoModal();
	 
    data.setCyclesColr(color.GetColor());
}



void CCyclesTestSet::OnBnClickedButtonMainTestFrmColorSet()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CColorDialog color;
	color.DoModal();
	data.setmainTestColr(color.GetColor());
}
