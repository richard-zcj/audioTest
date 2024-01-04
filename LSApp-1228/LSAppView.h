
// LSAppView.h : CLSAppView ��Ľӿ�
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
protected: // �������л�����
	CLSAppView();
	DECLARE_DYNCREATE(CLSAppView)

// ����
public:
	CLSAppDoc* GetDocument() const;

// ����
public:



private:

	CButton m_button1;
	bool m_onceExcute;
	

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	

// ʵ��
public:
	virtual ~CLSAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreatestruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LSAppView.cpp �еĵ��԰汾
inline CLSAppDoc* CLSAppView::GetDocument() const
   { return reinterpret_cast<CLSAppDoc*>(m_pDocument); }
#endif

