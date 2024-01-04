// TestBorderLineSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "TestBorderLineSettings.h"
#include "afxdialogex.h"
#include <thread>



extern int golble_dotNum;
extern int globle_w;
extern int globle_h;
// CTestBorderLineSettings 对话框

IMPLEMENT_DYNAMIC(CTestBorderLineSettings, CDialogEx)

CTestBorderLineSettings::CTestBorderLineSettings(CWnd* pParent /*=NULL*/)
: CDialogEx(CTestBorderLineSettings::IDD, pParent), m_singleOrmuti(false),
m_singleup(0), m_singledown(0), m_mutiup(0), m_mutidown(0), m_rowsList(0)
{
	slotParam* pSlot = GetSlotParam(0);
	vector<testItem>* pItems;
	vector<testItem>::iterator it;

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			pItems = &pSlot->vSpkItems;
		}
		else
		{
			pItems = &pSlot->vMicItems;
		}

		it = pItems->begin();
		while (it != pItems->end())
		{
			if (StrCmpI(it->sItemName, _T("FR")) == 0)
			{
				for (int j = 0; j < it->vLimit.size(); j++)
				{
					itemLimit* pLimit = &it->vLimit[j];
					Mutidata *data = new Mutidata(pLimit->nFrequency, pLimit->nLower, pLimit->nUpper);
					if (i == 0)
						spk_fr_borderLine.push_back(data);
					else
						mic_fr_borderLine.push_back(data);
				}
			}
			else if (StrCmpI(it->sItemName, _T("THD")) == 0)
			{
				for (int j = 0; j < it->vLimit.size(); j++)
				{
					itemLimit* pLimit = &it->vLimit[j];
					Mutidata *data = new Mutidata(pLimit->nFrequency, pLimit->nLower, pLimit->nUpper);
					if (i == 0)
						spk_thd_borderLine.push_back(data);
					else
						mic_thd_borderLine.push_back(data);
				}
			}
			else if (StrCmpI(it->sItemName, _T("RB")) == 0)
			{
				for (int j = 0; j < it->vLimit.size(); j++)
				{
					itemLimit* pLimit = &it->vLimit[j];
					Mutidata *data = new Mutidata(pLimit->nFrequency, pLimit->nLower, pLimit->nUpper);
					if (i == 0)
						rb_borderLine.push_back(data);
				}
			}


			it++;
		}
	}
}

CTestBorderLineSettings::~CTestBorderLineSettings()
{
	for (int i = 0; i < spk_fr_borderLine.size(); i++)
	{
		Mutidata *data = spk_fr_borderLine[i];
		delete data;
	}

	for (int i = 0; i < mic_thd_borderLine.size(); i++)
	{
		Mutidata *data = mic_thd_borderLine[i];
		delete data;
	}

	for (int i = 0; i < rb_borderLine.size(); i++)
	{
		Mutidata *data = rb_borderLine[i];
		delete data;
	}
}

void CTestBorderLineSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_limitscmbunits, m_uints);
	DDX_Control(pDX, IDC_limitscmbtestitemmuti, m_cmbmuti);
	DDX_Control(pDX, IDC_limitscmbtestitemsingle, m_cmbsingle);

	DDX_Text(pDX, IDC_limitsHz, m_hz);
	DDX_Text(pDX, IDC_limitsup2, m_yup);
	DDX_Text(pDX, IDC_limitsdown2, m_ydown);

	DDX_Check(pDX, IDC_checklimitssingleup, m_singleup);
	DDX_Check(pDX, IDC_checklimitssingledown, m_singledown);
	DDX_Check(pDX, IDC_checklimitsmutiup, m_mutiup);
	DDX_Check(pDX, IDC_checklimitsmutidown, m_mutidown);

	DDX_Text(pDX, IDC_limitsup, m_yup1);//单点
	DDX_Text(pDX, IDC_limitsdown, m_ydown1);




	DDX_Control(pDX, IDC_LIST1, m_listlinedata);
}

