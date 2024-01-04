// BlueTestD.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "BlueTestD.h"
#include "afxdialogex.h"
//#include "libdaq.h"
#include "resource.h"

#include "fftw3.h"
#include <fstream>
#include <iomanip>
#include "fftzcj.h"
#include "FrHarm.h"
#include "JiLiSet.h"
#define N 4000




/***************外部对象申明******************/

extern CJiLiSet jiliset;

// CBlueTestD 对话框

IMPLEMENT_DYNAMIC(CBlueTestD, CDialogEx)

CBlueTestD::CBlueTestD(CWnd* pParent /*=NULL*/)
: CDialogEx(CBlueTestD::IDD, pParent), /*daqdevice(NULL), */fs(44100), m_count(4000), testfs(1000),
xscale(2), yscale(10)
{

	/*bool b_ok = fftzcjInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("fftzcjInitialize err"));

	}*/

	/*b_ok = FrHarmInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("FrHarmInitialize err"));

	}*/



	
	

}

CBlueTestD::~CBlueTestD()
{
	/*if (daqdevice != NULL)
		delete daqdevice;

	libdaq_exit();*/
}

void CBlueTestD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbtestitem);
	DDX_Text(pDX, IDC_fmax, f);
	DDX_Text(pDX, IDC_EDITpinlv, fs);
	DDX_Text(pDX, IDC_EDITdianshu, m_count);
	DDX_Text(pDX, IDC_EDITtestpinlv, testfs);
	DDX_Text(pDX, IDC_EDITXSCALE, xscale);
	DDX_Text(pDX, IDC_EDITYSCALE, yscale);

}

BOOL CBlueTestD::OnInitDialog()
{

	UpdateData(false);

	char device_name[20]{0};
	char device_type[20]{0};
	int index = 0;

	/*libdaq_init();
	int device_count = libdaq_device_get_count();
	if (device_count == 0)
	{
		AfxMessageBox(_T("Error! NO DAQ Device connected to computor,Press Any Key to Exit!"));

	}
	libdaq_device_get_name(index, device_name, sizeof(device_name));
	libdaq_device_get_type(device_name, device_type, sizeof(device_type));
	daqdevice = new daqusb402x(device_name);*/

	GetDlgItem(IDC_ZSDEVICENAME)->SetWindowTextW(CString(device_name));
	GetDlgItem(IDC_ZSDEVICETYPE)->SetWindowTextW(CString(device_type));

	m_cmbtestitem.InsertString(0, _T("THD"));
	m_cmbtestitem.InsertString(1, _T("THD+N"));
	m_cmbtestitem.InsertString(2, _T("信噪比"));
	m_cmbtestitem.InsertString(3, _T("底噪"));
	m_cmbtestitem.InsertString(4, _T("隔离度"));
	m_cmbtestitem.InsertString(5, _T("稳定周期"));
	m_cmbtestitem.SetCurSel(0);

	GetDlgItem(IDC_pictrue)->GetClientRect(drawrect);
	GetDlgItem(IDC_pictrue)->GetWindowRect(drawrect);

	return TRUE;

}


BEGIN_MESSAGE_MAP(CBlueTestD, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CBlueTestD::OnBnClickedButtonTest)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON4, &CBlueTestD::OnBnClickedButtonFR)
END_MESSAGE_MAP()


// CBlueTestD 消息处理程序


void CBlueTestD::OnBnClickedButtonTest()
{
	// TODO:  在此添加控件通知处理程序代码

	int index=m_cmbtestitem.GetCurSel();
	int res = -1;
	char ss[512]{0};
	switch (index)
	{
	case 0:
		res=thd();
		memset(ss, 0, 512);
		sprintf(ss, "%d",res);
		GetDlgItem(IDC_ZSRESKKK)->SetWindowTextW(CString(ss));
		break;
	case 1:
		res=thdAndN();
		memset(ss, 0, 512);
		sprintf(ss, "%d", res);
		GetDlgItem(IDC_ZSRESKKK)->SetWindowTextW(CString(ss));
		UpdateData(false);
		break;
	case 2:
		res=xinzaobi();
		memset(ss, 0, 512);
		sprintf(ss, "%d", res);
		GetDlgItem(IDC_ZSRESKKK)->SetWindowTextW(CString(ss));
		UpdateData(false);
		break;
	case 3:
		res=dizao();
		memset(ss, 0, 512);
		sprintf(ss, "%d", res);
		GetDlgItem(IDC_ZSRESKKK)->SetWindowTextW(CString(ss));
		UpdateData(false);
		break;
	case 4:
		res=gelidu();
		memset(ss, 0, 512);
		sprintf(ss, "%d", res);
		GetDlgItem(IDC_ZSRESKKK)->SetWindowTextW(CString(ss));
		UpdateData(false);
		break;
	case 5:

		break;

	}
}

int CBlueTestD::thd()
{
	UpdateData();
	//1.生成1KHz的激励信号
	//dac_wavepara_t dac_cfg;
	//输出周期波形，本例以输出1KHz正弦波为例，一个正弦波形周期取10个点，数据点更新频率为10KHz输出的波形为1KHz
	/*printf("DAC输出1KHz正弦波形\n");*/
	double* buf=new double[2000];
	int f = testfs;
	for (int x = 0; x<2000; x++){
		//angle change to arc
		buf[x] = 1.0*(sin(x*(2 * 3.1415926*f / fs)));

	}

	//dac_cfg.buf = buf; //数据点缓冲区
	//dac_cfg.buflen = 2000;
	//dac_cfg.cycles = 100; //0代表周期性循环输出，非0代表波形输出周期数
	//dac_cfg.frequency = 96000;//点更新频率，96KHz
	////dac_cfg.trigger_mode = DAC_TRIGGER_MODE_SOFT;//软件触发
	///*daqdevice->DAC.set_wavepara(&dac_cfg);
	//daqdevice->DAC.start();*/
	////Sleep(2000);
	////daqdevice->DAC.stop();


	////2.回采信号
	//memset(buf, 0, 2000);
	//adc_config_channel(daqdevice);

	//3.fft计算
	//int res=adc_n_sample_example(daqdevice);
	//4.分析fft后的结果，频点分析

	delete [] buf;
	//return res;
	return 0;
}

