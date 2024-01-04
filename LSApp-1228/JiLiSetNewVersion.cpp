// JiLiSetNewVersion.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "JiLiSetNewVersion.h"
#include "afxdialogex.h"
#include <fstream>
#include "wavHandler.h"
#include "DeviceManage.h"
#include "PlayRecordSettings.h"
#include "GetInput.h"
#include <sstream>
#include "resource.h"
#include "InputDialg.h"
#include "AudioAlgorithm.h"

using namespace std;

extern CDeviceManage devicemanage;
extern CPlayRecordSettings playrecordset;
// JiLiSetNewVersion 对话框

IMPLEMENT_DYNAMIC(JiLiSetNewVersion, CDialogEx)

JiLiSetNewVersion::JiLiSetNewVersion(CWnd* pParent /*=NULL*/)
: CDialogEx(JiLiSetNewVersion::IDD, pParent), voltage(0.1), startfs(20), stopfs(20000), fs(44100),
cycle(10), mintime(10)
{

	ifstream in;
	ifstream out;
	//char* fname = "C:\\Users\\55304\\Desktop\\Sdcard.txt";
	char* fname = "f.ini";
	in.open(fname, ios::in);
	char buf[4096]{0};
	char tempstr[100]{0};
	if (in.is_open())
	{
		string str;
		streamsize s = 4096;
		
		//in.getline(buf, s);
		//int len=strlen(buf);
		////in.seekg(len);
		//memset(buf, 0, 4096);
		in.getline(buf, s);

		
		int i = 0;
		int j = 0;
		while (i<40967&&buf[i] != '\n')
		{
			if (buf[i] == '\t'||i>2246)
			{
				/*int l = strlen(tempstr);
				while (tempstr[l - 1] == '0')
				{
					memcpy(tempstr, tempstr, l - 2);
					if (tempstr[l - 2] == '.')
					{
						memcpy(tempstr, tempstr, l - 3);

					}

				}*/
				
				pindians.push_back(atof(tempstr));
				
				memset(tempstr, 0, 100);
				i++;
				j = 0;

				if (i>2246)
				{
					break;
				}

			}
			else
			{
				if (j<100)
				  tempstr[j++] = buf[i++];
				 
			}
			
			
		}

		
		
	}



	m_bInitFinish = false;
	



}

JiLiSetNewVersion::~JiLiSetNewVersion()
{
}

void JiLiSetNewVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_editjilisetnewv, voltage);
	DDV_MinMaxFloat(pDX, voltage, 0, 10);
	DDX_Text(pDX, IDC_editjilisetnewstart, startfs);
	DDV_MinMaxInt(pDX, startfs, 0, 400000);
	DDX_Text(pDX, IDC_editjilisetnewstop, stopfs);
	DDV_MinMaxInt(pDX, stopfs, 0, 400000);
	DDX_Text(pDX, IDC_COMBfs, fs);
	DDV_MinMaxInt(pDX, fs, 1, 200000);
	DDX_Text(pDX, IDC_jilinewcycle, cycle);
	DDV_MinMaxInt(pDX, cycle, 1, 100000);
	DDX_Text(pDX, IDC_jilinewmintime, mintime);
	DDV_MinMaxInt(pDX, mintime, 1, 100000);

	DDX_Text(pDX, IDC_jilinewTime, playtime);

	DDX_Check(pDX, IDC_jilisetCheckEQ, m_eq);
	DDX_Text(pDX, IDC_jilieqPath, m_eqpath);
	DDX_Control(pDX, IDC_LIST2, m_listJili);
	DDX_Control(pDX, IDC_COMBO1, m_cmbPlayChs);
}


BEGIN_MESSAGE_MAP(JiLiSetNewVersion, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &JiLiSetNewVersion::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON2, &JiLiSetNewVersion::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON3, &JiLiSetNewVersion::OnBnClickedButtonDeljili)
	ON_BN_CLICKED(IDH_OK, &JiLiSetNewVersion::OnOK)
	ON_BN_CLICKED(IDC_BUTTON4, &JiLiSetNewVersion::OnBnClickedButtonImportEq)
	ON_BN_CLICKED(IDC_BUTTON5, &JiLiSetNewVersion::OnBnClickedButtonplayJili)

	ON_COMMAND(ID_32791, &JiLiSetNewVersion::OnDelJili)
	ON_COMMAND(ID_32792, &JiLiSetNewVersion::OnshowJili)
	ON_COMMAND(ID_32793, &JiLiSetNewVersion::OnplayJili)

	ON_LBN_SELCHANGE(IDC_LIST2, &JiLiSetNewVersion::OnSelchangeList2)
	ON_EN_CHANGE(IDC_editjilisetnewv, &JiLiSetNewVersion::OnChangeEditjilisetnewv)
	ON_EN_CHANGE(IDC_editjilisetnewstart, &JiLiSetNewVersion::OnChangeEditjilisetnewstart)
	ON_EN_CHANGE(IDC_editjilisetnewstop, &JiLiSetNewVersion::OnChangeEditjilisetnewstop)
	ON_CBN_SELCHANGE(IDC_COMBfs, &JiLiSetNewVersion::OnCbnSelchangeCombfs)
	ON_CBN_SELCHANGE(IDC_COMBbeipin, &JiLiSetNewVersion::OnCbnSelchangeCombbeipin)
	ON_CBN_SELCHANGE(IDC_COMBjilitype, &JiLiSetNewVersion::OnCbnSelchangeCombjilitype)
	ON_EN_CHANGE(IDC_jilinewcycle, &JiLiSetNewVersion::OnEnChangejilinewcycle)
	ON_EN_CHANGE(IDC_jilinewmintime, &JiLiSetNewVersion::OnEnChangejilinewmintime)
	ON_BN_CLICKED(IDC_jilisetCheckEQ, &JiLiSetNewVersion::OnBnClickedjilisetcheckeq)
	ON_EN_CHANGE(IDC_jilieqPath, &JiLiSetNewVersion::OnEnChangejilieqpath)
	ON_BN_CLICKED(IDC_BUTTON6, &JiLiSetNewVersion::SpkCalibration)
END_MESSAGE_MAP()


// JiLiSetNewVersion 消息处理程序


