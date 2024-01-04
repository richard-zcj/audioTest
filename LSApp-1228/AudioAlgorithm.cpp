

/***********************spk************************
Sensitivity
：（定频）1khz   电压设置   计算到的fr就是Sensitivity


SNR(信噪比)：（rms）
（定频）1khz   电压设置(两个电压0v1 空侧；设置电压v2再测试一次)
两次测试的电压值v 10 * lg(v2 / v1)


Polarity(极性)：
（定频）1khz   电压设置 基波的相位角如果在0 - 90度 极性为正 其他角度为负


Balance(平衡度)：（测试两个耳机的fr）
（扫频信号）一副耳机 左耳fr值减右耳的fr值 就是Balance



******************mic**************************
Sensitivity

1khz 94db  分析到的fr就是Sensitivity

Noise Reduction（降噪性能）：
测试耳机的降噪开关，打开和关闭状态下的fr之差
打开之前fr - 打开之后的fr
*******************************************************/

#include "stdafx.h"
#include "AudioAlgorithm.h"

#include "LSAppView.h" //全局对象头文件
#include <fstream>
#include "TestBorderLineSettings.h"

#include "CalibInput.h"
#include "wav.h"


#define SAVEALG_FILE 1

/*******************以下外部引用可以优化到当前类的成员中，通过外部设置********************************/
extern JiLiSetNewVersion jilisetnew;//激励设置界面
extern CTestBorderLineSettings testborderline;//测试框线
extern CAlogriSettins  alogriSettins;
extern CCalibration calibration;

CAudioAlgorithm::CAudioAlgorithm() :driverMode(0), m_bNorse(false), m_bfr_v(false)
{
#if AUDIOALG_START
	/*bool b_ok = FrHarmInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("FrHarmInitialize err"));

	}*/
#endif

	
	jilisetnew.LoadSenoFSpk();
	jilisetnew.LoadSenoFMic();
	m_bSaveSrcFile = ReadBool(_T("Common"), _T("SaveInitFile"), false, GetConfigIniFile());
}


CAudioAlgorithm::~CAudioAlgorithm()
{
}

vector<double>* CAudioAlgorithm::getpindiandata()
{
	if (jilisetnew.inuse_pindians.size() == 0)
	{
		//AfxMessageBox(_T("频点数错误！"));
		return &jilisetnew.inuse_pindians;
	}
	return &jilisetnew.inuse_pindians;
}

void  CAudioAlgorithm::setFrMode(bool isV)
{
	m_bfr_v = isV;
}