int CBlueTestD::thdAndN()
{
	UpdateData();
	//1.生成1KHz的激励信号
	//dac_wavepara_t dac_cfg;
	//输出周期波形，本例以输出1KHz正弦波为例，一个正弦波形周期取10个点，数据点更新频率为10KHz输出的波形为1KHz
	/*printf("DAC输出1KHz正弦波形\n");*/
	double* buf = new double[m_count];
	int f = testfs;
	for (int x = 0; x<m_count; x++){
		//angle change to arc
		buf[x] = 1.0*(sin(x*(2 * 3.1415926*f / fs)));

	}

	//dac_cfg.buf = buf; //数据点缓冲区
	//dac_cfg.buflen = m_count;
	//dac_cfg.cycles = 10; //0代表周期性循环输出，非0代表波形输出周期数
	//dac_cfg.frequency = fs;//点更新频率，96KHz
	//dac_cfg.trigger_mode = DAC_TRIGGER_MODE_SOFT;//软件触发
	//daqdevice->DAC.set_wavepara(&dac_cfg);
	//daqdevice->DAC.start();
	//Sleep(1000);
	//daqdevice->DAC.stop();


	//2.回采信号
	//memset(buf, 0, 2000);
	//adc_config_channel(daqdevice);

	//3.fft计算
	//int ret =adc_n_sample_ThdN(daqdevice);
	//4.分析fft后的结果，频点分析

	delete[] buf;

	//return ret;
	return 0;
}

//void CBlueTestD::adc_config_channel(daqusb402x* daqdevice)
//{
//	//channel config
//	adc_channelpara_t channels[8];
//	for (int ch_index = 0; ch_index<sizeof(channels) / sizeof(adc_channelpara_t); ch_index++)
//	{
//		channels[ch_index].channel = ch_index;
//		channels[ch_index].couplemode = ADC_CHANNEL_COUPLE_MODE_DC;//直流耦合,不可修改
//		channels[ch_index].range = CHANNEL_RANGE_N10V_P10V; //量程，可根据实际需要修改
//		channels[ch_index].refground = ADC_CHANNEL_REFGND_DIFF; //接地终端配置，不可修改
//	}
//	daqdevice->ADC.config_channel(channels, sizeof(channels) / sizeof(adc_channelpara_t));
//}
//
//// ADC N点采样
//int CBlueTestD::adc_n_sample_example(daqusb402x* daqdevice)
//{
//	UpdateData();
//	unsigned int actual_len;
//	adc_samplepara_t samplepara;
//	int len = m_count;
//	int m_fs = fs;
//
//	// 配置数字触发方式为电平触发(高电平触发)
//	daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD, 0, ADC_TRIG_TYPE_LEVEL, ADC_TRIG_EDGE_RISE, ADC_TRIG_LEVEL_HIGH, 0);
//	// 配置数字触发方式为边沿触发(上升沿触发)
//	// daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD,0,ADC_TRIG_TYPE_EDGE,ADC_TRIG_EDGE_RISE,ADC_TRIG_LEVEL_HIGH,0);
//
//	//配置触发方式,触发方式支持软件触发 ADC_TRIG_SRC_SW 和数字电平触发 ADC_TRIG_SRC_HWD
//	daqdevice->ADC.select_triggerSrc(ADC_TRIG_SRC_SW);//选择软件触发
//
//	//设置采样参数
//	unsigned char channel_list[] = { 0 }; //采样的通道列表，根据需求任意设置
//	samplepara.channel_list = channel_list;
//	samplepara.channel_count = sizeof(channel_list);  //采样的通道数
//	samplepara.sample_mode = ADC_SAMPLE_MODE_SYNC;//只支持同步采样，不可修改
//	samplepara.frequency = m_fs;//采样频率96kHz
//	samplepara.cycles = len;//设置每通道的采样数，连续采样时设置为0 采样点
//	daqdevice->ADC.set_sample_parameter(&samplepara);
//
//	daqdevice->ADC.clear_buffer();//清除软件和硬件buffer中的数据
//	daqdevice->ADC.start_task(); //启动采样任务，启动采样任务后并不会立即采样，需要满足触发条件后才开始采样，软件触发时等待软件触发命令，数字电平触发时等待触发引脚变为高电平
//	daqdevice->ADC.send_trigger();//发送软件触发命令，启动采样
//
//	//分配缓冲区用于存储采样结果
//	int size = samplepara.cycles*samplepara.channel_count; //数据长度根据每通道采样数和采样通道数的计算得到
//	vector<double> data;
//	data.resize(size);
//	daqdevice->ADC.read_analog_sync(data.data(), size, &actual_len, 1000); //等待返回指定长度的数据，1000ms超时
//	std::ofstream outfile1;
//	outfile1.open(_T("b.txt"), std::ios::trunc);
//	for (size_t i = 0; i < data.size(); i++)
//	{
//		outfile1 << data[i] << setiosflags(ios::fixed) << setprecision(2) << "\n";
//	}
//	outfile1.close();
//	//停止采样
//	daqdevice->ADC.stop(); //停止ADC
//	daqdevice->ADC.stop_task();//停止采样任务
//	daqdevice->DAC.stop();
//
//	//执行fft变换
//	fftw_plan p;
//	
//	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
//	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
//
//	for (int i = 0; i < len; i++)
//	{
//
//		in[i][0] = data[i];//虚数的实部
//		//in[i][0] = 0.0001*sin(2 * 3.1415926 * 1000 * i / m_fs);
//		in[i][1] = 0;//虚数的虚部
//	}
//
//	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
//
//	fftw_execute(p);
//
//	double real = 0.0;
//	double vitru = 0.0;
//	//int* fs = new int[len];
//	int* fs=new int[m_count];
//	//double* Ap = new double[len];
//	double* Ap=new double[m_count];
//	double ap[5];
//
//	Ap[0] = sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;
//	for (int i = 1; i < len; i++)
//	{
//		Ap[i] = 2 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / len;
//	}
//
//	for (int i = 1; i < len; i++)
//	{
//		fs[i] = i*m_fs/len;
//
//		if (fs[i] == 1000)
//		{
//			ap[0] = Ap[i];
//		}
//		if (fs[i] == 2000)
//		{
//			ap[1] = Ap[i];
//		}
//		if (fs[i] == 3000)
//		{
//			ap[2] = Ap[i];
//		}
//		if (fs[i] == 4000)
//		{
//			ap[3] = Ap[i];
//		}
//		if (fs[i] == 5000)
//		{
//			ap[4] = Ap[i];
//		}
//		//printf("%.2f + %.2fi", out[i][0], out[i][1]);
//	}
//
//	double sum = 0;
//	double apmax = 0;
//	for (size_t i = 1; i < 5; i++)
//	{
//		sum += pow(ap[i], 2);
//		if (ap[i]>apmax)
//		{
//			apmax = ap[i];
//		}
//
//	}
//
//	sum = sqrt(sum);
//
//	double sumv = sqrt(pow(apmax, 2));
//
//	sum = 1.0*sum / sumv;
//
//	sumv = 20.0*log10(sum);
//	sum=-20.0*log10(2.0*pow(10, -5));
//
//	sumv -= sum;
//
//
//	//需要释放内存
//	fftw_destroy_plan(p);
//	fftw_free(in);
//	fftw_free(out);
//	delete[] fs;
//	delete[] Ap;
//
//	return (int)sumv;
//
//	//显示采样结果
//	/*printf("ADC N点采样结果：\n");
//	vector<double> ch_data;
//	ch_data.resize(size);
//	for (int ch_index = 0; ch_index<sizeof(channel_list); ch_index++)
//	{
//		libdaq_adc_extractChannelData(data.data(), actual_len, sizeof(channel_list), ch_index, ch_data.data());
//		printf("ch_%02d: ", channel_list[ch_index]);
//
//		for (unsigned int i = 0; i<actual_len / sizeof(channel_list); i++)
//			printf("%1.4f  ", ch_data[i]);
//		printf("\n");
//	}*/
//}

