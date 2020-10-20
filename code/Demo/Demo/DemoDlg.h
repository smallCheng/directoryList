
// DemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"



// CDemoDlg �Ի���
class CDemoDlg : public CDialogEx
{
// ����
public:
	CDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDemoDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

// ��д
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog(); // ��ʼ��
	virtual void OnOK();


// ʵ��
protected:
	HICON m_hIcon;

private:
	CTreeCtrl m_showFileTree;

// ���ɵ���Ϣӳ�亯��
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnShowdirs();
	afx_msg void OnBnClickedBtnExit();

public:
	void BroseAllFiles(CString filepath, HTREEITEM tree_Root);
	void MyExpandTree(HTREEITEM hTreeItem);
};
