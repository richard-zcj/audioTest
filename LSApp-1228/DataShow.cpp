// DataShow.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "DataShow.h"
#include "afxdialogex.h"
#include <string>
#include <fstream>


using namespace std;

extern int golble_dotNum;

// CDataShow 对话框

IMPLEMENT_DYNAMIC(CDataShow, CDialogEx)

CDataShow::CDataShow( CWnd* pParent /*=NULL*/)
: CDialogEx(CDataShow::IDD, pParent), yScale(1), dengfenshu(8), marshCount_X(15), marshCount_Y(10)
{
	dotN = 1;
	orgX = 80 + xMove;
	orgY = 700;
	endX = 1300 + xMove + addXlen;//x轴最远位置
	endY = 40;//y轴最远位置

	frm_h = 778;
	frm_w = 1521;
	
	res_h = 778;//独立变化

	cy_titl = 0;//y坐标轴标题位置移动量

	m_bownerDrwa = false;
	m_truncolor = true;
	
}

CDataShow::~CDataShow()
{
	m_showTestdata.clear();
}

void CDataShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_datashowTestResult, result);
}

void CDataShow::setwindowtile(CString title)
{
	SetWindowTextW(title);

}
void CDataShow::setchartname(CString charttitle)
{
	GetDlgItem(IDC_showdatachartname)->SetWindowTextW(charttitle);
	
	title = charttitle;
	CRect rc;
	this->GetWindowRect(rc);

	int w = rc.Width();
	int h = rc.Height();



	if (charttitle.Compare(CString("FR"))==0)
	{
		rc.top = moveYFR;
		rc.bottom = rc.top + h;

		rc.left = moveXFR;
		rc.right = rc.left + w;

	}
	else if (charttitle.Compare(CString("THD"))==0)
	{
		rc.top = moveYTHD;
		rc.bottom = rc.top + h;

		rc.left = moveXTHD;
		rc.right = rc.left + w;
	}
	else if (charttitle.Compare(CString("THD+N"))==0)
	{
		int moveY = -60;
		rc.top = moveYTHDN + moveY;
		rc.bottom = rc.top + h;

		rc.left = moveXTHDN;
		rc.right = rc.left + w;
	}
	else if (charttitle.Compare(CString("RB")) == 0)
	{
		rc.top = moveYRB;
		rc.bottom = rc.top + h;

		rc.left = moveXRB;
		rc.right = rc.left + w;
	}
	else if (charttitle.Compare(CString("Phase")) == 0)
	{
		rc.top = moveYPhase;
		rc.bottom = rc.top + h;

		rc.left = moveXPhase;
		rc.right = rc.left + w;
	}

	//MoveWindow(rc);
}

void CDataShow::initPos()
{

	
	return;//新版本不全显示
	CRect rc;
	this->GetWindowRect(rc);

	int w = rc.Width();
	int h = rc.Height();

	moveYFR = rc.top+350;
	moveXFR = 0;

	moveYTHD = rc.top+350;
	moveXTHD = rc.left + w;

	moveYTHDN = rc.top + h-60;
	moveXTHDN = 0;

	moveYRB = rc.top + h-60;
	moveXRB = rc.left + w;

	moveYPhase = rc.top + h/2;
	moveXPhase = rc.left + w/2;




}

BOOL CDataShow::OnInitDialog()
{
	/*XFORM xformat;
	double mangle = 90;
	double angle = mangle / 180. * 3.1415926;
	xformat.eM11 = (FLOAT)cos(angle);
	xformat.eM12 = (FLOAT)sin(angle);
	xformat.eM21 = (FLOAT)-sin(angle);
	xformat.eM22 = (FLOAT)cos(angle);
	CRect rect;
	GetDlgItem(IDC_showdataYtitle)->GetWindowRect(&rect);
	CPoint centerPt;
	centerPt.x = rect.left;
	centerPt.y = rect.top;
	xformat.eDx = (FLOAT)((centerPt.x - cos(angle))*centerPt.x + sin(angle)*centerPt.y);
	xformat.eDy = (FLOAT)((centerPt.y - cos(angle))*centerPt.y - cos(angle)*centerPt.x);
	
	HDC hdc = ::GetDC(GetDlgItem(IDC_showdataYtitle)->GetSafeHwnd());
	
	SetGraphicsMode(hdc, GM_ADVANCED);
	SetWorldTransform(hdc, &xformat);*/


	GetDlgItem(IDC_CHART)->GetWindowRect(chart_rc);
	ScreenToClient(chart_rc);

	GetDlgItem(IDC_datashowTestResult)->GetWindowRect(res_rc);
	ScreenToClient(res_rc);

	GetDlgItem(IDC_showdataX)->GetWindowRect(x_rc);
	ScreenToClient(x_rc);


	oldDlgcolor = GetSysColor(COLOR_3DFACE);
	
	HICON icon =AfxGetApp()->LoadIconW(IDI_LK);

	SetIcon(icon, 0);

	mainf = (CMainFrame*)theApp.GetMainWnd();

	m_blindTimes = 0;

	slot1color = RGB(240, 155, 89);
	slot2color = RGB(120, 67, 21);
	slot3color = RGB(117, 247, 77);
	slot4color = RGB(115, 43, 245);
	return TRUE;
}
void CDataShow::rotalchartY()
{
	/*XFORM xformat;
	double mangle = 90;
	double angle = mangle / 180 * 3.1415926;
	xformat.eM11 = (FLOAT)cos(angle);
	xformat.eM12 = (FLOAT)sin(angle);
	xformat.eM21 = (FLOAT)-sin(angle);
	xformat.eM22 = (FLOAT)cos(angle);


	HDC hdc = ::GetDC(GetDlgItem(IDC_showdataYtitle)->GetSafeHwnd());

	SetWorldTransform(hdc, &xformat);*/
}

void CDataShow::SetTimerStart()
{
	SetTimer(1, 1000, NULL);
}

void CDataShow::SetTimerStop()
{
	KillTimer(1);
}


BEGIN_MESSAGE_MAP(CDataShow, CDialogEx)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDataShow 消息处理程序


void CDataShow::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CDataShow::OnPaint()
{

	//drawProportional();
	GetDlgItem(IDC_datashowTestResult)->MoveWindow(res_rc);
	GetDlgItem(IDC_CHART)->MoveWindow(chart_rc);
	GetDlgItem(IDC_showdataX)->MoveWindow(x_rc);



	//drawLog10();
	drawLog10_muti();
}