void CAudioAlgorithm::Alg_Fr(double* response, double fs)
{
	if (response == NULL)
		return;
	/*bool b_ok = FrHarmInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("FrHarmInitialize err"));

	}*/

	if (jilisetnew.data.size() > 0)
	{
		data = &jilisetnew.data;

	}
	else
	{
		AfxMessageBox(_T("激励信号未确认！"));
		return;
	}



	int len = data->size();

	int recordlen = _msize(response) / sizeof(double);
	if (abs(len - recordlen) > fs)
	{
		//if (AfxMessageBox(_T("录音点数与播放点数不同,确认计算吗?"), MB_OKCANCEL) != IDOK)
		return;

		/*AfxMessageBox(_T("录音点数与播放点数不同,确认计算吗?");
		return;*/
	}
	int buling = fs*0.05;
	len -= 2 * buling;
	int retlen = (int)fs / ((int)jilisetnew.inuse_pindians[0] * 2) - 1;
	mwArray out1(jilisetnew.pos.size(), 1, mxDOUBLE_CLASS);
	mwArray out2(jilisetnew.pos.size()*retlen, 1, mxDOUBLE_CLASS);
	mwArray out3(jilisetnew.pos.size(), 1, mxDOUBLE_CLASS);
	mwArray out4(jilisetnew.pos.size()*retlen, 1, mxDOUBLE_CLASS);
	mwArray out5(1, 1, mxDOUBLE_CLASS);
	mwArray mwStim(len, 1, mxDOUBLE_CLASS);
	mwArray mwResponse(recordlen, 1, mxDOUBLE_CLASS);
	double *stim = new double[len];
	double *resp = new double[recordlen];

#if SAVEALG_FILE
	std::ofstream outfile1;
	if (m_bSaveSrcFile)
	{
		CString path1("D:\\TestRecord\\stim.txt");
		outfile1.open(path1, std::ios::out | std::ios::trunc);
	}
#endif
	double ap = 0.0;
	double vp = m_vp_in;// 10;
	double outvp = m_vp_out;//1.45
	int intap = 0;
	for (size_t i = 0 + buling; i < buling + len; i++)
	{
		ap = data->at(i);
		//if (driverMode==0)
		ap /= pow(2, 31)*1.0;
		//ap /= sqrt(2)*1.0;
		ap *= outvp;
		//intap = ap ;
		//ap = 1.0*intap;
		//stim[i - buling] = data->at(i);//根据输出vp转化为实际的电压有效值
		stim[i - buling] = ap;
#if SAVEALG_FILE
		if (m_bSaveSrcFile)
		{
			outfile1 << ap << endl;
		}
#endif


	}
#if SAVEALG_FILE
	if (m_bSaveSrcFile)
	{
		outfile1.close();
	}
#endif
	mwStim.SetData(stim, len);
	/*char* fileName1 = "D:\\TestRecord\\not0stim.wav";
	wavwrite(fileName1, 1, fs, 32, (char*)stim, _msize(stim));*/

#if SAVEALG_FILE
	std::ofstream outfile3;
	if (m_bSaveSrcFile)
	{
		CString path3("D:\\TestRecord\\response.txt");
		outfile3.open(path3, std::ios::out | std::ios::trunc);
	}
#endif
	bool isemptyrespose = true;
	for (size_t i = 0; i < recordlen; i++)
	{
		ap = response[i];

		if (abs(ap - 0) > 0.0001)
		{
			isemptyrespose = false;
		}

		if (driverMode == 0)
		{
			ap /= pow(2, 31)*1.0;
			//ap /= sqrt(2)*1.0;

		}
		ap *= vp;
		//intap = ap ;
		//ap = 1.0*intap;
		//resp[i] = 1.0*response[i];
		resp[i] = ap;
#if SAVEALG_FILE
		if (m_bSaveSrcFile)
		{
			outfile3 << ap << endl;
		}
#endif
	}
#if SAVEALG_FILE
	if (m_bSaveSrcFile)
	{
		outfile3.close();
	}
#endif
	mwResponse.SetData(resp, recordlen);

	/*char* fileName1 = "D:\\TestRecord\\resp.wav";
	wavwrite(fileName1, 1, fs, 32, (char*)resp, _msize(resp));*/

	if (isemptyrespose)
	{
		//AfxMessageBox(_T("录音数据为空\n检查设备通道是否配置正确..."));
		return;
	}

	mwArray mwFs(1, 1, mxDOUBLE_CLASS);
	mwArray mwbNorse(1, 1, mxDOUBLE_CLASS);
	mwArray mwbRMS(1, 1, mxDOUBLE_CLASS);
	if (m_bNorse)
	{
		mwbNorse(1, 1) = 1;
	}
	else
	{
		mwbNorse(1, 1) = 0;
	}
	if (m_bSNR)
	{
		mwbRMS(1, 1) = 1;
	}
	else
	{
		mwbRMS(1, 1) = 0;
	}
	mwFs(1, 1) = fs;
	int l = 0;
	int i = 1;
#if SAVEALG_FILE
	std::ofstream outfile;
	if (m_bSaveSrcFile)
	{
		CString path("D:\\TestRecord\\pos.txt");
		outfile.open(path, std::ios::out | std::ios::trunc);
	}
#endif
	mwArray mwPosition(jilisetnew.pos.size(), 3, mxDOUBLE_CLASS);
	int flag_senOffr = 0;
	int sen_i = 0;
	vector<double> fsList;
	for (std::map<double, int>::iterator it = jilisetnew.pos.begin(); it != jilisetnew.pos.end(); it++)
	{
		mwPosition(i, 1) = (*it).first;
		fsList.push_back((*it).first);
		mwPosition(i, 2) = l + 1;

		mwPosition(i, 3) = l + (*it).second;

		i++;
#if SAVEALG_FILE
		if (m_bSaveSrcFile)
		{
			outfile << (*it).first << "  " << l + 1 << "  " << l + (*it).second << endl;
		}
#endif
		l += (*it).second;

		if ((*it).first == 1000)
		{
			flag_senOffr = sen_i;
		}
		sen_i++;

	}
#if SAVEALG_FILE
	if (m_bSaveSrcFile)
	{
		outfile.close();
	}
#endif

	FrHarm(5, out1, out2, out3, out4, out5, mwStim, mwResponse, mwFs, mwPosition, mwbNorse, mwbRMS);
	//FrHarm(3, out1, out2, out3, mwStim, mwStim, mwFs, mwPosition, mwbNorse);
	mxDouble* result = new mxDouble[jilisetnew.pos.size()];

	out1.GetData(result, jilisetnew.pos.size());

	out5.GetData(&m_snr, 1);
#if SAVEALG_FILE
	std::ofstream outfilefr;
	if (m_bSaveSrcFile)
	{
		CString pathfr("D:\\TestRecord\\fr.txt");
		outfilefr.open(pathfr, std::ios::out | std::ios::trunc);

		for (size_t i = 0; i < jilisetnew.pos.size(); i++)
		{
			outfilefr << result[i] << endl;
		}
		outfilefr.close();
	}
#endif
	/*double ap = 0.0;
	double vp = 7.143;
	int intap = 0;*/
	//double micsen = 2.78e-6;// wdm:0.114;// asio:0.124;
	//micsen = calibration.getinputSens()/1000.0;
	sen_i = 0;
	vector<double>* senOfspk = NULL;
	senOfspk = jilisetnew.getsenofSpk(m_algParam.recordIndex, fsList);
	for (size_t i = 0; i < jilisetnew.pos.size(); i++)
	{

		ap = result[i];
		//ap /= sqrt(2)*1.0;
		/*ap /= pow(2, 31)*1.0;
		ap /= sqrt(2)*1.0;
		ap *= vp;
		intap = ap * 10000;
		ap = 1.0*intap / 10000.0;*/

		if (flag_senOffr == sen_i&&!m_bfr_v)
		{
			m_micSensitivity = 20.0* log10(1.0*ap / senOfspk->at(i)) + 94;
		}
		sen_i++;
		if (m_bfr_v)
		{
			fr_data.push_back(ap);//校准spk使用
		}
		else
		{
			fr_data.push_back(20.0* log10(1.0*ap / senOfspk->at(i)) + 94);//实际测试使用
			
		}

	}

	if (m_bfr_v)
		return;//返回，只计算fr

	/*std::vector<double> data1;
	data1.clear();
	for (int h = 0; h < jilisetnew.pos.size(); h++)
	{
	data1.push_back(result[h]);
	}*/

	//int retlen = (int)fs / ((int)jilisetnew.inuse_pindians[0] * 2);
	mxDouble* result2 = new mxDouble[jilisetnew.pos.size()*retlen];
	size_t count = out2.NumberOfElements();
	out2.GetData(result2, jilisetnew.pos.size()*retlen);

#if SAVEALG_FILE
	std::ofstream outfilethd;
	if (m_bSaveSrcFile)
	{
		CString paththd("D:\\TestRecord\\thd.txt");
		outfilethd.open(paththd, std::ios::out | std::ios::trunc);
		for (size_t i = 0; i < count; i++)
		{
			outfilethd << result2[i] << endl;
		}
		outfilethd.close();
	}
#endif
	//计算thd
	ThdData thdalogriset = alogriSettins.getThdData();

	CaculateTHD(thdalogriset, retlen, result2, result);

	//根据mic灵敏度计算出实际的声压值  result/sen   result*1000/mv/Pa
	//实际显示的db值


	/****************************RB计算************************************/
	CaculateRB(thdalogriset, result2, result, retlen);


	/*************************************计算thd+n**********************************************/
	mxDouble* result3 = new mxDouble[jilisetnew.pos.size()];
	out3.GetData(result3, jilisetnew.pos.size());

	CaculateTHD_N(result3, result);

	mxDouble* result4 = new mxDouble[jilisetnew.pos.size()*retlen];
	out4.GetData(result4, jilisetnew.pos.size()*retlen);
	CaculatePhase(result4, retlen);

	delete[] stim;
	delete[] resp;
	delete[] result;
	delete[] result2;
	delete[] result3;
	delete[] result4;

	//FrHarmTerminate();




}

