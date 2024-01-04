// Geyin.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "Geyin.h"
#include "afxdialogex.h"
#include <thread>
#include "JiLiSet.h"
#include "DeviceSet.h"
#include "fftw3.h"
#include "Fr.h"
#include <fstream>

#define PI (3.14159265)

extern CJiLiSet jiliset;
extern CDeviceSet deviceSet;


// CGeyin 对话框

IMPLEMENT_DYNAMIC(CGeyin, CDialogEx)

CGeyin::CGeyin(CWnd* pParent /*=NULL*/)
: CDialogEx(CGeyin::IDD, pParent), m_fs(44100), m_sen1(0), m_sen2(0), isstopjiaozhun(false)
{
	mBrush.CreateSolidBrush(RGB(255, 255, 255));

#if MATLAB_COMPILE
	/*if (!mclInitializeApplication(nullptr, 0))
	{

		AfxMessageBox(_T("mclInitializeApplication err"));
	}

	
	bool b_ok = FrInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("FrInitialize err"));

	}*/
	
	

#endif
	

	
	
		/*
		dataRes.push_back(0);
		dataRes.push_back(10);
		dataRes.push_back(20);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(60);
		dataRes.push_back(40);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(70);
		dataRes.push_back(40);
		dataRes.push_back(20);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(20);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(60);
		dataRes.push_back(40);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(50);
		dataRes.push_back(70);
		dataRes.push_back(40);
		dataRes.push_back(20);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(20);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(50);
		dataRes.push_back(70);
		dataRes.push_back(40);
		dataRes.push_back(20);
		dataRes.push_back(30);
		dataRes.push_back(50);
		dataRes.push_back(20);
		dataRes.push_back(30);
		*/
		
	
	

	
}

CGeyin::~CGeyin()
{
	
}

void CGeyin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_vvv, m_playing);
	DDX_Control(pDX, IDC_COMPINLV, m_cmbpinlv);
	DDX_Control(pDX, IDC_COMSHENGYA, m_cmbshengya);
	DDX_Control(pDX, IDC_EDITSHENGYA, m_mubiaovalue);
	DDX_Control(pDX, IDC_EDITCHUSHIDIANYA, m_initvalote);
	DDX_Control(pDX, IDC_BUTTON1, m_btnMic1);
	DDX_Control(pDX, IDC_BUTTON3, m_btnMic2);
	DDX_Control(pDX, IDC_BUTTON4, m_btnDb);
	DDX_Control(pDX, IDC_BUTTON5, m_btnTest);
	DDX_Control(pDX, IDC_EDITCHUSHIDIANYA2, m_editValote);
}


BEGIN_MESSAGE_MAP(CGeyin, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON5, &CGeyin::OnBnClickedButtonTest)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, &CGeyin::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON6, &CGeyin::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON1, &CGeyin::OnBnClickedButtonMic1)
	ON_BN_CLICKED(IDC_BUTTON3, &CGeyin::OnBnClickedButtonMic2)
	ON_BN_CLICKED(IDC_BUTTON4, &CGeyin::OnBnClickedButtonShengYaJiaoZhun)
	ON_BN_CLICKED(IDC_BtnStopJiaozhun, &CGeyin::OnBnClickedBtnstopjiaozhun)
END_MESSAGE_MAP()


// CGeyin 消息处理程序


HBRUSH CGeyin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if (pWnd->GetDlgCtrlID() == IDC_RES)
	{
		
		
		/*pDC->SetTextColor(RGB(100, 0, 0));
		pDC->SetBkColor(RGB(30, 100, 30));*/
		
		return mBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_BUTTON1 || pWnd->GetDlgCtrlID() == IDC_BUTTON3
		|| pWnd->GetDlgCtrlID() == IDC_BUTTON4 || pWnd->GetDlgCtrlID() == IDC_BUTTON5)
	{
		pDC->SetTextColor(RGB(153, 51, 250));
		pDC->SetBkColor(RGB(0, 255, 0));

	}


	return hbr;
}