BOOL CTestBorderLineSettings::OnInitDialog()
{

	UpdateData(FALSE);
	m_uints.InsertString(0, _T("dB"));
	m_uints.InsertString(1, _T("%"));
	m_uints.InsertString(2, _T("dB/v"));
	m_uints.SetCurSel(0);

	m_cmbmuti.InsertString(0, _T("FR"));
	m_cmbmuti.InsertString(1, _T("THD"));
	m_cmbmuti.InsertString(2, _T("THD+N"));
	m_cmbmuti.InsertString(3, _T("RB"));
	m_cmbmuti.InsertString(4, _T("Balance"));
	m_cmbmuti.InsertString(5, _T("NoiseReduction"));
	m_cmbmuti.SetCurSel(0);

	m_cmbsingle.InsertString(0, _T("Sensitivity"));
	m_cmbsingle.SetCurSel(0);


	DWORD dwStyle = m_listlinedata.GetExtendedStyle();
	/*dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_INFOTIP |
		LVS_EX_DOUBLEBUFFER;*/
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_listlinedata.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	
	m_listlinedata.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listlinedata.InsertColumn(1, _T("Hz"), LVCFMT_LEFT,130);
	m_listlinedata.InsertColumn(2, _T("下限"), LVCFMT_LEFT,130);
	m_listlinedata.InsertColumn(3, _T("上限"), LVCFMT_LEFT, 130);
	
	/*for (size_t i = 0; i < 100; i++)
	{
		m_listlinedata.InsertItem(i, _T("1"));
		m_listlinedata.SetItemText(i, 1, _T("1"));
		m_listlinedata.SetItemText(i, 2, _T("2"));
		m_listlinedata.SetItemText(i, 3, _T("2"));
	}*/
	
	
	/*m_listlinedata.InsertItem(1, _T("1"));
	m_listlinedata.SetItemText(1,1, _T("3"));
	m_listlinedata.SetItemText(1,2, _T("4"));*/
	

	

	return TRUE;
}

BEGIN_MESSAGE_MAP(CTestBorderLineSettings, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON6, &CTestBorderLineSettings::OnBnClickedButtonmuti)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestBorderLineSettings::OnBnClickedButtonsingle)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestBorderLineSettings::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestBorderLineSettings::OnBnClickedButtonSaveLineBorederdata)
	ON_CBN_SELCHANGE(IDC_limitscmbtestitemmuti, &CTestBorderLineSettings::OnCbnSelchangelimitscmbtestitemmuti)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTestBorderLineSettings::OnNMDblclkList1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDH_OK, &CTestBorderLineSettings::OnOK)
END_MESSAGE_MAP()


// CTestBorderLineSettings 消息处理程序


int CTestBorderLineSettings::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	

	

	/*ThdItem = new LineBorderSetingItem;
	ThdItem->Create(LineBorderSetingItem::IDD, GetDlgItem(IDC_bordersettingpic));
	ThdItem->setTitles(CString("ThdItem"));*/

	/*Thd_NItem = new LineBorderSetingItem;
	Thd_NItem->Create(LineBorderSetingItem::IDD, GetDlgItem(IDC_customitem));
	Thd_NItem->setTitles(CString("Thd_NItem"));

	PhaseItem = new LineBorderSetingItem;
	PhaseItem->Create(LineBorderSetingItem::IDD, GetDlgItem(IDC_customitem));
	PhaseItem->setTitles(CString("PhaseItem"));*/

	
	
	//ThdItem->ShowWindow(SW_SHOW);
	//Thd_NItem->ShowWindow(SW_SHOW);
	//PhaseItem->ShowWindow(SW_SHOW);

	//ShowWindow(SW_HIDE);

	return 0;
}

void CTestBorderLineSettings::showItem(int index, CWnd* wnd)
{
	switch (index)
	{
	case 0:
		senstivityItem = new LineBorderSetingItem;
		senstivityItem->Create(LineBorderSetingItem::IDD, wnd);
		senstivityItem->setTitles(CString("灵敏度"));
		senstivityItem->ShowWindow(SW_SHOW);
		

		break;
	case 1:
		
		break;
	case 2:
		Thd_NItem->ShowWindow(SW_SHOW);
		break;
	case 3:
		PhaseItem->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}


void CTestBorderLineSettings::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	drawGroupBox(pwnd, &dc);

	
	pwnd = GetDlgItem(IDC_STATICduodian);
	drawGroupBox(pwnd, &dc);

	CRect rc, rc2;//绘图区域
	GetDlgItem(IDC_bordersettingpic)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
	dc.FillPath();

	thread thread_draw_bitmap(&CTestBorderLineSettings::DrawThreadBitMap, this);
	thread_draw_bitmap.detach();

	//IDC_bordersettingpic
}


