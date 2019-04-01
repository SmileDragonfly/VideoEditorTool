
// VideoEditorToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoEditorTool.h"
#include "VideoEditorToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoEditorToolDlg dialog



CVideoEditorToolDlg::CVideoEditorToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEOEDITORTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoEditorToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoEditorToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INPUT_VIDEO, &CVideoEditorToolDlg::OnBnClickedInputVideo)
	ON_BN_CLICKED(IDC_OUTPUT_VIDEO, &CVideoEditorToolDlg::OnBnClickedOutputVideo)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPEED_SPIN, &CVideoEditorToolDlg::OnDeltaposSpeedSpin)
	ON_BN_CLICKED(IDOK, &CVideoEditorToolDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVideoEditorToolDlg message handlers

BOOL CVideoEditorToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// Set default speed is 1x, and accelerator
	CSpinButtonCtrl* pSpeedSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPEED_SPIN);
	pSpeedSpin->SetRange(MIN_SPEED, MAX_SPEED);	// 0,5 to 5 real speed, respectively
	pSpeedSpin->SetPos(2);						// 1x speed
	UDACCEL udaccel;
	udaccel.nSec = 1;
	udaccel.nInc = 1;
	pSpeedSpin->SetAccel(1, &udaccel);
	// Display default speed on edit speed box
	CEdit* pSpeedView = (CEdit*)GetDlgItem(IDC_SPEED_VIEW);
	pSpeedView->SetWindowText(L"1x");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoEditorToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoEditorToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoEditorToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoEditorToolDlg::OnBnClickedInputVideo()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("MP4 Files (*.mp4)|*.mp4||");
	CFileDialog dlgInput(TRUE, _T(".mp4"), L"D:\\", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter); // @1st param: True is File Open, false is File Save As
	if (dlgInput.DoModal() != IDOK)
	{
		return;
	}
	else
	{
		CEdit* pEditInputPath = (CEdit*) GetDlgItem(IDC_INPUT_PATH);
		pEditInputPath->SetWindowText(dlgInput.GetPathName());
		m_strInputPath = dlgInput.GetPathName();
		return;
	}
}


void CVideoEditorToolDlg::OnBnClickedOutputVideo()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("MP4 Files (*.mp4)|*.mp4||");
	CFileDialog dlgOutput(FALSE, _T(".mp4"), L"D:\\", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter); // @1st param: True is File Open, false is File Save As
	if (dlgOutput.DoModal() != IDOK)
	{
		return;
	}
	else
	{
		CEdit* pEditOutputPath = (CEdit*)GetDlgItem(IDC_OUTPUT_PATH);
		pEditOutputPath->SetWindowText(dlgOutput.GetPathName());
		m_strOutputPath = dlgOutput.GetPathName();
	}
}



void CVideoEditorToolDlg::OnDeltaposSpeedSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CEdit* pSpeedView = (CEdit*)GetDlgItem(IDC_SPEED_VIEW);
	CSpinButtonCtrl* pSpeedSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPEED_SPIN);
	float fRealSpeed;
	if (pNMUpDown->iDelta > 0)
	{
		int range = pSpeedSpin->GetRange();
		if ((pNMUpDown->iPos + pNMUpDown->iDelta) > MAX_SPEED)
		{
			fRealSpeed = (float)(MAX_SPEED) / 2;
		}
		else
		{
			fRealSpeed = (float)(pNMUpDown->iPos + pNMUpDown->iDelta) / 2;
		}
	}
	else
	{
		if ((pNMUpDown->iPos + pNMUpDown->iDelta) < MIN_SPEED)
		{
			fRealSpeed = (float)(MIN_SPEED) / 2;
		}
		else
		{
			fRealSpeed = (float)(pNMUpDown->iPos + pNMUpDown->iDelta) / 2;
		}
	}
	
	CString sSpeedView;
	sSpeedView.Format(L"%.1fx", fRealSpeed);
	pSpeedView->SetWindowText(sSpeedView);
}


void CVideoEditorToolDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
	// 1 step: Check input and output path is exist
}
