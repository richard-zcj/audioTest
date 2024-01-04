#pragma once
#include <vector>
#include "FrHarm.h"  //�㷨ͷ�ļ�
#include "AlogriSettins.h"
#include "Public.h"


//����ṹ��ֻ��ʹ�û�����������
struct CAlgParam
{
	int recordIndex;
};

using namespace std;
#define AUDIOALG_START 1  //�����㷨����
 class CAudioAlgorithm 
{
public:
	CAudioAlgorithm();
	~CAudioAlgorithm();


	void Alg_Fr(double* response, double fs);//Ƶ�����
	void Alg_Fr(double* response, double fs, prParam* pPlayRecord,int slotId);//Ƶ�����
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

//�ⲿ�ɷ�������
public:
	
	vector<double> fr_data;
	vector<double> thd_data;
	vector<double> rb_data;
	vector<double> thdn_data;
	//vector<double>* m_pindians;
	vector<double> td_data;
	vector<double> phase_data;
	double m_snr;
	double m_micSensitivity;//��Էܴ�ʹ�� ֱ����ȡ1kHzƵ���frֵ

	//�����ڲ���������
private:
	vector<int>* data;              //�ź����ݼ���
	int driverMode;

	double m_vp_in;
	double m_vp_out;

	bool m_bNorse;
	bool m_bSNR;
	double micsen;
	bool m_bfr_v;//ֻ����fr
	vector<int>* thdhamni;
	vector<int>* rbhamni;
	bool isIEEE;
	bool m_bMicMode;
	int m_micChs;//¼��ͨ��
	
	CAlgParam m_algParam;

	bool m_bSaveSrcFile;
};

