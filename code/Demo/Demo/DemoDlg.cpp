
// DemoDlg.cpp : ʵ���ļ�
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


// CDemoDlg �Ի���

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


// CDemoDlg ��Ϣ�������

BOOL CDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������ 


	SetWindowText(WINDOWS_TITLE);
	GetDlgItem(IDC_SHOW_FILEPATH)->SetWindowText(ENTER_FILEPATH);
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(EMPTRY_STR);
	GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(BTN_SEARCH);
	GetDlgItem(IDC_BTN_SHOWDIRS)->SetWindowText(BTN_SHOWDIRS);
	GetDlgItem(IDC_BTN_EXIT)->SetWindowText(BTN_EXIT);

	GetDlgItem(IDC_BTN_SHOWDIRS)->EnableWindow(0);//����

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ��������Ƹ�ͼ�ꡣ
// ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó��� �⽫�ɿ���Զ���ɡ�

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���ΰ��»س����Ի���ر�
void CDemoDlg::OnOK()
{
	//CDialogEx::OnOK();
}


//��� ��ѯ
void CDemoDlg::OnBnClickedBtnSearch()
{
	m_showFileTree.DeleteAllItems();//ÿ����������ؼ�֮ǰ�����ݡ�
	GetDlgItem(IDC_BTN_SHOWDIRS)->EnableWindow(0);//����

	//������������
	CString filepath;
	GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(filepath);
	if (filepath.IsEmpty())
	{
		AfxMessageBox(MSG_WARNING);
	}
	else
	{
		//���ж�filepath�ǲ�����Ч·��
		//�ļ����ļ��ж������жϣ�����\\�����޶�û��ϵ!
		if ( !(bool)PathFileExists(filepath) ) 
		{
			AfxMessageBox(MSG_NOTEXITS);
		}
		else
		{
			//��ʼ�����οؼ�
			HTREEITEM treeRoot = m_showFileTree.InsertItem(filepath);//������ڵ�
			BroseAllFiles(filepath, treeRoot);//��C:\testΪ��Ŀ¼���б���
			GetDlgItem(IDC_BTN_SHOWDIRS)->EnableWindow(1);//����
		}
	}
}


// ��� չ������Ŀ¼
void CDemoDlg::OnBnClickedBtnShowdirs()
{
	MyExpandTree(m_showFileTree.GetRootItem());
}


// ��� �˳�
void CDemoDlg::OnBnClickedBtnExit()
{
	CDialogEx::OnCancel();
}


// ����ָ��·�����������ļ���������ʾ���б�ؼ����档
void CDemoDlg::BroseAllFiles(CString filepath, HTREEITEM tree_Root)
{
	//�ݹ�ö���ļ����µ�����
	CFileFind find;
	CString str_path;//�ļ�·��
	CString str_fileName;//�ļ�����
	CString fullname;//�ļ��ľ���·������
	HTREEITEM tree_Temp;//��ʱ���������Լ�¼���ص����ڵ�

	//�ж�����Ŀ¼����Ƿ����'\'���������򲹳�
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
		if (find.IsDirectory() && !find.IsDots())//Ŀ¼���ļ���
		{
			str_path = find.GetFilePath(); //�õ�·������Ϊ�ݹ���õĿ�ʼ
			str_fileName = find.GetFileName();//�õ�Ŀ¼������Ϊ���صĽ��
			tree_Temp = m_showFileTree.InsertItem(str_fileName, 0, 0, tree_Root);
			BroseAllFiles(str_path, tree_Temp);
		}
		else if (!find.IsDirectory() && !find.IsDots())//������ļ�
		{
			str_path = find.GetFilePath(); //�õ�·������Ϊ�ݹ���õĿ�ʼ
			str_fileName = find.GetFileName();//�õ��ļ�������Ϊ���صĽ��
			m_showFileTree.InsertItem(str_fileName, 0, 0, tree_Temp);

			//��ʾ�ķ��ļ��д�ȫ·��
			//fullname = str_path + str_fileName;
			//m_showFileTree.InsertItem(fullname, 0, 0, tree_Temp);
		}
	}
	find.Close();
}


// չ�����ؼ������н�㣬�������ؼ�һ����
void CDemoDlg::MyExpandTree(HTREEITEM hTreeItem)
{
	if (!m_showFileTree.ItemHasChildren(hTreeItem))//������ؼ����ڵ�û���ӽڵ㣬�򷵻�   
	{        
		return;    
	}    
	HTREEITEM hNextItem = m_showFileTree.GetChildItem(hTreeItem);//�����ؼ��ĸ��ڵ����ӽڵ㣬���ȡ���ڵ���ӽڵ�    
	while (hNextItem != NULL)//����    
	{        
		MyExpandTree(hNextItem);//�ݹ飬չ���ӽڵ��µ������ӽڵ�        
		hNextItem = m_showFileTree.GetNextItem(hNextItem, TVGN_NEXT);//��ȡ���ڵ����һ���ӽڵ�    
	}    
	m_showFileTree.Expand(hTreeItem,TVE_EXPAND);//չ���ڵ�
}
	