HBRUSH JiLiSetNewVersion::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (pWnd->GetDlgCtrlID() == IDC_stimdrawnew)
	{
		/*CBrush bm;
		pDC->SetBkColor(RGB(255, 255, 255));
		bm.CreateSolidBrush(RGB(255, 255, 255));
		CRect rc;
		pWnd->GetClientRect(rc);
		FillRect(pWnd->GetDC()->GetSafeHdc(),&rc , bm);
		
		return bm;*/
	}

	COLORREF clr;
	if (IDC_STATIC_TIPS == pWnd->GetDlgCtrlID())
	{
		clr = RGB(0, 0, 255);
		pDC->SetTextColor(clr);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//拦截消息
BOOL JiLiSetNewVersion::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->hwnd == m_listJili.m_hWnd)
	{

		if (pMsg->message == WM_LBUTTONDBLCLK)
		{
			onDbClicklist();
			//AfxMessageBox(_T("ok"));
		}
		else if (pMsg->message==WM_RBUTTONUP)
		{

			onRClicklist();

		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}



void JiLiSetNewVersion::onRClicklist()
{

	CMenu menu;
	POINT  pt{ 0 };
	GetCursorPos(&pt);
	menu.LoadMenuW(IDR_MENUJiliSet);

	menu.GetSubMenu(0)->TrackPopupMenu(0, pt.x, pt.y, this);



}

void JiLiSetNewVersion::onDbClicklist()
{
	CString name;
	m_listJili.GetText(m_listJili.GetCurSel(), name);
	

	data.clear();
	for (auto it = mapData.begin(); it != mapData.end(); it++)
	{
		if (it->first == name)
		{
			for (size_t i = 0; i < _msize(it->second)/4; i++)
			{
				data.push_back(it->second[i]);
			}
			break;
		}
	}

	double time = 1.0*data.size() / fs;

	playtime = int(time * 1000) / 1000.0;

	CRect rc;//绘图区域
	GetDlgItem(IDC_stimdrawnew)->GetClientRect(rc);

	CRect rc2(0, rc.top, rc.right + 80, rc.bottom + 90);
	rc.bottom += 90;
	InvalidateRect(rc2);
	UpdateData(FALSE);
}


void JiLiSetNewVersion::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	drawGroupBox(pwnd, &dc);

	CRect rc;
	pwnd = GetDlgItem(IDC_stimdrawnew);

	pwnd->GetWindowRect(rc);
	ScreenToClient(rc);
	CBrush brush(RGB(0, 0 ,0));
	dc.FillRect(rc, &brush);

	thread thread_draw_bitmap(&JiLiSetNewVersion::DrawThreadBitMap, this);
	thread_draw_bitmap.detach();
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	//return;
	////drawWave(GetDlgItem(IDC_stimdrawnew)->GetDC());
	//
	//CDC* dc1 = GetDlgItem(IDC_stimdrawnew)->GetDC();
	//CRect rc;//绘图区域
	//GetDlgItem(IDC_stimdrawnew)->GetClientRect(rc);

	//BOOL b = 0;
	//CBitmap membitmap;
	//CDC memDC;

	//b = memDC.CreateCompatibleDC(dc1);
	//b = membitmap.CreateCompatibleBitmap(dc1, rc.Width(), rc.Height());
	//memDC.SelectObject(membitmap);

	//double perpointlen = 1.0*rc.Width() / data.size();//每个信号的点占用绘图实际的长度
	//double jiange = perpointlen;//每个信号的显示间隔长度

	//if (data.size() == 0)
	//	return;


	////Y根据显示的高度计算   需要显示的信号幅值
	//size_t v = voltage * pow(2, 31)*sqrt(2);
	//double perpointhight = 1.0*rc.Height() / 2.0 / v;
	//int ymove = rc.Height() / 2.0;


	//vector<int> tempdata;
	//tempdata.resize(data.size());
	//for (size_t i = 0; i < data.size() - 1; i++)
	//{
	//	tempdata[i] = data[i] * perpointhight;

	//}

	//CPen pen;
	//pen.CreatePen(0, 1, RGB(0, 0, 255));
	//CPen* oldpen = (CPen*)memDC.SelectObject(pen);
	//for (size_t i = 0; i < tempdata.size() - 1; i++)
	//{

	//	/*MoveToEx(memDC, i*jiange, tempdata[i] + ymove, NULL);
	//	LineTo(memDC, (i + 1)*jiange, tempdata[i + 1] + ymove);*/

	//	memDC.MoveTo(i*jiange, tempdata[i] + ymove);
	//	memDC.LineTo((i + 1)*jiange, tempdata[i + 1] + ymove);


	//}
	//memDC.TextOutW(0,0, CString("asdasfsfd"));

	//memDC.MoveTo(0, 50);
	//memDC.LineTo(10, 20);
	//memDC.MoveTo(30, 10);
	//memDC.LineTo(40, 20);

	//dc1->BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);



}

void JiLiSetNewVersion::drawWave(CDC* dc)
{
	if (data.size() == 0)
		return;

	//UpdateData(TRUE);
	//CDC* dc = GetDlgItem(IDC_stimdrawnew)->GetDC();

	CPen pen;
	pen.CreatePen(0, 1, RGB(0, 0, 255));
	CPen* oldpen = (CPen*)dc->SelectObject(pen);

	/********************背景色设置**************************/
	CRect rc;//绘图区域
	GetDlgItem(IDC_stimdrawnew)->GetClientRect(rc);
	CBrush bm;
	bm.CreateSolidBrush(RGB(255, 255, 255));
	//FillRect(dc->GetSafeHdc(), &rc, bm);

	/********************绘图测试数据**************************/
	/*CRect rc1(0, 10, 0, 10);
	dc->DrawText(CString("asdcsadcsdvcds"), 1, &rc1, 0);
	dc->TextOutW(100, 20, CString("wwwwwwwwww"));
	MoveToEx(*dc, 0, 0, NULL);
	LineTo(*dc, 10, 100);*/


	//绘制一个正弦信号
	/*int f = 1000;
	int v = voltage*100000000;*/
	/*data.resize(8000);
	for (size_t i = 0; i < 8000; i++)
	{
	if (i<=1000)
	data[i] = v*sin(2.0*3.1415926*f*i / fs);
	else if (i>1000&&i<=2000)
	data[i] = v*sin(4.0*3.1415926*f*i / fs);
	else if (i>2000 && i<=4000)
	data[i] = v*sin(6.0*3.1415926*f*i / fs);
	else if (i>4000 && i<8000)
	data[i] = v*sin(10.0*3.1415926*f*i / fs);
	}*/


	/**************************信号显示接口**********************************/
	//1.确定显示区域大小 

	//显示区域的x y坐标换算

	//x 根据显示的宽度计算   需要显示的信号长度
	double perpointlen = 1.0*rc.Width() / data.size();//每个信号的点占用绘图实际的长度
	double jiange = perpointlen;//每个信号的显示间隔长度




	//Y根据显示的高度计算   需要显示的信号幅值
	int v = voltage * pow(2, 31)*sqrt(2);
	double perpointhight = 1.0*rc.Height() / 2.0 / v;
	int ymove = rc.Height() / 2.0;


	vector<int> tempdata;
	tempdata.resize(data.size());
	for (size_t i = 0; i < data.size() - 1; i++)
	{
		tempdata[i] = data[i] * perpointhight;

	}







	//将激励信号绘制到内存



	BOOL b = 0;
	CBitmap membitmap;
	CDC memDC;

	b = memDC.CreateCompatibleDC(dc);
	b = membitmap.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	memDC.SelectObject(membitmap);


	for (size_t i = 0; i < tempdata.size() - 1; i++)
	{

		/*MoveToEx(memDC, i*jiange, tempdata[i] + ymove, NULL);
		LineTo(memDC, (i + 1)*jiange, tempdata[i + 1] + ymove);*/

		memDC.MoveTo(i*jiange, tempdata[i] + ymove);
		memDC.LineTo((i + 1)*jiange, tempdata[i + 1] + ymove);
	}

	dc->BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

}

//mfc 禁止在线程中操作界面数据
void JiLiSetNewVersion::DrawThreadBitMap()
{

	
	if (data.size() == 0)
	{
		return;
	}

	

	//UpdateData(TRUE);
	CDC* dc = GetDlgItem(IDC_stimdrawnew)->GetDC();

	CPen pen;
	pen.CreatePen(0, 1, RGB(0, 0, 255));
	CPen* oldpen = (CPen*)dc->SelectObject(pen);

	/********************背景色设置**************************/
	//CRect rc;//绘图区域
	//GetDlgItem(IDC_stimdrawnew)->GetClientRect(rc);
	CBrush bm;
	bm.CreateSolidBrush(RGB(255, 255, 255));
	//FillRect(dc->GetSafeHdc(), &rc, bm);

	/********************绘图测试数据**************************/
	/*CRect rc1(0, 10, 0, 10);
	dc->DrawText(CString("asdcsadcsdvcds"), 1, &rc1, 0);
	dc->TextOutW(100, 20, CString("wwwwwwwwww"));
	MoveToEx(*dc, 0, 0, NULL);
	LineTo(*dc, 10, 100);*/


	//绘制一个正弦信号
	/*int f = 1000;
	int v = voltage*100000000;*/
	/*data.resize(8000);
	for (size_t i = 0; i < 8000; i++)
	{
		if (i<=1000)
		  data[i] = v*sin(2.0*3.1415926*f*i / fs);
		else if (i>1000&&i<=2000)
			data[i] = v*sin(4.0*3.1415926*f*i / fs);
		else if (i>2000 && i<=4000)
			data[i] = v*sin(6.0*3.1415926*f*i / fs);
		else if (i>4000 && i<8000)
			data[i] = v*sin(10.0*3.1415926*f*i / fs);
	}*/


	/**************************信号显示接口**********************************/
	//1.确定显示区域大小 

	//显示区域的x y坐标换算

	//x 根据显示的宽度计算   需要显示的信号长度
	double perpointlen = 1.0*m_rc.Width() / data.size();//每个信号的点占用绘图实际的长度
	double jiange = perpointlen;//每个信号的显示间隔长度

	


	//Y根据显示的高度计算   需要显示的信号幅值
  	double v = voltage * pow(2, 31)*sqrt(2)/vp;

	if (m_eq)
	{
		v = maxeq * pow(2, 31)*sqrt(2) / vp;
	}

	/*if (CString(driver) == "WDM")
	{
		
		v = 1.0*voltage*sqrt(2);
	}*/

	double perpointhight = 1.0*m_rc.Height() / 2.0 / v;
	int ymove = m_rc.Height() / 2.0;


	vector< int> tempdata;
	tempdata.resize(data.size());
	for (size_t i = 0; i < data.size()-1; i++)
	{
		tempdata[i] = -data[i] * perpointhight;
		
	}







	//将激励信号绘制到内存
	
	
	
	BOOL b = 0;
	CBitmap membitmap;
	CDC memDC;
	
	b=memDC.CreateCompatibleDC(dc);
	b = membitmap.CreateCompatibleBitmap(dc, m_rc.Width(), m_rc.Height());
	memDC.SelectObject(membitmap);
	
	pen.DeleteObject();
	pen.CreatePen(0, 1, RGB(56, 94, 15));
	memDC.SelectObject(pen);
	//memDC.FillRect(rc, &bm);
	for (size_t i = 0; i < tempdata.size() - 1; i++)
	{

		MoveToEx(memDC, i*jiange, tempdata[i] + ymove, NULL);
		LineTo(memDC, (i + 1)*jiange, tempdata[i + 1] + ymove);

		/*memDC.MoveTo(i*jiange, tempdata[i] + ymove);
		memDC.LineTo((i + 1)*jiange, tempdata[i + 1] + ymove);*/
	}
	
	dc->BitBlt(0, 0, m_rc.Width(), m_rc.Height(), &memDC, 0, 0, SRCCOPY);
	
	ReleaseDC(&memDC);
}