void CDataShow::drawLog10_muti()
{

	CPaintDC dc(this); // device context for painting
	CFont* font = dc.GetCurrentFont();
	LOGFONT logfont;
	font->GetLogFont(&logfont);
	logfont.lfEscapement = 900;
	HFONT hFont = CreateFontIndirect(&logfont);
	dc.SelectObject(hFont);

	dc.TextOutW(30, 450 + cy_titl, m_Ytitle);//CString("图表按照Y轴旋转")
	dc.SelectObject(font);


	/***************X轴*****************/
	dc.TextOutW(endX - 100, orgY + 25, m_Xtitle);

	lenX = endX - orgX;//x轴实际绘制长度
	lenY = orgY - endY;//y轴实际绘制长度

	/****************绘制坐标轴*************************/
	CPen pen;
	pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen = dc.SelectObject(&pen);
	//绘制X轴
	dc.MoveTo(orgX, orgY);
	dc.LineTo(endX, orgY);

	//绘制Y轴
	dc.MoveTo(orgX, orgY);
	dc.LineTo(orgX, endY);

	dc.SelectObject(oldpen);
	/****************************************************/

	/********************以下横坐标点按照等比关系绘制*********************************/
	double proportional = 10;
	//int dengfenshu = 10;//约定必须为偶数，方便开方计算

	if (m_showTestdataX.size() == 0)
		return;

	double minxval = m_showTestdataX[0];
	double maxxval = m_showTestdataX.back();

	proportional = maxxval / minxval;
	proportional = pow(proportional, 1.0 / dengfenshu);
	if (proportional == 1)
	{
		AfxMessageBox(_T("单个频点不绘制曲线!!"));
		return;
	}

	/*********************在等分处绘制坐标点****************************/
	double xproval = 1;
	for (size_t i = 0; i <= dengfenshu; i++)
	{
		xproval = 1;

		xproval = pow(proportional, i);

		//dc.TextOutW(orgX, orgY + 5, CString(to_string((int)(minxval)).c_str()));
		dc.TextOutW(orgX + lenX*i / dengfenshu*1.0, orgY + 5, getshowXstr(minxval*xproval));

		//2.绘制X轴的网格线
		if (i != 0)
		{
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1, RGB(141, 238, 238));
			oldpen = dc.SelectObject(&pen);
			SetBkMode(dc, TRANSPARENT);


			MoveToEx(dc, orgX + lenX*i / dengfenshu*1.0, orgY, NULL);
			LineTo(dc, orgX + lenX*i / dengfenshu*1.0, endY);

			dc.SelectObject(oldpen);
		}


	}




	
	if (!checkDataIsNULL())
	{
		//return;
	}

	double max = 0.0;
	double max2 = 0.0;
	double max3 = 0.0;
	double max4 = 0.0;
	double upmax = 0.0;
	double downmax = 0.0;
	showDataMax(m_showTestdata, max);
	showDataMax(m_showTestdata2, max2);
	showDataMax(m_showTestdata3, max3);
	showDataMax(m_showTestdata4, max4);

	showDataMax(m_showTestdataBorderLineup, upmax);
	showDataMax(m_showTestdataBorderLinedown, downmax);



	
	vector<double> max_list;
	max_list.push_back(max);
	max_list.push_back(max2);
	max_list.push_back(max3);
	max_list.push_back(max4);
	max_list.push_back(upmax);
	max_list.push_back(downmax);

	showDataMax(max_list, max,false);
	
	vector<int>showdat;
	GetShowData(m_showTestdata, showdat, max);
	int ymove = lenY / 2.0;
	//x 根据显示的宽度计算   需要显示的信号长度
	double perpointlen = 1.0*lenX / showdat.size();//每个信号的点占用绘图实际的长度
	double jiange = perpointlen;//每个信号的显示间隔长度

	double perpointhight = 0;

	if (max > 0)
	{
		perpointhight = 1.0*lenY / max;
	}



	if (max < lenY)
	{
		perpointhight = 1;
	}



	/************************************绘制网格线****************************************/
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(141, 238, 238));
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);

	char dotstr[1024];
	char ystr[1024];
	int strlen = 0;
	//1.分别绘制X 与Y轴网格线
	double per_marshLen_Y = 0;
	double permarshVal = 0;
	per_marshLen_Y = 1.0*lenY / (marshCount_Y + 1);//绘图长度  -->计算对应实际刻度
	permarshVal = 1.0*max / (marshCount_Y + 1);
	//if (title == "Phase")
	{
		max -= m_bownerDrwa_min*yScale;
		permarshVal = 1.0*(max + m_bownerDrwa_min*yScale) / (marshCount_Y + 1);
	}


	//判断使用几位小数



	for (size_t i = 1; i <= marshCount_Y + 1; i++)
	{
		MoveToEx(dc, orgX, orgY - per_marshLen_Y*i, NULL);
		LineTo(dc, endX, orgY - per_marshLen_Y*i);

		//刻度
		memset(dotstr, 0, 1024);
		sprintf(dotstr, "%%.%df", golble_dotNum);
		//if (title == "Phase")
		sprintf(ystr, dotstr, permarshVal*i / yScale - m_bownerDrwa_min);
		/*else
		{
		sprintf(ystr, dotstr, permarshVal*i / yScale);
		}*/
		strlen = std::strlen(ystr);
		dc.TextOutW(orgX - 8 * strlen - 5, orgY - per_marshLen_Y*i - 5, CString(ystr));

	}




	/***************绘制第一个图*******************/
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, slot1color);
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	double Xval, Xval2;
	int X, Xnext;
	if (m_showTestdata.size() > 2)
	{

		for (size_t i = 0; i < showdat.size() - 1; i++)
		{
			Xval = m_showTestdataX[i];
			//计算出x坐标点位置
			X = getDrawlogX(lenX, Xval, proportional, minxval, orgX);
			m_showTestdataXPos.insert(X);
			Xval2 = m_showTestdataX[i + 1];
			Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
			m_showTestdataXPos.insert(Xnext);

			MoveToEx(dc, X, orgY - showdat[i] * perpointhight, NULL);
			LineTo(dc, Xnext, orgY - showdat[i + 1] * perpointhight);

			//MoveToEx(dc, i*jiange, showdat[i] + ymove, NULL);
			//LineTo(dc, (i + 1)*jiange, showdat[i + 1] + ymove);
		}
	}
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, slot2color);
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	
	/********************第二个图*******************/
	showdat.clear();
	GetShowData(m_showTestdata2, showdat, max);
	if (m_showTestdata2.size())
	{

		for (size_t i = 0; i < showdat.size() - 1; i++)
		{

			Xval = m_showTestdataX[i];
			//计算出x坐标点位置
			X = getDrawlogX(lenX, Xval, proportional, minxval, orgX);
			m_showTestdataXPos.insert(X);
			Xval2 = m_showTestdataX[i + 1];
			Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
			m_showTestdataXPos.insert(Xnext);

			MoveToEx(dc, X, orgY - showdat[i] * perpointhight, NULL);
			LineTo(dc, Xnext, orgY - showdat[i + 1] * perpointhight);

			//MoveToEx(dc, i*jiange, showdat[i] + ymove, NULL);
			//LineTo(dc, (i + 1)*jiange, showdat[i + 1] + ymove);
		}

	}
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, slot3color);
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	/********************第3个图*******************/
	showdat.clear();
	GetShowData(m_showTestdata3, showdat, max);

	if (m_showTestdata3.size() > 2)
	{


		for (size_t i = 0; i < showdat.size() - 1; i++)
		{

			Xval = m_showTestdataX[i];
			//计算出x坐标点位置
			X = getDrawlogX(lenX, Xval, proportional, minxval, orgX);
			m_showTestdataXPos.insert(X);
			Xval2 = m_showTestdataX[i + 1];
			Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
			m_showTestdataXPos.insert(Xnext);

			MoveToEx(dc, X, orgY - showdat[i] * perpointhight, NULL);
			LineTo(dc, Xnext, orgY - showdat[i + 1] * perpointhight);

			//MoveToEx(dc, i*jiange, showdat[i] + ymove, NULL);
			//LineTo(dc, (i + 1)*jiange, showdat[i + 1] + ymove);
		}
	}

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, slot4color);
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	/********************第4个图*******************/
	showdat.clear();
	GetShowData(m_showTestdata4, showdat, max);

	if (m_showTestdata4.size() > 2)
	{

		for (size_t i = 0; i < showdat.size() - 1; i++)
		{
			Xval = m_showTestdataX[i];
			//计算出x坐标点位置
			X = getDrawlogX(lenX, Xval, proportional, minxval, orgX);
			m_showTestdataXPos.insert(X);
			Xval2 = m_showTestdataX[i + 1];
			Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
			m_showTestdataXPos.insert(Xnext);

			MoveToEx(dc, X, orgY - showdat[i] * perpointhight, NULL);
			LineTo(dc, Xnext, orgY - showdat[i + 1] * perpointhight);
			//MoveToEx(dc, i*jiange, showdat[i] + ymove, NULL);
			//LineTo(dc, (i + 1)*jiange, showdat[i + 1] + ymove);
		}
	}


	



	/***************************上宽限绘制****************************/

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	showdat.clear();
	GetShowData(m_showTestdataBorderLineup, showdat, max);
	if (m_showTestdataBorderLineup.size() > 2)
	{

		for (size_t i = 0; i < showdat.size() - 1; i++)
		{
			Xval = m_showTestdataBorderLine[i];
			//计算出x坐标点位置
			X = getDrawlogX(lenX, Xval, proportional, minxval, orgX);
			m_showTestdataXPos.insert(X);
			Xval2 = m_showTestdataBorderLine[i + 1];
			Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
			m_showTestdataXPos.insert(Xnext);

			MoveToEx(dc, X, orgY - showdat[i] * perpointhight, NULL);
			LineTo(dc, Xnext, orgY - showdat[i + 1] * perpointhight);
			//MoveToEx(dc, i*jiange, showdat[i] + ymove, NULL);
			//LineTo(dc, (i + 1)*jiange, showdat[i + 1] + ymove);
		}
	}

	/***************************下宽限绘制****************************/
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	showdat.clear();
	GetShowData(m_showTestdataBorderLinedown, showdat, max);
	if (m_showTestdataBorderLinedown.size() > 2)
	{

		for (size_t i = 0; i < showdat.size() - 1; i++)
		{
			Xval = m_showTestdataBorderLine[i];
			//计算出x坐标点位置
			X = getDrawlogX(lenX, Xval, proportional, minxval, orgX);
			m_showTestdataXPos.insert(X);
			Xval2 = m_showTestdataBorderLine[i + 1];
			Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
			m_showTestdataXPos.insert(Xnext);

			MoveToEx(dc, X, orgY - showdat[i] * perpointhight, NULL);
			LineTo(dc, Xnext, orgY - showdat[i + 1] * perpointhight);
			//MoveToEx(dc, i*jiange, showdat[i] + ymove, NULL);
			//LineTo(dc, (i + 1)*jiange, showdat[i + 1] + ymove);
		}

	}

	//Y轴0***************************************Y轴的原点坐标显示*************************************
	//if (title == "Phase")
	{
		memset(ystr, 0, 1024);
		if (m_bownerDrwa_min!=0)
		  sprintf(ystr, "%f", -m_bownerDrwa_min);
		else
		{
			sprintf(ystr, "%d", 0);
		}
		strlen = std::strlen(ystr);
		dc.TextOutW(orgX - 8 * strlen - 5, orgY - 15, CString(ystr));
	}



	//绘制slot1图线
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&pen);
	dc.TextOutW(endX + 45, orgY - per_marshLen_Y*marshCount_Y-20, CString("Slot1"));

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, slot1color);
	oldpen = dc.SelectObject(&pen);
	MoveToEx(dc, endX + 10, orgY - per_marshLen_Y*marshCount_Y, NULL);
	LineTo(dc, endX+100, orgY - per_marshLen_Y*marshCount_Y);

	//绘制slot2图线
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&pen);
	dc.TextOutW(endX + 45, orgY - per_marshLen_Y*marshCount_Y+20 - 20, CString("Slot2"));

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, slot2color);
	oldpen = dc.SelectObject(&pen);
	MoveToEx(dc, endX + 10, orgY - per_marshLen_Y*marshCount_Y + 20, NULL);
	LineTo(dc, endX + 100, orgY - per_marshLen_Y*marshCount_Y + 20);

	//绘制slot3图线
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&pen);
	dc.TextOutW(endX + 45, orgY - per_marshLen_Y*marshCount_Y + 40 - 20, CString("Slot3"));

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, slot3color);
	oldpen = dc.SelectObject(&pen);
	MoveToEx(dc, endX + 10, orgY - per_marshLen_Y*marshCount_Y + 40, NULL);
	LineTo(dc, endX + 100, orgY - per_marshLen_Y*marshCount_Y + 40);


	//绘制slot4图线
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&pen);
	dc.TextOutW(endX + 45, orgY - per_marshLen_Y*marshCount_Y+60 - 20, CString("Slot4"));

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, slot4color);
	oldpen = dc.SelectObject(&pen);
	MoveToEx(dc, endX + 10, orgY - per_marshLen_Y*marshCount_Y + 60, NULL);
	LineTo(dc, endX + 100, orgY - per_marshLen_Y*marshCount_Y + 60);





}



