// JiLiSet.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "JiLiSet.h"
#include "afxdialogex.h"
#include "fftw3.h"
#include "resource.h"
#include <string.h>
#include <string>
#include <math.h>


extern int m_docId;

// CJiLiSet 对话框

IMPLEMENT_DYNAMIC(CJiLiSet, CDialogEx)

CJiLiSet::CJiLiSet(CWnd* pParent /*=NULL*/)
: CDialogEx(CJiLiSet::IDD, pParent), x_point(NULL),
fs(44100.0), min_period(15), min_time(15), voltage(0.5), scale(1),
start_frequency(600), stop_frequency(2000), num_dot(0), scaleY(1), isdrawrun(false), scaleY1(1.0),
bfenli(false)
{

	//double db = 100 - 20 * log10(2);
	/*double a = 114.0 / 20.0;
	double l2 = log10(2.0);
	int zhishu = a + l2 - 5;
	double pa = pow(10, zhishu);

	pa = 0.0;
	pa = getPa(114);

	double db = getDb(1);


	int N=2000;
	double myPI=3.1415926;
	int f=22;
	int f2=1000;
	int A=10;
	int sample=44100;

	fftw_plan p;

	fftw_complex* in =(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
	fftw_complex* out =(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);


	for (int i = 0; i < N; i++)
	{
		in[i][0]=A*sin(2.0*myPI*f*i/sample)+20*sin(2.0*myPI*f2*i/sample);
		in[i][1]=0;
	}

	p=fftw_plan_dft_1d(N,in,out,FFTW_FORWARD,FFTW_ESTIMATE);

	fftw_execute(p);

	double real=0.0;
	double vitru=0.0;
	double fs[2000];
	double Ap[2000];
	for (int i = 0; i < N; i++)
	{
		fs[i]=(i*sample)/N;
		printf("%.2f + %.2fi",out[i][0],out[i][1]);
	}
	Ap[0]=sqrt(out[0][0]*out[0][0]+out[0][1]*out[0][1])/N;
	for (int i = 1; i < N; i++)
	{
	  Ap[i]=2*sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1])/N;
	}
*/

	pindian.clear();

	//pindian.push_back(20);
	//pindian.push_back(1000);

	pindian.push_back(20);//0
	pindian.push_back(25);
	pindian.push_back(32);
	pindian.push_back(40);
	pindian.push_back(50);
	pindian.push_back(63);
	pindian.push_back(80);
	pindian.push_back(100);
	pindian.push_back(125);
	pindian.push_back(160);
	pindian.push_back(200);
	pindian.push_back(250);
	pindian.push_back(315);
	pindian.push_back(400);//13

	pindian.push_back(500);//14
	pindian.push_back(630);
	pindian.push_back(800);
	pindian.push_back(1000);
	pindian.push_back(1250);
	pindian.push_back(1600);
	pindian.push_back(2000);
	pindian.push_back(2500);
	pindian.push_back(3150);
	pindian.push_back(4000);
	pindian.push_back(5000);
	pindian.push_back(6300);
	pindian.push_back(8000);
	pindian.push_back(10000);
	pindian.push_back(12500);
	pindian.push_back(16000);
	pindian.push_back(20000);



}

CJiLiSet::~CJiLiSet()
{
}

void CJiLiSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDITV, editV);
	DDX_Text(pDX, IDC_EDITV, voltage);
	//DDV_MinMaxFloat(pDX, voltage, 0, 3.5);
	DDX_Control(pDX, IDC_GENXIN, m_title);

	DDX_Check(pDX, IDC_CHECKfenli, bfenli);

	
}

void AFXAPI CJiLiSet::DDV_MinMaxFloat(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{


	return;

}

double CJiLiSet::getDb(double p)
{
	return 20.0*log10(p) - 20.0*log10(2.0*pow(10, -5));
}
double CJiLiSet::getPa(double db)
{

	double a = db / 20.0;
	double l2 = log10(2.0);
	int zhishu = a + l2 - 5;
	double pa = pow(10, zhishu);

	return pa;
}


BOOL CJiLiSet::OnInitDialog()
{

	

	UpdateData(FALSE);//更新界面数据，默认方向控件到变量
	


	
	
	
	
	return TRUE;
}

void CJiLiSet::updatePindian()
{

}

LRESULT CJiLiSet::DrawJiLiSet(WPARAM wParam,LPARAM lParam)
{
	

	
	//pBmp.GetCurrentBmp();



	return 0;
}


BEGIN_MESSAGE_MAP(CJiLiSet, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(ID_JILISET,DrawJiLiSet)
	ON_BN_CLICKED(IDC_BUTTON1, &CJiLiSet::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CJiLiSet::OnBnClickedButtonFangDa)
	ON_BN_CLICKED(IDC_BUTTON3, &CJiLiSet::OnBnClickedButtonSuoXiao)
	ON_STN_CLICKED(IDC_JILISET, &CJiLiSet::OnStnClickedJiliset)
	ON_WM_CTLCOLOR()
	ON_EN_UPDATE(IDC_EDITV, &CJiLiSet::OnEnUpdateEditv)
	ON_WM_MOUSEWHEEL()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CJiLiSet::OnBnClickedButtonYfangda)
END_MESSAGE_MAP()


// CJiLiSet 消息处理程序


void CJiLiSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting

		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CDialogEx::OnPaint()
	
	//if (m_docId==5)
	   OnBnClickedButton1();

}