bool CGeyin::OpenDrawThread()
{

	bExit = false;
	
	std::thread thread_draw_bitmap(&CGeyin::DrawThreadBitMap, this);
	thread_draw_bitmap.detach();
	return true;
}
void CGeyin::DrawThreadBitMap()
{

	
	pictrue = GetDlgItem(IDC_RES);

	CPaintDC dc1(pictrue);

	//dc = pictrue->GetDC();
	CRect rect;
	pictrue->GetClientRect(rect);
	int wi = rect.Width();
	int hi = rect.Height();

	dc1.FillRect(rect, &mBrush);

	dc1.TextOutW(0, hi-20, _T("O"));
	/*************坐标轴*********************/
	MoveToEx(dc1,15, hi-10, NULL);
	LineTo(dc1, wi-10, hi-10);

	MoveToEx(dc1, 15, hi - 10, NULL);
	LineTo(dc1,15, 10);
	/*******************************/

	/**************框线*********************/
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* oldpen=dc1.SelectObject(&pen);

	dc1.SetDCPenColor(RGB(200, 40, 100));
	MoveToEx(dc1, 15 + 10, hi - hi*0.1, NULL);
	LineTo(dc1, 15 + wi / 3, hi - hi*0.1);

	MoveToEx(dc1, 15 + wi / 3, hi - hi*0.1, NULL);
	LineTo(dc1, 15 + wi / 3, hi - hi*0.2);

	MoveToEx(dc1, 15 + wi / 3, hi - hi*0.2, NULL);
	LineTo(dc1, 15 + 2 * wi / 3, hi - hi*0.2);

	MoveToEx(dc1, 15 + 2 * wi / 3, hi - hi*0.2, NULL);
	LineTo(dc1, 15 + 2 * wi / 3, hi - hi*0.3);

	MoveToEx(dc1, 15 + 2 * wi / 3, hi - hi*0.3, NULL);
	LineTo(dc1, 15 + wi - 30, hi - hi*0.3);

	dc1.SelectObject(oldpen);
	/************************************/
	//drawTestkx(dc1);
	//OpenDrawThreadDrawRes();

	/****************绘制测试结果曲线***********************/
	//CPen pen1;
	int xzhou = 30;
	int sacle = 30;
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	oldpen = dc1.SelectObject(&pen);
	if (dataRes.size() > 0)
	{
		for (size_t i = 0; i < dataRes.size() - 1; i++)
		{
			if (dataRes[i]>1000 )
				dataRes[i] = 1000;
			if ( dataRes[i]<0)
				dataRes[i] = 0;
			MoveToEx(dc1, xzhou + i * sacle, dataRes[i], NULL);
			LineTo(dc1, xzhou + (i + 1) * sacle, dataRes[i + 1]);
		}
		
	}
	
	dc1.SelectObject(oldpen);
    /************************************/
	

}

void CGeyin::drawTestkx(CPaintDC dc)
{
	
	CRect rect;
	pictrue->GetClientRect(rect);
	int wi = rect.Width();
	int hi = rect.Height();

	MoveToEx(dc, 15+10, hi - 20, NULL);
	LineTo(dc, 15+200, hi - 20);


}

BOOL CGeyin::OnInitDialog()
{

	
	card.LoadAsioLib();
	

	UpdateData(false);
	m_cmbpinlv.InsertString(0, _T("1000"));
	m_cmbpinlv.SetCurSel(0);

	m_cmbshengya.InsertString(0, _T("94"));
	m_cmbshengya.InsertString(1, _T("114"));
	m_cmbshengya.SetCurSel(0);

	m_mubiaovalue.SetWindowTextW(_T("60"));

	m_initvalote.SetWindowTextW(_T("0.01"));
	m_editValote.SetWindowTextW(_T("0.00"));

	/*m_btnMic1.m_bDontUseWinXPTheme = TRUE;
	m_btnMic1.m_bDrawFocus = false;
	m_btnMic1.SetFaceColor(RGB(153,51,255));*/
	/*CFont font;
	font.CreatePointFont(500, _T("宋体"));
	m_btnMic1.SetFont(&font);*/


	//读取系统配置文件


	return true;
}