BOOL JiLiSetNewVersion::OnInitDialog()
{
	UpdateData(FALSE);

	for (int i = 0; i < GetStimCount(); i++)
	{
		stimParam* pStim = GetStimParam(i);
		m_listJili.AddString(pStim->sName);
	}

	CRect rc;
	GetDlgItem(IDC_editjilisetnewstop)->GetClientRect(&rc);
	(CComboBox*)GetDlgItem(IDC_COMBfs)->SendMessage(CB_SETITEMHEIGHT, -1, rc.Height() - 1);
	(CComboBox*)GetDlgItem(IDC_COMBbeipin)->SendMessage(CB_SETITEMHEIGHT, -1, rc.Height() - 1);
	(CComboBox*)GetDlgItem(IDC_COMBjilitype)->SendMessage(CB_SETITEMHEIGHT, -1, rc.Height() - 1);

	
	((CComboBox*)GetDlgItem(IDC_COMBbeipin))->InsertString(0, _T("1/3 Oct"));
	((CComboBox*)GetDlgItem(IDC_COMBbeipin))->InsertString(1, _T("1/6 Oct"));
	((CComboBox*)GetDlgItem(IDC_COMBbeipin))->InsertString(2, _T("1/12 Oct"));
	((CComboBox*)GetDlgItem(IDC_COMBbeipin))->InsertString(3, _T("1/24 Oct"));
	((CComboBox*)GetDlgItem(IDC_COMBbeipin))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_COMBjilitype))->InsertString(0, _T("扫频"));
	((CComboBox*)GetDlgItem(IDC_COMBjilitype))->InsertString(1, _T("定频"));
	((CComboBox*)GetDlgItem(IDC_COMBjilitype))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_COMBfs))->InsertString(0, _T("44100"));
	((CComboBox*)GetDlgItem(IDC_COMBfs))->InsertString(1, _T("96000"));
	((CComboBox*)GetDlgItem(IDC_COMBfs))->InsertString(2, _T("192000"));
	((CComboBox*)GetDlgItem(IDC_COMBfs))->InsertString(3, _T("48000"));
	((CComboBox*)GetDlgItem(IDC_COMBfs))->SetCurSel(3);


	//OnBnClickedButtonUpdate();

	GetDlgItem(IDC_stimdrawnew)->GetClientRect(m_rc);


	//m_listJili.AddString(CString("系统保存激励"));

	//vector<CString> temp=devicemanage.getInnerNames_OutCHS();

	/*for (size_t i = 0; i < temp.size(); i++)
	{
		m_cmbPlayChs.AddString(temp[i]);
	}*/
	for (size_t i = 0; i < GetDevCount(false); i++)
	{
		devParam* pDev = GetDevParam(i, false);
		m_cmbPlayChs.AddString(pDev->sName);
		m_cmbPlayChs.SetCurSel(0);
	}

	if (GetStimCount() > 0)
	{
		stimParam* pStim = GetStimParam(0);
		voltage = pStim->nVoltage;
		startfs = pStim->nFrequencyStart;
		stopfs = pStim->nFrequencyStop;

		int nIndex = -1;
		CString sText;
		for (int i = 0; i < ((CComboBox*)GetDlgItem(IDC_COMBfs))->GetCount(); i++)
		{
			sText = _T("");
			((CComboBox*)GetDlgItem(IDC_COMBfs))->GetLBText(i, sText);
			if (_ttof(sText) == pStim->nSamplingRate)
			{
				nIndex = i;
				break;
			}
		}

		((CComboBox*)GetDlgItem(IDC_COMBfs))->SetCurSel(nIndex);
		((CComboBox*)GetDlgItem(IDC_COMBbeipin))->SetCurSel(pStim->nOctave);
		((CComboBox*)GetDlgItem(IDC_COMBjilitype))->SetCurSel(pStim->nType);
		cycle = pStim->nCycle;
		mintime = pStim->nDuration;
		m_eq = pStim->bEQEnable;
		m_eqpath = pStim->sEQPath;

		UpdateData(FALSE);
	}

	m_listJili.SetSel(0);
	m_bInitFinish = true;
	return TRUE;
}

void JiLiSetNewVersion::GenerateV(double v, double f)
{
	if (m_hWnd)
	{
	
		voltage = v;
		startfs = 1000;
		stopfs = 1000;
		cycle = 10000;
		mintime = 1000;
		m_eq = FALSE;
		//((CComboBox*)GetDlgItem(IDC_COMBfs))->SetCurSel(0);
		fs = f;
		((CComboBox*)GetDlgItem(IDC_COMBbeipin))->SetCurSel(0);

		UpdateData(FALSE);

		OnBnClickedButtonUpdate();
	}
	else
	{
		AfxMessageBox(_T("请打开激励设置模块..."));
	}
}

void JiLiSetNewVersion::LoadTxt(vector<double>* data, CString path1)
{

	//CString path1("D:\\TestRecord\\response.txt");
	ifstream outfile1;
	outfile1.open(path1, std::ios::in);

	double val = 0;
	string line;
	double val2 = 0;
	while (outfile1.peek() != EOF)
	{
		//std::getline(outfile1, line);
		outfile1 >> val2 >> val;
		if (val > maxeq)
		{
			maxeq = val;
		}

		m_listEQ.push_back(val);
	}


	outfile1.close();
}

void JiLiSetNewVersion::LoadCsv(vector<double>* data, CString path1)
{

	
	ifstream outfile1;
	outfile1.open(path1, std::ios::in);

	double val = 0;
	string line;
	std::getline(outfile1, line);
	std::getline(outfile1, line);
	
	/*int pos1 = line.find('\t');
	m_listEQ.push_back(atof(line.substr(0, pos1).c_str()));*/
	int pos2 = 0;

	while ((pos2=line.find(','))!=-1 )
	{
		val = atof(line.substr(0, pos2).c_str());
		if (maxeq < val)
		{
			maxeq = val;
		}
		m_listEQ.push_back(val);
		line=line.substr(pos2+1, line.length());
		
	}
	val = atof(line.c_str());
	if (maxeq < val)
	{
		maxeq = val;
	}
	m_listEQ.push_back(val);
	outfile1.close();
}