//int CBlueTestD::adc_n_sample_ThdN(daqusb402x* daqdevice)
//{
//	UpdateData();
//	unsigned int actual_len;
//	adc_samplepara_t samplepara;
//	int len = m_count;
//	int m_fs = fs;
//
//	// 配置数字触发方式为电平触发(高电平触发)
//	daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD, 0, ADC_TRIG_TYPE_LEVEL, ADC_TRIG_EDGE_RISE, ADC_TRIG_LEVEL_HIGH, 0);
//	// 配置数字触发方式为边沿触发(上升沿触发)
//	// daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD,0,ADC_TRIG_TYPE_EDGE,ADC_TRIG_EDGE_RISE,ADC_TRIG_LEVEL_HIGH,0);
//
//	//配置触发方式,触发方式支持软件触发 ADC_TRIG_SRC_SW 和数字电平触发 ADC_TRIG_SRC_HWD
//	daqdevice->ADC.select_triggerSrc(ADC_TRIG_SRC_SW);//选择软件触发
//
//	//设置采样参数
//	unsigned char channel_list[] = { 0 }; //采样的通道列表，根据需求任意设置
//	samplepara.channel_list = channel_list;
//	samplepara.channel_count = sizeof(channel_list);  //采样的通道数
//	samplepara.sample_mode = ADC_SAMPLE_MODE_SYNC;//只支持同步采样，不可修改
//	samplepara.frequency = m_fs;//采样频率96kHz
//	samplepara.cycles = len;//设置每通道的采样数，连续采样时设置为0 采样点
//	daqdevice->ADC.set_sample_parameter(&samplepara);
//
//	daqdevice->ADC.clear_buffer();//清除软件和硬件buffer中的数据
//	daqdevice->ADC.start_task(); //启动采样任务，启动采样任务后并不会立即采样，需要满足触发条件后才开始采样，软件触发时等待软件触发命令，数字电平触发时等待触发引脚变为高电平
//	daqdevice->ADC.send_trigger();//发送软件触发命令，启动采样
//
//	//分配缓冲区用于存储采样结果
//	int size = samplepara.cycles*samplepara.channel_count; //数据长度根据每通道采样数和采样通道数的计算得到
//	//vector<double> data;
//	m_data.resize(size);
//	daqdevice->ADC.read_analog_sync(m_data.data(), size, &actual_len, 2000); //等待返回指定长度的数据，1000ms超时
//	std::ofstream outfile1;
//	outfile1.open(_T("b.txt"), std::ios::trunc);
//	double* fftdata = new double[m_data.size()];
//	for (size_t i = 0; i < m_data.size(); i++)
//	{
//		outfile1 << m_data[i] << setiosflags(ios::fixed) << setprecision(2) << "\n";
//		fftdata[i] = m_data[i];
//	}
//	outfile1.close();
//	//停止采样
//	daqdevice->ADC.stop(); //停止ADC
//	daqdevice->ADC.stop_task();//停止采样任务
//	daqdevice->DAC.stop();
//
//
//
//	//执行fft变换
//	fftw_plan p;
//
//	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
//	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
//
//	for (int i = 0; i < len; i++)
//	{
//
//		in[i][0] = m_data[i];//虚数的实部
//		//in[i][0] = 0.0001*sin(2 * 3.1415926 * 1000 * i / m_fs);
//		in[i][1] = 0;//虚数的虚部
//	}
//
//	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
//
//	fftw_execute(p);
//
//	double real = 0.0;
//	double vitru = 0.0;
//	//int* fs = new int[len];
//	int* fs=new int[m_count];
//	//double* Ap = new double[len];
//	double* Ap=new double[m_count];
//	double ap[5];
//	double xiebo = 0;
//	Ap[0] = 2*sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;
//
//	std::ofstream outfile;
//	outfile.open(_T("a.txt"),std::ios::trunc);
//
//
//	for (int i = 1; i < len; i++)
//	{
//		Ap[i] = 2*sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1])/len;
//		outfile << Ap[i]<<"\n";
//	}
//	outfile.close();
//	double apmax = 0;
//	int maxindex = 0;
//	for (int i = 0; i < len; i++)
//	{
//		fs[i] = i*m_fs/len;//10000/1000=10
//
//		/*int p2 = ;
//		double p = ;*/
//		if ((int)(Ap[i] * 1000000) / 1000000.0>apmax)
//		{
//			ap[0] = Ap[i];
//			apmax = Ap[i];
//			maxindex = i;
//		}
//		
//	}
//	f = fs[maxindex];
//	for (int i = 0; i < len; i++)
//	{
//		if (i != maxindex)
//		{
//			xiebo += pow(Ap[i], 2);
//		}
//	}
//
//
//	mwArray mwN(1, 1, mxINT32_CLASS);
//	mwN(1, 1) = m_data.size();
//	mwArray mwfs(1, 1, mxDOUBLE_CLASS);
//	mwfs(1, 1) = m_fs;
//	mwArray mwResult1(1, m_data.size(), mxDOUBLE_CLASS);
//	mwArray mwResult2(1, m_data.size(), mxDOUBLE_CLASS);
//
//	mwArray mwin(1, m_data.size(), mxDOUBLE_CLASS);
//
//	mwin.SetData(fftdata, m_data.size());
//
//	
//	fftzcj(2, mwResult1, mwResult2, mwin);
//
//	mxDouble* result = new mxDouble[m_data.size() / 2];
//	mwResult1.GetData(result, m_data.size()/2);
//	
//	
//	mxDouble* result2 = new mxDouble[m_data.size() / 2];
//	mwResult2.GetData(result2, m_data.size() / 2);
//
//	delete[] result;
//	delete[] result2;
//	
//	double sum = 0;
//	double sumv = 0;
//	sum = 1.0*sqrt(xiebo) / sqrt(pow(ap[0], 2));
//
//	sumv = 20.0*log10(sum);
//	sum = -20.0*log10(2.0*pow(10, -5));
//
//	sumv -= sum;
//
//
//	//需要释放内存
//	fftw_destroy_plan(p);
//	fftw_free(in);
//	fftw_free(out);
//	delete[] fs;
//	delete[] Ap;
//	delete fftdata;
//
//	InvalidateRect(drawrect);
//	return (int)sumv;
//	//显示采样结果
//	/*printf("ADC N点采样结果：\n");
//	vector<double> ch_data;
//	ch_data.resize(size);
//	for (int ch_index = 0; ch_index<sizeof(channel_list); ch_index++)
//	{
//		libdaq_adc_extractChannelData(data.data(), actual_len, sizeof(channel_list), ch_index, ch_data.data());
//		printf("ch_%02d: ", channel_list[ch_index]);
//
//		for (unsigned int i = 0; i<actual_len / sizeof(channel_list); i++)
//			printf("%1.4f  ", ch_data[i]);
//		printf("\n");
//	}*/
//}