void CAudioAlgorithm::Alg_Fr(double* response, double fs, prParam* pPlayRecord, int slotId)
{
	if (response == NULL)
		return;
	/*bool b_ok = FrHarmInitialize();

	if (!b_ok)
	{
		AfxMessageBox(_T("FrHarmInitialize err"));

	}*/
	
	if (pPlayRecord->vDatas.size() > 0)
	{
		data = &pPlayRecord->vDatas;

	}
	else
	{
		AfxMessageBox(_T("激励信号未确认！"));
		return;
	}

	

	int len = data->size();
	
	int recordlen = _msize(response)/sizeof(double);
	if (abs(len -recordlen)>2*fs)
	{
		//if(AfxMessageBox(_T("录音点数与播放点数不同,确认计算吗?"),MB_OKCANCEL)!=IDOK)
                  return;

		/*AfxMessageBox(_T("录音点数与播放点数不同,确认计算吗?");
		return;*/
	}
	int buling = fs*0.05;
	len -= 2*buling;
	//int retlen = (int)fs / ((int)jilisetnew.inuse_pindians[0] * 2) - 1;
	int retlen = (int)fs / ((int)pPlayRecord->vPoints[0] * 2) - 1;
	mwArray out1(pPlayRecord->vPointPos.size(), 1, mxDOUBLE_CLASS);
	mwArray out2(pPlayRecord->vPointPos.size() * retlen, 1, mxDOUBLE_CLASS);
	mwArray out3(pPlayRecord->vPointPos.size(), 1, mxDOUBLE_CLASS);
	mwArray out4(pPlayRecord->vPointPos.size() * retlen, 1, mxDOUBLE_CLASS);
	mwArray out5(1, 1, mxDOUBLE_CLASS);
	mwArray mwStim(len, 1, mxDOUBLE_CLASS);
	mwArray mwResponse(recordlen, 1, mxDOUBLE_CLASS);
	double *stim = new double[len];
	double *resp = new double[recordlen];
#if SAVEALG_FILE
	std::ofstream outfile1;
	if (m_bSaveSrcFile)
	{
		CString path1("D:\\TestRecord\\stim.txt");
		outfile1.open(path1, std::ios::out | std::ios::trunc);
	}
#endif
	double ap = 0.0;
	double vp = m_vp_in;// 10;
	double outvp = m_vp_out;//1.45
	int intap = 0;
	for (size_t i = 0 + buling; i < buling+len; i++)
	{
		ap = data->at(i);
		//if (driverMode==0)
		     ap /= pow(2, 31)*1.0;
		//ap /= sqrt(2)*1.0;
		ap *= outvp;
		//intap = ap ;
		//ap = 1.0*intap;
		//stim[i - buling] = data->at(i);//根据输出vp转化为实际的电压有效值
		stim[i - buling] = ap;
#if SAVEALG_FILE
		if (m_bSaveSrcFile)
		{
			outfile1 << ap << endl;
		}
#endif
		

	}
#if SAVEALG_FILE
	if (m_bSaveSrcFile)
	{
		outfile1.close();
	}
#endif
	mwStim.SetData(stim, len);
	/*char* fileName1 = "D:\\TestRecord\\not0stim.wav";
	wavwrite(fileName1, 1, fs, 32, (char*)stim, _msize(stim));*/

#if SAVEALG_FILE
	std::ofstream outfile3;
	if (m_bSaveSrcFile)
	{
		CString path3("D:\\TestRecord\\response.txt");
		outfile3.open(path3, std::ios::out | std::ios::trunc);
	}
#endif
	bool isemptyrespose = true;


	int cut = 0;
	if (pPlayRecord->nMode == 0)
		cut=ReadInt(_T("AlgConf"), _T("Cut"), 0, GetConfigIniFile());
	
	for (size_t i = cut; i < recordlen; i++)
	{
		ap = response[i];

		if (abs(ap - 0) > 0.0001)
		{
			isemptyrespose = false;
		}
		
		if (driverMode == 0)
		{
			ap /= pow(2, 31)*1.0;
			//ap /= sqrt(2)*1.0;
			
		}
		ap *= vp;
		//intap = ap ;
		//ap = 1.0*intap;
		//resp[i] = 1.0*response[i];
		resp[i] = ap;
#if SAVEALG_FILE
		if (m_bSaveSrcFile)
		{
			outfile3 << ap << endl;
		}
#endif
	}
#if SAVEALG_FILE
	if (m_bSaveSrcFile)
	{
		outfile3.close();
	}
#endif
	mwResponse.SetData(resp, recordlen);

	/*char* fileName1 = "D:\\TestRecord\\resp.wav";
	wavwrite(fileName1, 1, fs, 32, (char*)resp, _msize(resp));*/

	if (isemptyrespose)
	{
		//AfxMessageBox(_T("录音数据为空\n检查设备通道是否配置正确..."));
		return;
	}

	mwArray mwFs(1, 1, mxDOUBLE_CLASS);
	mwArray mwbNorse(1, 1, mxDOUBLE_CLASS);
	mwArray mwbRMS(1, 1, mxDOUBLE_CLASS);
	if (m_bNorse)
	{
		mwbNorse(1, 1) = 1;
	}
	else
	{
		mwbNorse(1, 1) = 0;
	}
	if (m_bSNR)
	{
		mwbRMS(1, 1) = 1;
	}
	else
	{
		mwbRMS(1, 1) = 0;
	}
	mwFs(1, 1) = fs;
	int l = 0;
	int i = 1;

#if SAVEALG_FILE
	std::ofstream outfile;
	if (m_bSaveSrcFile)
	{
		CString path("D:\\TestRecord\\pos.txt");
		outfile.open(path, std::ios::out | std::ios::trunc);
	}
#endif
	mwArray mwPosition(pPlayRecord->vPointPos.size(), 3, mxDOUBLE_CLASS);
	int flag_senOffr = 0;
	
	/*
	    mwPosition(i, 1) = (*it).first;
		mwPosition(i, 2) = l + 1;

		mwPosition(i, 3) = l + (*it).second;
	*/

	for (int n = 0; n < pPlayRecord->vPointPos.size(); n++)
	{
		mwPosition(i, 1) = pPlayRecord->vPoints[n];
		mwPosition(i, 2) = l + 1;

		mwPosition(i, 3) = l + pPlayRecord->vPointPos[n];

		i++;
#if SAVEALG_FILE
		if (m_bSaveSrcFile)
		{
			outfile << pPlayRecord->vPoints[n] << "  " << l + 1 << "  " << l + pPlayRecord->vPointPos[n] << endl;
		}
#endif
		l += pPlayRecord->vPointPos[n];

		if (pPlayRecord->vPoints[n] == 1000)
		{
			flag_senOffr =n;
		}
		
	}
	/*for (std::map<double, int>::iterator it = jilisetnew.pos.begin(); it != jilisetnew.pos.end(); it++)
	{
		mwPosition(i, 1) = (*it).first;
		mwPosition(i, 2) = l + 1;

		mwPosition(i, 3) = l + (*it).second;

		i++;
		outfile << (*it).first << "  " << l + 1 << "  " << l + (*it).second << endl;
		l += (*it).second;

		if ((*it).first == 1000)
		{
			flag_senOffr = sen_i;
		}
		sen_i++;

	}*/

#if SAVEALG_FILE
	if (m_bSaveSrcFile)
	{
		outfile.close();
	}
#endif

	FrHarm(5, out1, out2, out3, out4, out5, mwStim, mwResponse, mwFs, mwPosition, mwbNorse, mwbRMS);
	//FrHarm(3, out1, out2, out3, mwStim, mwStim, mwFs, mwPosition, mwbNorse);
	mxDouble* result = new mxDouble[pPlayRecord->vPointPos.size()];

	out1.GetData(result, pPlayRecord->vPointPos.size());

	out5.GetData(&m_snr,1);

#if SAVEALG_FILE
	std::ofstream outfilefr;
	if (m_bSaveSrcFile)
	{
		CString pathfr("D:\\TestRecord\\fr.txt");
		outfilefr.open(pathfr, std::ios::out | std::ios::trunc);

		for (size_t i = 0; i < pPlayRecord->vPointPos.size(); i++)
		{
			outfilefr << result[i] << endl;
		}
		outfilefr.close();
	}
#endif
	/*double ap = 0.0;
	double vp = 7.143;
	int intap = 0;*/
	//double micsen = 2.78e-6;// wdm:0.114;// asio:0.124;
	//micsen = calibration.getinputSens()/1000.0;
	

	vector<double>* senOfspk = NULL;
	vector<double>* senOfmic = NULL;
	
	if (pPlayRecord->nMode == 1)
	{//MIC模式
		jilisetnew.LoadSenoFSpk();
		senOfspk = jilisetnew.getsenofSpk(slotId - 1, pPlayRecord->vPoints);
	}
	else
	{//spk 模式
		jilisetnew.LoadSenoFMic();
		senOfmic = jilisetnew.getsenofMic(slotId - 1, pPlayRecord->vPoints);
	}


	for (size_t i = 0; i < pPlayRecord->vPointPos.size(); i++)
	{
		fr_data.push_back(result[i]);
	}

	if (pPlayRecord->nMode == 0)//spk 模式平滑
	{
		if (ReadBool(_T("AlgConf"), _T("SoomthSPK"), false, GetConfigIniFile()))
		{
			Smooth(fr_data, pPlayRecord);
		}
	}
	else
	{

		if (ReadBool(_T("AlgConf"), _T("SoomthMIC"), false, GetConfigIniFile()))
		{
			Smooth(fr_data, pPlayRecord);
		}

	}
	
	
    

	for (size_t i = 0; i < pPlayRecord->vPointPos.size(); i++)
	{

		ap = fr_data[i];
		//ap /= sqrt(2)*1.0;
		/*ap /= pow(2, 31)*1.0;
		ap /= sqrt(2)*1.0;
		ap *= vp;
		intap = ap * 10000;
		ap = 1.0*intap / 10000.0;*/
		
		if (flag_senOffr == i&&pPlayRecord->nMode==1)
		{
			if (ReadBool(_T("AlgConf"), _T("MICOFFSET"), false, GetConfigIniFile()))
			{
				m_micSensitivity = 20.0* log10(1.0*ap) * senOfspk->at(i);

			}
			else
			{
				m_micSensitivity = 20.0* log10(1.0*ap);
			}
			
			
			
		}
		if (pPlayRecord->nMode == 0)
		{
			if (ReadBool(_T("AlgConf"), _T("SPKOFFSET"), false, GetConfigIniFile()))
			{
				fr_data[i] = ((20.0* log10(1000.0*ap / micsen) + 94)*senOfmic->at(i));//校准spk使用
			}
			else
			{
				fr_data[i]=(20.0* log10(1000.0*ap / micsen) + 94);//校准spk使用
			}
			
			
		}
		else
		{
			if (ReadBool(_T("AlgConf"), _T("MICOFFSET"), false, GetConfigIniFile()))
			{
				fr_data[i] = (20.0* log10(1.0*ap) * senOfspk->at(i));//实际测试使用
			}
			else
			{
				fr_data[i]=(20.0* log10(1.0*ap));//实际测试使用
			}
			
			
		}
		
	}


	if (pPlayRecord->nMode == 1)
	{
		senOfspk->clear();
		delete senOfspk;

		
	}
	else
	{
		senOfmic->clear();
		delete senOfmic;
	}

	
	
	/*std::vector<double> data1;
	data1.clear();
	for (int h = 0; h < jilisetnew.pos.size(); h++)
	{
		data1.push_back(result[h]);
	}*/

	//int retlen = (int)fs / ((int)jilisetnew.inuse_pindians[0] * 2);
	mxDouble* result2 = new mxDouble[pPlayRecord->vPointPos.size()*retlen];
	size_t count = out2.NumberOfElements();
	out2.GetData(result2, pPlayRecord->vPointPos.size()*retlen);
	
#if SAVEALG_FILE
	std::ofstream outfilethd;
	if (m_bSaveSrcFile)
	{
		CString paththd("D:\\TestRecord\\thd.txt");
		outfilethd.open(paththd, std::ios::out | std::ios::trunc);
		for (size_t i = 0; i < count; i++)
		{
			outfilethd << result2[i] << endl;
		}
		outfilethd.close();
	}
#endif

	//计算thd
	ThdData thdalogriset = alogriSettins.getThdData();

	CaculateTHD(thdalogriset, retlen, result2, result, pPlayRecord);

	/*if (ReadBool(_T("AlgConf"), _T("Soomth"), false, GetConfigIniFile()))
	{

		double step = 0.0;

		int nstep = ReadInt(_T("AlgConf"), _T("Step"), 1, GetConfigIniFile());

		if (nstep == 1)
		{
			step = 1;
		}
		else if (nstep == 3)
		{
			step = 1.0 / 3.0;
		}
		else if (nstep == 6)
		{
			step = 1.0 / 6.0;
		}
		else if (nstep == 12)
		{
			step = 1.0 / 12.0;
		}
		else if (nstep == 24)
		{
			step = 1.0 / 24.0;
		}
		else
		{
			step = 1;
		}

		vector<double> smoothFr;
		for (size_t i = 0; i < thd_data.size(); i++)
		{
			smoothFr.push_back(CalSmooth(pPlayRecord->vPoints[i], thd_data, step, pPlayRecord->vPoints));
		}

		thd_data.clear();
		for (size_t i = 0; i < smoothFr.size(); i++)
		{
			thd_data.push_back(smoothFr[i]);
		}
	}*/

	//根据mic灵敏度计算出实际的声压值  result/sen   result*1000/mv/Pa
	//实际显示的db值


	/****************************RB计算************************************/
	CaculateRB(thdalogriset, result2, result, retlen, pPlayRecord);
	/*if (ReadBool(_T("AlgConf"), _T("Soomth"), false, GetConfigIniFile()))
	{

		double step = 0.0;

		int nstep = ReadInt(_T("AlgConf"), _T("Step"), 1, GetConfigIniFile());

		if (nstep == 1)
		{
			step = 1;
		}
		else if (nstep == 3)
		{
			step = 1.0 / 3.0;
		}
		else if (nstep == 6)
		{
			step = 1.0 / 6.0;
		}
		else if (nstep == 12)
		{
			step = 1.0 / 12.0;
		}
		else if (nstep == 24)
		{
			step = 1.0 / 24.0;
		}
		else
		{
			step = 1;
		}

		vector<double> smoothFr;
		for (size_t i = 0; i < rb_data.size(); i++)
		{
			smoothFr.push_back(CalSmooth(pPlayRecord->vPoints[i], rb_data, step, pPlayRecord->vPoints));
		}

		rb_data.clear();
		for (size_t i = 0; i < smoothFr.size(); i++)
		{
			rb_data.push_back(smoothFr[i]);
		}
	}*/


	/*************************************计算thd+n**********************************************/
	mxDouble* result3 = new mxDouble[pPlayRecord->vPointPos.size()];
	out3.GetData(result3, pPlayRecord->vPointPos.size());

	CaculateTHD_N(result3, result);

	mxDouble* result4 = new mxDouble[pPlayRecord->vPointPos.size()*retlen];
	out4.GetData(result4, pPlayRecord->vPointPos.size()*retlen);
	CaculatePhase(result4, retlen);
	
	delete[] stim;
	delete[] resp;
	delete[] result;
	delete[] result2;
	delete[] result3;
	delete[] result4;

	//FrHarmTerminate();


	

}