void JiLiSetNewVersion::LoadSenoFSpk()
{
	ifstream outfile1;
	m_senofSpk.clear();
	m_senofSpk2.clear();
	m_senofSpk3.clear();
	m_senofSpk4.clear();
	for (size_t i = 0; i < 4; i++)
	{
		if (i==0)
		   outfile1.open("./m_senofSpk.csv", std::ios::in);
		else if (i == 1)
		{
			outfile1.open("./m_senofSpk2.csv", std::ios::in);
		}
		else if (i == 2)
		{
			outfile1.open("./m_senofSpk3.csv", std::ios::in);
		}
		else if (i == 3)
		{
			outfile1.open("./m_senofSpk4.csv", std::ios::in);
		}

		double val = 0;
		string line, linefs;
		std::getline(outfile1, linefs);
		std::getline(outfile1, line);
		
		if (i == 0)
		{
			loadOffsetFs(linefs, offsetFs_mic);
		}
		loadSpkOffsetval(line, i);

		/*int pos2 = 0;
		while ((pos2 = line.find(',')) != -1)
		{
			val = atof(line.substr(0, pos2).c_str());

			if (i == 0)
				m_senofSpk.push_back(val);
			else if (i == 1)
			{
				m_senofSpk2.push_back(val);
			}
			else if (i == 2)
			{
				m_senofSpk3.push_back(val);
			}
			else if (i == 3)
			{
				m_senofSpk4.push_back(val);
			}
			line = line.substr(pos2 + 1, line.length());

		}
		val = atof(line.c_str());
		if (i==0)
		   m_senofSpk.push_back(val);
		else if (i == 1)
		{
			m_senofSpk2.push_back(val);
		}
		else if (i == 2)
		{
			m_senofSpk3.push_back(val);
		}
		else if (i == 3)
		{
			m_senofSpk4.push_back(val);
		}*/


		outfile1.close();
	}
	
}

void JiLiSetNewVersion::loadSpkOffsetval(string line, int i)
{
	double val = 0;
	int pos2 = 0;
	while ((pos2 = line.find(',')) != -1)
	{
		val = atof(line.substr(0, pos2).c_str());

		if (i == 0)
			m_senofSpk.push_back(val);
		else if (i == 1)
		{
			m_senofSpk2.push_back(val);
		}
		else if (i == 2)
		{
			m_senofSpk3.push_back(val);
		}
		else if (i == 3)
		{
			m_senofSpk4.push_back(val);
		}
		line = line.substr(pos2 + 1, line.length());

	}
	val = atof(line.c_str());
	if (i == 0)
		m_senofSpk.push_back(val);
	else if (i == 1)
	{
		m_senofSpk2.push_back(val);
	}
	else if (i == 2)
	{
		m_senofSpk3.push_back(val);
	}
	else if (i == 3)
	{
		m_senofSpk4.push_back(val);
	}
}


void JiLiSetNewVersion::LoadSenoFMic()
{
	ifstream outfile1;
	m_senofMic.clear();
	m_senofMic2.clear();
	m_senofMic3.clear();
	m_senofMic4.clear();
	for (size_t i = 0; i < 4; i++)
	{
		if (i == 0)
			outfile1.open("./m_senofspkTest.csv", std::ios::in);
		else if (i == 1)
		{
			outfile1.open("./m_senofspkTest2.csv", std::ios::in);
		}
		else if (i == 2)
		{
			outfile1.open("./m_senofspkTest3.csv", std::ios::in);
		}
		else if (i == 3)
		{
			outfile1.open("./m_senofspkTest4.csv", std::ios::in);
		}

		double val = 0;
		string line, linefs;
		std::getline(outfile1, linefs);
		std::getline(outfile1, line);
		
		if (i == 0)
		{
			loadOffsetFs(linefs, offsetFs_spk);
		}

		loadMicOffsetval(line, i);

		/*int pos2 = 0;
		while ((pos2 = line.find(',')) != -1)
		{
			val = atof(line.substr(0, pos2).c_str());

			if (i == 0)
				m_senofMic.push_back(val);
			else if (i == 1)
			{
				m_senofMic2.push_back(val);
			}
			else if (i == 2)
			{
				m_senofMic3.push_back(val);
			}
			else if (i == 3)
			{
				m_senofMic4.push_back(val);
			}
			line = line.substr(pos2 + 1, line.length());

		}
		val = atof(line.c_str());
		if (i == 0)
			m_senofMic.push_back(val);
		else if (i == 1)
		{
			m_senofMic2.push_back(val);
		}
		else if (i == 2)
		{
			m_senofMic3.push_back(val);
		}
		else if (i == 3)
		{
			m_senofMic4.push_back(val);
		}*/


		outfile1.close();
	}

}


void JiLiSetNewVersion::loadMicOffsetval(string line, int i)
{

	int pos2 = 0;
	double val = 0;
	while ((pos2 = line.find(',')) != -1)
	{
		val = atof(line.substr(0, pos2).c_str());

		if (i == 0)
			m_senofMic.push_back(val);
		else if (i == 1)
		{
			m_senofMic2.push_back(val);
		}
		else if (i == 2)
		{
			m_senofMic3.push_back(val);
		}
		else if (i == 3)
		{
			m_senofMic4.push_back(val);
		}
		line = line.substr(pos2 + 1, line.length());

	}
	val = atof(line.c_str());
	if (i == 0)
		m_senofMic.push_back(val);
	else if (i == 1)
	{
		m_senofMic2.push_back(val);
	}
	else if (i == 2)
	{
		m_senofMic3.push_back(val);
	}
	else if (i == 3)
	{
		m_senofMic4.push_back(val);
	}
}
void JiLiSetNewVersion::loadOffsetFs(string line, vector<double>& vFsList)
{
	vFsList.clear();
	int pos2 = 0;
	double val = 0;
	while ((pos2 = line.find(',')) != -1)
	{
		val = atof(line.substr(0, pos2).c_str());
		vFsList.push_back(val);
		line = line.substr(pos2 + 1, line.length());
	}
	val = atof(line.c_str());
	vFsList.push_back(val);


}

//CString GetFilePath(LPCTSTR lpFile)
//{
//	CString sFile = CString(lpFile);
//	if (sFile.IsEmpty())
//	{
//		return _T("");
//	}
//	else
//	{
//		if (sFile.Find(_T(":")) < 0)
//		{
//			sFile = GetAppPath() + sFile;
//		}
//	}
//
//	sFile.Replace(_T("/"), _T("\\"));
//	sFile.Replace(_T("\\\\"), _T("\\"));
//
//	return sFile;
//}

void JiLiSetNewVersion::GetEQValues(stimParam* pStim, map<double, double>& mEQValue)
{
	double nVol, nAlarmVol;
	CString sData;
	vector<CString> vList;

	//stimParam* pStim = GetStimParam(GetStimIndex(sName));
	nAlarmVol = ReadDouble(_T("EQVoltage"), _T("AlarmVoltage"), 8, GetFilePath(pStim->sEQPath));
	int nItems = ReadInt(_T("EQVoltage"), _T("Items"), 0, GetFilePath(pStim->sEQPath));
	for (int i = 0; i < nItems; i++)
	{
		sData = ReadStr(_T("EQVoltage"), IntToStr(i), _T(""), GetFilePath(pStim->sEQPath));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		nVol = _ttof(vList[1]);
		if (nVol > nAlarmVol)
			nVol = nAlarmVol;
		mEQValue[_ttof(vList[0])] = nVol;
	}
}