void CDataShow::setdata1(const vector<double> dat, int nslot)
{
	switch (nslot)
	{
	case 1:
		setShowData(m_showTestdata, dat);
		break;
	case 2:
		setShowData(m_showTestdata2, dat);
		break;
	case 3:
		setShowData(m_showTestdata3, dat);
		break;
	case 4:
		setShowData(m_showTestdata4, dat);
		break;
	}
}

void CDataShow::setShowData(vector<double>& data, const vector<double> dat)
{
	data.clear();
	for (size_t i = 0; i < dat.size(); i++)
	{
		data.push_back(dat[i]);
	}
}

void CDataShow::GetShowData(const vector<double> data, vector< int>& tempdata, const double& max)
{
	


	
	


	
	tempdata.resize(data.size());
	if (data.size() == 0)
		return;

	for (size_t i = 0; i < data.size(); i++)
	{
		tempdata[i] = data[i] *yScale;

	}

}

int CDataShow::showDataMax(const vector<double>& data, double& max, bool m_scale)
{
	vector<int> tempdata;
	tempdata.resize(data.size());
	 max = 0;
	int maxi = 0;
	//yScale = 100; 外部控制
	for (size_t i = 0; i < data.size(); i++)
	{

		
		if (m_scale)
		  tempdata[i] = (data[i] * yScale);
		else
		{
			tempdata[i] = (data[i]);
		}

		if (tempdata[i]>max)
		{
			max = tempdata[i];
			maxi = i;
		}
	}

	return maxi;
}

