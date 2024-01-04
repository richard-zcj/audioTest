#pragma once
#include <vector>
#include "FrHarm.h"  //算法头文件
#include "AlogriSettins.h"
#include "Public.h"


//这个结构体只能使用基础数据类型
struct CAlgParam
{
	int recordIndex;
};

using namespace std;
#define AUDIOALG_START 1  //开启算法运算
 class CAudioAlgorithm 
{
public:
	CAudioAlgorithm();
	~CAudioAlgorithm();


	void Alg_Fr(double* response, double fs);//频响测试
	void Alg_Fr(double* response, double fs, prParam* pPlayRecord,int slotId);//频响测试
	vector<double>* getpindiandata();

	void setDriverMode(int mode);
	void setDeviceInputVpval(double vp);
	void setDeviceOutputVpval(double vp);
	void setIsComputerNorse(bool bNorse);
	void setIsComputerSNR(bool bsnr);
	void setHmani(vector<int>* thd);
	void setHmaniRB(vector<int>* rb);

	void saveData(CString testID,char* path,const vector<double>& fs, const vector<double>& data);

	void AudioAlgorithmInit();

	void CaculatePhase(mxDouble* result4, int rowlem);
	void CaculateTHD_N(mxDouble* result3, mxDouble* result);
	void CaculateRB(ThdData&  thdalogriset, mxDouble* result2, mxDouble* result, int rowlem, prParam* pPlayRecord = NULL);
	void CaculateTHD(ThdData& thdalogriset, int rowlem, mxDouble* result2, mxDouble* result, prParam* pPlayRecord=NULL);

	void wav_readBymatlab(double**data, double fs, double times, char* pathwav);

	void setSen(double sen);
	void setFrMode(bool isV);
	void setIEEE(bool ieee);
	void setAlgParam(CAlgParam algParam);

	double CalSmooth(double fs, vector<double> valFr, double step, vector<double> pindians);

	void Smooth(vector<double>& sm, prParam* pPlayRecord);
	void SmoothMIC(vector<double>& sm, prParam* pPlayRecord, int stp);

//外部可访问数据
public:
	
	vector<double> fr_data;
	vector<double> thd_data;
	vector<double> rb_data;
	vector<double> thdn_data;
	//vector<double>* m_pindians;
	vector<double> td_data;
	vector<double> phase_data;
	double m_snr;
	double m_micSensitivity;//针对奋达使用 直接提取1kHz频点的fr值

	//用于内部计算数据
private:
	vector<int>* data;              //信号数据集合
	int driverMode;

	double m_vp_in;
	double m_vp_out;

	bool m_bNorse;
	bool m_bSNR;
	double micsen;
	bool m_bfr_v;//只计算fr
	vector<int>* thdhamni;
	vector<int>* rbhamni;
	bool isIEEE;
	bool m_bMicMode;
	int m_micChs;//录音通道
	
	CAlgParam m_algParam;

	bool m_bSaveSrcFile;
};