void CTestBorderLineSettings::OnBnClickedButtonmuti()
{
	// TODO:  在此添加控件通知处理程序代码
	m_singleOrmuti = true;
	UpdateData();

	//错误判断
	bool isdup = false;
	double x, y, z;
	for (size_t i = 0; i < mutidata.size(); i++)
	{
		mutidata.at(i)->getdata(x, y, z);
		if (m_hz == x)
		{
			isdup = true;
			break;
		}
	}
	if (!isdup)
	{

		char buf[512]{0};
		
		mutidata.push_back(new Mutidata(m_hz, m_ydown, m_yup));
		sortLineData(mutidata);
		/*char dotstr[1024]{0};
		memset(dotstr, 0, 1024);
		sprintf(dotstr, "%%.%df", golble_dotNum);*/
		
		sprintf(buf, "%.4f", m_hz);
		m_listlinedata.InsertItem(m_rowsList, _T("1"));
		m_listlinedata.SetItemText(m_rowsList, 1, CString(buf));
		memset(buf, 0, 512);
		/*memset(dotstr, 0, 1024);
		sprintf(dotstr, "%%.%df", golble_dotNum);*/
		sprintf(buf, "%.4f", m_ydown);
		m_listlinedata.SetItemText(m_rowsList, 2, CString(buf));
		memset(buf, 0, 512);
		sprintf(buf, "%.4f", m_yup);
		m_listlinedata.SetItemText(m_rowsList, 3, CString(buf));
		m_rowsList++;
	}
	else
	{
		AfxMessageBox(_T("输入频点重复！"));
		return;
	}

	CRect rc, rc2;//绘图区域
	GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	rc2 = rc;
	rc2.bottom += 20;
	InvalidateRect(rc2);
	//DrawThreadBitMap();
	/*thread thread_draw_bitmap(&CTestBorderLineSettings::DrawThreadBitMap, this);
	thread_draw_bitmap.detach();*/

}


void CTestBorderLineSettings::OnBnClickedButtonsingle()
{
	// TODO:  在此添加控件通知处理程序代码
	m_singleOrmuti = false;
	UpdateData();
	mutidata2.push_back(new Mutidata(0, m_ydown, m_yup));
	CRect rc, rc2;//绘图区域
	GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	rc2 = rc;
	rc2.bottom += 20;
	InvalidateRect(rc2);
	
}