int CBlueTestD::xinzaobi()
{
	//daqdevice->DAC.stop();
	//daqdevice->ADC.stop(); //停止ADC
	//daqdevice->ADC.stop_task();//停止采样任务
	Sleep(500);
	UpdateData();
	unsigned int actual_len;
	//adc_samplepara_t samplepara;
	int len = m_count;
	int m_fs = fs;
	double res=-1;

	//1.空采一次
	// 配置数字触发方式为电平触发(高电平触发)
	//daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD, 0, ADC_TRIG_TYPE_LEVEL, ADC_TRIG_EDGE_RISE, ADC_TRIG_LEVEL_HIGH, 0);
	// 配置数字触发方式为边沿触发(上升沿触发)
	// daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD,0,ADC_TRIG_TYPE_EDGE,ADC_TRIG_EDGE_RISE,ADC_TRIG_LEVEL_HIGH,0);

	//配置触发方式,触发方式支持软件触发 ADC_TRIG_SRC_SW 和数字电平触发 ADC_TRIG_SRC_HWD
	//daqdevice->ADC.select_triggerSrc(ADC_TRIG_SRC_SW);//选择软件触发

	//设置采样参数
	unsigned char channel_list[] = { 0 }; //采样的通道列表，根据需求任意设置
	//samplepara.channel_list = channel_list;
	//samplepara.channel_count = sizeof(channel_list);  //采样的通道数
	//samplepara.sample_mode = ADC_SAMPLE_MODE_SYNC;//只支持同步采样，不可修改
	//samplepara.frequency = m_fs;//采样频率96kHz
	//samplepara.cycles = len;//设置每通道的采样数，连续采样时设置为0 采样点
	//daqdevice->ADC.set_sample_parameter(&samplepara);

	//daqdevice->ADC.clear_buffer();//清除软件和硬件buffer中的数据
	//daqdevice->ADC.start_task(); //启动采样任务，启动采样任务后并不会立即采样，需要满足触发条件后才开始采样，软件触发时等待软件触发命令，数字电平触发时等待触发引脚变为高电平
	//daqdevice->ADC.send_trigger();//发送软件触发命令，启动采样

	//分配缓冲区用于存储采样结果
	//int size = samplepara.cycles*samplepara.channel_count; //数据长度根据每通道采样数和采样通道数的计算得到
	
	//m_data2.resize(size);
	//daqdevice->ADC.read_analog_sync(m_data2.data(), size, &actual_len, 2000); //等待返回指定长度的数据，1000ms超时
	//std::ofstream outfile1;
	//outfile1.open(_T("b.txt"), std::ios::trunc);
	////double* fftdata = new double[data.size()];
	//for (size_t i = 0; i < data.size(); i++)
	//{
	//	outfile1 << data[i] << setiosflags(ios::fixed) << setprecision(2) << "\n";
	//	//fftdata[i] = data[i];
	//}
	//outfile1.close();
	//停止采样
	//daqdevice->ADC.stop(); //停止ADC
	//daqdevice->ADC.stop_task();//停止采样任务
	


	//计算幅值
	//执行fft变换
	fftw_plan p;

	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);

	for (int i = 0; i < len; i++)
	{

		in[i][0] = m_data2[i];//虚数的实部
		//in[i][0] = 0.0001*sin(2 * 3.1415926 * 1000 * i / m_fs);
		in[i][1] = 0;//虚数的虚部
	}

	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(p);

	double real = 0.0;
	double vitru = 0.0;
	//int* fs = new int[len];
	//int* fss = new int[m_count];
	//double* Ap = new double[len];
	double* Ap = new double[m_count];
	double ap[5];
	double xiebo = 0;
	Ap[0] = 2 * sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;

	//std::ofstream outfile;
	//outfile.open(_T("a.txt"), std::ios::trunc);


	for (int i = 1; i < len; i++)
	{
		Ap[i] = 2 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / len;
		//outfile << Ap[i] << "\n";
	}
	//outfile.close();
	double apmax = 0;
	int maxindex = 0;
	double zaoyin = 0;
	for (int i = 517; i < len; i++)
	{

		zaoyin += sqrt(pow(Ap[i], 2));
	}
	

	

	//2.输出1v电压
	UpdateData();
	//1.生成1KHz的激励信号
	//dac_wavepara_t dac_cfg;
	//输出周期波形，本例以输出1KHz正弦波为例，一个正弦波形周期取10个点，数据点更新频率为10KHz输出的波形为1KHz
	/*printf("DAC输出1KHz正弦波形\n");*/
	double* buf = new double[m_count];
	int f = testfs;
	for (int x = 0; x<m_count; x++){
		//angle change to arc
		buf[x] = 10.0*(sin(x*(2 * 3.1415926*f / fs)));

	}

	//dac_cfg.buf = buf; //数据点缓冲区
	//dac_cfg.buflen = m_count;
	//dac_cfg.cycles = 0; //0代表周期性循环输出，非0代表波形输出周期数
	//dac_cfg.frequency = fs;//点更新频率，96KHz
	//dac_cfg.trigger_mode = DAC_TRIGGER_MODE_SOFT;//软件触发
	//daqdevice->DAC.set_wavepara(&dac_cfg);
	//daqdevice->DAC.start();
	//Sleep(1000);
	//daqdevice->DAC.stop();


	//2.回采信号
	//memset(buf, 0, 2000);
	//adc_config_channel(daqdevice);
	

	
	//回采信号
	// 配置数字触发方式为电平触发(高电平触发)
	//daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD, 0, ADC_TRIG_TYPE_LEVEL, ADC_TRIG_EDGE_RISE, ADC_TRIG_LEVEL_HIGH, 0);
	// 配置数字触发方式为边沿触发(上升沿触发)
	// daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD,0,ADC_TRIG_TYPE_EDGE,ADC_TRIG_EDGE_RISE,ADC_TRIG_LEVEL_HIGH,0);

	//配置触发方式,触发方式支持软件触发 ADC_TRIG_SRC_SW 和数字电平触发 ADC_TRIG_SRC_HWD
	//daqdevice->ADC.select_triggerSrc(ADC_TRIG_SRC_SW);//选择软件触发

	////设置采样参数
	////channel_list[] = { 0 }; //采样的通道列表，根据需求任意设置
	//samplepara.channel_list = channel_list;
	//samplepara.channel_count = sizeof(channel_list);  //采样的通道数
	//samplepara.sample_mode = ADC_SAMPLE_MODE_SYNC;//只支持同步采样，不可修改
	//samplepara.frequency = m_fs;//采样频率96kHz
	//samplepara.cycles = len;//设置每通道的采样数，连续采样时设置为0 采样点
	//daqdevice->ADC.set_sample_parameter(&samplepara);

	//daqdevice->ADC.clear_buffer();//清除软件和硬件buffer中的数据
	//daqdevice->ADC.start_task(); //启动采样任务，启动采样任务后并不会立即采样，需要满足触发条件后才开始采样，软件触发时等待软件触发命令，数字电平触发时等待触发引脚变为高电平
	//daqdevice->ADC.send_trigger();//发送软件触发命令，启动采样

	//分配缓冲区用于存储采样结果
