// CalibInput.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "CalibInput.h"
#include "afxdialogex.h"
#include "DeviceManage.h"
#include "wavHandler.h"
#include <fstream>
#include "JiLiSet.h"
#include "fftw3.h"
#include "fftzcj.h"
#include "InputDialg.h"
#include "fftzcj.h"

#define EXCITATIONERCIVETASKNAME "testsensitivity"

using namespace std;

extern CDeviceManage devicemanage;
extern CJiLiSet jiliset;

// CCalibInput 对话框

IMPLEMENT_DYNAMIC(CCalibInput, CDialogEx)

CCalibInput::CCalibInput(CWnd* pParent /*=NULL*/)
: CDialogEx(CCalibInput::IDD, pParent), maxdeviceid(0), inputval(50),
calibrfs(1000)
{
	/*bool b_ok = fftzcjInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("fftzcjInitialize err"));

	}*/

}

CCalibInput::~CCalibInput()
{
}

void CCalibInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);

	DDX_Text(pDX, IDC_calibrainputDevices, devicename);
	DDX_Text(pDX, IDC_editcalbraininputval, inputval);

	DDX_Text(pDX, IDC_editcalbrafs, calibrfs);

	DDX_Text(pDX, IDC_editcalibrationinputVrms, invrms);
	DDX_Text(pDX, IDC_editcalibrationinputVPval, inVP);
	DDX_Control(pDX, IDC_calibrainputDevices, m_cmbinnerDevices);


	

}

BOOL CCalibInput::OnInitDialog()
{

	UpdateData(FALSE);

	
	hroot=m_tree.InsertItem(_T("输入通道1"), 0, 0,TVI_ROOT);
	hroot2=m_tree.InsertItem(_T("输入通道2"), 0, 0, TVI_ROOT);
	 
	
	m_tree.InsertItem(_T("设备1.cfg"), 1, 1, hroot, TVI_LAST);
	m_tree.InsertItem(_T("设备2.cfg"), 1, 1, hroot, TVI_LAST);
	m_tree.InsertItem(_T("设备3.cfg"), 1, 1, hroot, TVI_LAST);
	m_tree.InsertItem(_T("设备4.cfg"), 1, 1, hroot, TVI_LAST);

	m_tree.InsertItem(_T("设备1.cfg"), 1, 1, hroot2, TVI_LAST);
	m_tree.Expand(hroot,TVE_EXPAND);


	
	getmaxdeviceId();
	SetBackgroundColor(RGB(248, 248, 255));

	//CRect rectDummy;
	//rectDummy.SetRectEmpty();

	//// 创建视图:
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//if (!m_treeview.Create(dwViewStyle, rectDummy, this, 2))
	//{
	//	TRACE0("未能创建类视图\n");
	//	return -1;      // 未能创建
	//}

	//HTREEITEM hRoot = m_treeview.InsertItem(_T("输入通道1"), 0, 0);
	//m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);


	/*pDevUSB4431 = new CUSB4431();
	if (pDevUSB4431)
		pDevUSB4431->AddCallBackPtr(this);*/

	OnCbnDropdowncalibrainputdevices();

	return TRUE;
}

void CCalibInput::getmaxdeviceId()
{
	HTREEITEM child = m_tree.GetChildItem(hroot);
	int id = 0;
	maxdeviceid = 0;
	while (child != NULL)
	{
		CString str = m_tree.GetItemText(child);
		if (str.GetLength() >=3)
		{
			if (str[0] == CString(_T("设")) &&
				str[1] == CString(_T("备")))
			{
				bool bisnum = true;
				for (size_t i = 2; i < str.GetLength(); i++)
				{
					if (str[i] == (CString(_T("0")) || CString(_T("0"))) || CString(_T("1")) || CString(_T("2"))
						|| CString(_T("3")) || CString(_T("4")) || CString(_T("5")) || CString(_T("6")) || CString(_T("7"))
						|| CString(_T("8")) || CString(_T("9")))
					{

					}
					else
					{
						bisnum = false;

					}
				}
				

				if (bisnum)
				{
					
					CString sss = str.Right(str.GetLength()-2);
					int len1 = WideCharToMultiByte(CP_ACP, 0, sss, -1, NULL, 0, NULL, NULL);
					char* ch = new char[len1 + 1]{0};
					WideCharToMultiByte(CP_ACP, 0, sss, -1, ch, len1, NULL, NULL);
					id = atoi(ch);
				}
			}
		}

		if (maxdeviceid < id)
		{
			maxdeviceid = id;
		}

		child = m_tree.GetNextSiblingItem(child);


	}

}


