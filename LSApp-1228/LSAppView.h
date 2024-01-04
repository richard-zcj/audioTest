
// LSAppView.h : CLSAppView 类的接口
//

#pragma once
#include "LSAppDoc.h"
#include "MyDlg.h"
#include "Lmd.h"
#include "DeviceSet.h"
#include "Geyin.h"
#include "JiLiSet.h"
#include "BlueComm.h"
#include "TestRME.h"
#include "BlueTestD.h"
#include "BlueEreTest.h"
#include "JiLiSetNewVersion.h"
#include "DeviceManage.h"
#include "MYcontrol2.h"
#include "Calibration.h"
#include "TestBorderLineSettings.h"
#include "AlogriSettins.h"
#include "PlayRecordSettings.h"
#include "CaculateSet.h"




class CLSAppView : public CView
{
protected: // 仅从序列化创建
	CLSAppView();
	DECLARE_DYNCREATE(CLSAppView)

// 特性
public:
	CLSAppDoc* GetDocument() const;

// 操作
public:



private:

	CButton m_button1;
	bool m_onceExcute;
	

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	

// 实现
public:
	virtual ~CLSAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreatestruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LSAppView.cpp 中的调试版本
inline CLSAppDoc* CLSAppView::GetDocument() const
   { return reinterpret_cast<CLSAppDoc*>(m_pDocument); }
#endif