//	int size = samplepara.cycles*samplepara.channel_count; //数据长度根据每通道采样数和采样通道数的计算得到
//	vector<double> data;
	//m_data.resize(size);
	//actual_len = 0;
	//daqdevice->ADC.read_analog_sync(m_data.data(), size, &actual_len, 1000); //等待返回指定长度的数据，1000ms超时
	//std::ofstream outfile1;
	//outfile1.open(_T("yinzaobi.txt"), std::ios::trunc);
	////double* fftdata = new double[data.size()];
	//for (size_t i = 0; i < data.size(); i++)
	//{
	//	outfile1 << data[i] << setiosflags(ios::fixed) << setprecision(2) << "\n";
	//	//fftdata[i] = data[i];
	//}
	//outfile1.close();
	//停止采样
	//daqdevice->ADC.stop(); //停止ADC
	//daqdevice->ADC.stop_task();//停止采样任务
	//daqdevice->DAC.stop();

	//计算幅值
	//执行fft变换
//	fftw_plan p;

	//fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
	//fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);

	

	for (int i = 1; i < len; i++)
	{

		in[i][0] = m_data[i];//虚数的实部
		//in[i][0] = 0.0001*sin(2 * 3.1415926 * 1000 * i / m_fs);
		in[i][1] = 0;//虚数的虚部
	}

	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(p);

	 real = 0.0;
	 vitru = 0.0;
	//int* fs = new int[len];
	//int* fs = new int[m_count];
	//double* Ap = new double[len];
	 //delete [] Ap;
	 //Ap = new double[m_count];
	//double ap[5];
	xiebo = 0;
	Ap[0] = 2 * sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;

	/*std::ofstream outfile;
	outfile.open(_T("a.txt"), std::ios::trunc);
*/

	for (int i = 1; i < len; i++)
	{
		Ap[i] = 2 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / len;
		//outfile << Ap[i] << "\n";
	}
	//outfile.close();
	apmax = 0;
	maxindex = 0;
	double yinhao = 0;
	for (int i = 517; i < len; i++)
	{

		yinhao += sqrt(pow(Ap[i], 2));
	}

	res = 1.0*yinhao / zaoyin;
	double sumv = 0, sum=0;
	sumv = 20.0*log10(res);
	//sum = -20.0*log10(2.0*pow(10, -5));

	delete[] buf;
	InvalidateRect(drawrect);
	return (int)(sumv);
}