void CAudioAlgorithm::Smooth(vector<double>& sm, prParam* pPlayRecord)
{
	//if (ReadBool(_T("AlgConf"), _T("Soomth"), false, GetConfigIniFile()))
	{

		double step = 0.0;

		int nstep = ReadInt(_T("AlgConf"), _T("Step"), 1, GetConfigIniFile());

		if (nstep == 1)
		{
			step = 1;
		}
		else if (nstep == 3)
		{
			step = 1.0 / 3.0;
		}
		else if (nstep == 6)
		{
			step = 1.0 / 6.0;
		}
		else if (nstep == 12)
		{
			step = 1.0 / 12.0;
		}
		else if (nstep == 24)
		{
			step = 1.0 / 24.0;
		}
		else
		{
			step = 1;
		}

		vector<double> smoothFr;
		for (size_t i = 0; i < sm.size(); i++)
		{
			smoothFr.push_back(CalSmooth(pPlayRecord->vPoints[i], sm, step, pPlayRecord->vPoints));
		}

		sm.clear();
		for (size_t i = 0; i < smoothFr.size(); i++)
		{
			sm.push_back(smoothFr[i]);
		}
	}
}

void CAudioAlgorithm::SmoothMIC(vector<double>& sm, prParam* pPlayRecord, int stp)
{
	//if (ReadBool(_T("AlgConf"), _T("Soomth"), false, GetConfigIniFile()))
	{

		double step = 0.0;

		int nstep = stp;

		if (nstep == 1)
		{
			step = 1;
		}
		else if (nstep == 3)
		{
			step = 1.0 / 3.0;
		}
		else if (nstep == 6)
		{
			step = 1.0 / 6.0;
		}
		else if (nstep == 12)
		{
			step = 1.0 / 12.0;
		}
		else if (nstep == 24)
		{
			step = 1.0 / 24.0;
		}
		else
		{
			step = 1;
		}

		vector<double> smoothFr;
		for (size_t i = 0; i < sm.size(); i++)
		{
			smoothFr.push_back(CalSmooth(pPlayRecord->vPoints[i], sm, step, pPlayRecord->vPoints));
		}

		sm.clear();
		for (size_t i = 0; i < smoothFr.size(); i++)
		{
			sm.push_back(smoothFr[i]);
		}
	}
}