void CTestBorderLineSettings::DrawThreadBitMap()
{

	if (mutidata.size() == 0 && mutidata2.size() == 0)
		return;

	CDC* dc = GetDlgItem(IDC_bordersettingpic)->GetDC();

	
	//rc2 = rc;
	//InvalidateRect(rc2);

	

	if (m_singleOrmuti)
	{
	
		//绘制
		int orgX, orgY;
		int lenX, lenY;
		int endX, endY;
		orgX = 80 / (2880.0 / globle_w);
		orgY = 460.0 / (1800.0 / globle_h);
		endX = 1700 / (2880.0 / globle_w);//x轴最远位置
		endY = 0;//y轴最远位置
		lenX = (endX - orgX) ;//x轴实际绘制长度
		lenY = (orgY - endY);//y轴实际绘制长度
		//x 根据显示的宽度计算   需要显示的信号长度
		double perpointlen = 1.0*lenX / mutidata.size();//每个信号的点占用绘图实际的长度
		double jiange = perpointlen;//每个信号的显示间隔长度

		//Y根据显示的高度计算   需要显示的信号幅值

		//double perpointhight = 1.0*rc.Height() / 2.0;
		//int ymove = rc.Height() / 2.0;

		/****************绘制坐标轴*************************/
		CPen pen;
		pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
		CPen* oldpen = dc->SelectObject(&pen);
		//绘制X轴
		dc->MoveTo(orgX, orgY);
		dc->LineTo(endX, orgY);

		//绘制Y轴
		dc->MoveTo(orgX, orgY);
		dc->LineTo(orgX, endY);

		dc->SelectObject(oldpen);
		/****************************************************/


		vector< double> tempdata;
		tempdata.resize(mutidata.size());

		vector< double> tempdata2;
		tempdata2.resize(mutidata.size());
		double maxY = 0;
		double maxY2 = 0;//为了考虑上下限
		int maxi = 0;
		int ymove = 10;
		lenY -= ymove;
		bool btomax = false;
		double x1, y1, y2;
		int yScale = 10000;
		
			for (size_t i = 0; i < mutidata.size(); i++)
			{
				mutidata[i]->getdata(x1, y1, y2);
				if (y1 < 0)
				{
					//AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
					
					//return;
				}


				if (y1 * yScale<0)
					btomax = true;

				tempdata[i] = (y1 * yScale);

				if (tempdata[i]>maxY)
				{
					maxY = tempdata[i];
					maxi = i;
				}



			}

			



			if (btomax)
			{
				for (size_t i = 0; i < mutidata.size(); i++)
				{
					mutidata[i]->getdata(x1, y1, y2);
					tempdata[i] = y1;

					if (tempdata[i]>maxY)
					{
						maxY = tempdata[i];
						maxi = i;
					}
				}
			}

			/*****************************************************/
			btomax = false;
			for (size_t i = 0; i < tempdata2.size(); i++)
			{
				mutidata[i]->getdata(x1, y1, y2);
				if (y2 < 0)
				{
					//AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
					//return;
				}


				if (y2 * yScale<0)
					btomax = true;

				tempdata2[i] = (y2 * yScale);

				if (tempdata2[i]>maxY2)
				{
					maxY2 = tempdata2[i];
					maxi = i;
				}

			}

			if (btomax)
			{
				for (size_t i = 0; i < mutidata.size(); i++)
				{
					mutidata[i]->getdata(x1, y1, y2);
					tempdata2[i] = y2;

					if (tempdata2[i]>maxY2)
					{
						maxY2 = tempdata2[i];
						maxi = i;
					}
				}
			}
			/********************************************************/

			//综合考虑上下限
			if (maxY2 > maxY)
			{
				maxY = maxY2;
			}



			
			



	/**************************以下代码为下框线绘制**************************************/
	    if (m_mutidown == 1)
		{
			double perpointhight = 0;

			if (maxY > 0)
			{
				perpointhight = 1.0*lenY / maxY;
			}



			if (maxY < lenY)
			{
				perpointhight = 1;
			}

			


			/***************绘制坐标轴***********************/
			CPen pen;
			////X轴
			//MoveToEx(*dc,orgX-40, orgY+10, NULL);
			//LineTo(*dc, endX, orgY+10);

			////Y轴
			//MoveToEx(*dc, orgX-40, orgY+10, NULL);
			//LineTo(*dc, orgX-40, endY+10);

			
			pen.DeleteObject();
			pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 255));
			CPen*  oldpen = dc->SelectObject(&pen);
			SetBkMode(*dc, TRANSPARENT);
			if (tempdata.size() > 1)
			{

				for (size_t i = 0; i < tempdata.size() - 1; i++)
				{
					MoveToEx(*dc, i*jiange + orgX, orgY - tempdata[i] * perpointhight, NULL);
					LineTo(*dc, (i + 1)*jiange + orgX, orgY - tempdata[i + 1] * perpointhight);
				}
			}
			else
			{
				//绘制一个点
				pen.DeleteObject();
				pen.CreatePen(BS_SOLID, 10, RGB(0, 0, 255));
				oldpen = dc->SelectObject(&pen);
				dc->SetPixel(0 * jiange + orgX + 50, orgY - tempdata[0] * perpointhight, RGB(0, 0, 255));

				//dc->MoveTo(orgX + 70, orgY - tempdata[0] * perpointhight+4);
				//dc->MoveTo(200,100);
				//dc->Rectangle(0, 0, 2, 2);

				MoveToEx(*dc, orgX /*+ 70*/, orgY - tempdata[0] * perpointhight + 4, NULL);
				LineTo(*dc, orgX /*+ 70*/ + 2, orgY - tempdata[0] * perpointhight + 4);
				MoveToEx(*dc, orgX /*+ 70*/ + 2, orgY - tempdata[0] * perpointhight + 4+2, NULL);
				LineTo(*dc, orgX /*+ 70*/, orgY - tempdata[0] * perpointhight + 4+2);

			

			}

			dc->SelectObject(oldpen);

		}

		/**************************以下代码为上框线绘制**************************************/
		
		if (m_mutiup == 1)
		{
			for (size_t i = 0; i < mutidata.size(); i++)
			{
				mutidata[i]->getdata(x1, y1, y2);
				if (y2 < 0)
				{
					//AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
					//return;
				}


				if (y2 * yScale < 0)
					btomax = true;

				tempdata[i] = (y2 * yScale);

				/*if (tempdata[i]>maxY)
				{
				maxY = tempdata[i];
				maxi = i;
				}*/

			}

			if (btomax)
			{
				for (size_t i = 0; i < mutidata.size(); i++)
				{
					mutidata[i]->getdata(x1, y1, y2);
					tempdata[i] = y2;

					/*if (tempdata[i]>maxY)
					{
					maxY = tempdata[i];
					maxi = i;
					}*/
				}
			}



			double perpointhight = 0;

			if (maxY > 0)
			{
				perpointhight = 1.0*lenY / maxY;
			}



			if (maxY < lenY)
			{
				perpointhight = 1;
			}

			CPen pen;
			pen.DeleteObject();
			pen.CreatePen(BS_SOLID, 2, RGB(255, 0, 12));
			CPen* oldpen = dc->SelectObject(&pen);
			SetBkMode(*dc, TRANSPARENT);


			/***************绘制坐标轴***********************/



			if (tempdata.size() > 1)
			{

				for (size_t i = 0; i < tempdata.size() - 1; i++)
				{
					MoveToEx(*dc, i*jiange + orgX, orgY - tempdata[i] * perpointhight, NULL);
					LineTo(*dc, (i + 1)*jiange + orgX, orgY - tempdata[i + 1] * perpointhight);
				}
			}
			else
			{
				pen.DeleteObject();
				pen.CreatePen(BS_SOLID, 10, RGB(0, 0, 255));
				oldpen = dc->SelectObject(&pen);
				dc->SetPixelV(0 * jiange + orgX+50, orgY - tempdata[0] * perpointhight, RGB(0, 0, 255));

				//dc->MoveTo(orgX + 70, orgY - tempdata[0] * perpointhight+4);
				//dc->Rectangle(0, 0, 2, 2);
				MoveToEx(*dc, orgX/* + 70*/, orgY - tempdata[0] * perpointhight + 4, NULL);
				LineTo(*dc, orgX /*+ 70*/ + 2, orgY - tempdata[0] * perpointhight + 4);
				MoveToEx(*dc, orgX /*+ 70*/ + 2, orgY - tempdata[0] * perpointhight + 4 + 2, NULL);
				LineTo(*dc, orgX/* + 70*/, orgY - tempdata[0] * perpointhight + 4 + 2);
			}
			dc->SelectObject(oldpen);

		}

		//这里绘制Y坐标轴的刻度线
		dengfenshu = 10;
		char ch_str[1024]{0};
		double perpointhight = 0;
		
		if (maxY > 0)
		{
			perpointhight = 1.0*lenY / maxY;
		}



		if (maxY < lenY)
		{
			perpointhight = 1;
		}
		for (size_t i = 0; i <= dengfenshu; i++)
		{
			sprintf(ch_str, "%.4f", i*maxY / dengfenshu / yScale);
			dc->TextOutW(2, orgY - perpointhight*i*maxY / dengfenshu-10, CString(ch_str));
			memset(ch_str, 0, 1024);
		}



		for (size_t i = 0; i < mutidata.size(); i++)
		{
			mutidata[i]->getdata(x1, y1, y2);

			sprintf(ch_str, "%.4f", x1);
			dc->TextOutW(i*jiange + orgX, orgY+2, CString(ch_str));
			memset(ch_str, 0, 1024);

		}

	}
	else
	{

	}


	

	

}