BEGIN_MESSAGE_MAP(CCalibInput, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalibInput::OnBnClickedButtonStartCalibration)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalibInput::OnBnClickedButtonDelInstru)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CCalibInput::OnNMClickTree1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TREE1, &CCalibInput::OnNMKillfocusTree1)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, &CCalibInput::OnTvnKeydownTree1)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalibInput::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalibInput::OnBnClickedButtonVpCalibration)
//	ON_CBN_DROPDOWN(IDC_calibrainputDevices, &CCalibInput::OnCbnDropdowncalibrainputdevices)
	ON_EN_CHANGE(IDC_editcalbraininputval, &CCalibInput::OnEnChangeeditcalbraininputval)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON5, &CCalibInput::OnBnClickedButtonUpdateVp)
	ON_CBN_DROPDOWN(IDC_calibrainputDevices, &CCalibInput::OnCbnDropdowncalibrainputdevices)
	ON_BN_CLICKED(IDH_OK, &CCalibInput::OnOK)
END_MESSAGE_MAP()


// CCalibInput 消息处理程序

//double vp = 0.00445;//liss
//rme in 0.366 0.038  vp=0.366/0.038=9.63
// 0.1 /0.0001=10000
//校准mic使用外部标准声源，因此这里只录音
void CCalibInput::OnBnClickedButtonStartCalibration()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	//getApzcj(NULL, 96000 * 10, 96000, calibrfs);


	double vp = devicemanage.getinputvp(devicename);//10
	if (!devicemanage.checkhasinputname(devicename))
	{
		AfxMessageBox(_T("设备管理中不存在此设备标识符！"));
		return;
	}


	//判断是否重名
	if (isdupname(devicename))
	{
		AfxMessageBox(_T("设备标识符重名！"));
		return;
	}

	
	char buf[10];
	sprintf(buf, "设备%d.cfg", ++maxdeviceid);
	CString decname = CString(buf);
	if (devicename != "")
	{
		decname = devicename;
		maxdeviceid--;
	}
	m_tree.InsertItem(decname, 1, 1, hroot, TVI_LAST);
	m_tree.Expand(hroot, TVE_EXPAND);

	int** dat=NULL;
	double*dat2 = NULL;
	int fs = 96000;
	fs = devicemanage.getfs(decname);
	int recordtime = 10;//秒
	devicemanage.calibrationMic_NotPlay(decname, &dat, fs, recordtime, &dat2);
	if (dat&&*dat)
	{
		char* fileName1 = "D:\\TestRecord\\mic01.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[0], fs*recordtime * 4);

		/*fileName1 = "D:\\TestRecord\\mic02.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[1], fs*recordtime * 4);

		fileName1 = "D:\\TestRecord\\mic03.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[2], fs*recordtime * 4);

		fileName1 = "D:\\TestRecord\\mic04.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[3], fs*recordtime * 4);

		fileName1 = "D:\\TestRecord\\mic05.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[4], fs*recordtime * 4);

		fileName1 = "D:\\TestRecord\\mic06.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[5], fs*recordtime * 4);

		fileName1 = "D:\\TestRecord\\mic07.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[6], fs*recordtime * 4);

		fileName1 = "D:\\TestRecord\\mic08.wav";
		wavwrite(fileName1, 1, fs, 32, (char*)dat[7], fs*recordtime * 4);*/


		//分析计算出灵敏度值
		double sensitivity = 0.0;
		//1.计算信号幅值 0.118v=118mv
		long bili = 1;
		double ap = 0;//电压大小

		char* driver = NULL;;
		devicemanage.getDriverType(devicename, &driver);
		if (strcmp(driver, "ASIO") == 0)
		{
		
			//ap = getAp<int>(dat[0], _msize(dat[0])/4/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap = getApzcj(dat[0], _msize(dat[0]) / 4/*fs*recordtime*/, fs, calibrfs);
			ap /= pow(2, 31)*1.0;
			ap /= sqrt(2)*1.0;
			ap *= vp;
			ap *= 1000;//化为mv
			int intap = ap * 10000;
			inputval = 1.0*intap / 10000.0;
		}
		else
		{
			
			ap = getApzcjdouble(dat2, _msize(dat2) / sizeof(double)/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap /= sqrt(2)*1.0;
			ap *= vp;
			ap *= 1000;//化为mv
			int intap = ap * 10000;
			inputval = 1.0*intap / 10000.0;

		}

		UpdateData(FALSE);

		AfxMessageBox(_T("校准完成！"));
		//double v = ap*vp;

	   // vp = 0.118 / ap;
		
		
		//2.根据标准声压值换算出系统灵敏度
		//2.1把mic的声压(db)级换算成pa
		CString str("94");
		//m_cmbshengya.GetWindowTextW(str);
		int len1 = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* ch = new char[len1 + 1];
		WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
		double pa = jiliset.getPa(atof(ch));
		delete[] ch;

		//2.2 把幅值(mv)除以pa
		sensitivity = 1.0*ap / pa;





		//AfxMessageBox(_T("校准完成"));




		//把灵敏度值写入到文件
		ofstream out;
		CString filename("./DeviceConf/");
		filename.Append(decname);
		filename.Append(CString(".txt"));
		out.open(filename, ios::out | ios::trunc);
		if (!out.is_open())
		{
			system("mkdir .\\DeviceConf");
			out.open(filename, ios::out | ios::trunc);
			
		}

		if (!out.is_open())
		{
			AfxMessageBox(_T("写入配置文件失败!"));
			return;
		}

		out << getcharp(decname) << ":" << sensitivity<<"mv/Pa" << endl;

		out.close();

		//清楚录音数据
		delete[] dat[0];
		delete[] dat;
		delete[] dat2;
		
	}
	

}