int CBlueTestD::gelidu()
{
	double res = -1;
	UpdateData();
	unsigned int actual_len;
	//adc_samplepara_t samplepara;
	int len = m_count;
	int m_fs = fs;
	//2.输出1v电压
	UpdateData();
	//1.生成1KHz的激励信号
	//dac_wavepara_t dac_cfg;
	//输出周期波形，本例以输出1KHz正弦波为例，一个正弦波形周期取10个点，数据点更新频率为10KHz输出的波形为1KHz
	/*printf("DAC输出1KHz正弦波形\n");*/
	double* buf = new double[m_count];
	int f = testfs;
	for (int x = 0; x<m_count; x++){
		//angle change to arc
		buf[x] = 1.0*(sin(x*(2 * 3.1415926*f / fs)));

	}

	//dac_cfg.buf = buf; //数据点缓冲区
	//dac_cfg.buflen = m_count;
	//dac_cfg.cycles = 0; //0代表周期性循环输出，非0代表波形输出周期数
	//dac_cfg.frequency = fs;//点更新频率，96KHz
	//dac_cfg.trigger_mode = DAC_TRIGGER_MODE_SOFT;//软件触发
	//daqdevice->DAC.set_wavepara(&dac_cfg);
	//daqdevice->DAC.start();
	//Sleep(1000);
	//daqdevice->DAC.stop();


	//2.回采信号
	//memset(buf, 0, 2000);
	//adc_config_channel(daqdevice);



	//回采信号
	// 配置数字触发方式为电平触发(高电平触发)
	//daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD, 0, ADC_TRIG_TYPE_LEVEL, ADC_TRIG_EDGE_RISE, ADC_TRIG_LEVEL_HIGH, 0);
	// 配置数字触发方式为边沿触发(上升沿触发)
	// daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD,0,ADC_TRIG_TYPE_EDGE,ADC_TRIG_EDGE_RISE,ADC_TRIG_LEVEL_HIGH,0);

	//配置触发方式,触发方式支持软件触发 ADC_TRIG_SRC_SW 和数字电平触发 ADC_TRIG_SRC_HWD
	//daqdevice->ADC.select_triggerSrc(ADC_TRIG_SRC_SW);//选择软件触发

	//设置采样参数
	unsigned char channel_list[] = { 2 }; //采样的通道列表，根据需求任意设置 //采样的通道列表，根据需求任意设置
	//samplepara.channel_list = channel_list;
	//samplepara.channel_count = sizeof(channel_list);  //采样的通道数
	//samplepara.sample_mode = ADC_SAMPLE_MODE_SYNC;//只支持同步采样，不可修改
	//samplepara.frequency = m_fs;//采样频率96kHz
	//samplepara.cycles = len;//设置每通道的采样数，连续采样时设置为0 采样点
	//daqdevice->ADC.set_sample_parameter(&samplepara);

	//daqdevice->ADC.clear_buffer();//清除软件和硬件buffer中的数据
	//daqdevice->ADC.start_task(); //启动采样任务，启动采样任务后并不会立即采样，需要满足触发条件后才开始采样，软件触发时等待软件触发命令，数字电平触发时等待触发引脚变为高电平
	//daqdevice->ADC.send_trigger();//发送软件触发命令，启动采样

	//分配缓冲区用于存储采样结果
	//	int size = samplepara.cycles*samplepara.channel_count; //数据长度根据每通道采样数和采样通道数的计算得到
	////	vector<double> data;
	//m_data.resize(size);
	//actual_len = 0;
	//daqdevice->ADC.read_analog_sync(m_data.data(), size, &actual_len, 1000); //等待返回指定长度的数据，1000ms超时
	//std::ofstream outfile1;
	//outfile1.open(_T("yinzaobi.txt"), std::ios::trunc);
	double* fftdata = new double[m_data.size()];
	for (size_t i = 0; i < m_data.size(); i++)
	{
		//outfile1 << data[i] << setiosflags(ios::fixed) << setprecision(2) << "\n";
		fftdata[i] = m_data[i];
	}
	//outfile1.close();
	//停止采样
	//daqdevice->ADC.stop(); //停止ADC
	//daqdevice->ADC.stop_task();//停止采样任务
	//daqdevice->DAC.stop();


//计算1口的电压
	mwArray mwN(1, 1, mxINT32_CLASS);
	mwN(1, 1) = m_data.size();
	mwArray mwfs(1, 1, mxDOUBLE_CLASS);
	mwfs(1, 1) = m_fs;
	mwArray mwResult1(1, m_data.size() , mxDOUBLE_CLASS);
	mwArray mwResult2(1, m_data.size(), mxDOUBLE_CLASS);

	mwArray mwin(1, m_data.size(), mxDOUBLE_CLASS);

	mwin.SetData(fftdata, m_data.size());

	delete[] fftdata;
	fftzcj(2, mwResult1, mwResult2, mwin);

	mxDouble* result = new mxDouble[m_data.size() / 2];
	mwResult1.GetData(result, m_data.size() / 2);


	mxDouble* result2 = new mxDouble[m_data.size() / 2];
	mwResult2.GetData(result2, m_data.size() / 2);

	double sum = 0;
	for (int i = 0; i < m_data.size() / 2; i++)
	{
		sum += sqrt(pow(result2[i],2));
	}

	double sumv = 0;
	sumv = 20.0*log10(sum);
	sum = -20.0*log10(2.0*pow(10, -5));

	delete[] result;
	delete[] result2;

	InvalidateRect(drawrect);
	return (int)(sumv - sum);

}