void JiLiSetNewVersion::OnBnClickedButtonUpdate()
{





	if (m_hWnd == NULL)
	{
		AfxMessageBox(_T("激励模块未加载！！"));
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	stimParam* pStim = GetStimParam(nStimSel);

	// TODO:  在此添加控件通知处理程序代码
	CString playstr, recordstr;

	m_cmbPlayChs.GetWindowTextW(playstr);

	int nIndex = GetDevIndex(playstr, false);
	if (nIndex < 0)
	{
		return;
	}

	devParam* pDev = GetDevParam(nIndex, false);

	//playrecordset.GetPlayAndRecord(playstr, recordstr);
	double vpactrul = pDev->nVp; //devicemanage.getoutvp(playstr);
	vp = vpactrul;// 1.45;//ASIO驱动
	if (UpdateData() == 0)
		return;

	/****************输入判断不合法输入**********************/

	/*if (playstr == "")
	{
		AfxMessageBox(_T("播放采集模块，输出通道未选择!"));
		return;
	}*/


	if (startfs > stopfs)
	{
		AfxMessageBox(_T("输入频率错误！开始频率应小于结束频率"));
		return;
	}

	if (voltage > 0.2)
	{
		AfxMessageBox(_T("电压应低于0.2v，请重新设置！"));
		//return;
	}



	data.clear();
	pos.clear();
	maxeq = 0;
	//1.生成激励信号
	//1.1 根据倍频程确定频点
	int curpos=((CComboBox*)GetDlgItem(IDC_COMBbeipin))->GetCurSel();

	vector<double> pinduan;
	getpindian(curpos, pinduan);
	map<double, double> mEQValue;

	if (m_eq)
	{
		GetEQValues(pStim, mEQValue);
		/*if (m_listEQ.size() != pinduan.size())
		{
			AfxMessageBox(_T("EQ文件，频点数错误！"), MB_ICONERROR);
			return;
		}*/
	}
	
	
	//1.2 根据频点集合创建激励信号
	int count = 0;
	double v = voltage*pow(2, 31)*sqrt(2) / vp;//pow(2, 31)

	if (pDev->sDriver.IsEmpty())
	{
		AfxMessageBox(_T("设备管理，输出通道未设置！"));
		return;
	}


	/*if (CString(driver) == "WDM")
	{
		
		v = 1.0*voltage*pow(2, 31)*sqrt(2) / vp;
	}*/
	int f1 = cycle/(mintime *0.001);
	double f;
	char buf[1024];
	sprintf(buf, "%dHz", f1);
	GetDlgItem(IDC_jilif1)->SetWindowTextW(CString(buf));

	//信号前补0
	double offsetTime = 0.05;
	int buling = fs*offsetTime;
	for (size_t i = 0; i < buling; i++)
	{
		data.push_back(0);
	}
	inuse_pindians.clear();
	for (size_t i = 0; i < pinduan.size(); i++)
	{
		f = pinduan[i];
		inuse_pindians.push_back(f);
		int cur_samples = 0;

		if (f <= f1)
		{
			cur_samples = cycle * fs / f;//cycle/f cycle个周期 乘以1/f 一个周期的时间 等于 cycle个周期的时间 每秒的点数fs  总点数 fs*总时间

		}
		else
		{

			cur_samples = round(mintime*0.001 * f)*fs / f;
		}

		if (m_eq)
		{
			if (maxeq < mEQValue[f])
			{
				maxeq = mEQValue[f];
			}

			v = mEQValue[f] * pow(2, 31)*sqrt(2) / vp;
		}

		pos[f] = cur_samples;
		for (size_t j = 0; j <cur_samples; j++)
		{
			data.push_back(1.0*v*sin(2.0 * 3.1415926*f*j / fs));
		}
	}
	buling = fs*0.05;
	for (size_t i = 0; i < buling; i++)
	{
		data.push_back(0);
	}
	int* data1 = new int[data.size()]{0};
	for (size_t i = 0; i < data.size(); i++)
	{
		data1[i] = data[i];
	}

	/*CString sStimFile = ReadStr(_T("Common"), _T("StimulusPath"), _T(""), GetConfigIniFile());
	if (sStimFile.IsEmpty())
	{
		sStimFile = GetAppPath() + _T("StimulusFile\\");
	}
	else
	{
		if (sStimFile.Find(_T(":")) < 0)
		{
			sStimFile = GetAppPath() + sStimFile;
		}

		if (sStimFile[sStimFile.GetLength() - 1] != '\\' && sStimFile[sStimFile.GetLength() - 1] != '/')
		{
			sStimFile += _T("\\");
		}
	}*/

	CString sStimFile = GetPathFromCfg(_T("Common"), _T("StimulusPath"), _T("StimulusFile"));
	sStimFile += pStim->sName + _T(".wav");
	char szFileName[MAX_PATH];
	WCharToChar(sStimFile, szFileName, sizeof(szFileName));//"D:\\TestRecord\\playjili.wav";
	double time = 1.0*data.size() / fs;
		
	playtime = int(time*1000)/1000.0;

	//if (playtime / 60>1)
	//{
	//	AfxMessageBox(_T("激励信号超过1分钟了"));
	//	//return;
	//}
	uint32 bps = 32;//32
	int chs = 1;
	if (fs == 44100)
	{
		chs = 1;

	}
	wavwrite(szFileName, 1, fs, bps, (char*)data1, fs * time * bps / 8);

	delete[] data1;

	//2.创建线程绘图显示
	CRect rc;//绘图区域
	GetDlgItem(IDC_stimdrawnew)->GetClientRect(rc);

	CRect rc2(0, rc.top, rc.right+80, rc.bottom+90);
	rc.bottom += 90;
	InvalidateRect(rc2);
	
	/*thread thread_draw_bitmap(&JiLiSetNewVersion::DrawThreadBitMap, this);
	thread_draw_bitmap.detach();*/


	UpdateData(FALSE);
}

double JiLiSetNewVersion::getPlayTimes()
{
	return playtime;
}

void JiLiSetNewVersion::getpindian(int pos, vector<double>& pindiandata)
{
	int step = 0;
	switch (pos)
	{
	
	case 0:
		//1/3
		step = 24 / 3;

		break;
	case 1:
		//1/6
		step = 24 / 6;
		break;
	case 2:
		//1/12
		step = 24 / 12;
		break;
	case 3:
		//1/24
		step = 24 / 24;
		break;
	default:
		break;
	}


	//1.确定开始频点
	int pos1 = 0;
	int pos2 = 0;
	double vale=findpos(startfs, pos1);
	vale = findpos(stopfs, pos2);

	//2.根据步长确定取频点
	getsteppindian(pos1, step, pos2, pindiandata);
	pindiandata.push_back(vale);

}
void JiLiSetNewVersion::getsteppindian(int startstep, int step, int stoppos, vector<double>& stepdata)
{
	


	for (size_t i = startstep; i < pindians.size(); i += step)
	{
		if (stoppos <= i)
			break;
		stepdata.push_back(pindians[i]);
	}


	

}

double JiLiSetNewVersion::findpos(double inval, int& pos)
{


	double purval=0;
	double min = 0;
	min = abs(inval - pindians[0]);
	purval = pindians[0];
	pos = 0;
	for (size_t i = 1; i < pindians.size(); i++)
	{
		
		if (abs(inval - pindians[i])< min)
		{
			purval = pindians[i];
			min = abs(inval - pindians[i]);
			pos = i;
			
		}
		

	}


	return purval;

}

//激励信号保存
void JiLiSetNewVersion::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码

	CGetInput input;
	input.DoModal();

	CString sName(_T(""));
	input.getjiliName(sName);

	if (!sName.IsEmpty())//这里需要考虑名称重复时，覆盖原来的数据
	{
		stimParam* pStim;
		int nCount = GetStimCount();
		for (int i = 0; i < nCount; i++)
		{
			pStim = GetStimParam(i);
			if (StrCmp(sName, pStim->sName) == 0)
			{
				m_listJili.SetCurSel(i);
				return;
			}
		}

		CString sValue(_T(""));
		AddStimParam(sName);
		sValue.Format(_T("%.3f"), voltage);
		EditStimParam(_T("voltage"), sValue, nCount);
		sValue.Format(_T("%.3f"), startfs);
		EditStimParam(_T("frstart"), sValue, nCount);
		sValue.Format(_T("%.3f"), stopfs);
		EditStimParam(_T("frstop"), sValue, nCount);
		sValue.Format(_T("%.3f"), fs);
		EditStimParam(_T("sample"), sValue, nCount);
		EditStimParam(_T("octave"), IntToStr(((CComboBox*)GetDlgItem(IDC_COMBbeipin))->GetCurSel()), nCount);
		EditStimParam(_T("type"), IntToStr(((CComboBox*)GetDlgItem(IDC_COMBjilitype))->GetCurSel()), nCount);
		EditStimParam(_T("cycle"), IntToStr(cycle), nCount);
		EditStimParam(_T("duration"), IntToStr(mintime), nCount);
		EditStimParam(_T("eqenable"), IntToStr(m_eq), nCount);
		EditStimParam(_T("eqpath"), m_eqpath, nCount);

		m_listJili.AddString(sName);
		m_listJili.SetCurSel(nCount);
		OnBnClickedButtonUpdate();
		/*bool b_findname = false;
		for (auto it = mapData.begin(); it!= mapData.end(); it++)
		{
			if (it->first == name)
			{
				delete[] it->second;
				mapData.erase(it);
				b_findname = true;
				break;
			}
		}

		if (!b_findname)
		    m_listJili.AddString(name);*/

		//int* dat = new int[data.size()];


	
		//

		//for (size_t i = 0; i < data.size(); i++)
		//{
		//	dat[i] = data[i];
		//}
		//mapData[name] = dat;
		////写入wdm文件
		//char fileName1[512]{0};

		//int len1 = WideCharToMultiByte(CP_ACP, 0, name, -1, NULL, 0, NULL, NULL);
		//char* ch = new char[len1 + 1];
		//WideCharToMultiByte(CP_ACP, 0, name, -1, ch, len1, NULL, NULL);
		//sprintf(fileName1, "D:\\TestRecord\\%s.wav", ch);
		//delete ch;
		//double time = 1.0*data.size() / fs;

		//playtime = int(time * 1000) / 1000.0;

		//uint32 bps = 32;//32
		//int chs = 1;
		//if (fs == 44100)
		//{
		//	chs = 1;

		//}
		//wavwrite(fileName1, 1, fs, bps, (char*)dat, fs * time * bps / 8);
	}


}


