// TestRME.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "TestRME.h"
#include "afxdialogex.h"
#include <thread>




// TestRME �Ի���

IMPLEMENT_DYNAMIC(TestRME, CDialogEx)

TestRME::TestRME(CWnd* pParent /*=NULL*/)
: CDialogEx(TestRME::IDD, pParent), recordtime(1000)
{


	test.SetCallBackFunc(calldraw);
	

}
BOOL TestRME::OnInitDialog()
{

	UpdateData(FALSE);

	m_comfilename.InsertString(0, _T("play.wav"));
	m_comfilename.InsertString(1, _T("play1.wav"));
	m_comfilename.InsertString(2, _T("test.mp3"));


	OnBnClickedRecord();

	return TRUE;

}

TestRME::~TestRME()
{
}

void TestRME::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_recordtime, recordtime);
	DDX_Control(pDX, IDC_COMBO1, m_comfilename);
}


BEGIN_MESSAGE_MAP(TestRME, CDialogEx)
	ON_BN_CLICKED(IDOK, &TestRME::OnBnClickedPlay)
	ON_BN_CLICKED(IDCANCEL, &TestRME::OnBnClickedRecord)
	ON_BN_CLICKED(IDC_BUTTON1, &TestRME::OnBnClickedButtonStopRecord)
	ON_BN_CLICKED(IDC_BUTTON2, &TestRME::OnBnClickedButtonShowData)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, &TestRME::OnBnClickedButtonPlayAndRecord)
	ON_BN_CLICKED(IDC_BUTTON4, &TestRME::OnBnClickedButtonStartRME)
	ON_BN_CLICKED(IDC_BUTTON5, &TestRME::OnBnClickedButtonStopRME)
	ON_BN_CLICKED(IDC_BUTTON6, &TestRME::OnBnClickedButtonRMEinit)
END_MESSAGE_MAP()


// TestRME ��Ϣ�������


void TestRME::OnBnClickedPlay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//TestASIUO test;

	CString filename;
	m_comfilename.GetWindowTextW(filename);
	
	test.play(filename, recordtime,0,_T(""));

	
}

void TestRME::play(CString filename, int playtime)
{
	test.play(filename, recordtime,0,_T(""));
}


void TestRME::OnBnClickedRecord()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	//TestASIUO test;
	CString filename;
	m_comfilename.GetWindowTextW(filename);
	
	
	test.record(recordtime,this);
	//test.play(filename, recordtime);

	
	
	
}

void TestRME::record(int recordtime)
{
	test.openRecordThread(recordtime,0);
}

void calldraw()
{
	

}
extern double bufferrme[SAMPLENUMS];
extern int** rd;
void TestRME::drawrmeRecordData()
{

	pictrue=GetDlgItem(IDC_PICRME);
	CPaintDC dc1(pictrue);

	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* oldpen = dc1.SelectObject(&pen);
	dc1.SetDCPenColor(RGB(200, 40, 100));

	//BOOL b=dc1.TextOutW(100,  120, _T("111111"));

	//dc = pictrue->GetDC();
	CRect rect;
	pictrue->GetClientRect(rect);
	int wi = rect.Width();
	int hi = rect.Height();

	

	int xzhou = 30;
	int ymove = 220;
	
	
	int scx = 1;

	while (rd!=NULL&&*rd!=NULL)
	{
		
		for (size_t i = 0; i < 44100; i++)
		{
			
			MoveToEx(dc1, xzhou + i*scx, ymove + *rd[i] / 1000000.0, NULL);
			LineTo(dc1, xzhou + (i + 1)*scx, ymove + *rd[i + 1] / 1000000.0);
		}
		
		
		
	}
	

	dc1.SelectObject(oldpen);
}


void TestRME::OnBnClickedButtonStopRecord()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TestASIUO test;
	test.stopRe();
}


void TestRME::OnBnClickedButtonShowData()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	std::thread play(&TestRME::drawrmeRecordData, this);
	play.detach();
}


void TestRME::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	OnBnClickedButtonShowData();
	
}


void TestRME::OnBnClickedButtonPlayAndRecord()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	std::thread prthread(&TestRME::playAndRecordThread, this);
	prthread.detach();
	
}

void TestRME::playAndRecordThread()
{
	

	CString filename;
	m_comfilename.GetWindowTextW(filename);
	test.playAndRecord(recordtime, filename);
}

extern size_t len;
void TestRME::OnBnClickedButtonStartRME()
{


	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (len!= 0)
		return;

	if (test.StartRME())
	{
		//�жϲ�����ϻ�ͼ
		while (len>0)
		{
			Sleep(100);
		}
		Invalidate();
	}
	else
	{
	
	}

	
	

}


void TestRME::OnBnClickedButtonStopRME()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	test.StopRME();
}


void TestRME::OnBnClickedButtonRMEinit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	test.initRMECard();
}

void TestRME::setIndex(int num)
{
	
}
bool TestRME::wdmIsRecordDone()
{
	return test.wdmisRecordDone();
}
void TestRME::setwdmIsRecordDone(bool b)
{
	test.setwdmisRecordDone(b);
}