void CGeyin::OnBnClickedButtonTest()
{
	// TODO:  在此添加控件通知处理程序代码
	
	UpdateData(false);

	
	if (jiliset.data.size() > 0)
	{
		data = &jiliset.data;
	}
	else
	{
		AfxMessageBox(_T("激励信号未确认！"));
		return;
	}
	if (jiliset.data2.size() > 0)
	{
		data2 = &jiliset.data2;
	}
	
	card.isruning = true;
	
	if (!card.initCard())
		return;
	m_playing.SetWindowTextW(_T("正在测试中..."));
	
	
	int len1 = data->size();
	int len2 = data2->size();
	double*data4 = new double[len1 + len2];//用于外部mic
	double*data42 = new double[len1 + len2];//用于内部mic
	double* data3 = new double[len1];//内部mic
	double* dataout2 = new double[len1];//外部mic
	play(data, data3, dataout2);//播放低频
	Sleep(3000);
	for (size_t i = 0; i < len1; i++)
	{
		data4[i] = data3[i];
	}
	for (size_t i = 0; i < len1; i++)
	{
		data42[i] = dataout2[i];
	}


	delete[] data3;
	delete[] dataout2;


	
	
	data3 = new double[len2];
	dataout2 = new double[len2];
	play(data2, data3, dataout2);//播放高频
	Sleep(3000);
	for (size_t i = 0; i < len2; i++)
	{
		data4[len1+i] = data3[i];
	}
	for (size_t i = 0; i < len2; i++)
	{
		data42[len1 + i] = dataout2[i];
	}
	

	

	char* fileName = "D:\\TestRecord\\record22.wav";
	wavwrite(fileName, 1, m_fs, 32, (char*)data4, sizeof(int)*(len1+len2));
	m_playing.SetWindowTextW(_T("正在计算中..."));
	double* res1=caculateRes(data4, len1+len2 );

	double* res2=caculateRes(data42, len1 + len2);
	dataRes.clear();
	for (size_t i = 0; i < jiliset.pos.size(); i++)
	{
		dataRes.push_back(res1[i] - res2[i]);
		//dataRes.push_back(res1[i] - 0);
	}

	delete[] data4;
	delete[] data42;
	delete[] res1;
	delete[] res2;
	delete[] data3;
	delete[] dataout2;

	m_playing.SetWindowTextW(_T("测试结束!"));
	Invalidate();

	
	
}

void CGeyin::play(std::vector<double>* in, double*out, double*out2)
{
	int len = in->size();
	int* dataout = new int[len]{0};
	int* dataout2 = new int[len]{0};


	int* dat = new int[len];
	for (size_t i = 0; i < len; i++)
	{
		dat[i] = (*in)[i];
	}

	card.playwithRecord(dat, len, 0, dataout, dataout2);//测试隔音特殊函数，先录音同时播放

	
	for (size_t i = 0; i < len; i++)
	{
		out[i] = dataout[i];
	}

	for (size_t i = 0; i < len; i++)
	{
		out2[i] = dataout2[i];
	}

	delete[] dat;
	delete[] dataout;
	delete[] dataout2;
	

}

