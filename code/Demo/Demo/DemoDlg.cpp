
// DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DemoApp.h"
#include "DemoDlg.h"
#include "Common.h"
#include "afxdialogex.h"
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemoDlg 对话框

CDemoDlg::CDemoDlg(CWnd* pParent)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CDemoDlg::~CDemoDlg()
{
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SHOWALLFILE, m_showFileTree);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDemoDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SHOWDIRS, &CDemoDlg::OnBnClickedBtnShowdirs)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDemoDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CDemoDlg 消息处理程序

BOOL CDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码 


	SetWindowText(WINDOWS_TITLE);
	GetDlgItem(IDC_SHOW_FILEPATH)->SetWindowText(ENTER_FILEPATH);
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(EMPTRY_STR);
	GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(BTN_SEARCH);
	GetDlgItem(IDC_BTN_SHOWDIRS)->SetWindowText(BTN_SHOWDIRS);
	GetDlgItem(IDC_BTN_EXIT)->SetWindowText(BTN_EXIT);

	GetDlgItem(IDC_BTN_SHOWDIRS)->EnableWindow(0);//禁用

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
// 对于使用文档/视图模型的 MFC 应用程序， 这将由框架自动完成。

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//屏蔽按下回车键对话框关闭
void CDemoDlg::OnOK()
{
	//CDialogEx::OnOK();
}


//点击 查询
void CDemoDlg::OnBnClickedBtnSearch()
{
	m_showFileTree.DeleteAllItems();//每次先清空树控件之前的数据。
	GetDlgItem(IDC_BTN_SHOWDIRS)->EnableWindow(0);//禁用

	//获到输入框的数据
	CString filepath;
	GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(filepath);
	if (filepath.IsEmpty())
	{
		AfxMessageBox(MSG_WARNING);
	}
	else
	{
		//先判断filepath是不是有效路径
		//文件或文件夹都可以判断，最后的\\号有无都没关系!
		if ( !(bool)PathFileExists(filepath) ) 
		{
			AfxMessageBox(MSG_NOTEXITS);
		}
		else
		{
			//初始化树形控件
			HTREEITEM treeRoot = m_showFileTree.InsertItem(filepath);//插入根节点
			BroseAllFiles(filepath, treeRoot);//以C:\test为根目录进行遍历
			GetDlgItem(IDC_BTN_SHOWDIRS)->EnableWindow(1);//启用
		}
	}
}


// 点击 展开所有目录
void CDemoDlg::OnBnClickedBtnShowdirs()
{
	MyExpandTree(m_showFileTree.GetRootItem());
}


// 点击 退出
void CDemoDlg::OnBnClickedBtnExit()
{
	CDialogEx::OnCancel();
}


// 遍历指定路径下面所有文件，并且显示到列表控件上面。
void CDemoDlg::BroseAllFiles(CString filepath, HTREEITEM tree_Root)
{
	//递归枚举文件夹下的内容
	CFileFind find;
	CString str_path;//文件路径
	CString str_fileName;//文件名称
	CString fullname;//文件的绝对路径名称
	HTREEITEM tree_Temp;//临时变量，用以记录返回的树节点

	//判断输入目录最后是否存在'\'，不存在则补充
	if (filepath.Right(1) != "\\")
	{
		filepath += "\\";
	}
	filepath += "*.*";
	BOOL IsFind = find.FindFile(filepath);

	while (IsFind)
	{
		tree_Temp = tree_Root;
		IsFind = find.FindNextFile();
		if (find.IsDirectory() && !find.IsDots())//目录是文件夹
		{
			str_path = find.GetFilePath(); //得到路径，做为递归调用的开始
			str_fileName = find.GetFileName();//得到目录名，做为树控的结点
			tree_Temp = m_showFileTree.InsertItem(str_fileName, 0, 0, tree_Root);
			BroseAllFiles(str_path, tree_Temp);
		}
		else if (!find.IsDirectory() && !find.IsDots())//如果是文件
		{
			str_path = find.GetFilePath(); //得到路径，做为递归调用的开始
			str_fileName = find.GetFileName();//得到文件名，做为树控的结点
			m_showFileTree.InsertItem(str_fileName, 0, 0, tree_Temp);

			//显示的非文件夹带全路径
			//fullname = str_path + str_fileName;
			//m_showFileTree.InsertItem(fullname, 0, 0, tree_Temp);
		}
	}
	find.Close();
}


// 展开树控件的所有结点，结合上面控件一起用
void CDemoDlg::MyExpandTree(HTREEITEM hTreeItem)
{
	if (!m_showFileTree.ItemHasChildren(hTreeItem))//如果树控件根节点没有子节点，则返回   
	{        
		return;    
	}    
	HTREEITEM hNextItem = m_showFileTree.GetChildItem(hTreeItem);//若树控件的根节点有子节点，则获取根节点的子节点    
	while (hNextItem != NULL)//若有    
	{        
		MyExpandTree(hNextItem);//递归，展开子节点下的所有子节点        
		hNextItem = m_showFileTree.GetNextItem(hNextItem, TVGN_NEXT);//获取根节点的下一个子节点    
	}    
	m_showFileTree.Expand(hTreeItem,TVE_EXPAND);//展开节点
}
	