void CDataShow::drawLog10()
{


	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	SetBkMode(dc, TRANSPARENT);
	CRect showdataXrc, TestResultrc;
	GetDlgItem(IDC_showdataX)->GetWindowRect(showdataXrc);
	GetDlgItem(IDC_datashowTestResult)->GetWindowRect(TestResultrc);
	ScreenToClient(showdataXrc);
	ScreenToClient(TestResultrc);
	if (!m_truncolor)
	{
		
		//dc.FillSolidRect(showdataXrc, RGB(255, 0, 0));
		//dc.FillSolidRect(TestResultrc, RGB(255, 0, 0));
		
	}
	else
	{
		//dc.FillSolidRect(showdataXrc, oldDlgcolor);
		//dc.FillSolidRect(TestResultrc, oldDlgcolor);
	}

	CFont* font = dc.GetCurrentFont();
	LOGFONT logfont;
	font->GetLogFont(&logfont);
	logfont.lfEscapement = 900;
	HFONT hFont = CreateFontIndirect(&logfont);
	dc.SelectObject(hFont);

	dc.TextOutW(30, 450 + cy_titl, m_Ytitle);//CString("图表按照Y轴旋转")
	dc.SelectObject(font);


	/***************X轴*****************/
	dc.TextOutW(endX - 100, orgY+25, m_Xtitle);

	/***************绘制测试结果************************/
	CFont resfont;
	resfont.CreatePointFont(200, _T("宋体"));
	CFont* oldf = dc.SelectObject(&resfont);
	//dc.TextOutW(endX - 250, endY - 100, result);//取消在结果对话框中显示测试结果
	dc.SelectObject(oldf);

	/*CRect rc_chart;
	GetDlgItem(IDC_CHART)->GetWindowRect(rc_chart);
	ScreenToClient(rc_chart);
	int l, t, r, b;
	l = rc_chart.left;
	t = rc_chart.top;
	r = rc_chart.right;
	b = rc_chart.bottom;*/
	//绘制
	/*int xMove = 50;
	int addXlen = 30;
	int orgX, orgY;
	int lenX, lenY;
	int endX, endY;*/
	//orgX = 80 + xMove;
	//orgY = 700;
	//endX = 1300 + xMove + addXlen;//x轴最远位置
	//endY = 180;//y轴最远位置

	//{
	//	orgX = l + xMove;
	//	orgY = t;
	//	endX = r + xMove + addXlen;//x轴最远位置
	//	endY = b;//y轴最远位置
	//}

	lenX = endX - orgX;//x轴实际绘制长度
	lenY = orgY - endY;//y轴实际绘制长度

	/****************绘制坐标轴*************************/
	CPen pen;
	pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen = dc.SelectObject(&pen);
	//绘制X轴
	dc.MoveTo(orgX, orgY);
	dc.LineTo(endX, orgY);

	//绘制Y轴
	dc.MoveTo(orgX, orgY);
	dc.LineTo(orgX, endY);

	dc.SelectObject(oldpen);
	/****************************************************/


	/********************以下横坐标点按照等比关系绘制*********************************/
	 double proportional = 10;
	//int dengfenshu = 10;//约定必须为偶数，方便开方计算

	if (m_showTestdataX.size() == 0)
		return;

	double minxval = m_showTestdataX[0];
	double maxxval = m_showTestdataX.back();

	proportional = maxxval / minxval;
	proportional = pow(proportional, 1.0 / dengfenshu);
	/*for (size_t i = 0; i < dengfenshu/2; i++)
	{
		proportional = sqrt(proportional);

	}*/

	/*double yval = 1;
	for (size_t j = 0; j < dengfenshu; j++)
	{
		yval *= proportional;
	}*/
	
	//proportional = ceil(proportional);
	
	/*********************在等分处绘制坐标点****************************/
	double xproval = 1;
	for (size_t i = 0; i <= dengfenshu; i++)
	{
		xproval = 1;
		
	     xproval = pow(proportional,i);
		
		//dc.TextOutW(orgX, orgY + 5, CString(to_string((int)(minxval)).c_str()));
		dc.TextOutW(orgX + lenX*i / dengfenshu*1.0, orgY + 5, getshowXstr(minxval*xproval));

		//2.绘制X轴的网格线
		if (i != 0)
		{
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1, RGB(141, 238, 238));
			oldpen = dc.SelectObject(&pen);
			SetBkMode(dc, TRANSPARENT);
		
		
			MoveToEx(dc, orgX + lenX*i / dengfenshu*1.0, orgY, NULL);
			LineTo(dc, orgX + lenX*i / dengfenshu*1.0, endY);

			dc.SelectObject(oldpen);
		}
		////dc.TextOutW(orgX + lenX / 4.0, orgY + 5, CString(to_string((int)(minxval*proportional)).c_str()));
		//dc.TextOutW(orgX + lenX / 4.0, orgY + 5, getshowXstr(minxval*proportional));
		////dc.TextOutW(orgX + lenX / 2.0, orgY + 5, CString(to_string((int)(minxval*proportional*proportional)).c_str()));
		//dc.TextOutW(orgX + lenX / 2.0, orgY + 5, getshowXstr(minxval*proportional*proportional));
		////dc.TextOutW(orgX + 3.0*lenX / 4.0, orgY + 5, CString(to_string((int)(minxval*proportional*proportional*proportional)).c_str()));
		//dc.TextOutW(orgX + 3.0*lenX / 4.0, orgY + 5, getshowXstr(minxval*proportional*proportional*proportional));
		////dc.TextOutW(orgX + lenX, orgY + 5, CString(to_string((int)(maxxval)).c_str()));
		//dc.TextOutW(orgX + lenX, orgY + 5, getshowXstr(minxval*proportional*proportional*proportional*proportional));


	}
	
	if (m_showTestdata.size() == 0 || m_showTestdataX.size() == 0)
	{
		//AfxMessageBox(_T("没有数据显示！"));
		return;
	}
	if (m_showTestdata.size() != m_showTestdataX.size())
	{
		//AfxMessageBox(_T("横纵坐标数目不等！"));
		//return;
		mainf->setStatusStr(_T("横纵坐标数目不等！"));
		return;
	}

	/*******************************Y轴的绘制，根据X轴的起点等差绘制**********************************************/
	//1.点坐标的绘制需要计算X坐标的位置区间


	vector<int> tempdata;
	tempdata.resize(m_showTestdata.size());
	double maxY = 0;
	double maxi = 0;

	int minY = 0;
	int mini = 0;
	bool btomax = false;
	//yScale = 100; 外部控制
	for (size_t i = 0; i < m_showTestdata.size(); i++)
	{
		if (m_showTestdata[i] < 0)
		{
			//AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
			mainf->setStatusStr(_T("显示数据格式错误，不能有负数！"));
			//return;
			m_showTestdata[i] = 0;
		}

		if (m_showTestdata[i] * yScale<0)
			btomax = true;

		tempdata[i] = (m_showTestdata[i] * yScale);

		if (i == 0)
			minY = tempdata[i];

		if (tempdata[i]>maxY)
		{
			maxY = tempdata[i];
			maxi = i;
		}

		if (tempdata[i]<minY)
		{
			minY = tempdata[i];
			mini = i;
		}
	}


	if (btomax)
	{
		for (size_t i = 0; i < m_showTestdata.size(); i++)
		{
			tempdata[i] = m_showTestdata[i];

			if (i == 0)
				minY = tempdata[i];

			if (tempdata[i]>maxY)
			{
				maxY = tempdata[i];
				maxi = i;
			}

			if (tempdata[i]<minY)
			{
				minY = tempdata[i];
				mini = i;
			}
		}
	}

	int maxvali = maxi;
	int minvali = mini;


	//这里根据客户需求增加多个测试结果曲线


	/**********************考虑框线的最值******************************/

	//。这里只考虑上框线
	vector<int> tempdataline;
	tempdataline.resize(m_showTestdataBorderLineup.size());
	btomax = false;
	for (size_t i = 0; i < m_showTestdataBorderLineup.size(); i++)
	{
		//if (m_showTestdataBorderLineup[i] < 0)
		//{
		//	AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
		//	//return;
		//	m_showTestdataBorderLineup[i] = 0;
		//}

		if (m_showTestdataBorderLineup[i] * yScale<0)
			btomax = true;

		tempdataline[i] = (m_showTestdataBorderLineup[i] * yScale);

		/*if (i == 0)
			minY = tempdataline[i];*/

		if (tempdataline[i]>maxY)
		{
			maxY = tempdataline[i];
			maxi = i;
		}

		/*if (tempdataline[i]<minY)
		{
			minY = tempdataline[i];
			mini = i;
		}*/
	}


	if (btomax)
	{
		for (size_t i = 0; i < m_showTestdataBorderLineup.size(); i++)
		{
			tempdataline[i] = m_showTestdataBorderLineup[i];

			/*if (i == 0)
				minY = tempdataline[i];*/

			if (tempdataline[i]>maxY)
			{
				maxY = tempdataline[i];
				maxi = i;
			}

			/*if (tempdataline[i]<minY)
			{
				minY = tempdataline[i];
				mini = i;
			}*/
		}
	}


	tempdataline.clear();
	tempdataline.resize(m_showTestdataBorderLinedown.size());
	for (size_t i = 0; i < m_showTestdataBorderLinedown.size(); i++)
	{
		//if (m_showTestdataBorderLineup[i] < 0)
		//{
		//	AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
		//	//return;
		//	m_showTestdataBorderLineup[i] = 0;
		//}

		if (m_showTestdataBorderLinedown[i] * yScale<0)
			btomax = true;

		tempdataline[i] = (m_showTestdataBorderLinedown[i] * yScale);

		/*if (i == 0)
		minY = tempdataline[i];*/

		//if (tempdataline[i]>maxY)
		//{
		//	maxY = tempdataline[i];
		//	maxi = i;
		//}

		if (tempdataline[i]<minY)
		{
			minY = tempdataline[i];
			mini = i;
		}
	}


	if (btomax)
	{
		for (size_t i = 0; i < m_showTestdataBorderLinedown.size(); i++)
		{
			tempdataline[i] = m_showTestdataBorderLinedown[i];

			if (i == 0)
			minY = tempdataline[i];

			/*if (tempdataline[i]>maxY)
			{
				maxY = tempdataline[i];
				maxi = i;
			}*/

			if (tempdataline[i]<minY)
			{
			minY = tempdataline[i];
			mini = i;
			}
		}
	}


	/***********************考虑框线的最值*****************************/



	double perpointhight = 0;

	if (maxY > 0)
	{
		perpointhight = 1.0*lenY / maxY;
	}



	if (maxY < lenY)
	{
		perpointhight = 1;
	}


	/****************************绘制Y坐标的刻度*****************************************/
	//最大值
	char ystr[1024]{0};
	char dotstr[1024]{0};
	memset(dotstr, 0, 1024);
	sprintf(dotstr, "%%.%df", golble_dotNum);
	double phase_max = 0;
	//if (title=="Phase")
	{
		phase_max=m_showTestdata[maxvali] - m_bownerDrwa_min;
		sprintf(ystr, dotstr, phase_max);
	}
	/*else
	{
		sprintf(ystr, dotstr, m_showTestdata[maxvali]);
	}*/
	
	int strlen = std::strlen(ystr);

	CString maxYshow(ystr);

	if (m_showTestdataBorderLineup.size() == 0 || maxi>=m_showTestdataBorderLineup.size())
	{
		//dc.TextOutW(orgX - 10 * strlen + 8, orgY - maxY * perpointhight, CString(ystr));
	}
	else
	{



		if (maxvali == maxi)
		{

			if (maxvali == 0 && m_showTestdata[maxvali] > m_showTestdataBorderLineup[maxi])
			{
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - maxY * perpointhight, CString(ystr));
			}
			else if (m_showTestdata[maxvali] > m_showTestdataBorderLineup[maxi])
			{
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - maxY * perpointhight, CString(ystr));
			}
			else
			{
				memset(ystr, 0, 1024);
				sprintf(ystr, "%.1f", m_showTestdataBorderLineup[maxi]);
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - maxY * perpointhight, CString(ystr));
			}

		}
		else
		{
			if (m_showTestdata[maxvali] > m_showTestdataBorderLineup[maxi])
			{

				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - maxY * perpointhight, CString(ystr));
			}
			else
			{
				memset(ystr, 0, 1024);
				sprintf(ystr, "%.1f", m_showTestdataBorderLineup[maxi]);
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - m_showTestdataBorderLineup[maxi] * perpointhight* yScale, CString(ystr));

			}
		}

	}


	//最小值
	memset(ystr, 0, 1024);
	memset(dotstr, 0, 1024);
	sprintf(dotstr, "%%.%df", golble_dotNum);
	//if (title == "Phase")
	{
		phase_max=m_showTestdata[minvali] - m_bownerDrwa_min;
		sprintf(ystr, dotstr, phase_max);
	}
	/*else
	{
		sprintf(ystr, dotstr, m_showTestdata[minvali]);
	}*/
	
	strlen = std::strlen(ystr);
	CString minYshow(ystr);
	if (m_showTestdataBorderLinedown.size() == 0 || mini >= m_showTestdataBorderLinedown.size())
	{
		//dc.TextOutW(orgX - 10 * strlen + 8, orgY - minY* perpointhight, CString(ystr));
	}
	else
	{



		if (minvali == mini)
		{
			if (minvali == 0 && m_showTestdata[minvali] < m_showTestdataBorderLinedown[mini])
			{
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - minY* perpointhight, CString(ystr));
			}
			else if (m_showTestdata[minvali] < m_showTestdataBorderLinedown[mini])
			{
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - minY* perpointhight, CString(ystr));
			}
			else
			{
				memset(ystr, 0, 1024);
				sprintf(ystr, "%.1f", m_showTestdataBorderLinedown[mini]);
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - minY* perpointhight, CString(ystr));
			}

		}
		else
		{
			if (m_showTestdata[minvali] < m_showTestdataBorderLinedown[mini])
			{
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - minY * perpointhight, CString(ystr));
			}
			else
			{
				memset(ystr, 0, 1024);
				sprintf(ystr, "%.1f", m_showTestdataBorderLinedown[mini]);
				//dc.TextOutW(orgX - 10 * strlen + 8, orgY - m_showTestdataBorderLinedown[mini] * perpointhight* yScale, CString(ystr));

			}
		}
	}


	/************************************绘制网格线****************************************/
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(141, 238, 238));
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
    //1.分别绘制X 与Y轴网格线
	double per_marshLen_Y = 0;
	double permarshVal = 0;
	per_marshLen_Y = 1.0*lenY / (marshCount_Y+1);//绘图长度  -->计算对应实际刻度
	permarshVal = 1.0*maxY / (marshCount_Y + 1);
	//if (title == "Phase")
	{
		maxY -= m_bownerDrwa_min*yScale;
		permarshVal = 1.0*(maxY + m_bownerDrwa_min*yScale) / (marshCount_Y + 1);
	}
	

	//判断使用几位小数
	
	

	for (size_t i = 1; i <= marshCount_Y+1; i++)
	{
		MoveToEx(dc, orgX, orgY - per_marshLen_Y*i,NULL);
		LineTo(dc, endX, orgY - per_marshLen_Y*i);

		//刻度
		memset(dotstr, 0, 1024);
		sprintf(dotstr, "%%.%df", golble_dotNum);
		//if (title == "Phase")
			sprintf(ystr, dotstr, permarshVal*i / yScale - m_bownerDrwa_min);
		/*else
		{
			sprintf(ystr, dotstr, permarshVal*i / yScale);
		}*/
		strlen = std::strlen(ystr);
		dc.TextOutW(orgX - 8*strlen-5, orgY - per_marshLen_Y*i-5, CString(ystr));

	}

	//Y轴0***************************************Y轴的原点坐标显示*************************************
	//if (title == "Phase")
	{
		memset(ystr, 0, 1024);
		sprintf(ystr, "%f", -m_bownerDrwa_min);
		strlen = std::strlen(ystr);
		dc.TextOutW(orgX - 8 * strlen - 5, orgY - 15, CString(ystr));
	}
	/*else
	{
		
		dc.TextOutW(orgX - 10, orgY - 15, CString("0"));
	}*/
	

	






	/*****************************绘制测试结果图像****************************************/

	CString zeroStr("0.");
	for (size_t i = 0; i < golble_dotNum; i++)
	{
		zeroStr.Append(_T("0"));
	}

	double Xval = 0;
	double Xval2 = 0;
	int X = 0;
	int Xnext = 0;
	//设置绘图颜色
	pen.DeleteObject();
	//pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 255));
	pen.CreatePen(BS_SOLID, 2, m_color);
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	for (size_t i = 0; i < m_showTestdataX.size()-1; i++)
	{

		Xval = m_showTestdataX[i];
		//计算出x坐标点位置
		X=getDrawlogX(lenX, Xval, proportional, minxval, orgX);
		m_showTestdataXPos.insert(X);
		Xval2 = m_showTestdataX[i+1];
		Xnext = getDrawlogX(lenX, Xval2, proportional, minxval, orgX);
		m_showTestdataXPos.insert(Xnext);

		MoveToEx(dc, X, orgY - tempdata[i] * perpointhight, NULL);
		LineTo(dc, Xnext, orgY - tempdata[i + 1] * perpointhight);



		/***************************绘制Y最值************************************/
		
		if (maxvali == m_showTestdataX.size() - 1)
		{
			
			if (i == maxvali-1)
			{
				if (maxYshow.Compare(zeroStr) != 0)
				  dc.TextOutW(Xnext, orgY - tempdata[i+1] * perpointhight, maxYshow);
			}
		}
		else
		{
			if (i == maxvali)
			{
				if (maxYshow.Compare(zeroStr) != 0)
				   dc.TextOutW(X, orgY - tempdata[i] * perpointhight, maxYshow);
			}
		}


		if (minvali == m_showTestdataX.size() - 1)
		{
			
			if (i == minvali-1)
			{
				if (minYshow.Compare(zeroStr) != 0)
				   dc.TextOutW(Xnext, orgY - tempdata[i+1] * perpointhight, minYshow);
			}
		}
		else
		{
			if (i == minvali)
			{
				if (minYshow.Compare(zeroStr) != 0)
				    dc.TextOutW(X, orgY - tempdata[i] * perpointhight, minYshow);
			}

		}

		
		



	}

	


	/************************绘制框线******************************/
	double borderX = 0;
	int borderX1 = 0;
	int borderX2 = 0;


	if (m_showTestdataBorderLine.size() > 1)
	{
	
		for (size_t i = 0; i < m_showTestdataBorderLine.size() - 1; i++)
		{

			borderX = m_showTestdataBorderLine[i];

			//计算出实际绘图的坐标点
			borderX1 = getDrawlogX(lenX, borderX, proportional, minxval, orgX);
			borderX = m_showTestdataBorderLine[i+1];
			borderX2 = getDrawlogX(lenX, borderX, proportional, minxval, orgX);

			//set color
			pen.DeleteObject();
			//pen.CreatePen(BS_SOLID, 2, RGB(87, 0, 120));
			pen.CreatePen(BS_SOLID, 2, RGB(255, 0, 0));
			dc.SelectObject(&pen);
			SetBkMode(dc, TRANSPARENT);
			if (m_showTestdataBorderLinedown.size()>0 && i<m_showTestdataBorderLinedown.size()-1)
			{
				MoveToEx(dc, borderX1, orgY - m_showTestdataBorderLinedown[i] * perpointhight* yScale, NULL);
				LineTo(dc, borderX2, orgY - m_showTestdataBorderLinedown[i + 1] * perpointhight* yScale);
			}
			//set color
			pen.DeleteObject();
			pen.CreatePen(BS_SOLID, 2, RGB(255, 0, 0));
			dc.SelectObject(&pen);
			SetBkMode(dc, TRANSPARENT);
			if (m_showTestdataBorderLineup.size()>0)
			{
				MoveToEx(dc, borderX1, orgY - m_showTestdataBorderLineup[i] * perpointhight* yScale, NULL);
				LineTo(dc, borderX2, orgY - m_showTestdataBorderLineup[i + 1] * perpointhight* yScale);
			}
		}
	}


	/******************************************************/


	/*****************************************************************************/
	
	dc.SelectObject(oldpen);

	
}