void CAudioAlgorithm::setHmani(vector<int>* thd)
{
	thdhamni = thd;
}

void CAudioAlgorithm::setHmaniRB(vector<int>* rb)
{
	rbhamni = rb;
}
void CAudioAlgorithm::setIEEE(bool ieee)
{
	isIEEE = ieee;
}
void CAudioAlgorithm::CaculateTHD(ThdData& thdalogriset, int rowlem, mxDouble* result2, mxDouble* result, prParam* pPlayRecord)
{
	

	double td = 0;
	double temptd = 0;
	double thd_fr = 0;

	if (!isIEEE)
	{//IEC

		for (size_t j = 0; j < pPlayRecord->vPoints.size(); j++)
		{

			td = 0;
			for (size_t i = 0; i <thdhamni->size(); i++)
			{
				temptd = result2[j*rowlem + (thdhamni->at(i))];
				td += pow(temptd, 2);//每个频点的 谐波次数
			}
			td = sqrt(td);
			td_data.push_back(td);
			thd_fr = result[j];
			thd_data.push_back(td / sqrt(pow(thd_fr, 2) + pow(td, 2)) * 100);

		}

	}
	else
	{//IEEE
		for (size_t j = 0; j < pPlayRecord->vPoints.size(); j++)
		{

			td = 0;
			for (size_t i = 0; i <thdhamni->size(); i++)
			{
				td += pow(result2[j*rowlem + (thdhamni->at(i))], 2);//每个频点的 谐波次数
			}
			td = sqrt(td);
			td_data.push_back(td);
			thd_data.push_back(td / sqrt(pow(result[j], 2)) * 100);

		}

	}



	/**************************/

}

