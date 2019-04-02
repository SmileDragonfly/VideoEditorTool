// CProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "VideoEditorTool.h"
#include "CProgressBar.h"
#include "afxdialogex.h"


// CProgressBar dialog

IMPLEMENT_DYNAMIC(CProgressBar, CDialogEx)

CProgressBar::CProgressBar(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROGRESS_BAR, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
}


BEGIN_MESSAGE_MAP(CProgressBar, CDialogEx)
	ON_MESSAGE(UPDATE_PROGRESS_BAR, &CProgressBar::OnUpdateProgressBar)
	ON_MESSAGE(CLOSE_PROGRESS_BAR, &CProgressBar::OnCloseProgressBar)
END_MESSAGE_MAP()


// CProgressBar message handlers


BOOL CProgressBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	// Set range, and start state for progress bar
	SetWindowText(L"Progress");
	m_progressBar.SetRange(0, 100);
	m_progressBar.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


afx_msg LRESULT CProgressBar::OnUpdateProgressBar(WPARAM wParam, LPARAM lParam)
{
	m_progressBar.SetPos(wParam);
	return 0;
}


afx_msg LRESULT CProgressBar::OnCloseProgressBar(WPARAM wParam, LPARAM lParam)
{
	if (this)
		this->DestroyWindow();  // destroy it
	return 0;
}