double CDataShow::GetbownerDrwa_min()
{
	return m_bownerDrwa_min;
}

void CDataShow::setOwnerDraw(bool bowner, double min)
{
	
		m_bownerDrwa = bowner;
		m_bownerDrwa_min = min;
	
	
}

bool CDataShow::checkDataIsNULL()
{
	bool ret = true;
	if (m_showTestdata.size() == 0 || m_showTestdataX.size() == 0)
	{
		//AfxMessageBox(_T("没有数据显示！"));
		return false;
	}
	if (m_showTestdata.size() != m_showTestdataX.size())
	{
		//AfxMessageBox(_T("横纵坐标数目不等！"));
		//return;
		mainf->setStatusStr(_T("横纵坐标数目不等！"));
		return false;
	}

	if (m_showTestdata2.size() == 0 || m_showTestdataX.size() == 0)
	{
		//AfxMessageBox(_T("没有数据显示！"));
		return false;
	}
	if (m_showTestdata2.size() != m_showTestdataX.size())
	{
		//AfxMessageBox(_T("横纵坐标数目不等！"));
		//return;
		mainf->setStatusStr(_T("横纵坐标数目不等！"));
		return false;
	}


	if (m_showTestdata3.size() == 0 || m_showTestdataX.size() == 0)
	{
		//AfxMessageBox(_T("没有数据显示！"));
		return false;
	}
	if (m_showTestdata3.size() != m_showTestdataX.size())
	{
		//AfxMessageBox(_T("横纵坐标数目不等！"));
		//return;
		mainf->setStatusStr(_T("横纵坐标数目不等！"));
		return false;
	}


	if (m_showTestdata4.size() == 0 || m_showTestdataX.size() == 0)
	{
		//AfxMessageBox(_T("没有数据显示！"));
		return false;
	}
	if (m_showTestdata4.size() != m_showTestdataX.size())
	{
		//AfxMessageBox(_T("横纵坐标数目不等！"));
		//return;
		mainf->setStatusStr(_T("横纵坐标数目不等！"));
		return false;
	}

	return true;
}

