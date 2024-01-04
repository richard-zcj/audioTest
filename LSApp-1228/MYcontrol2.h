
/*
    ����ֱ�ӷ�װ�࣬ÿ���ؼ���Ӧ��һ��driver����
	ע��driver������ڴ��������
*/
#pragma once
#include "afxwin.h"
#include "TestASIUO.h"
#include <vector>



using namespace std;



// MYcontrol2 �Ի���

class MYcontrol2 : public CDialogEx
{
	DECLARE_DYNAMIC(MYcontrol2)

public:
	//MYcontrol2(bool isinput, CWnd* pParent = NULL);   // ��׼���캯��
	MYcontrol2(bool isinput,int id, CWnd* pParent = NULL); // ��׼���캯��
	virtual ~MYcontrol2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void setChecked();
	bool isCheck();

	CString getInnerName();
	CString getChsName();
	CString getDriverType();
	CString getdeviceChsName();
	double getFs();
	double getVpValues();
	void playASIO(double fs,double v);
	void playASIO2(double fs, int asioIndex);
	void recordASIO(double fs, int*** data, double& recordtime,int* playchs,int* recordchs);
	void micCalibrationRecordASIO(double fs, int*** data, int recordtime);
	void micCalibrationRecordASIO2(double fs, int*** data, double recordtime, int asioIndex);//�޽������ݷ���
	void micCalibrationRecordWDM(double fs, int*** data, int recordtime);
	void recordWDM(double time,char* filename);
	void playWDM(double time, char* filename,CString playChsName);
	void playWDM2(double time, char* filename);

	void playMME();
	void recordMME(double recordTimes, void* obj, int chs, int bits, DWORD fs, int i);
	void startMM();
	void MMEinit(int n, double time, const vector<MMERecordParams>& mmesparam, int nIndex = -1);
	void createMMEbuffer(int n);
	void delMMEbuffer();
	bool wdmisdone();
	int getASIOchIndex();
	void wdmplayWithRecord(int time, char* filename, int index_out, int index_in,CString inChsName,double fs);

	void mmeplayWithRecord(int playIndex);

	int getchsIndex();



	void setVpVal(double vpval);

	void setWDMChs(int index);

	void setwdmindex_out(CString name);
	void setwdmName_in(CString name);

	CString getChsName_Show();
	void setMainf(CMainFrame* mf);

	int getMMEchs();
	int getMMEbits();
	int getMMEdevID();

	void setInnerChsName(CString name);
	void updatePlayAndRecordCmb();

	void generateV(double v,double f);
	void setASIOStop(bool bstop);

	void stopRe();

	void setCalVp(double vp);
	double getCalVp();

	double getSen();
	bool cal_vpByMME(double fs,double outvp=1);
	void saveMMEFile(double time);

	void playMME(int* data, int playIndex);

	void SetDriverSelected(int nIndex);
	void SetChannelSelected(int nIndex);
	void SetSamplingRateVal(double nValue);
	void SetSensitivityOrGainVal(double nValue);
	void SetBitDepthVal(int nValue);
	void SetDeviceSelected(CString name);
	void calSpkASIO(int*** recorddata, int playchs, double fs);
	int getCMBname(CString name);
	void setIsNoPlay(bool bplay);

// �Ի�������
	enum { IDD = IDD_MYCONTROL1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	HWND getHwnd();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusmycontrolchname();
	afx_msg void OnEnKillfocusmycontrolchname();

	

private:
	bool ischecked;

	bool m_isinput;

	bool m_bVP;

	CString m_factoryname;
	CString m_innerDevicename;
	CString m_drivertype;
	CString m_chsname;
	CString m_devicechsname;

	TestASIUO audioDriver;

	vector<wstring> wdmDeviceName;
	vector<int> wdmChName;
	

	vector<string> asioDeviceName;
	vector<string> asioChName;
	vector<string> asiotypeName;

	double fs;
	double vp;
	double sen;

	double cal_vp;//У׼ʹ�õ�vpֵ

	

	void loadWDM(bool binput);
	void loadASIO(bool binput);

	void clearCmb();
	void clearchCmb();

	void readRegsterVal(vector<string>& asioDriverName);
	void openMenu();

	bool isHwnd(HWND wnd);

	HWND mywnd;

	int m_id;
	
public:
	CComboBox m_cmbDriverName;
	afx_msg void OnCbnSelchangemycontroldrivername();
	CComboBox m_cmbdevicename;
	CComboBox m_cmbChs;//ASIO��ͨ��or WDM���豸 wdmʱ��˫ͨ��
	afx_msg void OnCbnSelchangemycontroldevicename2();
	afx_msg void OnClose();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEnChangemycontrolchname();
	afx_msg void OnCbnEditchangemycontrolchnames();
	CComboBox m_samplebits;
	CComboBox m_chsFs;
	afx_msg void OnPaint();
	afx_msg void openASIOPanle();
	afx_msg void addChs();
	afx_msg void cal_sen();
	afx_msg void cal_Ap();
	afx_msg void cal_VP();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeMycontroldevicename();
//	afx_msg void OnEditchangeMycontrolfs();
//	afx_msg void OnEditchangeMycontrolbits();
	afx_msg void OnEditchangeMycontrolsenorzengyi();
	afx_msg void OnSelchangeMycontrolfs();
	afx_msg void OnSelchangeMycontrolbits();
};
