#pragma once


// CScanlot 对话框

class CScanlot : public CDialogEx
{
	DECLARE_DYNAMIC(CScanlot)

public:
	CScanlot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScanlot();
	virtual BOOL OnInitDialog();


	bool GetDialogRes();

	CString getBuleID(int index);
	CString getBarCodeSn(int index);
	bool isRetestMode();
	void ClearLots(bool bcls);

// 对话框数据
	enum { IDD = IDD_DlgScanLot };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnOK();


private:

	bool m_dialogRes;
	bool m_bcls;

	bool m_lot1_focus;
	bool m_lot2_focus;
	bool m_lot3_focus;
	bool m_lot4_focus;

	BOOL m_lot1_ck;
	BOOL m_lot2_ck;
	BOOL m_lot3_ck;
	BOOL m_lot4_ck;

	

	CString lot1;
	CString lot2;
	CString lot3;
	CString lot4;

	CString buleid1;
	CString buleid2;
	CString buleid3;
	CString buleid4;

	CString sn1;
	CString sn2;
	CString sn3;
	CString sn4;

	void CheckSetFocus();

public:
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnSetfocusEdit6();
	afx_msg void OnEnKillfocusEdit6();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();

	
	BOOL m_Retest;
};