int CDataShow::getDrawBorderLineX(int XLen, double xdata, const set<double>& XPos)
{
	set<double>::iterator var;
	for (var = XPos.begin(); var != XPos.end(); var++)
	{
		if (*var > xdata || abs(*var - xdata) <= 0.0000000001)
		{
               
		}
	}
	return 0;
}

int CDataShow::getDrawlogX(int XLen, double xdata, double proportional, double xorgval,int XOrg)
{
	int X=0;

	double xproval = 1;
	double xperorg = 0;
	double xdiffer = 0;
	double perlen = 0;
	double curlen = 0;
	for (size_t i = 0; i <= dengfenshu; i++)
	{
		xproval = 1;

		xproval = xorgval*pow(proportional, i);

		if (abs(xdata - xproval) <= 0.0000000001 || xdata < xproval)//从最小值开始比较的，因此小于最小值
		{
			//这里计算出X轴坐标点 起点为i-1

			if (i >1)
			{
				X += XOrg;
				X += (i - 1)*XLen / dengfenshu;
			}
			else
			{
				
				X += XOrg;
				if (i == 0)
					return X;
			}

			//以下按照等差绘制
			xperorg = xorgval*pow(proportional, i - 1);
			xdiffer = xproval - xperorg;//数值差 与绘图坐标差比
			perlen = XLen / dengfenshu;
			curlen = (perlen / xdiffer)*(xdata - xperorg);
			X += curlen;

			break;

		}

	}

	return X;
}

void CDataShow::setProportional(int dengfen)
{
	dengfenshu = dengfen;
}

CString CDataShow::getshowXstr(double xval)
{
	
	bool havedot = false;
	char buf[100];
	//单位换算
	if (xval >= 1000)
	{
		xval /= 1000.0;

		//判断一个数是否含有小数

		sprintf(buf, "%.1fk", xval);

		for (size_t i = 0; i < 100; i++)
		{
			if (buf[i] == '.')
			{
				if (i<99 && buf[i + 1]>48)
				{
					havedot = true;
				}

				break;
			}
		}

		if (!havedot)
			sprintf(buf, "%.0fk", xval);
	}
	else
	{
		sprintf(buf, "%.0f", xval);
	}
     
	return CString(buf);
}