double* CGeyin::caculateRes(double* data, int len)
{

#if MATLAB_COMPILE



	double fs = m_fs;

	mwArray mwFs(1, 1, mxDOUBLE_CLASS);
	mwFs(1, 1) = fs;
	double ts = 1.0 / fs;
	int N = len;
	double *stim = new double[N];
	double fsig = 1.0e3;

	/*for (int i = 0; i < N; i++) {
		stim[i] = cos(2 * PI * fsig * i * ts);
		}

		for (int i = 10001; i < N; i++) {
		stim[i] = 10 * sin(2 * PI * 2 * fsig * i * ts);
		}*/

	ofstream outfile;
	CString path = getExcutePath();
	path.Append(_T("\\1.txt"));
	outfile.open(path, ios::out | ios::trunc);

	for (size_t i = 0; i < len; i++)
	{
		stim[i] = data[i];
		outfile << data[i] << endl;;
	}

	outfile.close();


	mwArray mwStim(N, 1, mxDOUBLE_CLASS);
	mwStim.SetData(stim, N);

	mwArray mwResponse(N, 1, mxDOUBLE_CLASS);
	mwResponse.SetData(stim, N);
	delete[]stim;

	mwArray mwPosition(jiliset.pos.size(), 3, mxDOUBLE_CLASS);

	/*************在此添加信号*****************/
	int l = 0;
	int i = 1;
	//std::ofstream outfile;
	//CString path("D:\\TestRecord\\pos.txt");
	//outfile.open(path, std::ios::out | std::ios::trunc);
	for (std::map<int, int>::iterator it = jiliset.pos.begin(); it != jiliset.pos.end();it++)
	{
		mwPosition(i, 1) = (*it).first;
		mwPosition(i, 2) = l+1;
		mwPosition(i, 3) = l+(*it).second;
		
		i++;		
		//outfile << (*it).first<<"  "<<l << "  " << l + (*it).second << endl;
		l += (*it).second;
		



	}
	//outfile.close();



	/************************************/

	mwArray mwSen(1, 1, mxDOUBLE_CLASS);
	mwSen(1, 1) = m_sen1/1000.0;

	mwArray mwResult(jiliset.pos.size(), 1, mxDOUBLE_CLASS);
	Fr(1, mwResult, mwStim, mwResponse, mwFs, mwPosition, mwSen);
	mxDouble* result = new mxDouble[jiliset.pos.size()];
	mwResult.GetData(result, jiliset.pos.size());




	for (size_t i = 0; i < jiliset.pos.size(); i++)
	{
		
		result[i] -= 160;
		dataRes.push_back(result[i]);
	}
	return result;
#else	
	mxDouble* result = new mxDouble[jiliset.pos.size()]{0};
	return result;
#endif
}

void CGeyin::OpenDrawThreadDrawRes()
{

	

}
void CGeyin::DrawThreadRes()
{
	
	
	
}


void CGeyin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	OpenDrawThread();
	
	
	

	/*MoveToEx(dc, 550, 780, NULL);
	LineTo(dc, 1500, 780);*/
	
	
}

void CGeyin::OnDraw()
{
	OnPaint();

}


void CGeyin::OnBnClickedButtonUpdate()
{
	// TODO:  在此添加控件通知处理程序代码
	
	
}



#define TYPE int



void CGeyin::recoradthread()
{
	/*RecordPartmeter* pMy = (RecordPartmeter*)lparam;
	CGeyin *dlg = (CGeyin *)pMy->pThis;*/
	//char data[44100*16];

	int len = 44100 * 4;
	TYPE* data2 = new TYPE[len];

	int port = 0;

	//dlg->SetCaptureBuffer(data,44100*16,0);
	card.SetCaptureBuffer(data2, sizeof(TYPE)*len, port);

	card.StartCapture();
	char temp[4];
	int val = 0.0;
	while (card.IsPortCaptureRunning(port))
	{
		;
	}

	if (!card.IsPortCaptureRunning(port))
	{
		/*for (int i = 0; i < 44100*4; i++)
		{
		temp[0]=data[i*4];
		temp[1]=data[i*4+1];
		temp[2]=data[i*4+2];
		temp[3]=data[i*4+3];
		val=atof(temp);

		}*/
		char* fileName = "D:\\TestRecord\\1.wav";
		wavwrite(fileName, 1, 44100, 32, (char*)data2, sizeof(TYPE)*len);
		//AfxMessageBox(_T("record ok"));
		m_playing.SetWindowTextW(_T("record ok"));
	}


}
extern DWORD dwRecordThreadID;
DWORD WINAPI ThreadFuncMyRecord1(LPVOID lparam)
{
	RecordPartmeter* pMy = (RecordPartmeter*)lparam;
	CGeyin *dlg = (CGeyin *)pMy->pThis;
	//char data[44100*16];

	int len = 44100 * 4;
	TYPE* data2 = new TYPE[len];

	int port = 0;

	//dlg->SetCaptureBuffer(data,44100*16,0);
	dlg->card.SetCaptureBuffer(data2, sizeof(TYPE)*len, port);

	dlg->card.StartCapture();
	char temp[4];
	int val = 0.0;
	while (dlg->card.IsPortCaptureRunning(port))
	{
		;
	}

	if (!dlg->card.IsPortCaptureRunning(port))
	{
		/*for (int i = 0; i < 44100*4; i++)
		{
		temp[0]=data[i*4];
		temp[1]=data[i*4+1];
		temp[2]=data[i*4+2];
		temp[3]=data[i*4+3];
		val=atof(temp);

		}*/
		char* fileName = "D:\\TestRecord\\1.wav";
		wavwrite(fileName, 1, 44100, 32, (char*)data2, sizeof(TYPE)*len);
		//AfxMessageBox(_T("record ok"));
		dlg->m_playing.SetWindowTextW(_T("record ok"));
	}


	return 0;

}