void CAudioAlgorithm::CaculateRB(ThdData&  thdalogriset, mxDouble* result2, mxDouble* result, int rowlem, prParam* pPlayRecord)
{
	RBdata rbalogriset = alogriSettins.getRbData();

	/*if (thdalogriset.getAlogrithm() ^ rbalogriset.getAlogrithm()
		&& (thdalogriset.getHamnics()->size()>0 && rbhamni->size()>0))
	{
		AfxMessageBox(_T("THD与RB计算标准不同！"));
	}
*/
	double rb = 0;

	if (!isIEEE)
	{//IEC

		for (size_t j = 0; j < pPlayRecord->vPoints.size(); j++)
		{

			rb = 0;
			for (size_t i = 0; i <rbhamni->size(); i++)
			{
				rb += pow(result2[j*rowlem + (rbhamni->at(i))], 2);//每个频点的 谐波次数 rb为取结果集中的高次谐波
			}
			rb = sqrt(rb);

			rb_data.push_back(rb / sqrt(pow(result[j], 2) + pow(rb, 2)) * 100);

		}

	}
	else
	{//IEEE
		for (size_t j = 0; j < pPlayRecord->vPoints.size(); j++)
		{

			rb = 0;
			for (size_t i = 0; i <rbhamni->size(); i++)
			{
				rb += pow(result2[j*rowlem + (rbhamni->at(i))], 2);//每个频点的 谐波次数
			}
			rb = sqrt(rb);
			rb_data.push_back(rb / sqrt(pow(result[j], 2)) * 100);

		}

	}
}