Mutidata::Mutidata(double x1, double y1, double y2)
{
	x=x1;
	y_down = y1;
	y_up = y2;
}

Mutidata::~Mutidata()
{
}

void Mutidata::getdata(double& x1, double& y1, double& y2)
{
	 x1=x;
	 y1=y_down;
	 y2=y_up;
}


void CTestBorderLineSettings::OnBnClickedButtonClear()
{
	
	// TODO:  在此添加控件通知处理程序代码

	if (mutidata2.size() == 0 && mutidata.size() == 0)
	{
		return;
	}

	if (MessageBox(_T("是否确认清楚数据？"), _T("框线设置"), MB_YESNO | MB_ICONQUESTION) != IDYES)
	{
		return;
	}

	for (size_t i = 0; i < mutidata.size(); i++)
	{
		delete mutidata[i];
	}
	mutidata.clear();
	for (size_t i = 0; i < mutidata2.size(); i++)
	{
		delete mutidata2[i];
	}
	mutidata2.clear();


	int index_muti = m_cmbmuti.GetCurSel();
	int index_sing = m_cmbsingle.GetCurSel();

	switch (index_muti)
	{
	case 0:
		deleData(fr_borderLine);
		break;
	case 1:
		deleData(thd_borderLine);
		break;
	default:
		break;
	}

	switch (index_sing)
	{
	case 0:
		break;
	case 1:
		break;
	default:
		break;
	}

	for (int i = m_rowsList-1; i >= 0; i--)
	{
		m_listlinedata.DeleteItem(i);
	}
	m_rowsList = 0;
	

	CRect rc;//绘图区域
	GetDlgItem(IDC_bordersettingpic)->GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect(rc);
}
void CTestBorderLineSettings::deleData(vector<Mutidata*>& data)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		delete data[i];
	}

	data.clear();
}