void CDataShow::drawProportional()
{
	CString charttitle;
	GetDlgItem(IDC_showdatachartname)->GetWindowTextW(charttitle);

	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()


	CFont* font = dc.GetCurrentFont();
	LOGFONT logfont;
	font->GetLogFont(&logfont);
	logfont.lfEscapement = 900;
	HFONT hFont = CreateFontIndirect(&logfont);
	dc.SelectObject(hFont);

	dc.TextOutW(30, 450, m_Ytitle);//CString("图表按照Y轴旋转")
	dc.SelectObject(font);



	//绘制
	int orgX, orgY;
	int lenX, lenY;
	int endX, endY;
	orgX = 80;
	orgY = 700;
	endX = 1300;//x轴最远位置
	endY = 180;//y轴最远位置
	lenX = endX - orgX;//x轴实际绘制长度
	lenY = orgY - endY;//y轴实际绘制长度

	int xvalmove = 35;

	//绘制坐标原点
	SetBkMode(dc, TRANSPARENT);
	//dc.TextOutW(orgX - 25, orgY - 10, CString("0.0"));

	SetBkMode(dc, OPAQUE);

	CPen pen;
	pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen = dc.SelectObject(&pen);
	//绘制X轴
	dc.MoveTo(orgX, orgY);
	dc.LineTo(endX, orgY);

	//绘制Y轴
	dc.MoveTo(orgX, orgY);
	dc.LineTo(orgX, endY);

	dc.SelectObject(oldpen);

	//测试数据显示，归一从处理
	int val = 0;
	//x 根据显示的宽度计算   需要显示的信号长度

	if (m_showTestdata.size() == 0 || m_showTestdataX.size() == 0)
	{
		//AfxMessageBox(_T("没有数据显示！"));
		return;
	}
	if (m_showTestdata.size() != m_showTestdataX.size())
	{
		//AfxMessageBox(_T("横纵坐标数目不等！"));
		//return;
		mainf->setStatusStr(_T("横纵坐标数目不等！"));
	}

	double perpointlen = 1.0*lenX / m_showTestdata.size();//每个信号的点占用绘图实际的长度
	double jiange = perpointlen;//每个信号的显示间隔长度

	vector<int> tempdata;
	tempdata.resize(m_showTestdata.size());
	int maxY = 0;
	int maxi = 0;
	bool btomax = false;
	//yScale = 100; 外部控制
	for (size_t i = 0; i < m_showTestdata.size(); i++)
	{
		if (m_showTestdata[i] < 0)
		{
			//AfxMessageBox(_T("显示数据格式错误，不能有负数！"));
			//return;
			m_showTestdata[i] = 0;
		}

		if (m_showTestdata[i] * yScale<0)
			btomax = true;

		tempdata[i] = (m_showTestdata[i] * yScale);

		if (tempdata[i]>maxY)
		{
			maxY = tempdata[i];
			maxi = i;
		}
	}


	if (btomax)
	{
		for (size_t i = 0; i < m_showTestdata.size(); i++)
		{
			tempdata[i] = m_showTestdata[i];

			if (tempdata[i]>maxY)
			{
				maxY = tempdata[i];
				maxi = i;
			}
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

	//设置绘图颜色
	pen.DeleteObject();
	pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 255));
	oldpen = dc.SelectObject(&pen);
	SetBkMode(dc, TRANSPARENT);
	char buf[100];
	double xval = 0;
	double yval = 0;
	bool havedot = false;

	if (tempdata.size() == 1)
	{
		pen.DeleteObject();
		pen.CreatePen(BS_SOLID, 10, RGB(0, 0, 255));
		dc.SelectObject(&pen);
		MoveToEx(dc, orgX, orgY - tempdata[0] * perpointhight, NULL);
		LineTo(dc, orgX + 4, orgY - tempdata[0] * perpointhight);
		MoveToEx(dc, orgX + 4, orgY - tempdata[0] * perpointhight + 4, NULL);
		LineTo(dc, orgX, orgY - tempdata[0] * perpointhight + 4);
	}

	for (size_t i = 0; i < tempdata.size() - 1; i++)
	{
		MoveToEx(dc, i*jiange + orgX, orgY - tempdata[i] * perpointhight, NULL);
		LineTo(dc, (i + 1)*jiange + orgX, orgY - tempdata[i + 1] * perpointhight);

		if (i == (tempdata.size() - 1) / 2)
		{
			//显示x 与y轴数值
			xval = m_showTestdataX.back() / 2;// i*jiange + orgX;

			//单位换算
			if (xval > 1000)
			{
				xval /= 1000.0;

				//判断一个数是否含有小数

				sprintf(buf, "%.1fk", xval);

				for (size_t i = 0; i < 100; i++)
				{
					if (buf[i] == '.')
					{
						if (i<99 && buf[i + 1]>48)
						{
							havedot = true;
						}

						break;
					}
				}

				if (!havedot)
					sprintf(buf, "%.0fk", xval);
			}
			else
			{
				sprintf(buf, "%.0f", xval);
			}


			dc.TextOutW(i*jiange + orgX, orgY, CString(buf));


			memset(buf, 0, 100);
			yval = 0.5;// m_showTestdata[i];
			sprintf(buf, "%.1f", yval);
			//dc.TextOutW(orgX - 25, orgY - tempdata[i] * perpointhight, CString(buf));


		}




	}

	//if (i == tempdata.size() - 2)
	{
		//绘制x轴终点值
		memset(buf, 0, 100);
		xval = m_showTestdataX.back();// (tempdata.size() - 1)*jiange + orgX;
		//sprintf(buf, "%.0f", xval);
		//单位换算
		if (xval > 1000)
		{
			xval /= 1000.0;
			sprintf(buf, "%.1fk", xval);
		}
		else
		{
			sprintf(buf, "%.0f", xval);
		}
		dc.TextOutW(endX, orgY, CString(buf));

		//绘制起点的频率 X坐标起点值
		memset(buf, 0, 100);
		xval = m_showTestdataX[0];
		//单位换算
		if (xval > 1000)
		{
			xval /= 1000.0;
			sprintf(buf, "%.1fk", xval);
		}
		else
		{
			sprintf(buf, "%.0f", xval);
		}
		//sprintf(buf, "%.0f", xval);
		dc.TextOutW(orgX - 5, orgY, CString(buf));

		//Y坐标终点值
		memset(buf, 0, 100);
		if (charttitle == "thd")
		{

			int convertint = m_showTestdata[maxi] * 1000;

			yval = 1.0*convertint / 1000.0;
			sprintf(buf, "%.2f", yval);
			int strlen = std::strlen(buf);

			dc.TextOutW(orgX - 10 * strlen + 8, endY, CString(buf));
		}
		else
		{
			yval = m_showTestdata[maxi] / 1;
			sprintf(buf, "%.1f", yval);
			int strlen = std::strlen(buf);
			dc.TextOutW(orgX - 10 * strlen + 8, endY, CString(buf));
		}



	}

	dc.SelectObject(oldpen);
}

void CDataShow::setData(vector<double>* data, int size)
{
	m_showTestdata.clear();
	for (size_t i = 0; i < size; i++)
	{
		m_showTestdata.push_back(data->at(i));
	}
	
}

void CDataShow::getData(vector<double>& data, int nSlot)
{
	data.clear();
	if (nSlot == 1)
	{
		for (size_t i = 0; i < m_showTestdata.size(); i++)
		{
			data.push_back(m_showTestdata[i]);
		}
	}
	else if (nSlot == 2)
	{
		for (size_t i = 0; i < m_showTestdata2.size(); i++)
		{
			data.push_back(m_showTestdata2[i]);
		}
	}
	else if (nSlot == 3)
	{
		for (size_t i = 0; i < m_showTestdata3.size(); i++)
		{
			data.push_back(m_showTestdata3[i]);
		}
	}
	else if (nSlot == 4)
	{
		for (size_t i = 0; i < m_showTestdata4.size(); i++)
		{
			data.push_back(m_showTestdata4[i]);
		}
	}
	
	
}

