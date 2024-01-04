#pragma once
#include "afxcmn.h"
#include <vector>
#include "MainFrm.h"
#include "afxwin.h"

using namespace std;

// CPlayRecordSettings 对话框

struct PlayRecordDialogData
{
	CString play;
	CString jili;
	CString record;
	CString path;
	int mode;		// 0-MIC, 1-SPK
};

class CPlayRecordSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayRecordSettings)

public:
	CPlayRecordSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayRecordSettings();
	virtual BOOL OnInitDialog();

	void GetPlayAndRecord(CString& playname, CString& recordname);

	void GetPlayRecordInfo(int index,CString& palyChs, CString& jili, CString& recordChs, CString& path);

	vector<PlayRecordDialogData*>* getPlayRecordData();
	vector<PlayRecordDialogData*>* getSpkPlayRecordData();


// 对话框数据
	enum { IDD = IDD_dlgPlayandRecord };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnCmbDropDown3(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl mlistPlayRecordItems;
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnCmbDropDown(bool isIn);
	void OnCmbDropDown2(bool isIn,int cWnd);
	

private:

	int mlistId;

	vector<CComboBox*> mCmblistIn;
	vector<CComboBox*> mCmblistOut;
	vector<CComboBox*> m_jili_list;

	CMainFrame* mainf;

	int m_curIndex_in;
	int m_curIndex_out;

	vector<PlayRecordDialogData*> m_playrecorddata;
	vector<PlayRecordDialogData*> m_playrecorddata2;//spk

	void InitUIData();

	/************私有函数****************/
	void drawGrid(CDC* dc);
	int findComx(const vector<CComboBox*>& coblist, HWND wnd);
public:
	afx_msg void OnPaint();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cmbTestModes;
	afx_msg void OnBnClickedButtonSavedata();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton2();
};