void CGeyin::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码


	card.LoadAsioLib();
	if (!card.initCard())
		return;

	m_RecordParameter.pThis = this;
	

	//char data[44100*16];
	int len = 44100 * 4;
	//TYPE* data2=new TYPE[len];

	TYPE data2[44100 * 4];



	int sample = 44100;
	double A = 1000000000.0;
	int port = 0;
	double f = 1000;
	double MmPI = 3.1415926;
	double val = 0.0;
	int nTbp = 0;
	for (int i = 0; i < 44100 * 4; i++)
	{
		data2[i] = A*sin(2.0*MmPI*f*i / sample);
		//val=0;
		//data2[i]=(TYPE)val;

	}

	CreateThread(
		NULL,
		0,
		ThreadFuncMyRecord1,
		&m_RecordParameter,
		0,
		&dwRecordThreadID);
	
	//char* fileName="D:\\TestRecord\\Untitled1111.wav";
	////char* fileName="D:\\TestRecord\\3.wav";
	//wave_t pWave;
	//if(!wavread(fileName,&pWave))
	//	AfxMessageBox(_T("err  read"));

	//fclose(pWave.fp);



	//if(SetPlayBuffer(pWave.pData,pWave.length*4,port)!=0)
	if (card.SetPlayBuffer(data2, sizeof(TYPE)*len, port) != 0)
		AfxMessageBox(_T("err setbuff"));




	/*for (int i = 0; i <len; i++)
	{
	data2[i]=pWave.pData[i];
	}*/
	card.StartPlayback();

	m_playing.SetWindowTextW(_T("正在播放"));
	while (true == card.IsPortPlaybackRunning(0))
	{
		Sleep(100);
	}
	m_playing.SetWindowTextW(_T("播放结束"));

	/*std::ofstream outfile;
	outfile.open("D:\\Projects\\Demo - mfc - src - 20220722\\x64\\Debug\\read.csv",
	std::ios::out|std::ios::trunc);
	if(!outfile.is_open())
	{
	AfxMessageBox(_T("open file err"));
	}
	for (int i = 0; i < 44100; i++)
	{

	outfile<<data2[i]<<std::endl;

	}
	outfile.close();*/
	char* fileName1 = "D:\\TestRecord\\3.wav";
	//wavwrite(fileName1 , 1 , 44100 , 32 , (char*)pWave.pData , pWave.length*4);
	wavwrite(fileName1, 1, 44100, 32, (char*)data2, sizeof(TYPE)*len);
	m_playing.SetWindowTextW(_T("写文件完成"));
	/*free(data2);*/
}
/******************采样点数需要大于1秒的采样频率点数*****************************/
double CGeyin::getAp(int* data, int len)
{
	double ap = 0.0;

	//m_fs = 96000;
	//int N = 2000;
	//double myPI = 3.1415926;
	//int f = 22;
	//int f2 = 1000;
	//int A = 10;
	//int sample = 44100;

	fftw_plan p;

	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);

	int temp = 0.0;
	for (int i = 0; i < len; i++)
	{
		
		in[i][0] = data[i];//虚数的实部
		//in[i][0] = 0.0001*sin(2 * 3.1415926 * 1000 * i / m_fs);
		in[i][1] = 0;//虚数的虚部
	}

	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(p);

	double real = 0.0;
	double vitru = 0.0;
	double* fs=  new double[len];
	double* Ap = new double[len];
	

	Ap[0] = sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;
	for (int i = 1; i < len; i++)
	{
		Ap[i] = 2 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / len;
	}

	for (int i = 0; i < len; i++)
	{
		fs[i] = (i*m_fs) / len;

		if (fs[i]==1000)
		{
			ap = Ap[i];
		}
		//printf("%.2f + %.2fi", out[i][0], out[i][1]);
	}
	

	//需要释放内存
	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);
	delete[] fs;
	delete[] Ap;
	return ap;

}