void JiLiSetNewVersion::OnBnClickedButtonDeljili()
{
	// TODO:  在此添加控件通知处理程序代码
	data.clear();
	CRect rc;//绘图区域
	GetDlgItem(IDC_stimdrawnew)->GetClientRect(rc);

	CRect rc2(0, rc.top, rc.right + 80, rc.bottom + 90);
	rc.bottom += 90;
	InvalidateRect(rc2);

	playtime = 0;
	UpdateData(FALSE);
}

void JiLiSetNewVersion::drawGroupBox(CWnd* wnd, CPaintDC* dc)
{
	CRect rc;

	int dc_id = dc->SaveDC();
	CBrush groupbox_brush(RGB(255, 0, 8));
	CPen pen;
	pen.CreatePen(0, 1, RGB(255, 0, 8));
	//dc->SelectObject(pen);

	wnd->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.top += 14;
	MoveToEx(*dc, rc.left + 100, rc.top, NULL);
	LineTo(*dc, rc.right, rc.top);

	MoveToEx(*dc, rc.right, rc.top, NULL);
	LineTo(*dc, rc.right, rc.bottom);

	MoveToEx(*dc, rc.right, rc.bottom, NULL);
	LineTo(*dc, rc.left - 1, rc.bottom);

	MoveToEx(*dc, rc.left - 1, rc.bottom, NULL);
	LineTo(*dc, rc.left - 1, rc.top);

	MoveToEx(*dc, rc.left - 1, rc.top, NULL);
	LineTo(*dc, rc.left + 19, rc.top);

	dc->RestoreDC(dc_id);
}

void  JiLiSetNewVersion::OnOK()
{
	return;

}

void JiLiSetNewVersion::setVoltage(double v)
{
	voltage = v;
	if (m_hWnd)
	   UpdateData(FALSE);
}
double JiLiSetNewVersion::getVoltage()
{
	if (m_hWnd)
	{
		UpdateData();
	}
	return voltage;
}


void JiLiSetNewVersion::OnBnClickedButtonImportEq()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	if (m_eq)
	{
		m_listEQ.clear();
		//读取文件
		CFileDialog filedia(TRUE);
		//filedia.m_ofn.lpstrFileTitle = _T("选择EQ文件");
		if (filedia.DoModal() != IDOK)
			return;
			
		m_eqpath = filedia.GetPathName();
		CString s = m_eqpath.Right(m_eqpath.GetLength()- m_eqpath.ReverseFind('.')-1);
		if ( s== "csv")
		{
			LoadCsv(&m_listEQ, m_eqpath);
		}
		else if (s == "txt")
		{
			
				LoadTxt(&m_listEQ, m_eqpath);
			
		}
		else
		{
			AfxMessageBox(_T("文件类型错误，使用csv文件..."),MB_ICONERROR);
		}
		
		



	}

	UpdateData(FALSE);
}


void JiLiSetNewVersion::precudeplayWav()
{

	int* data1 = new int[data.size()]{0};
	for (size_t i = 0; i < data.size(); i++)
	{
		data1[i] = data[i];
	}
	char* fileName1 = "D:\\TestRecord\\playjili.wav";
	double time = 1.0*data.size() / fs;

	playtime = int(time * 1000) / 1000.0;

	uint32 bps = 32;//32
	int chs = 1;
	if (fs == 44100)
	{
		chs = 1;

	}
	wavwrite(fileName1, 1, fs, bps, (char*)data1, fs * time * bps / 8);

	delete[] data1;
}


void JiLiSetNewVersion::OnBnClickedButtonplayJili()
{
	// TODO:  在此添加控件通知处理程序代码

	CString playstr;

	m_cmbPlayChs.GetWindowTextW(playstr);

	precudeplayWav();

	devicemanage.play(playstr,0);
}

void JiLiSetNewVersion::OnDelJili()
{
	CString name;
	m_listJili.GetText(m_listJili.GetCurSel(), name);

	for (auto it = mapData.begin(); it != mapData.end(); it++)
	{
		if (it->first == name)
		{
			delete[] it->second;
			m_listJili.DeleteString(m_listJili.GetCurSel());
			//m_listJili.DeleteItem
			mapData.erase(it);
			break;
		}
	}

}

void JiLiSetNewVersion::OnshowJili()
{

	onDbClicklist();

}

void JiLiSetNewVersion::OnplayJili()
{
	OnBnClickedButtonplayJili();
}
vector<CString> JiLiSetNewVersion::getJiliname()
{
	vector<CString> list;

	for (auto it = mapData.begin(); it != mapData.end(); it++)
	{
		list.push_back(it->first);
	}

	return list;

}


void JiLiSetNewVersion::OnSelchangeList2()
{
	// TODO:  在此添加控件通知处理程序代码
	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		return;
	}

	stimParam* pStim = GetStimParam(nStimSel);
	voltage = pStim->nVoltage;
	startfs = pStim->nFrequencyStart;
	stopfs = pStim->nFrequencyStop;

	int nIndex = -1;
	CString sText;
	for (int i = 0; i < ((CComboBox*)GetDlgItem(IDC_COMBfs))->GetCount(); i++)
	{
		sText = _T("");
		((CComboBox*)GetDlgItem(IDC_COMBfs))->GetLBText(i, sText);
		if (_ttof(sText) == pStim->nSamplingRate)
		{
			nIndex = i;
			break;
		}
	}

	((CComboBox*)GetDlgItem(IDC_COMBfs))->SetCurSel(nIndex);
	((CComboBox*)GetDlgItem(IDC_COMBbeipin))->SetCurSel(pStim->nOctave);
	((CComboBox*)GetDlgItem(IDC_COMBjilitype))->SetCurSel(pStim->nType);
	cycle = pStim->nCycle;
	mintime = pStim->nDuration;
	m_eq = pStim->bEQEnable;
	m_eqpath = pStim->sEQPath;

	UpdateData(FALSE);
}


void JiLiSetNewVersion::OnChangeEditjilisetnewv()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	CString sValue(_T(""));
	sValue.Format(_T("%.3f"), voltage);
	EditStimParam(_T("voltage"), sValue, nStimSel);
}


void JiLiSetNewVersion::OnChangeEditjilisetnewstart()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	CString sValue(_T(""));
	sValue.Format(_T("%.3f"), startfs);
	EditStimParam(_T("frstart"), sValue, nStimSel);
}


void JiLiSetNewVersion::OnChangeEditjilisetnewstop()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	CString sValue(_T(""));
	sValue.Format(_T("%.3f"), stopfs);
	EditStimParam(_T("frstop"), sValue, nStimSel);
}


void JiLiSetNewVersion::OnCbnSelchangeCombfs()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	CString sValue(_T(""));
	sValue.Format(_T("%.3f"), fs);
	EditStimParam(_T("sample"), sValue, nStimSel);
}


void JiLiSetNewVersion::OnCbnSelchangeCombbeipin()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	EditStimParam(_T("octave"), IntToStr(((CComboBox*)GetDlgItem(IDC_COMBbeipin))->GetCurSel()), nStimSel);
}


void JiLiSetNewVersion::OnCbnSelchangeCombjilitype()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	EditStimParam(_T("type"), IntToStr(((CComboBox*)GetDlgItem(IDC_COMBjilitype))->GetCurSel()), nStimSel);
}