double CCalibInput::cal_sensitivity(CString decname,double vp, double fs)
{
	int** dat = NULL;
	double*dat2 = NULL;
	int recordtime = ReadInt(_T("RecordData"), _T("MicCalibrationTime"), 10, GetScriptIniFile());//秒
	devicemanage.calibrationMic_NotPlay(decname, &dat, fs, recordtime, &dat2);
	if (dat&&*dat)
	{
		

		


		//分析计算出灵敏度值
		double sensitivity = 0.0;
		//1.计算信号幅值 0.118v=118mv
		long bili = 1;
		double ap = 0;//电压大小

		char* driver = NULL;;
		devicemanage.getDriverType(decname, &driver);
		if (strcmp(driver, "ASIO") == 0)
		{

			std::ofstream outfile;
			char sss1[512];
			sprintf(sss1, "D:\\TestRecord\\spk_cal.txt");
			CString path(sss1);
			outfile.open(path, std::ios::out | std::ios::trunc);

			for (size_t ih = 0; ih < _msize(dat[0]) / 4; ih++)
			{
				outfile << dat[0][ih] << endl;
			}
			
			outfile.close();


			char* fileName1 = "D:\\TestRecord\\ASIOmic01.wav";
			wavwrite(fileName1, 1, fs, 32, (char*)dat[0], fs*recordtime * 4);
			//ap = getAp<int>(dat[0], _msize(dat[0])/4/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap = getApzcj(dat[0], _msize(dat[0]) / 4/*fs*recordtime*/, fs, calibrfs);
			ap /= pow(2, 31)*1.0;
			ap /= sqrt(2)*1.0;
			ap *= vp;
			ap *= 1000;//化为mv
			int intap = ap * 10000;
			inputval = 1.0*intap / 10000.0;
		}
		else
		{

			ap = getApzcjdouble(dat2, _msize(dat2) / sizeof(double)/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap /= sqrt(2)*1.0;
			ap *= vp;
			ap *= 1000;//化为mv
			int intap = ap * 10000;
			inputval = 1.0*intap / 10000.0;

		}

		//UpdateData(FALSE);

		//AfxMessageBox(_T("校准完成！"));
		//double v = ap*vp;

		// vp = 0.118 / ap;


		//2.根据标准声压值换算出系统灵敏度
		//2.1把mic的声压(db)级换算成pa
		CString str("94");
		//m_cmbshengya.GetWindowTextW(str);
		int len1 = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* ch = new char[len1 + 1];
		WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
		double pa = jiliset.getPa(atof(ch));
		delete[] ch;

		//2.2 把幅值(mv)除以pa
		sensitivity = 1.0*ap / pa;





		//AfxMessageBox(_T("校准完成"));




		//把灵敏度值写入到文件
		/*ofstream out;
		CString filename("./DeviceConf/");
		filename.Append(decname);
		filename.Append(CString(".txt"));
		out.open(filename, ios::out | ios::trunc);
		if (!out.is_open())
		{
			system("mkdir .\\DeviceConf");
			out.open(filename, ios::out | ios::trunc);

		}

		if (!out.is_open())
		{
			AfxMessageBox(_T("写入配置文件失败!"));
			return;
		}

		out << getcharp(decname) << ":" << sensitivity << "mv/Pa" << endl;

		out.close();*/

		//清楚录音数据
		delete[] dat[0];
		delete[] dat;
		delete[] dat2;


		//int temsem = sensitivity * 100;
		//sensitivity = temsem/ 100;

		return sensitivity;

	}

	return 0;
}

//也可以考虑将数据写入文件
double CCalibInput::getInDeviceSensitivity()
{
	return inputval;
}

//void CCalibInput::NIDaqStartAi()
//{
//	bool b_ok = false;
//	///pDevUSB4431->AddCallBackPtr(this);
//	//pDrawBmprecivedata->LoadPoint(AITASKNAME, new double[1]{0}, 1, RGB(0xCC, 0xFF, 0));
//	//pDrawBmp2->LoadPoint("FR", new double[1]{0}, 1, RGB(0x66, 0xFF, 0xFF));
//	//pDrawBmprecivedata->Show();
//	//pDrawBmp2->Show();
//	DAQmxAiTaskParam param;
//	//pDevUSB4431->GetAiTaskParam(AITASKNAME, &param);
//	param.samples = 44100 * 3;
//	param.totalSamples = 44100 * 3;
//	param.sampleMode = DAQmx_Val_FiniteSamps;
//	param.sampleRate = 44100;
//	param.maxSndPressLevel = 3;
//	//param.trigLvl = m_num_trigLvl;
//	param.physicalChannel = "Dev1/ai0";
//	param.sensitivity = 1000;
//	//param.currentExcitSource = DAQmx_Val_External;
//	b_ok=pDevUSB4431->LoadAiTask(EXCITATIONERCIVETASKNAME, param, NULL);
//	b_ok=pDevUSB4431->StartAiTask(EXCITATIONERCIVETASKNAME);
//}

// 0.25  1

// 0.5  2

void CCalibInput::RecieveUSB4431Data(double * readArrary, UINT32 dataLength)
{
	double vp = 0;
	/*mwArray mwN(1, 1, mxINT32_CLASS);
	mwN(1, 1) = dataLength;
	mwArray mwfs(1, 1, mxDOUBLE_CLASS);
	mwfs(1, 1) = 44100;
	mwArray mwResult1(1, dataLength, mxDOUBLE_CLASS);
	mwArray mwResult2(1, dataLength, mxDOUBLE_CLASS);

	mwArray mwin(1, dataLength, mxDOUBLE_CLASS);

	mwin.SetData(readArrary, dataLength);


	fftzcj(2, mwResult1, mwResult2, mwin);

	mxDouble* result = new mxDouble[dataLength / 2];
	mwResult1.GetData(result, dataLength / 2);


	mxDouble* result2 = new mxDouble[dataLength / 2];
	mwResult2.GetData(result2, dataLength / 2);

	double ap = 0;
	for (size_t i = 0; i < dataLength / 2; i++)
	{
		if (result[i] == 1000)
		{
			ap = result2[i];
		}
	}*/

	/*std::ofstream outfile;
	outfile.open("D:\\TestRecord\\outdata.csv", std::ios::out | std::ios::trunc);
	if (!outfile.is_open())
	{
		MessageBox(_T("data.csv文件打开失败"), _T("失败！"));
	}
	
	
	int* dat1 = new int[dataLength]{0};
	for (size_t i = 0; i < dataLength; i++)
	{
		dat1[i] = readArrary[i]*1000000000;
	}
	for (size_t i = 0; i < dataLength; i++)
	{
		outfile << dat1[i] << std::endl;

	}
	outfile.close();
	char* fileName1 = "D:\\TestRecord\\ni.wav";
	wavwrite(fileName1, 1, 44100, 32, (char*)dat1, dataLength * 4);*/
	double ap = getAp<double>(readArrary, dataLength);
	vp = ap / 1.414;//0.043622930221678589

}

/******************采样点数需要大于1秒的采样频率点数*****************************/
template<typename T1>
double CCalibInput::getAp(T1* data, int len, int nfs, double mubiaofs)
{
	
	double ap = 0.0;

	int m_fs = nfs;
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
		
		in[i][0] = ((T1*)(data))[i];//虚数的实部
		//in[i][0] = 0.001*sin(2 * 3.1415926 * 1000 * i / m_fs);
		in[i][1] = 0;//虚数的虚部
	}

	p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(p);

	double real = 0.0;
	double vitru = 0.0;
	double* fs = new double[len];
	double* Ap = new double[len];


	Ap[0] = sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]) / len;
	for (int i = 1; i < len; i++)
	{
		Ap[i] = 2 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / len;
	}

	for (int i = 0; i < len; i++)
	{
		fs[i] = (1.0*i*m_fs) / len;

		if (fs[i] == mubiaofs)
		{
			ap = Ap[i];
			break;
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

double CCalibInput::getApzcj(int* data, int len, int nfs, double mubiaofs )
{
	double ap = 0.0;

	double* fftdata = new double[len];
	for (size_t i = 0; i < len; i++)
	{
		
		fftdata[i] = data[i];
		//fftdata[i] = 12345678*sin(2 * 3.1415926 * 1000 * i / nfs);
	}

	mwArray mwN(1, 1, mxINT32_CLASS);
	mwN(1, 1) = len;
	mwArray mwfs(1, 1, mxDOUBLE_CLASS);
	mwfs(1, 1) = nfs;
	mwArray mwResult1(1, len, mxDOUBLE_CLASS);
	mwArray mwResult2(1, len, mxDOUBLE_CLASS);

	mwArray mwin(1, len, mxDOUBLE_CLASS);

	mwin.SetData(fftdata, len);


	fftzcj(2, mwResult1, mwResult2, mwin);

	mxDouble* result = new mxDouble[len / 2];
	mwResult1.GetData(result, len / 2);


	mxDouble* result2 = new mxDouble[len / 2];
	mwResult2.GetData(result2, len / 2);

	double max = 0;
	for (size_t i = 0; i < len / 2; i++)
	{

		/*if (abs(result[i]- mubiaofs)<=10)
		{
			ap = result2[i];
			break;
		}*/

		if (result2[i]>max/*&&abs(result[i] - mubiaofs) <= 1*/)
		{
			max = result2[i];
		}

	}

	ap = max;

	delete[] result;
	delete[] result2;


	return ap;
}

double CCalibInput::getApzcjdouble(double* data, int len, int nfs, double mubiaofs)
{
	double ap = 0.0;

	double* fftdata = new double[len];
	for (size_t i = 0; i < len; i++)
	{

		fftdata[i] = data[i];
		//fftdata[i] = 12345678*sin(2 * 3.1415926 * 1000 * i / nfs);
	}

	mwArray mwN(1, 1, mxINT32_CLASS);
	mwN(1, 1) = len;
	mwArray mwfs(1, 1, mxDOUBLE_CLASS);
	mwfs(1, 1) = nfs;
	mwArray mwResult1(1, len, mxDOUBLE_CLASS);
	mwArray mwResult2(1, len, mxDOUBLE_CLASS);

	mwArray mwin(1, len, mxDOUBLE_CLASS);

	mwin.SetData(fftdata, len);


	fftzcj(2, mwResult1, mwResult2, mwin);

	mxDouble* result = new mxDouble[len / 2];
	mwResult1.GetData(result, len / 2);


	mxDouble* result2 = new mxDouble[len / 2];
	mwResult2.GetData(result2, len / 2);

	double max = 0;
	for (size_t i = 0; i < len / 2; i++)
	{

		//if (abs(result[i] - mubiaofs) <= 10)
		//{
		//	ap = result2[i];
		//	break;
		//}
		if (result2[i]>max/*&&abs(result[i] - mubiaofs) <= 1*/)
		{
			max = result2[i];
		}
		

	}

	ap = max;

	delete[] result;
	delete[] result2;


	return ap;
}


char* CCalibInput::getcharp(CString str)
{

	int len1 = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char* ch = new char[len1 + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, len1, NULL, NULL);
	return ch;
}

void  CCalibInput::OnOK()
{
	return;

}

bool CCalibInput::isdupname(CString addname)
{
	bool isdup = false;

	HTREEITEM child = m_tree.GetChildItem(hroot);

	while (child!=NULL)
	{
		CString name=m_tree.GetItemText(child);
		if (name == addname)
			isdup = true;

		child=m_tree.GetNextSiblingItem(child);

	}


	return isdup;

}


void CCalibInput::OnBnClickedButtonDelInstru()
{
	// TODO:  在此添加控件通知处理程序代码
	CString s = m_tree.GetItemText(hItemdel);
	if (s.Compare(CString("输入通道1"))==0)
	{
		return;
	}
	if (s.Compare(CString("输入通道2")) == 0)
	{
		return;
	}
	if (s.Compare(CString("")) == 0)
	{
		return;
	}
	
	
	
	for (size_t i = 0; i < deldat.size(); i++)
	{
		m_tree.DeleteItem(deldat[i]);
	}
	
	if (deldat.size() == 0)
	{
		m_tree.DeleteItem(hItemdel);
	}
	else
	{
		deldat.clear();
	}

		

	getmaxdeviceId();
			
		
}


void CCalibInput::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码

	CTreeCtrl* ctreectrl = (CTreeCtrl*)GetDlgItem(IDC_TREE1);

	CPoint pt = GetCurrentMessage()->pt;

	ctreectrl->ScreenToClient(&pt);

	UINT uFlags = 0;

	HTREEITEM hItem = ctreectrl->HitTest(pt, &uFlags);

	if (hItem != NULL && (TVHT_ONITEM&uFlags))
	{
		ctreectrl->SelectItem(hItem);

		int nDat = ctreectrl->GetItemData(hItem);
		
		if (iskeysheetf)
		{
			deldat.clear();
			hItemdel2 = hItem;

			HTREEITEM child = m_tree.GetChildItem(hroot);
			bool istag = false;
			while (child != NULL)
			{
				if (m_tree.GetItemText(child) == m_tree.GetItemText(hItemdel))
					istag = true;

				

				if (istag)
				{
					m_tree.SetItemState(child, TVIS_SELECTED, TVIS_SELECTED);
					deldat.push_back(child);
				}

				if (m_tree.GetItemText(child) == m_tree.GetItemText(hItemdel2))
					istag = false;




				child = m_tree.GetNextSiblingItem(child);
			}

			iskeysheetf = false;


		}else
		   hItemdel = hItem;


		


	}
else
{
	//遍历需要删除的节点集合，把需要删除的节点设置为选中状态
	if (deldat.size() > 1)
	{
		HTREEITEM child = m_tree.GetChildItem(hroot);
		while (child != NULL)
		{

			m_tree.SetItemState(child, 0, TVIS_SELECTED);

			child = m_tree.GetNextSiblingItem(child);
		}
	}
	

}


	*pResult = 0;
}


void CCalibInput::OnNMKillfocusTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	if (deldat.size() > 1)
	{
		HTREEITEM child = m_tree.GetChildItem(hroot);
		while (child != NULL)
		{

			m_tree.SetItemState(child, 0, TVIS_SELECTED);

			child = m_tree.GetNextSiblingItem(child);
		}
	}
	
	if (iskeysheetf)
		return;
	CString title;
	CWnd* wnd = m_tree.GetFocus();
	if (wnd)
	{
		wnd->GetWindowTextW(title);
		if (title != ""&&title.Compare(CString("删除仪器")) == 0)
		{
			return;
		}

	}

	hItemdel = NULL;
	*pResult = 0;
}