bool CGeyin::jiaozhunMic(double& sen)
{
	UpdateData();
	//m_fs = 96000;
	//采集mic信号
	//card.LoadAsioLib();
	if (!card.initCard())
		return false;
	int len = m_fs*2;
	int* data3 = new int[len]{0};

	card.record(data3, len, 0);

	
	//等待录音完成
	Sleep(3000);

	/*char* fileName = "D:\\TestRecord\\record1.wav";
	wavwrite(fileName, 1, 44100, 32, (char*)(data3), sizeof(TYPE)*len);*/
	/*if (card.isruning)
	{
		;
	}*/
	//1.计算信号幅值
	double ap = getAp(data3, len);//电压大小
	ap /= bili*1.0;
	//2.根据标准声压值换算出系统灵敏度
	//2.1把mic的声压(db)级换算成pa
	CString str;
	m_cmbshengya.GetWindowTextW(str);
	int len1=WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char* ch = new char[len1 + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
	double pa = jiliset.getPa(atof(ch));
	delete[] ch;

	//2.2 把幅值(mv)除以pa
	sen = 1.0*ap*1000.0 / pa;



	
	
	delete[] data3;
	AfxMessageBox(_T("校准完成"));
	return true;

}


void CGeyin::OnBnClickedButtonMic1()
{
	// TODO:  在此添加控件通知处理程序代码

	double sen = 0.0;
	jiaozhunMic(sen);
	m_sen1 = sen;
	deviceSet.setsen1(m_sen1);
	std::ofstream outfile;

	CString path = getExcutePath();
	path.Append(_T("\\conf1.ini"));
	outfile.open(path, std::ios::out | std::ios::trunc);

	outfile << "Sen1:" << m_sen1 << endl;
	outfile.close();


}

//CString CGeyin::getExcutePath()
//{
//	
//
//	char pFilePath[MAX_PATH] = { 0 };
//
//	int len=MultiByteToWideChar(0, 0, pFilePath, -1, NULL, 0);
//
//	wchar_t *wide = new wchar_t[len];
//
//	MultiByteToWideChar(0, 0, pFilePath, -1, wide, len);
//
//    int pos = GetCurrentDirectory(MAX_PATH, wide);
//
//	if (pos < 0)
//	{
//		AfxMessageBox(_T("get path err"));
//		return CString("");
//	}
//	len=WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, NULL, 0, NULL, FALSE);
//	char *pfile = new char[len];
//	WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, pfile, len, NULL, FALSE);
//	CString path(pfile);
//	path.Append(_T("\\conf.ini"));
//	delete[] pfile;
//	return path;
//
//}

CString CGeyin::getExcutePath()
{	
	HMODULE module = GetModuleHandle(0);
	wchar_t* pwstr = getLPWSTR(MAX_PATH);
	int pos = GetModuleFileName(module, pwstr, MAX_PATH);

	if (pos < 0)
	{
		AfxMessageBox(_T("get path err"));
		return CString("");
	}
	char* p = getcharFromLPWSTR(pwstr, pos);
	CString path(p);
	delete[] pwstr;
	pos=path.ReverseFind('\\');
	CString leftstr=path.Left(pos);
	delete[] p;
	
	return leftstr;

}
/*********使用后需要删除wchar_t* ***************/
wchar_t* CGeyin::getLPWSTR(int len)
{
	wchar_t *wide = new wchar_t[len];
	len = WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, NULL, 0, NULL, FALSE);
	char* p = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, p, len, NULL, FALSE);

	delete[] p;
	
	return wide;

}
/*********使用后需要删除char* ***************/
char* CGeyin::getcharFromLPWSTR(wchar_t* wide, int len)
{
	len = WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, NULL, 0, NULL, FALSE);
	char *pfile = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, pfile, len, NULL, FALSE);

	return pfile;

}