void CAudioAlgorithm::CaculateTHD_N(mxDouble* result3, mxDouble* result)
{
	

	std::ofstream outfilethdn;
	CString paththdn("D:\\TestRecord\\thdn.txt");
	outfilethdn.open(paththdn, std::ios::out | std::ios::trunc);
	for (size_t i = 0; i < jilisetnew.pos.size(); i++)
	{
		outfilethdn << result3[i] << endl;
	}
	outfilethdn.close();

	double thdn = 0;
	if (m_bNorse)
	{
		for (size_t i = 0; i < jilisetnew.pos.size(); i++)
		{
			thdn = sqrt(pow((td_data[i]), 2) + pow(result3[i], 2)) /
				sqrt(pow((td_data[i]), 2) + pow(result3[i], 2) + pow(result[i], 2));

			thdn_data.push_back(thdn * 100);


		}


	}
}

void CAudioAlgorithm::CaculatePhase(mxDouble* result4, int rowlem)
{

	phase_data.clear();
	double temppase;
	std::ofstream outfilephase;
	CString paththd("D:\\TestRecord\\phase.txt");
	outfilephase.open(paththd, std::ios::out | std::ios::trunc);
	for (size_t j = 0; j < jilisetnew.pos.size(); j++)
	{
		
		temppase = result4[j*rowlem];
		phase_data.push_back(temppase);
		outfilephase << temppase << endl;
		
		

		
	}


	

	outfilephase.close();


}

void CAudioAlgorithm::setDriverMode(int mode)
{
	driverMode = mode;
}

void CAudioAlgorithm::setDeviceInputVpval(double vp)
{
	m_vp_in = vp;
}

void CAudioAlgorithm::setDeviceOutputVpval(double vp)
{
	m_vp_out = vp;
}

