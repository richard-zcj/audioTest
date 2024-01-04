#pragma once
#include "Mycontrol2.h"
#include <vector>
#include<list>
#include <map>
#include "MainFrm.h"
#include "afxwin.h"

using namespace std;


#ifndef MyControlDATA
#define MyControlDATA
	class MyControldata
	{
	public:
		MyControldata(CString innername, CString drivername, CString chsname);

		~MyControldata();

		void getdata(CString& innername, CString& drivername, CString& chsname);
		void setdata(CString innername, CString drivername, CString chsname);

	private:

		CString m_innername;
		CString m_drivername;
		CString m_chsname;

	};
#endif

	//10, NULL, 2, 16, 48000, i
	

// CDeviceManage 对话框
//该类用于管理每一条数据信息
//用户关于设备的配置信息，由该类统一管理
//若要获取设备的配置及使用，使用该类为接口类
class CDeviceManage : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceManage)

public:
	CDeviceManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceManage();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	void play(CString innerDeviceName,double v);
	void play(int innerDeviceName, CString jili, bool isasio, double fs, int asioIndex,char* playWdmName,CString playChsName);
	bool calbrationplay(CString innerDeviceName, double v);
	void record(CString innerDeviceName, CString innerDeviceNameplay, int***indata, int fs, double& recordtime);
	void record_new(int innerDeviceName, int innerDeviceNameplay, int***indata, int fs, double& recordtime,int asioindex_in,int asioindex_out);//录音+播放
	void record(CString innerDeviceName, CString path, int***indata);
	void record2(int innerDeviceName, CString path, int***indata,int asioIndex,bool isasio,double fs,double recordtime);//只录音无界面数据访问

	void calibrationMic(CString innerDeviceName, int***indata, int fs, double recordtime, double**indata2, int outchs, int inchs,double outvp);
	void calibrationMic_NotPlay(CString innerDeviceName, int***indata, int fs, double recordtime, double**indata2);

	bool checkhasinputname(CString name);
	bool checkhasoutputname(CString name);

	double getfs(CString index);
	double getinputvp(CString innerName);
	double getoutvp(CString innerName);
	void getDriverType(CString index, char** drivertype);
	void getOutDriverType(int index, char** drivertype);
	int getASIOindex(CString innerDeviceName,bool isInput,int* dataindex,bool& isasio,double& fs,CString& wdmDeviceName,int& chs,int& bits,int& mmeindex);
	

	//以下函数是具体测试项使用的播放与录音流程函数
	void wdmPlayWithRecord(CString innerDeviceName_in, CString innerDeviceName_out, double playTimes, char* playFileName);
	void wdmPlayWithRecord_new(int innerDeviceName_in, int innerDeviceName_out, double playTimes, char* playFileName,CString playName,CString recordName,double fs);

	void wav_readBymatlab(double**data, double fs, double times, char* pathwav);

	void setDeviceVpVal(CString innerName, double vp, bool isinput);

	void setApval(double apval);
	double getApval();

	double getSensitivity(CString innerName);
	vector<CString> getInnerNames_InCHS();
	vector<CString> getInnerNames_OutCHS();

	void setASIOStop(CString innername,bool isInput);
	
	void MMERecord(const vector<MMERecordParams>& mmeParams);
	void MMERecord(const MMERecordParams& mmeParams);
	void MMEinit(double times, const vector<MMERecordParams>& mmesparam, int nIndex = -1);
	void MMEinit(double times, int chs, int bit, DWORD fs);

	int getChsIndexByName(CString chsName,bool isInput);
	int getMMEChsIndexByName(CString chsName, bool isInput);
	
	void initDialogData();

void calSpkMME(CString recordChs, int* indata, int playChs,double** outdata,double recordtime);
	bool isInitFinish();
	void setInitFinish(bool binit);

	void calSpkASIO(CString outname, int*** recorddata);

private:

	char* getcharp(CString str);
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);

	void openMenu();
	

// 对话框数据
public:
	enum { IDD = IDD_devicemanange };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	

	
	

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void openASIOPanle();
	


private:
	

	

	int curindex;//当前的开始位置
	int curindex2;//当前的开始位置

	int curindexout;//当前的开始位置
	int curindex2out;//当前的开始位置

	int testi;
	int testi2;
	MYcontrol2* cont;

	map<CString, int>name2ch;

	HWND itemdatawnd;

	CMainFrame* mainf;

	double ap;
	double m_sen;
	CBrush m_btn01Brush;
	CBrush brush1;

	bool m_bInitFinish;

	bool m_bInitDialog;

public:
	std::vector<MYcontrol2*> data;
	std::vector<MYcontrol2*> data2;//输出通道数据
	list<MyControldata*> inlist;

public:
	afx_msg void OnBnClickedButtonaddInCh();
	afx_msg void OnBnClickedButtonDelInCh();
	afx_msg void OnBnClickedButtonaddOutChs();
	afx_msg void OnBnClickedButtonDelOutChs();
	afx_msg void OnClose();
	afx_msg void OnBnClickeddevicemangeplay();
	afx_msg void OnBnClickeddevicemangerecord();
	afx_msg void OnBnClickeddevicesaveout();
	afx_msg void OnMove(int x, int y);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CButton m_addinBtn;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};