void CGeyin::OnBnClickedButtonMic2()
{
	// TODO:  在此添加控件通知处理程序代码
	double sen = 0.0;
	jiaozhunMic(sen);
	m_sen2 = sen;

	deviceSet.setsen2(m_sen2);
	std::ofstream outfile;

	CString path = getExcutePath();
	path.Append(_T("\\conf2.ini"));
	outfile.open(path, std::ios::out | std::ios::trunc);

	outfile << "Sen2:" << m_sen2 << endl;
	outfile.close();
}
/****需要删除返回的char指针******/
char* CGeyin::getcharp(CString str)
{

	int len1 = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char* ch = new char[len1 + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
	return ch;
}


void CGeyin::OnBnClickedButtonShengYaJiaoZhun()
{
	if (!card.initCard())
		return;
	std::thread thread_threadjiaozhuan(&CGeyin::threadjiaozhuan, this);
	thread_threadjiaozhuan.detach();
	

	

}

void CGeyin::threadjiaozhuan()
{
	if (deviceSet.m_hWnd == NULL)
	{
		AfxMessageBox(_T("未确认Mic灵敏度！"));
		return;
	}

	CString str;
	m_mubiaovalue.GetWindowTextW(str);
	char* ch = getcharp(str);
	double puepuse = atof(ch);//目标校准声压
	delete[] ch;
	
	/*if (!card.initCard())
		return;*/
	
	
	//mv/p

	int port = 0;
	int fs = 1000;
	double ts = 1.0 / m_fs;

	int len = 44100;
	int* playdata = new int[len];
	int* recorddata = new int[len]{0};

	double resdb = 0.0;
	double ap = 0.0;
	double ki = 0.0;
	m_initvalote.GetWindowTextW(str);
	ch = getcharp(str);
	double volate = atof(ch);
	delete[] ch;
	bool issucess = true;
	m_sen1 = deviceSet.sen1;
	isstopjiaozhun = true;
	while (!(puepuse - resdb>-0.5 && puepuse - resdb<0.5))
	{

		if (!isstopjiaozhun)
		{
			issucess = false;
			break;
		}

		if (volate > 3)
		{
			issucess = false;
			AfxMessageBox(_T("校准失败"));
			break;

		}

		for (size_t i = 0; i< len; i++)
		{
			playdata[i] = pow(2, 0.5)*volate*bili*sin(2.0 * PI*fs*i*ts);
		}
		card.playwithRecord(playdata, len, port, recorddata);

		//计算电压值
		ap = getAp(recorddata, len);
		ap /= bili;
		//sen = 1.0*ap*1000.0 / pa;
		//m_sen1 = 26.45;

		resdb = jiliset.getDb(ap*1000.0 / m_sen1);

		ki = volate / resdb;

		if (resdb < puepuse)
		{
			ki *= 1.2;
		}
		else
		{
			ki *= 0.8;
		}

		volate = ki * puepuse;

		Sleep(500);

	}


	delete[] playdata;
	delete[] recorddata;
	jiliset.setvoltage(volate);

	char str1[256];
	sprintf(str1, "%.5f", volate);

	m_editValote.SetWindowTextW(CString(str1));
	if (issucess)
		AfxMessageBox(_T("校准成功!"));
    
}


void CGeyin::OnBnClickedBtnstopjiaozhun()
{
	// TODO:  在此添加控件通知处理程序代码

	thread stopcard(&CGeyin::threadstopCard, this);
	stopcard.detach();

	

}

void CGeyin::threadstopCard()
{

	if (!card.isCardInsert())
		return;

	card.StopPlayback();
	card.StopCapture();

	isstopjiaozhun = false;
	//AfxMessageBox(_T("已停止，请重新启动系统!"));
	//exit(0);
}