void CAudioAlgorithm::saveData(CString testID, char* path, const vector<double>& fs, const vector<double>& data)
{
	if (!ReadBool(_T("Common"), _T("DebugMode"), false, GetConfigIniFile()))
		return;

	std::ofstream outfile1;
	CString path1(path);
	outfile1.open(path1, std::ios::out | std::ios::app);
	//outfile1.open(path1, std::ios::out | std::ios::trunc);
	outfile1 << ",";
	for (size_t i = 0; i < fs.size(); i++)
	{

		//outfile1 <<fs[i]<<"	";
		outfile1 << fs[i] << ",";
		

	}
	outfile1 << endl;
	USES_CONVERSION;
	outfile1 << T2A(testID) << ",";
	for (size_t i = 0; i < data.size(); i++)
	{

		//outfile1 <<data[i]<<"	";
		outfile1 << data[i] << ",";

	}
	outfile1 << endl;
	outfile1.close();
}

void CAudioAlgorithm::setIsComputerNorse(bool bNorse)
{
	m_bNorse = bNorse;
}
void CAudioAlgorithm::setIsComputerSNR(bool bsnr)
{
	m_bSNR = bsnr;
}

void CAudioAlgorithm::AudioAlgorithmInit()
{
	 m_bNorse = false;
	 fr_data.clear();
	 thd_data.clear();
	 rb_data.clear();
	 thdn_data.clear();
	 td_data.clear();
	 data = NULL;
	 driverMode = 0;
	 m_vp_in = 0;
	 m_vp_out = 0;
	 m_bSNR = false;


	 //加载数据
	
	/* if (!m_bfr_v)
	 {
		 jilisetnew.LoadSenoFSpk();
	 }*/
	 
	 
}

void CAudioAlgorithm::setSen(double sen)
{
	micsen = sen;
}

void CAudioAlgorithm::wav_readBymatlab(double**data, double fs, double times, char* pathwav)
{
	//matlab 环境初始化,全局matlab环境初始化完成
	/*bool b_ok = wavInitialize();

	if (!b_ok)
	{
	AfxMessageBox(_T("wavInitialize err"));

	}*/

	if (strcmp(pathwav, "") == 0)
	{
		pathwav = "D:\\TestRecord\\wdmRecord1.wav";
	}

	//调用具体函数
	wave_t* wav1 = (wave_t*)malloc(sizeof(wave_t));
	wavread(pathwav, wav1);
	if (wav1->fp == NULL)
		return;

	fclose(wav1->fp);





	//const wchar_t* path = _T("D:\TestRecord\wdmRecord1.wav");
	char* temppath = "D:\\TestRecord\\wdmRecord_0.wav";
	int len = strlen(temppath);

	mxChar* path = new mxChar[len];
	mwArray in(len, 1, mxCHAR_CLASS);
	mwArray in2(len, 1, mxCHAR_CLASS);
	for (size_t i = 0; i < len; i++)
	{
		path[i] = temppath[i];
	}
	path[len] = 0;
	//in.SetCharData(path, len+1);
	//in.GetCharData(in2,len);

	mwArray out1(wav1->length, 1, mxDOUBLE_CLASS);
	if (wav1->length == 0)
	{

		//mainf->setStatusStr(_T("读取wav文件信息失败！"));
		return;
	}
	//wchar_t* ml = _T("D://TestRecord//wdmRecord_0.wav");

	CString path_wav(pathwav);
	mxChar output[100]{0};

	for (size_t i = 0; i < path_wav.GetLength(); i++)
	{
		output[i] = path_wav.GetAt(i);
	}



	mwArray matlabpath(1, path_wav.GetLength(), mxCHAR_CLASS);

	matlabpath.SetCharData(path_wav, path_wav.GetLength());
	wchar_t* ml2 = new wchar_t[path_wav.GetLength()];
	matlabpath.GetCharData(ml2, path_wav.GetLength());
	mwArray col(1, 1, mxDOUBLE_CLASS);
	//
	//double* vc_col=new double[1];
	//vc_col[0] = 1;
	//col.SetData(vc_col, 1);
	col(1, 1) = 1;

	wav(1, out1, matlabpath, col);
	len = out1.NumberOfElements();

	*data = new double[len];
	double* wavdata = new double[len]{0};
	out1.GetData(wavdata, len);

	/*int k=(times*fs);
	if (k > len)
	k = len;*/
	std::ofstream outfile1;
	CString path1("D:\\TestRecord\\wavread.txt");
	outfile1.open(path1, std::ios::out | std::ios::trunc);
	for (size_t i = 0; i <len; i++)
	{
		data[0][i] = wavdata[i];
		outfile1 << wavdata[i] << endl;
	}
	outfile1.close();
	//wavTerminate(); //后面全局清理
	delete[] wavdata;
}

void CAudioAlgorithm::setAlgParam(CAlgParam algParam)
{
	m_algParam = algParam;
}

double CAudioAlgorithm::CalSmooth(double fs, vector<double> valFr, double step, vector<double> pindians)
{

	double smooth = 0.0;

	double fl = fs / pow(2, step);
	double fh = fs*pow(2, step);

	double sum = 0.0;
	int count = 0;
	for (size_t i = 0; i < pindians.size(); i++)
	{
		if (pindians[i] >= fl&&pindians[i] <= fh)
		{
			sum += valFr[i];
			count++;
		}

		if (pindians[i]>fh)
		{
			break;
		}
	}

	smooth = sum / count;

	return smooth;

}
