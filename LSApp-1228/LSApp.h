#include <algorithm>
#include "CyclesTestSet.h"
#include "LogUnit.h"
#include "Public.h"
#include "TestData.h"
// LSApp.h : LSApp 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


#define CREATESHENKA WM_USER+1

#define ONLINEMODE 0
#define MATLAB_COMPILE 0


// CLSAppApp:
// 有关此类的实现，请参阅 LSApp.cpp
//

class CLSAppApp : public CWinAppEx
{
public:
	CLSAppApp();
	~CLSAppApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnAppMoKuaiSheZhi();
	afx_msg void OnFileClose();
	afx_msg void OnPassWd();
	afx_msg void OnOpenFile();
	afx_msg void OnDotSet();
	afx_msg void OnStartCyclesTest();
	afx_msg void OnStopCyclesTest();
	afx_msg void OnCyclesTestSet();
	afx_msg void OnResShow();
	DECLARE_MESSAGE_MAP()


	


private:

	bool m_bdotset;


	void CreateSavePath();
	bool CheckKeyExit();

	/*void WriteDoubleData(CArchive& ar, double dbData, int nNum = 0);
	void WriteStringData(CArchive& ar, const char* pData);
	void SaveLimitHead(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, prParam* pPR, bool bLower = true);
	void SaveDataToCsv();*/
	
};

extern CLSAppApp theApp;
