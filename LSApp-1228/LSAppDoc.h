
// LSAppDoc.h : CLSAppDoc ��Ľӿ�
//


#pragma once

#include <vector>
#include "CardSeting.h"

using namespace std;

class CLSAppDoc : public CDocument
{
protected: // �������л�����
	CLSAppDoc();
	DECLARE_DYNCREATE(CLSAppDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CLSAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	
	
#endif

protected:
	void SetDocTitile();


private:
	CCardSeting cardset;
	

	
	


// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