void CTestBorderLineSettings::OnBnClickedButtonSaveLineBorederdata()
{
	// TODO:  在此添加控件通知处理程序代码

	int index_muti=m_cmbmuti.GetCurSel();
	int index_sing = m_cmbsingle.GetCurSel();


	switch (index_muti)
	{
	case 0:
		if (fr_borderLine.size() != 0)
		{
			deleData(fr_borderLine);
		}
		setData(fr_borderLine);
		break;
	case 1:
		if (thd_borderLine.size() != 0)
		{
			deleData(thd_borderLine);
		}
		setData(thd_borderLine); 
		break;
	case 2:
		if (thdN_borderLine.size() != 0)
		{
			deleData(thdN_borderLine);
		}
		setData(thdN_borderLine);
		break;
	case 3:
		if (rb_borderLine.size() != 0)
		{
			deleData(rb_borderLine);
		}
		setData(rb_borderLine);
		break;
	case 4:
		if (Balance_borderLine.size() != 0)
		{
			deleData(Balance_borderLine);
		}
		setData(Balance_borderLine);
		break;
	case 5:
		if (NoiseReduction_borderLine.size() != 0)
		{
			deleData(NoiseReduction_borderLine);
		}
		setData(NoiseReduction_borderLine);
		break;
	default:
		break;
	}

	switch (index_sing)
	{
	case 0:

		break;
	case 1:

		break;
	default:
		break;
	}


	//CRect rc;//绘图区域
	//GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	//InvalidateRect(rc);

	AfxMessageBox(_T("保存成功..."));

}
//首尾频点必须设置
void CTestBorderLineSettings::setData(vector<Mutidata*>& data)
{
	double data_x, data_y, data_z;
	for (size_t i = 0; i < mutidata.size(); i++)
	{
		mutidata[i]->getdata(data_x, data_y, data_z);

		if (m_mutidown == 0)
		{
			data_y = -1;
		}
		if (m_mutiup == 0)
		{
			data_z = -1;
		}

		data.push_back(new Mutidata(data_x, data_y, data_z));

		

	}


	/*sortLineData(data);*/

}