void CJiLiSet::OnBnClickedButton1()
{
	
	min_time = 15;
	double A = 1000000000.0;
	
	
	if (this->m_hWnd)
	   UpdateData(TRUE);//更新界面数据，默认方向控件到变量

	CFont font;
	font.CreatePointFont(124, _T("宋体"));
	if (this->m_hWnd)
	   m_title.SetFont(&font);
	
	
	// TODO:  在此添加控件通知处理程序代码
	data.clear();
	data2.clear();
	//加10ms的延时信号
	
	int yanshi = round(0.01 *fs);

	for (size_t i = 0; i < 1000; i++)
	{

		//data.push_back(0);

	}

	int f;
	for (int i = 0; i < pindian.size();i++)
	{
		f = pindian[i];
		num_dot++;

		//if (f != 1000 /*&& f !=20*/)
		//	continue;


		int cur_samples = 0;
		
		if (f <= 1000)
		{
			cur_samples = min_period * fs / f;

		}
		else
		{

			cur_samples = round(min_time*0.001 * f)*fs / f;
		}

		
		pos[f] = cur_samples;

		//cur_samples = 44100 * 4;
		for (size_t j = 0; j <cur_samples; j++)
		{

			if (bfenli)
			{
				//计算当前频点下的数据
				if (f <= 400)
					data.push_back(pow(2, 0.5)*voltage*A*sin(2.0 * 3.1415926*f*j / fs));//
				else
					data2.push_back(pow(2, 0.5)*voltage*A*sin(2.0 * 3.1415926*f*j / fs));//
			}
			else
			{
				data.push_back(1.0*voltage*sin(2.0 * 3.1415926*f*j / fs));//
			}


		}

#if 0
		switch (4)
		{

		case 0:

			f = round(start_frequency*pow(2.0, num_dot));
			break;

		case 1:
			f = round(start_frequency*pow(2.0, num_dot / 3.0));
			break;
		case 2:
			f = round(start_frequency*pow(2.0, num_dot / 6.0));
			break;
		case 3:
			f = round(start_frequency*pow(2.0, num_dot / 12.0));
			break;

		case 4:
			f = round(start_frequency*pow(2.0, num_dot / 24.0));
			break;
		default:
			break;
		}

		if (f >= stop_frequency)
		{
			break;
		}

		
#endif
	}

	num_dot = 0;

	






	
	for (size_t i = 0; i < 1000; i++)
	{
		data2.push_back(0);
	}
	x_point = new double[data.size()]{0};
	outdata = new double[data.size()]{0};
	for (size_t i = 0; i < data.size(); i++)
	{
		outdata[i] = data[i];
		//outdata[i] = -voltage;
		x_point[i] = (double)i / fs;

		
	}

	outdata2 = new double[data2.size()]{0};
	for (size_t i = 0; i < data2.size(); i++)
	{
		outdata2[i] = data2[i];
		//outdata2[i] = 10;
		//x_point[i] = (double)i / fs;


	}

	
	if (this->m_hWnd)
	   OpenDrawThread();

}


void CJiLiSet::OnBnClickedButtonFangDa()
{
	// TODO:  在此添加控件通知处理程序代码
	scale *= 1.05;
	b_fanfda = true;
	Invalidate();

	//OnBnClickedButton1();
}


void CJiLiSet::OnBnClickedButtonSuoXiao()
{
	// TODO:  在此添加控件通知处理程序代码
	
	scale *= 0.95;// *2000 / data.size();
	Invalidate();
	b_fanfda = false;
	//OnBnClickedButton1();
}

bool CJiLiSet::OpenDrawThread()
{
	bExit = false;
	
	
	
	thread thread_draw_bitmap(&CJiLiSet::DrawThreadBitMap, this);
	thread_draw_bitmap.detach();
	
	
	return true;
}

