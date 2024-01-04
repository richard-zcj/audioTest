// TestBorderLineSettings.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "TestBorderLineSettings.h"
#include "afxdialogex.h"
#include <thread>



extern int golble_dotNum;
extern int globle_w;
extern int globle_h;
// CTestBorderLineSettings �Ի���

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

	DDX_Text(pDX, IDC_limitsup, m_yup1);//����
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
	m_listlinedata.InsertColumn(2, _T("����"), LVCFMT_LEFT,130);
	m_listlinedata.InsertColumn(3, _T("����"), LVCFMT_LEFT, 130);
	
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


// CTestBorderLineSettings ��Ϣ�������


int CTestBorderLineSettings::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	

	

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
		senstivityItem->setTitles(CString("������"));
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
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	drawGroupBox(pwnd, &dc);

	
	pwnd = GetDlgItem(IDC_STATICduodian);
	drawGroupBox(pwnd, &dc);

	CRect rc, rc2;//��ͼ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_singleOrmuti = true;
	UpdateData();

	//�����ж�
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
		AfxMessageBox(_T("����Ƶ���ظ���"));
		return;
	}

	CRect rc, rc2;//��ͼ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_singleOrmuti = false;
	UpdateData();
	mutidata2.push_back(new Mutidata(0, m_ydown, m_yup));
	CRect rc, rc2;//��ͼ����
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
	
		//����
		int orgX, orgY;
		int lenX, lenY;
		int endX, endY;
		orgX = 80 / (2880.0 / globle_w);
		orgY = 460.0 / (1800.0 / globle_h);
		endX = 1700 / (2880.0 / globle_w);//x����Զλ��
		endY = 0;//y����Զλ��
		lenX = (endX - orgX) ;//x��ʵ�ʻ��Ƴ���
		lenY = (orgY - endY);//y��ʵ�ʻ��Ƴ���
		//x ������ʾ�Ŀ�ȼ���   ��Ҫ��ʾ���źų���
		double perpointlen = 1.0*lenX / mutidata.size();//ÿ���źŵĵ�ռ�û�ͼʵ�ʵĳ���
		double jiange = perpointlen;//ÿ���źŵ���ʾ�������

		//Y������ʾ�ĸ߶ȼ���   ��Ҫ��ʾ���źŷ�ֵ

		//double perpointhight = 1.0*rc.Height() / 2.0;
		//int ymove = rc.Height() / 2.0;

		/****************����������*************************/
		CPen pen;
		pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
		CPen* oldpen = dc->SelectObject(&pen);
		//����X��
		dc->MoveTo(orgX, orgY);
		dc->LineTo(endX, orgY);

		//����Y��
		dc->MoveTo(orgX, orgY);
		dc->LineTo(orgX, endY);

		dc->SelectObject(oldpen);
		/****************************************************/


		vector< double> tempdata;
		tempdata.resize(mutidata.size());

		vector< double> tempdata2;
		tempdata2.resize(mutidata.size());
		double maxY = 0;
		double maxY2 = 0;//Ϊ�˿���������
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
					//AfxMessageBox(_T("��ʾ���ݸ�ʽ���󣬲����и�����"));
					
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
					//AfxMessageBox(_T("��ʾ���ݸ�ʽ���󣬲����и�����"));
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

			//�ۺϿ���������
			if (maxY2 > maxY)
			{
				maxY = maxY2;
			}



			
			



	/**************************���´���Ϊ�¿��߻���**************************************/
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

			


			/***************����������***********************/
			CPen pen;
			////X��
			//MoveToEx(*dc,orgX-40, orgY+10, NULL);
			//LineTo(*dc, endX, orgY+10);

			////Y��
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
				//����һ����
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

		/**************************���´���Ϊ�Ͽ��߻���**************************************/
		
		if (m_mutiup == 1)
		{
			for (size_t i = 0; i < mutidata.size(); i++)
			{
				mutidata[i]->getdata(x1, y1, y2);
				if (y2 < 0)
				{
					//AfxMessageBox(_T("��ʾ���ݸ�ʽ���󣬲����и�����"));
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


			/***************����������***********************/



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

		//�������Y������Ŀ̶���
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
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (mutidata2.size() == 0 && mutidata.size() == 0)
	{
		return;
	}

	if (MessageBox(_T("�Ƿ�ȷ��������ݣ�"), _T("��������"), MB_YESNO | MB_ICONQUESTION) != IDYES)
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
	

	CRect rc;//��ͼ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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


	//CRect rc;//��ͼ����
	//GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	//InvalidateRect(rc);

	AfxMessageBox(_T("����ɹ�..."));

}
//��βƵ���������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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

	CRect rc;//��ͼ����
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

	CRect rc;//��ͼ����
	GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
	InvalidateRect(rc);
	

	

}



void CTestBorderLineSettings::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


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

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_bordersettingpic)
	{
		//CRect rc, rc2;//��ͼ����
		//GetDlgItem(IDC_bordersettingpic)->GetClientRect(rc);
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->FillSolidRect(rc, RGB(255, 255, 255));
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
