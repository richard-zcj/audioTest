#pragma once


// LineBorderSetingItem �Ի���

class LineBorderSetingItem : public CDialogEx
{
	DECLARE_DYNAMIC(LineBorderSetingItem)

public:
	LineBorderSetingItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LineBorderSetingItem();


	void setTitles(CString title);

// �Ի�������
	enum { IDD = IDD_limitsItem };

private:

	CString m_title;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