void JiLiSetNewVersion::OnEnChangejilinewcycle()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	EditStimParam(_T("cycle"), IntToStr(cycle), nStimSel);
}


void JiLiSetNewVersion::OnEnChangejilinewmintime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	EditStimParam(_T("duration"), IntToStr(mintime), nStimSel);
}


void JiLiSetNewVersion::OnBnClickedjilisetcheckeq()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	EditStimParam(_T("eqenable"), IntToStr(m_eq), nStimSel);
}


void JiLiSetNewVersion::OnEnChangejilieqpath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_bInitFinish)
	{
		return;
	}

	int nStimSel = m_listJili.GetCurSel();
	if (nStimSel < 0)
	{
		AfxMessageBox(_T("请选择激励！"));
		return;
	}

	UpdateData();
	EditStimParam(_T("eqpath"), m_eqpath, nStimSel);
}


void JiLiSetNewVersion::SpkCalibration()
{
	CString playstr;

	m_cmbPlayChs.GetWindowTextW(playstr);

	/*CInputDialg inputdlg(true);
	inputdlg.setCaption(CString("输入通道："));
	inputdlg.setunit(_T(""));
	inputdlg.DoModal();*/
	

	vector<devParam> vCali;
	int nItems = ReadInt(_T("SpkCalibration"), _T("Items"), 0, GetScriptIniFile());
	for (int i = 0; i < nItems; i++)
	{
		CString sDevName = ReadStr(_T("SpkCalibration"), IntToStr(i), _T(""), GetScriptIniFile());
		if (sDevName.IsEmpty())
		{
			continue;
		}

		int nIndex = GetDevIndex(sDevName, true);
		devParam* pDev = GetDevParam(nIndex, true);
		vCali.push_back(*pDev);
	}

	if (vCali.size() == 0)
	{
		AfxMessageBox(_T("没有配置校准项..."));
		return;
	}
	CString inchsName = vCali[0].sName;//inputdlg.getV2();
	int inchs = devicemanage.getChsIndexByName(inchsName, true);//ASIO index

	int outchs = devicemanage.getChsIndexByName(playstr, false);
	char* drivername = NULL;
	devicemanage.getDriverType(playstr, &drivername);

	if (drivername == NULL)
		return ;

	if (strcmp(drivername, "WDM") == 0)
	{
		AfxMessageBox(_T("不支持WDM驱动,选择ASIO..."));
		return ;
		outchs = devicemanage.getMMEChsIndexByName(playstr, false);
		inchs = devicemanage.getMMEChsIndexByName(inchsName, true);
	}
	else
	{


	}

	//获取配置文件的激励
	vector<double> pinadianForSave;
	for (std::map<double, int>::iterator it = pos.begin(); it != pos.end(); it++)
	{
		pinadianForSave.push_back(it->first);
	}

	//播放激励
	//麦克风采集

	int * tempdata2 = new int[1];
	int **indata = &tempdata2;
	int ** data2 = NULL;
	int* palydata = new int[this->data.size()];
	for (size_t i = 0; i < this->data.size(); i++)
	{
		palydata[i] = this->data[i];
	}


	double* outdata = NULL;
	if (strcmp(drivername, "WDM") == 0)
	{
		devicemanage.calSpkMME(inchsName, palydata, outchs, &outdata, playtime);
	}
	else
	{
		devicemanage.calSpkASIO(playstr, &data2);
	}
	delete[] palydata;
	//devicemanage.calibrationMic(inchsName, &indata, fs, 5, &data2, outchs, inchs, devicemanage.getoutvp(playstr));



	//计算fr值
	CAudioAlgorithm audioAlg;


	m_senofSpk.clear();
	m_senofSpk2.clear();
	m_senofSpk3.clear();
	m_senofSpk4.clear();
	for (size_t i = 0; i < vCali.size(); i++)//这个循环变量就是ASIO录音通道的索引，默认，针对奋达项目，内部声卡，
	{
		audioAlg.setFrMode(true);
		audioAlg.AudioAlgorithmInit();
		audioAlg.setSen(/*devicemanage.getSensitivity(inchsName)*/vCali[i].nSensitivity);
		audioAlg.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		audioAlg.setDeviceInputVpval(/*devicemanage.getinputvp(inchsName)*/vCali[i].nVp);
		audioAlg.setIsComputerNorse(true);
		CAlgParam paramalg;
		paramalg.recordIndex = i;
		audioAlg.setAlgParam(paramalg);

		if (strcmp(drivername, "WDM") == 0)
		{
			/*double* res = new double[_msize(indata[0]) / 4];
			for (size_t i = 0; i < _msize(indata[0]) / 4; i++)
			{
			res[i] = indata[0][i];

			}*/
			audioAlg.setDriverMode(1);
			audioAlg.Alg_Fr(outdata, fs);
		}
		else
		{
			double* asiodata = new double[_msize(data2[i]) / 4];
			std::ofstream outfile2;
			CString path1(_T("./response.csv"));
			outfile2.open(path1, std::ios::out | std::ios::app);
			for (size_t j = 0; j < _msize(data2[i]) / 4; j++)
			{
				asiodata[j] = data2[i][j];
				outfile2 << data2[i][j] << endl;
			}
			outfile2.close();
			uint32 bps = 32;//32
			int chs = 1;
			if (fs == 44100)
			{
				chs = 1;

			}
			wavwrite("D:\\TestRecord\\wdmRecord1.wav", 1, fs, bps, (char*)data2[i], fs * playtime * bps / 8);

			//求出实际声压

			audioAlg.Alg_Fr(asiodata, fs);

			delete[] asiodata;
			asiodata = NULL;
			delete[] data2[i];
			data2[i] = NULL;
			for (size_t j = 0; j < audioAlg.fr_data.size(); j++)
			{
				if (i == 0)
					m_senofSpk.push_back(audioAlg.fr_data[j] * 1000.0 / vCali[i].nSensitivity/*devicemanage.getSensitivity(inchsName)*/);//EQ电压，修改电压使得每个点
				else if (i == 1)
				{
					m_senofSpk2.push_back(audioAlg.fr_data[j] * 1000.0 / vCali[i].nSensitivity/*devicemanage.getSensitivity(inchsName)*/);
				}
				else if (i == 2)
				{
					m_senofSpk3.push_back(audioAlg.fr_data[j] * 1000.0 / vCali[i].nSensitivity/*devicemanage.getSensitivity(inchsName)*/);
				}
				else if (i == 3)
				{
					m_senofSpk4.push_back(audioAlg.fr_data[j] * 1000.0 / vCali[i].nSensitivity/*devicemanage.getSensitivity(inchsName)*/);
				}
				//达到1pa  94db
			}
			audioAlg.fr_data.clear();
			std::ofstream outfile1;
			if (i == 0)
			{
				CString path1(_T("./m_senofSpk.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk[k] << ",";

				}
			}
			else if (i == 1)
			{
				CString path1(_T("./m_senofSpk2.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk2[k] << ",";

				}
			}
			else if (i == 2)
			{
				CString path1(_T("./m_senofSpk3.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk3[k] << ",";

				}
			}
			else if (i == 3)
			{
				CString path1(_T("./m_senofSpk4.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk4[k] << ",";

				}
			}
			outfile1 << endl;
			outfile1.close();

		}
	}

	AfxMessageBox(_T("声压校准完成..."));
}

void JiLiSetNewVersion::OnBnClickedButton_CalSPK()
{
	// TODO:  在此添加控件通知处理程序代码

	//选择录音通道
	CString playstr;

	m_cmbPlayChs.GetWindowTextW(playstr);

	CInputDialg inputdlg(true);
	inputdlg.setCaption(CString("输入通道："));
	inputdlg.setunit(_T(""));
	inputdlg.DoModal();
	CString inchsName = inputdlg.getV2();

	
	int inchs = devicemanage.getChsIndexByName(inchsName, true);//ASIO index

	int outchs = devicemanage.getChsIndexByName(playstr, false);
	char* drivername = NULL;
	devicemanage.getDriverType(playstr, &drivername);

	if (drivername == NULL)
		return;

	if (strcmp(drivername, "WDM") == 0)
	{
		AfxMessageBox(_T("不支持WDM驱动,选择ASIO..."));
		return;
		outchs = devicemanage.getMMEChsIndexByName(playstr, false);
		inchs = devicemanage.getMMEChsIndexByName(inchsName, true);
	}
	else
	{
		

	}

	//播放激励
	//麦克风采集

	int * tempdata2 = new int[1];
	int **indata = &tempdata2;
	int ** data2 = NULL;
	int* palydata = new int[this->data.size()];
	for (size_t i = 0; i < this->data.size(); i++)
	{
		palydata[i] = this->data[i];
	}

	
	double* outdata = NULL;
	if (strcmp(drivername, "WDM") == 0)
	{
		devicemanage.calSpkMME(inchsName, palydata, outchs, &outdata,playtime);
	}
	else
	{
		devicemanage.calSpkASIO(playstr, &data2);
	}
	delete[] palydata;
	//devicemanage.calibrationMic(inchsName, &indata, fs, 5, &data2, outchs, inchs, devicemanage.getoutvp(playstr));



	//获取配置文件的激励
	vector<double> pinadianForSave;
	for (std::map<double, int>::iterator it = pos.begin(); it != pos.end(); it++)
	{
		pinadianForSave.push_back(it->first);
	}

	//计算fr值
	CAudioAlgorithm audioAlg;
	

	m_senofSpk.clear();
	m_senofSpk2.clear();
	m_senofSpk3.clear();
	m_senofSpk4.clear();
	for (size_t i = 0; i < 4; i++)//这个循环变量就是ASIO录音通道的索引，默认，针对奋达项目，内部声卡，
	{
		audioAlg.setFrMode(true);
		audioAlg.AudioAlgorithmInit();
		audioAlg.setSen(devicemanage.getSensitivity(inchsName));
		audioAlg.setDeviceOutputVpval(devicemanage.getoutvp(playstr));
		audioAlg.setDeviceInputVpval(devicemanage.getinputvp(inchsName));
		audioAlg.setIsComputerNorse(true);
		CAlgParam paramalg;
		paramalg.recordIndex = i;
		audioAlg.setAlgParam(paramalg);

		if (strcmp(drivername, "WDM") == 0)
		{
			/*double* res = new double[_msize(indata[0]) / 4];
			for (size_t i = 0; i < _msize(indata[0]) / 4; i++)
			{
			res[i] = indata[0][i];

			}*/
			audioAlg.setDriverMode(1);
			audioAlg.Alg_Fr(outdata, fs);
		}
		else
		{
			double* asiodata = new double[_msize(data2[i]) / 4];
			std::ofstream outfile2;
			CString path1(_T("./response.csv"));
			outfile2.open(path1, std::ios::out | std::ios::app);
			for (size_t j = 0; j < _msize(data2[i]) / 4; j++)
			{
				asiodata[j] = data2[i][j];
				outfile2 << data2[i][j] << endl;
			}
			outfile2.close();
			uint32 bps = 32;//32
			int chs = 1;
			if (fs == 44100)
			{
				chs = 1;

			}
			wavwrite("D:\\TestRecord\\wdmRecord1.wav", 1, fs, bps, (char*)data2[i], fs * playtime * bps / 8);

			//求出实际声压

			audioAlg.Alg_Fr(asiodata, fs);
			
			delete[] asiodata;
			asiodata = NULL;
			delete[] data2[i];
			data2[i]=NULL;
			for (size_t j = 0; j < audioAlg.fr_data.size(); j++)
			{
				if (i == 0)
					m_senofSpk.push_back(audioAlg.fr_data[j] * 1000 / devicemanage.getSensitivity(inchsName));//EQ电压，修改电压使得每个点
				else if (i == 1)
				{
					m_senofSpk2.push_back(20*log10(audioAlg.fr_data[j] * 1000 / devicemanage.getSensitivity(inchsName))+94);
				}
				else if (i == 2)
				{
					m_senofSpk3.push_back(audioAlg.fr_data[j] * 1000 / devicemanage.getSensitivity(inchsName));
				}
				else if (i == 3)
				{
					m_senofSpk4.push_back(audioAlg.fr_data[j] * 1000 / devicemanage.getSensitivity(inchsName));
				}
				//达到1pa  94db
			}
			audioAlg.fr_data.clear();
			std::ofstream outfile1;
			if (i == 0)
			{
				CString path1(_T("./m_senofSpk.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk[k] << ",";

				}
			}
			else if (i == 1)
			{
				CString path1(_T("./m_senofSpk2.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk2[k] << ",";

				}
			}
			else if (i == 2)
			{
				CString path1(_T("./m_senofSpk3.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk3[k] << ",";

				}
			}
			else if (i == 3)
			{
				CString path1(_T("./m_senofSpk4.csv"));
				outfile1.open(path1, std::ios::out | std::ios::app);
				//outfile1.open(path1, std::ios::out | std::ios::trunc);
				outfile1 << ",";
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << pinadianForSave[k] << ",";

				}
				outfile1 << endl;
				for (size_t k = 0; k < m_senofSpk.size(); k++)
				{
					outfile1 << m_senofSpk4[k] << ",";

				}
			}
			outfile1 << endl;
			outfile1.close();
			
		}
	}

	

	
}

vector<double>* JiLiSetNewVersion::getsenofSpk(int index, const vector<double>& testFs)
{
	switch (index)
	{
	case 0:
		return GetOffsetFs(testFs, 0,false); //&m_senofSpk;
		break;
	case 1:
		return GetOffsetFs(testFs, 1, false); //&m_senofSpk2;
		break;
	case 2:
		return GetOffsetFs(testFs, 2, false); //&m_senofSpk3;
		break;
	case 3:
		return GetOffsetFs(testFs, 3, false); //&m_senofSpk4;
		break;
	}

	return NULL;
	
}

vector<double>* JiLiSetNewVersion::getsenofMic(int index, const vector<double>& testFs)
{
	switch (index)
	{
	case 0:
		return GetOffsetFs(testFs, 0); //&m_senofMic;
		break;
	case 1:
		return GetOffsetFs(testFs, 1); //&m_senofMic2;
		break;
	case 2:
		return GetOffsetFs(testFs, 2); //&m_senofMic3;
		break;
	case 3:
		return GetOffsetFs(testFs, 3); //&m_senofMic4;
		break;
	}

	return NULL;
}

vector<double>* JiLiSetNewVersion::GetOffsetFs(const vector<double> data, int nslot, bool bSpk)
{
	vector<double>* dat = new vector<double>;

	

	if (bSpk)
	{
		for (size_t j = 0; j < data.size(); j++)
		{


			for (size_t i = 0; i < offsetFs_spk.size(); i++)
			{
				if (nslot == 0)
				{

					if (offsetFs_spk[i] == data[j])
					{
						dat->push_back(m_senofMic[i]);
						break;
					}

				}
				else if (nslot == 1)
				{

					if (offsetFs_spk[i] == data[j])
					{
						dat->push_back(m_senofMic2[i]);
						break;
					}

				}
				else if (nslot == 2)
				{

					if (offsetFs_spk[i] == data[j])
					{
						dat->push_back(m_senofMic3[i]);
						break;
					}
				}
				else if (nslot == 3)
				{

					if (offsetFs_spk[i] == data[j])
					{
						dat->push_back(m_senofMic4[i]);
						break;
					}
				}

			}
		}
	}
	else
	{
		for (size_t j = 0; j < data.size(); j++)
		{


			for (size_t i = 0; i < offsetFs_mic.size(); i++)
			{
				if (nslot == 0)
				{

					if (offsetFs_mic[i] == data[j])
					{
						dat->push_back(m_senofSpk[i]);
						break;
					}

				}
				else if (nslot == 1)
				{


					if (offsetFs_mic[i] == data[j])
					{
						dat->push_back(m_senofSpk2[i]);
						break;
					}

				}
				else if (nslot == 2)
				{

					if (offsetFs_mic[i] == data[j])
					{
						dat->push_back(m_senofSpk3[i]);
						break;
					}

				}
				else if (nslot == 3)
				{

					if (offsetFs_mic[i] == data[j])
					{
						dat->push_back(m_senofSpk4[i]);
						break;
					}

				}
			}
		}
	}

	if (dat->size() != data.size())
	{
		AfxMessageBox(_T("校准倍频程过低!"));
	}


	return dat;
}