//实现shift功能
void CCalibInput::OnTvnKeydownTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码



	if (pTVKeyDown->wVKey == VK_SHIFT)
	{
		
		
		iskeysheetf = true;//防止killfous报错
	}


	*pResult = 0;
}


void CCalibInput::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	//NIDaqStartAi();
}


void CCalibInput::cal_Vp(int fs, CString decname, double& vp, int playchs, int recordchs, double outvp)
{
	/*CInputDialg inputdlg;
	inputdlg.setCaption(CString("输入电压值："));
	inputdlg.setunit(_T("VRms"));
	inputdlg.DoModal();*/
	invrms = 0.1;// inputdlg.getV();

	int** dat = NULL;
	double*dat2 = NULL;
	int recordtime =3;//秒
	devicemanage.calibrationMic(decname, &dat, fs, recordtime, &dat2, playchs, recordchs, outvp);
	double vpinner = 0;
	if (dat&&*dat)
	{

		double ap = 0;//电压大小

		char* driver = NULL;;
		devicemanage.getDriverType(decname, &driver);
		if (strcmp(driver, "ASIO") == 0)
		{
			char* fileName1 = "D:\\TestRecord\\mic01.wav";
			wavwrite(fileName1, 1, fs, 32, (char*)dat[0], fs * 5 * 4);

			ap = getApzcj(dat[0], _msize(dat[0]) / 4/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap /= pow(2, 31)*1.0;
			ap /= sqrt(2)*1.0;
			int intap = ap * 10000;
			vpinner = 1.0*intap / 10000.0;
		}
		else
		{
			ap = getApzcjdouble(dat2, _msize(dat2) / 8/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap /= sqrt(2)*1.0;
			int intap = ap * 10000;
			vpinner = 1.0*intap / 10000.0;


		}

		if (vpinner == 0)
		{
			inVP = 0;
			//UpdateData(FALSE);
			AfxMessageBox(_T("校准失败！"));
			delete[] dat[0];
			delete[] dat;
			delete[] dat2;
			//GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("开始校准"));
			return;
		}

		inVP = invrms / vpinner;

		int intap = inVP * 100;
		inVP = 1.0*intap / 100.0;

		//devicemanage.setDeviceVpVal(devicename, inVP, true);

		//UpdateData(FALSE);


		//清楚录音数据
		delete[] dat[0];
		delete[] dat;
		delete[] dat2;

		vp = inVP;


	}

}


void CCalibInput::OnBnClickedButtonVpCalibration()
{
	// TODO:  在此添加控件通知处理程序代码

	//AfxMessageBox(_T("请确认填入实际的电压输入值...."));
	UpdateData();
	

	/*************************错误处理********************************/
	if (devicename==""||!devicemanage.checkhasinputname(devicename))
	{
		AfxMessageBox(_T("设备管理中不存在此设备标识符！"));
		return;
	}
	//判断是否重名
	if (isdupname(devicename))
	{
		AfxMessageBox(_T("设备标识符重名！"));
		return;
	}
	/***********************错误处理**************************************/

	CInputDialg inputdlg(false);
	inputdlg.setCaption(CString("输入电压值："));
	inputdlg.DoModal();
	invrms = inputdlg.getV();

	GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("校准中"));
	char buf[10];
	sprintf(buf, "设备%d.cfg", ++maxdeviceid);
	CString decname = CString(buf);
	if (devicename != "")
	{
		decname = devicename;
		maxdeviceid--;
	}
	m_tree.InsertItem(decname, 1, 1, hroot, TVI_LAST);
	m_tree.Expand(hroot, TVE_EXPAND);

	int** dat = NULL;
	double*dat2 = NULL;
	int fs = 44100;
	fs = devicemanage.getfs(decname);
	int recordtime = 5;//秒
	devicemanage.calibrationMic(decname, &dat, fs, recordtime, &dat2,0,0,0);
	double vpinner = 0;
	if (dat&&*dat)
	{

		double ap = 0;//电压大小

		char* driver = NULL;;
		devicemanage.getDriverType(devicename, &driver);
		if (strcmp( driver,"ASIO")==0)
		{
			char* fileName1 = "D:\\TestRecord\\mic01.wav";
			wavwrite(fileName1, 1, fs, 32, (char*)dat[0], fs*10 * 4);

			ap = getApzcj(dat[0], _msize(dat[0])/4/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap /= pow(2, 31)*1.0;
			ap /= sqrt(2)*1.0;
			int intap = ap * 10000;
			vpinner = 1.0*intap / 10000.0;
		}
		else
		{
			ap = getApzcjdouble(dat2, _msize(dat2) / 8/*fs*recordtime*/, fs, calibrfs);//电压大小
			ap /= sqrt(2)*1.0;
			int intap = ap * 10000;
			vpinner = 1.0*intap / 10000.0;
			

		}

		if (vpinner == 0)
		{
			inVP = 0;
			UpdateData(FALSE);
			AfxMessageBox(_T("校准失败！"));
			delete[] dat[0];
			delete[] dat;
			delete[] dat2;
			GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("开始校准"));
			return;
		}
		   
		inVP=invrms / vpinner;

		int intap = inVP * 100;
		inVP = 1.0*intap / 100.0;

		//devicemanage.setDeviceVpVal(devicename, inVP, true);

		UpdateData(FALSE);

		AfxMessageBox(_T("校准完成！"));

		//清楚录音数据
		delete[] dat[0];
		delete[] dat;
		delete[] dat2;
		

	}
	GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("开始校准"));

}