void CTestBorderLineSettings::sortLineData(vector<Mutidata*>& data)
{
	vector<double> temp;	

	double hz = 0;
	double x, y, z;
	for (size_t i = 0; i < data.size(); i++)
	{
		data[i]->getdata(x, y, z);
		temp.push_back(x);
	}
	
	sort(temp.begin(), temp.end());

	vector<Mutidata*> temp2 = data;
	data.clear();

	for (size_t i = 0; i < temp.size(); i++)
	{
		for (size_t j = 0; j < temp2.size(); j++)
		{
			temp2[j]->getdata(x, y, z);
			if (x == temp[i])
			{
				data.push_back(temp2[j]);
			}
		}
		
	}

	temp2.clear();

}


void CTestBorderLineSettings::OnCbnSelchangelimitscmbtestitemmuti()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData();

	int index_muti = m_cmbmuti.GetCurSel();
	switch (index_muti)
	{
	case 0:
		if (fr_borderLine.size() == 0)
		{
			setData(fr_borderLine);
		}
		deleData(mutidata);
		setShowData(fr_borderLine);
		break;
	case 1:
		if (thd_borderLine.size() == 0)
		{
			setData(thd_borderLine);
		}
		deleData(mutidata);
		setShowData(thd_borderLine);
		break;
	case 2:
		if (thdN_borderLine.size() == 0)
		{
			setData(thdN_borderLine);
		}
		deleData(mutidata);
		setShowData(thdN_borderLine);
		break;
	case 3:
		if (rb_borderLine.size() == 0)
		{
			setData(rb_borderLine);
		}
		deleData(mutidata);
		setShowData(rb_borderLine);
		break;
	case 4:
		if (Balance_borderLine.size() == 0)
		{
			setData(Balance_borderLine);
		}
		deleData(mutidata);
		setShowData(Balance_borderLine);
		break;
	case 5:
		if (NoiseReduction_borderLine.size() == 0)
		{
			setData(NoiseReduction_borderLine);
		}
		deleData(mutidata);
		setShowData(NoiseReduction_borderLine);
		break;
	default:
		break;
	}

	CRect rc;//绘图区域
	GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	InvalidateRect(rc);

}

void CTestBorderLineSettings::setShowData(const vector<Mutidata*>& data)
{


	m_rowsList=m_listlinedata.GetItemCount();

	for (int i = m_rowsList - 1; i >= 0; i--)
	{
		m_listlinedata.DeleteItem(i);
	}
	m_rowsList = 0;

	double data_x, data_y, data_z;
	for (size_t i = 0; i < data.size(); i++)
	{

		data[i]->getdata(data_x, data_y, data_z);
		mutidata.push_back(new Mutidata(data_x, data_y, data_z));

		sortLineData(mutidata);
		char buf[512]{0};
		//for (size_t i = 0; i < data.size(); i++)
		{
			sprintf(buf, "%.4f", data_x);
			
			m_listlinedata.InsertItem(m_rowsList, _T("1"));
			m_listlinedata.SetItemText(m_rowsList, 1, CString(buf));
			memset(buf, 0, 512);
			/*memset(dotstr, 0, 1024);
			sprintf(dotstr, "%%.%df", golble_dotNum);*/
			sprintf(buf, "%.4f", data_y);
			m_listlinedata.SetItemText(m_rowsList, 2, CString(buf));
			memset(buf, 0, 512);
			sprintf(buf, "%.4f", data_z);
			m_listlinedata.SetItemText(m_rowsList, 3, CString(buf));
			m_rowsList++;
		}

	}

	CRect rc;//绘图区域
	GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	InvalidateRect(rc);
	

	

}



void CTestBorderLineSettings::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码


	NM_LISTVIEW* pNMListViwe = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	int row = pNMListViwe->iItem;
	int col = pNMListViwe->iSubItem;

	if (pNMListViwe->iSubItem != 0)
	{
		m_listlinedata.GetSubItemRect(row, col, LVIR_LABEL, rc);

	}

	*pResult = 0;
}

void CTestBorderLineSettings::drawGroupBox(CWnd* wnd, CPaintDC* dc)
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

void  CTestBorderLineSettings::OnOK()
{
	return;

}



HBRUSH CTestBorderLineSettings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_bordersettingpic)
	{
		//CRect rc, rc2;//绘图区域
		//GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->FillSolidRect(rc, RGB(255, 255, 255));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