int CBlueTestD::dizao()
{
	//daqdevice->DAC.stop();
	//daqdevice->ADC.stop(); //停止ADC
	//daqdevice->ADC.stop_task();//停止采样任务
	Sleep(500);
	double res = 0;
	UpdateData();
	unsigned int actual_len;
	//adc_samplepara_t samplepara;
	int len = m_count;
	int m_fs = fs;

	//空采
	//1.空采一次
	// 配置数字触发方式为电平触发(高电平触发)
	//daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD, 0, ADC_TRIG_TYPE_LEVEL, ADC_TRIG_EDGE_RISE, ADC_TRIG_LEVEL_HIGH, 0);
	// 配置数字触发方式为边沿触发(上升沿触发)
	// daqdevice->ADC.config_triggerSrc(ADC_TRIG_SRC_HWD,0,ADC_TRIG_TYPE_EDGE,ADC_TRIG_EDGE_RISE,ADC_TRIG_LEVEL_HIGH,0);

	//配置触发方式,触发方式支持软件触发 ADC_TRIG_SRC_SW 和数字电平触发 ADC_TRIG_SRC_HWD
	//daqdevice->ADC.select_triggerSrc(ADC_TRIG_SRC_SW);//选择软件触发

	//设置采样参数
	unsigned char channel_list[] = { 0 }; //采样的通道列表，根据需求任意设置
	//samplepara.channel_list = channel_list;
	//samplepara.channel_count = sizeof(channel_list);  //采样的通道数
	//samplepara.sample_mode = ADC_SAMPLE_MODE_SYNC;//只支持同步采样，不可修改
	//samplepara.frequency = m_fs;//采样频率96kHz
	//samplepara.cycles = len;//设置每通道的采样数，连续采样时设置为0 采样点
	//daqdevice->ADC.set_sample_parameter(&samplepara);

	//daqdevice->ADC.clear_buffer();//清除软件和硬件buffer中的数据
	//daqdevice->ADC.start_task(); //启动采样任务，启动采样任务后并不会立即采样，需要满足触发条件后才开始采样，软件触发时等待软件触发命令，数字电平触发时等待触发引脚变为高电平
	//daqdevice->ADC.send_trigger();//发送软件触发命令，启动采样

	//分配缓冲区用于存储采样结果
	//int size = samplepara.cycles*samplepara.channel_count; //数据长度根据每通道采样数和采样通道数的计算得到

	//m_data2.resize(size);
	//daqdevice->ADC.read_analog_sync(m_data2.data(), size, &actual_len, 2000); //等待返回指定长度的数据，1000ms超时
	//std::ofstream outfile1;
	//outfile1.open(_T("b.txt"), std::ios::trunc);
	////double* fftdata = new double[data.size()];
	//for (size_t i = 0; i < data.size(); i++)
	//{
	//	outfile1 << data[i] << setiosflags(ios::fixed) << setprecision(2) << "\n";
	//	//fftdata[i] = data[i];
	//}
	//outfile1.close();
	//停止采样
	//daqdevice->ADC.stop(); //停止ADC
	//daqdevice->ADC.stop_task();//停止采样任务


	m_data.clear();
	//计算幅值
	//执行fft变换
	fftw_plan p;

	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*len);

	for (int i = 0; i < len; i++)
	{

		in[i][0] = m_data2[i];//虚数的实部
		//in[i][0] = 0.0001*sin(2 * 3.1415926 * 1000 * i / m_fs);
		in[i][1] = 0;//虚数的虚部
	}

	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(p);

	double real = 0.0;
	double vitru = 0.0;
	//int* fs = new int[len];
	//int* fss = new int[m_count];
	//double* Ap = new double[len];
	double* Ap = new double[m_count];
	double ap[5];
	double xiebo = 0;
	Ap[0] = 2 * sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;

	//std::ofstream outfile;
	//outfile.open(_T("a.txt"), std::ios::trunc);


	for (int i = 1; i < len; i++)
	{
		Ap[i] = 2 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / len;
		//outfile << Ap[i] << "\n";
	}
	//outfile.close();
	double apmax = 0;
	int maxindex = 0;
	double zaoyin = 0;
	for (int i = 0; i < len; i++)
	{

		zaoyin += sqrt(pow(Ap[i], 2));
	}

	double sumv = 0, sum = 0;
	sumv = 20.0*log10(zaoyin);
	sum = -20.0*log10(2.0*pow(10, -5));
	InvalidateRect(drawrect);
	return (int)(sumv - sum);

}



