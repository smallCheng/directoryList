
// DemoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"



// CDemoDlg 对话框
class CDemoDlg : public CDialogEx
{
// 构造
public:
	CDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDemoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

// 重写
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog(); // 初始化
	virtual void OnOK();


// 实现
protected:
	HICON m_hIcon;

private:
	CTreeCtrl m_showFileTree;

// 生成的消息映射函数
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