void CJiLiSet::DrawThread()
{
	HDC dcc = ::GetDC(m_hWnd);
	
	//while (true)
	{
		
		for (size_t i = 0; i < data.size() - 1; i++)
		{
			MoveToEx(dcc, i*scale + 50, outdata[i] * 100 + 200, NULL);
			LineTo(dcc, (i + 1)*scale + 50, outdata[i + 1] * 100 + 200);
		}

		
		

		//MessageBox(_T("数据已更新"), _T("成功！"));
		SetDlgItemTextW(IDC_GENXIN, _T("数据已更新"));
	}


}

//******该线程体中需要做资源保护**************
void CJiLiSet::DrawThreadBitMap()
{

isdrawrun = false;
CWnd* pictrue= 	 GetDlgItem(IDC_JILISET);

CDC*dc=  pictrue->GetDC();
CRect rect1;
pictrue->GetClientRect(rect1);
int wi1 = rect1.Width();
int hi1 = rect1.Height();
CDC*mdc = this->GetDC();
CPen pen(0,0,RGB(100,200,4));
dc->SelectObject(&pen);
int ymove = hi1/2;//波形图y轴移动

//int savedcId=dc->SaveDC();

CRect rect;
pictrue->GetClientRect(rect);
int wi = rect.Width();
int hi = rect.Height();
while ((data.size()+data2.size())*scale > wi&&!b_fanfda)
{
	scale *= 0.95;
}





double A = 10000000.0;



if (voltage / 0.001 > 1)
{

}
double v = pow(2, 0.5)*voltage;
CString cs;
cs.Format(_T("%.2f"),v);

double vh = (v * 100.0*A*100 * scaleY / A + hi1 / 2 + 20);

while (vh> hi)
{
	scaleY *= 0.95;
	vh = (v * 100.0*A*100 * scaleY / A + hi1 / 2 + 20);
}

//while ((-v * 100.0*A * scaleY*scaleY1 + 200) / A < -200)
//{
//	scaleY1 *= -1.05;
//}


SetDlgItemTextW(IDC_STATV, cs);



//SetDlgItemTextW(IDC_GENXIN, _T("1"));
if (data.size() == 0)
return;
if (!bfenli)
{
	A = 1.0;
	scaleY=1.0;
	while((v * 100 * scaleY*scaleY1 + ymove)>hi)
	{
		scaleY *= 0.95;
	}

}
for (size_t i = 0; i < data.size() - 1; i++)
{
	
	MoveToEx(*dc, i*scale, outdata[i] * 100.0 * scaleY*scaleY1 / A*1.0 + ymove, NULL);
	LineTo(*dc, (i + 1)*scale, outdata[i + 1] * 100.0 * scaleY*scaleY1 / A*1.0 + ymove);

	
}
if (data2.size()>0)
{
	
	for (size_t i = 0; i < data2.size() - 1; i++)
	{
		MoveToEx(*dc, i*scale + data.size()*scale, outdata2[i] * 100.0 * scaleY*scaleY1 / A*1.0 + ymove, NULL);
		LineTo(*dc, (i + 1)*scale + data.size()*scale, outdata2[i + 1] * 100.0 * scaleY*scaleY1 / A*1.0 + ymove);


	}
}
//SetDlgItemTextW(IDC_GENXIN, _T("2"));

isdrawrun = true;

//dc->RestoreDC(savedcId);
}


void CJiLiSet::OnStnClickedJiliset()
{
	// TODO:  在此添加控件通知处理程序代码
}


HBRUSH CJiLiSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if (pWnd->GetDlgCtrlID() == IDC_STATIC || pWnd->GetDlgCtrlID() == IDC_STATMS)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	
	



	return hbr;
}


void CJiLiSet::OnEnUpdateEditv()
{
	UpdateData(TRUE);//更新界面数据，默认方向控件到变量
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (voltage < 0)
		return;
	Invalidate();
	scaleY = 1;
}


BOOL CJiLiSet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CWnd* pictrue = GetDlgItem(IDC_JILISET);
	CRect rect;
	pictrue->GetClientRect(rect);
	int ymove = 250;
	int xmove = 480;
	int wi = rect.Width() + xmove;
	int hi = rect.Height() + ymove;

	if (pt.x > xmove && pt.x<wi
		&&pt.y>ymove && pt.y < hi)
	{
		switch (zDelta)
		{

		case 120:
			OnBnClickedButtonFangDa();
			break;
		case -120:
			OnBnClickedButtonSuoXiao();
				break;
		default:
			break;
		}
	}


	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CJiLiSet::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (!isdrawrun)
		return;

	CDialogEx::OnClose();
}

void CJiLiSet::setvoltage(double v)
{
	
	this->voltage = v;
	OnBnClickedButton1();
	if (this->m_hWnd)
	   UpdateData(FALSE);
	
	
}


void CJiLiSet::OnBnClickedButtonYfangda()
{
	// TODO:  在此添加控件通知处理程序代码
}