void CBlueTestD::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	
	CDC* pdc=GetDlgItem(IDC_pictrue)->GetDC();
	CPen pen;
	pen.CreatePen(1, 1, RGB(0, 0, 0));
	CPen* oled = pdc->SelectObject(&pen);
	/****************设置背景色******************************/
	CRect rect;
	GetDlgItem(IDC_pictrue)->GetClientRect(&rect);
	FillRect(pdc->GetSafeHdc(), &rect, CBrush(RGB(255, 255, 255)));
	UpdateData(FALSE);
	/******************绘制图形********************************/
	int xcout = 0;
	if (m_data.size() > 0)
	{
		for (size_t i = xcout; i < 400; i++)
		{
			POINT p;
			p.x = (i - xcout)*xscale;
			p.y = m_data[i] * yscale + 150;
			pdc->MoveTo(p);
			p.x = (i - xcout + 1)*xscale;
			p.y = m_data[i  + 1] * yscale + 150;
			pdc->LineTo(p);
		}
	}
	//int xscale = 10;
	//int yscale = 1000;
	
	

	pen.DeleteObject();
	pen.CreatePen(1, 1, RGB(0, 0, 255));
	oled=pdc->SelectObject(&pen);
	if (m_data2.size() > 0)
	{
		for (size_t i = xcout; i < m_data2.size() - 1; i++)
		{
			POINT p;
			p.x = (i - xcout)*xscale;
			p.y = m_data2[i ] * yscale + 150;
			pdc->MoveTo(p);
			p.x = (i - xcout + 1)*xscale;
			p.y = m_data2[i  + 1] * yscale + 150;
			pdc->LineTo(p);
		}
	}
	

	pen.DeleteObject();
	//pdc->SelectObject(oled);


}


HBRUSH CBlueTestD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if (pWnd->GetDlgCtrlID() == IDC_pictrue)
	{
		

		pDC->SetBkColor(RGB(255, 255, 255));
	}

	return hbr;
}


void CBlueTestD::OnBnClickedButtonFR()
{
	// TODO:  在此添加控件通知处理程序代码
	/******************以下为测试代码**************************/

	UpdateData();
	if (jiliset.data.size() > 0)
	{
		data = &jiliset.data;
		
	}
	else
	{
		AfxMessageBox(_T("激励信号未确认！"));
		return;
	}
	int len = data->size();
	mwArray out1(jiliset.pos.size(), 1, mxDOUBLE_CLASS);
	mwArray out2(jiliset.pos.size(), 44100 / 40, mxDOUBLE_CLASS);
	mwArray out3(jiliset.pos.size(), 1, mxDOUBLE_CLASS);
	mwArray out4(jiliset.pos.size(), 1, mxDOUBLE_CLASS);
	mwArray mwStim(len, 1, mxDOUBLE_CLASS);
	double *stim = new double[len];
	m_data.clear();
	std::ofstream outfile1;
	CString path1("D:\\TestRecord\\dat.txt");
	outfile1.open(path1, std::ios::out | std::ios::trunc);
	for (size_t i = 0; i < len; i++)
	{
		stim[i] = data->at(i);
		m_data.push_back(data->at(i));
		outfile1 << data->at(i) << endl;
		
	}
	outfile1.close();
	mwStim.SetData(stim, len);
	
	mwArray mwFs(1, 1, mxDOUBLE_CLASS);
	mwArray mwbNorse(1, 1, mxDOUBLE_CLASS);
	mwFs(1, 1) = 44100;
	int l = 0;
	int i = 1;
	
	std::ofstream outfile;
	CString path("D:\\TestRecord\\pos.txt");
    outfile.open(path, std::ios::out | std::ios::trunc);
	mwArray mwPosition(jiliset.pos.size(), 3, mxDOUBLE_CLASS);
	for (std::map<int, int>::iterator it = jiliset.pos.begin(); it != jiliset.pos.end(); it++)
	{
		mwPosition(i, 1) = (*it).first;
		mwPosition(i, 2) =  l+1;
		
		mwPosition(i, 3) =  l+(*it).second-1;

		i++;
		outfile << (*it).first << "  " << l + 1 << "  " << l + (*it).second-1 << endl;
		l += (*it).second;		

	}
	outfile.close();

	FrHarm(2, out1, out2, out3, out4, out4, mwStim, mwStim, mwFs, mwPosition, mwbNorse, mwbNorse);
	mxDouble* result = new mxDouble[jiliset.pos.size()];
	out1.GetData(result, jiliset.pos.size());
	//m_data.clear();
	std::vector<double> data1;
	data1.clear();
	for (int h = 0; h < jiliset.pos.size(); h++)
	{
		data1.push_back(result[h]);
	}

	mxDouble* result2 = new mxDouble[44100 / 40];
	out2.GetData(result2, 1);
	m_data2.clear();
	for (int h = 0; h < 44100 / 40; h++)
	{
		m_data2.push_back(result2[h]);
	}
	delete[] stim;
	//Invalidate();
	InvalidateRect(drawrect);

}