void CDataShow::setDataX(vector<double>* data, int size)
{
	if (data == NULL)
		return;

	m_showTestdataX.clear();
	for (size_t i = 0; i < size; i++)
	{
		m_showTestdataX.push_back(data->at(i));
	}
}

void CDataShow::setDataBorderLine(const vector<double>& dataBorderLine, int size)
{
	if (&dataBorderLine == NULL)
		return;

	m_showTestdataBorderLine.clear();
	for (size_t i = 0; i < size; i++)
	{
		m_showTestdataBorderLine.push_back(dataBorderLine.at(i));
	}
}

void CDataShow::setDataBorderLinedown(const vector<double>& dataBorderLinedown, int size)
{
	if (&m_showTestdataBorderLinedown == NULL)
		return;

	m_showTestdataBorderLinedown.clear();
	for (size_t i = 0; i < size; i++)
	{
		m_showTestdataBorderLinedown.push_back(dataBorderLinedown.at(i));
	}
}

void CDataShow::setDataBorderLineup(const vector<double>& dataBorderLineup, int size)
{
	if (&m_showTestdataBorderLineup == NULL)
		return;
	m_showTestdataBorderLineup.clear();
	for (size_t i = 0; i < size; i++)
	{
		m_showTestdataBorderLineup.push_back(dataBorderLineup.at(i));
	}

}

void CDataShow::setYtitle(CString titleY)
{
	m_Ytitle = titleY;
}

void CDataShow::setYscale(int yscale)
{
	yScale = yscale;
}

void CDataShow::setXtitle(CString titleX)
{
	//GetDlgItem(IDC_showdataX)->SetWindowTextW(titleX);
	m_Xtitle = titleX;

}

void CDataShow::setTestResult(bool isSuccess)
{
	m_isSuccess = isSuccess;
	if (m_isSuccess)
	{
		result = "Pass";
	}
	else
	{
		result = "NG";
		//SetTimerStart();//刷新背景有bug
		//Invalidate();//不刷新背景色
	}

	UpdateData(FALSE);

}

HBRUSH CDataShow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	

	// TODO:  在此更改 DC 的任何特性

	if (pWnd->GetDlgCtrlID() == IDC_datashowTestResult)
	{
		CPen pen;
		pen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
		pDC->SelectObject(&pen);

		CFont font;
		font.CreatePointFont(150, _T("宋体"), NULL);
		pDC->SelectObject(&font);
		if (m_isSuccess)
		{	
			pDC->SetBkColor(RGB(56, 139, 87));
		
		}
		else
		{
			
			pDC->SetBkColor(RGB(255, 0, 0));	
		}
		
		
	}

	if (nCtlColor == CTLCOLOR_DLG)
	{
		HBRUSH br;
		/*if (m_truncolor)闪烁显示
		   br = CreateSolidBrush(oldDlgcolor);
		else
		{
			br = CreateSolidBrush(RGB(255,0,0));
		}*/

		//br = CreateSolidBrush(RGB(255, 215, 0));
		//hbr = br;


	}

	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDataShow::setResultStr(CString str)
{
	result = str;
}


void CDataShow::OnMove(int x, int y)
{


	return;

	CDialogEx::OnMove(x, y);

	// TODO:  在此处添加消息处理程序代码
	if (title.Compare(CString("")) == 0)
	{
		return;
	}


	std::fstream out;
	if (title.Compare(CString("FR")) == 0)
	{
		/*out.open("frpos.txt", ios::out | ios::trunc);
		if (!out.is_open())
			return;*/
		 moveYFR = y;
		 moveXFR =x;

		 //out << moveYFR << endl;
		 //out << moveXFR << endl;
	}
	else if (title.Compare(CString("THD")) == 0)
	{
		/*out.open("thd.txt", ios::out | ios::trunc);
		if (!out.is_open())
			return;*/
		 moveYTHD = y;
		 moveXTHD = x;
		// out << moveYTHD << endl;
		// out << moveXTHD << endl;
	}
	else if (title.Compare(CString("THD+N")) == 0)
	{
		/*out.open("thdn.txt", ios::out | ios::trunc);
		if (!out.is_open())
			return;*/
		moveYTHDN = y;
		 moveXTHDN = x;
		// out << moveYTHDN << endl;
		// out << moveXTHDN << endl;
	}
	else if (title.Compare(CString("RB")) == 0)
	{
		/*out.open("rb.txt", ios::out | ios::trunc);
		if (!out.is_open())
			return;*/
		 moveYRB = y;
		 moveXRB = x;
		// out << moveYRB << endl;
		// out << moveXRB << endl;

	}
	else if (title.Compare(CString("Phase")) == 0)
	{
		/*out.open("Phase.txt", ios::out | ios::trunc);
		if (!out.is_open())
			return;*/
		moveYPhase = y;
		moveXPhase = x;
		//out << moveYPhase << endl;
		//out << moveXPhase << endl;

	}
	


	out.close();

	

}

void CDataShow::setColor(COLORREF clo)
{
	m_color = clo;
}


void CDataShow::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO:  在此处添加消息处理程序代码
	GetWindowRect(rc);
	ScreenToClient(rc);
	

}


void CDataShow::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO:  在此处添加消息处理程序代码
	GetWindowRect(rc);
	ScreenToClient(rc);


}


void CDataShow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码

	//根据窗口变化，更改坐标轴
	double scale = 1000000.0;
	if (cx != frm_w || cy != frm_h)
	{


		if (cy != frm_h&&frm_h!=0)
		{
			if (cy < frm_h)
			{
				//orgY *= 1.0*cy / frm_h;
				
			}
			else
			{
				/*orgY *= ceil(scale*cy / frm_h);
				orgY /= scale;*/
				
			}
			orgY += (cy - frm_h);
			//
			if ((res_rc.top + (cy - res_h)) > 4
				&& (res_rc.top + (cy - res_h))<123)
			{
				//res_rc.top += (cy - res_h);
				//res_rc.bottom += (cy - res_h);
			}

			
			

			//chart_rc.top += (cy - frm_h);
			chart_rc.bottom += (cy - frm_h);

			x_rc.top += (cy - frm_h);
			x_rc.bottom += (cy - frm_h);


			//endY *= 1.0*cy / frm_h;;

			
			cy_titl += (cy - frm_h);
			if (cy_titl < -139)
			{
				cy_titl = -139;
			}

			if (cy_titl>45)
			{
				cy_titl = 45;
			}
		}

		if (cx != frm_w&&frm_w!=0)
		{
			/*if (cx < frm_w)
			{
				endX *= 1.0*cx / frm_w;
			}
			else
			{
				endX *= ceil(scale*cx / frm_w);
				endX /= scale;
			}*/
			endX += (cx - frm_w);
			res_rc.left += (cx - frm_w);
			res_rc.right += (cx - frm_w);

			//chart_rc.left += (cx - frm_w);
			chart_rc.right += (cx - frm_w);

			x_rc.left += (cx - frm_w);
			x_rc.right += (cx - frm_w);

		}


		if (title == "Phase")
		{
			for (size_t i = 0; i < m_showTestdata.size(); i++)
			{

			}
		}
		
		

		Invalidate();
		if ((res_rc.top + (cy - res_h)) > 4)
		{
			//res_w = frm_w;
			res_h = cy;
		}
		frm_w = cx;
		frm_h = cy;

	}

	
}


void CDataShow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	m_truncolor = !m_truncolor;

	CDialogEx::OnTimer(nIDEvent);

	Invalidate();
	m_blindTimes++;
	if (m_blindTimes > 10)
	{
		m_truncolor = true;
		SetTimerStop();
	}
}

bool CDataShow::getTestResult()
{
	return m_isSuccess;
}