//void CCalibInput::OnCbnDropdowncalibrainputdevices()
//{
//	// TODO:  在此添加控件通知处理程序代码
//
//	vector<CString> temp = devicemanage.getInnerNames_InCHS();
//
//		m_cmbinnerDevices.ResetContent();
//
//		for (size_t j = 0; j < temp.size(); j++)
//		{
//			m_cmbinnerDevices.InsertString(j, temp[j]);
//		}
//	
//}


void CCalibInput::OnEnChangeeditcalbraininputval()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

UpdateData();
	
}


void CCalibInput::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCalibInput::drawGroupBox(CWnd* wnd, CPaintDC* dc)
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


void CCalibInput::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICvp);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICcalindel);
	drawGroupBox(pwnd, &dc);
	
}


void CCalibInput::OnBnClickedButtonUpdateVp()
{
	// TODO:  在此添加控件通知处理程序代码
	if (devicename == "")
	{
		AfxMessageBox(_T("更新通道名称不存在在！！"));
		return;
	}
	devicemanage.setDeviceVpVal(devicename, inVP, true);

	UpdateData(FALSE);
	AfxMessageBox(_T("VP值更新完成！"));
}


void CCalibInput::OnCbnDropdowncalibrainputdevices()
{
	// TODO:  在此添加控件通知处理程序代码

	m_cmbinnerDevices.ResetContent();
	if (devicemanage.m_hWnd)
	{
		vector<CString> innernames = devicemanage.getInnerNames_InCHS();


		for (size_t i = 0; i < 1; i++)//innernames.size()
		{
			m_cmbinnerDevices.InsertString(i, innernames[i]);
		}
		m_cmbinnerDevices.SetCurSel(0);
	}
}
