#pragma once
#include "afxcmn.h"
#include "FrSettings.h"
#include "ThdSettings.h"
#include "RbSettings.h"


// CAlogriSettins �Ի���

class CAlogriSettins : public CDialogEx
{
	DECLARE_DYNAMIC(CAlogriSettins)

public:
	CAlogriSettins(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlogriSettins();
	virtual BOOL OnInitDialog();

	ThdData getThdData();
	RBdata getRbData();

	vector<int>* getAnalysisData_mic();
	vector<int>* getAnalysisData_spk();

	HWND GettreeH();


// �Ի�������
	enum { IDD = IDD_dlgAlgoriSettinds };

private:
	HTREEITEM hroot, hroot2;
	HTREEITEM hItemdel;
	HTREEITEM tree_spk;
	HTREEITEM tree_mic;
    
	CFrSettings frset;
	CThdSettings thd;
	CRbSettings rb;

	vector<int> analysis_mic;//mic
	vector<int> analysis_spk;


	/*****************˽�к���*******************/
	void delanalysis(vector<int>* data,int element);
	void addanalysis(vector<int>* data, int element);
	void updateAnalysisStatus(HTREEITEM item,int index,bool ismic);
	void updateAnalysisAll_mic(BOOL IsCkeck,const vector<int>* items);
	void updateAnalysisAll_spk(BOOL IsCkeck, const vector<int>* items);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_alogriTree;
	CTabCtrl m_tab;
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_analysisSelect;
	afx_msg void OnNMClickTreeanalysis(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMSetfocusTreeanalysis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTreeanalysis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnanalysisClick(WPARAM wPara,LPARAM lPara);
	afx_msg void OnBnClickedButtonsaveAnalysis();
	afx_msg void OnPaint();
};
