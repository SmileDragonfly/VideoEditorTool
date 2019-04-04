
// VideoEditorToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoEditorTool.h"
#include "VideoEditorToolDlg.h"
#include "afxdialogex.h"

using namespace std;
using namespace cv;

static unsigned int g_count = 0;

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
	, m_strComboBoxText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fSpeed = 1;
	m_iColor = RED;
	m_processThread = NULL;
}

void CVideoEditorToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FRAMESIZE, m_comboBox);
	DDX_CBString(pDX, IDC_COMBO_FRAMESIZE, m_strComboBoxText);
}

BEGIN_MESSAGE_MAP(CVideoEditorToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INPUT_VIDEO, &CVideoEditorToolDlg::OnBnClickedInputVideo)
	ON_BN_CLICKED(IDC_OUTPUT_VIDEO, &CVideoEditorToolDlg::OnBnClickedOutputVideo)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPEED_SPIN, &CVideoEditorToolDlg::OnDeltaposSpeedSpin)
	ON_BN_CLICKED(IDOK, &CVideoEditorToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_COLOR_RED, &CVideoEditorToolDlg::OnBnClickedColorRed)
	ON_BN_CLICKED(IDC_COLOR_GREEN, &CVideoEditorToolDlg::OnBnClickedColorGreen)
	ON_BN_CLICKED(IDC_COLOR_BLUE, &CVideoEditorToolDlg::OnBnClickedColorBlue)
	ON_BN_CLICKED(IDC_COLOR_GRAY, &CVideoEditorToolDlg::OnBnClickedColorGray)
	ON_BN_CLICKED(IDC_COLOR_RAW, &CVideoEditorToolDlg::OnBnClickedColorRaw)
	ON_EN_CHANGE(IDC_INPUT_PATH, &CVideoEditorToolDlg::OnEnChangeInputPath)
	ON_EN_CHANGE(IDC_OUTPUT_PATH, &CVideoEditorToolDlg::OnEnChangeOutputPath)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAMESIZE, &CVideoEditorToolDlg::OnCbnSelchangeComboFramesize)
END_MESSAGE_MAP()


// CVideoEditorToolDlg message handlers

BOOL CVideoEditorToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add title
	CWnd * pWnd = AfxGetMainWnd();
	pWnd->SetWindowTextW(L"VideoEditorTool");
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
	// Choose default color
	CButton* pRadioRed = (CButton*)GetDlgItem(IDC_COLOR_RED);
	pRadioRed->SetCheck(TRUE);
	// Set combo box
	m_comboBox.InsertString(0, L"1920x1080");
	m_comboBox.InsertString(1, L"1280x720");
	m_comboBox.InsertString(2, L"720x576");
	m_comboBox.InsertString(3, L"640x480");
	m_comboBox.InsertString(4, L"320x240");
	//m_comboBox.SelectString(0, L"1920x1080");
	//m_comboBox.GetLBText(m_comboBox.GetCurSel(), m_strComboBoxText);

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



void CVideoEditorToolDlg::ChangeFrameColor(Mat* frame, ColorSelection color)
{
	switch (color)
	{
		case RED:
		case GREEN:
		case BLUE:
		{
			unsigned int channel = 2 - color;
			vector<Mat> frameChannel;
			Size inputSize(frame->cols, frame->rows);
			split(*frame, frameChannel);
			for (int i = 0; i < 3; ++i)
			{
				if (i != channel)
				{
					frameChannel[i] = Mat::zeros(inputSize, frameChannel[0].type());
				}
			}
			merge(frameChannel, *frame);
			break;
		}
		case GRAY:
		{
			Mat tempFrame;
			CString fileName;
			fileName.Format(L"D:\\picture\\img_000%d.jpg", g_count);
			CT2A cFileName(fileName);
			cvtColor(*frame, tempFrame, COLOR_BGR2GRAY);
			*frame = tempFrame;
			imwrite(cFileName.m_szBuffer, tempFrame);
			*frame = imread(cFileName.m_szBuffer);
			DeleteFile(fileName);
			g_count++;
			break;
		}
		case RAW:
		{
			break;
		}
		default:
		{
			AfxMessageBox(L"This color conversion type is not supported!");
		}
	}
}

UINT CVideoEditorToolDlg::ProcessingThread(LPVOID param)
{
	// Step 1: Open Input
	CVideoEditorToolDlg* pTarget = static_cast<CVideoEditorToolDlg*>(param);
	VideoCapture inputVideo;
	CT2A cInputPath(pTarget->m_strInputPath);
	CT2A cOutputPath(pTarget->m_strOutputPath);
	inputVideo.open(cInputPath.m_szBuffer);
	unsigned int totalFrame = inputVideo.get(CAP_PROP_FRAME_COUNT);

	// Step 2: Get output FPS,Size (equal InputSize) and FOURCC (input) to open
	unsigned int iOutputVideoFPS;
	if (pTarget->m_fSpeed >= 1)
	{
		iOutputVideoFPS = (unsigned int)(pTarget->m_fSpeed * inputVideo.get(CAP_PROP_FPS));
	}
	else
	{
		// 0.5x
		iOutputVideoFPS = inputVideo.get(CAP_PROP_FPS);
	}
	//Size outputSize((int)inputVideo.get(CAP_PROP_FRAME_WIDTH), (int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));
	// int iFourcc = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));
	int iFourcc = VideoWriter::fourcc('H', '2', '6', '4');
	VideoWriter outputVideo(cOutputPath.m_szBuffer, iFourcc, iOutputVideoFPS, pTarget->m_szComboBoxSize);

	// Step 3: Init progress bar, text
	CProgressCtrl* pProgressBar = (CProgressCtrl*)pTarget->GetDlgItem(IDC_PROGRESS_BAR);
	CStatic* pStaticText = (CStatic*)pTarget->GetDlgItem(IDC_FRAME_TOTAL);
	pProgressBar->SetRange(0, totalFrame);
	pProgressBar->SetPos(0);
	CString frameTotal;
	frameTotal.Format(L"0/%d", totalFrame);
	pStaticText->SetWindowText(frameTotal);
	// Step 4: Get a frame from input, change it's color then put it to output

	unsigned int count = 0;
	while (1)
	{
		Mat frame;
		inputVideo >> frame;
		if (frame.empty())
		{
			break;
		}
		// Change frame color
		pTarget->ChangeFrameColor(&frame, pTarget->m_iColor);
		// Change frame size
		if (pTarget->m_szRawInputSize != pTarget->m_szComboBoxSize)
		{
			resize(frame, frame, pTarget->m_szComboBoxSize);
		}
		// Write frame to output
		if (pTarget->m_fSpeed >= 1)
		{
			outputVideo << frame;
		}
		else
		{
			outputVideo << frame;
			outputVideo << frame;
		}
		count++;
		if ((count % 10) == 0)
		{
			pProgressBar->SetPos(count);
		}
		frameTotal.Format(L"%d/%d (%0.2f", count, totalFrame, (float)count / totalFrame * 100);
		frameTotal += L"%)";
		pStaticText->SetWindowText(frameTotal);
	}
	inputVideo.release();
	outputVideo.release();
	pTarget->m_processThread = NULL;
	return 0;
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
		if (PathFileExists(m_strInputPath))
		{
			VideoCapture inputVideo;
			CT2A cInputPath(m_strInputPath);
			inputVideo.open(cInputPath.m_szBuffer);
			m_szComboBoxSize.width = inputVideo.get(CAP_PROP_FRAME_WIDTH);
			m_szComboBoxSize.height = inputVideo.get(CAP_PROP_FRAME_HEIGHT);
			m_strComboBoxText.Format(L"%dx%d", m_szComboBoxSize.width, m_szComboBoxSize.height);
			m_comboBox.SelectString(0, m_strComboBoxText);
			inputVideo.release();
			m_szRawInputSize = m_szComboBoxSize;
		}
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
	m_fSpeed = fRealSpeed;
	return;
}


void CVideoEditorToolDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
	// Step 1: Check input and output path is exist
	BOOL bIsInputFileExist = PathFileExists(m_strInputPath);
	if (!bIsInputFileExist)
	{
		AfxMessageBox(L"Input file is not exist!");
		return;
	}
	CString strOutputDir = m_strOutputPath.Left(m_strOutputPath.ReverseFind('\\'));
	BOOL bIsOutputDirExist = PathIsDirectory(strOutputDir);
	if (!bIsOutputDirExist)
	{
		AfxMessageBox(L"Output folder is not exist!");
		return;
	}

	//// Step 2: Open Input
	//VideoCapture inputVideo;
	//CT2A cInputPath(m_strInputPath);
	//CT2A cOutputPath(m_strOutputPath);
	//inputVideo.open(cInputPath.m_szBuffer);
	//unsigned int totalFrame = inputVideo.get(CAP_PROP_FRAME_COUNT);

	//// Step 3: Get output FPS,Size (equal InputSize) and FOURCC (input) to open
	//unsigned int iOutputVideoFPS = (unsigned int) (m_fSpeed * inputVideo.get(CAP_PROP_FPS));
	//Size outputSize((int)inputVideo.get(CAP_PROP_FRAME_WIDTH), (int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));
	//int iFourcc = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));
	//VideoWriter outputVideo(cOutputPath.m_szBuffer, iFourcc, iOutputVideoFPS, outputSize);

	//// Step 4: Init progress bar, text
	//CProgressCtrl* pProgressBar = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR);
	//CStatic* pStaticText = (CStatic*)GetDlgItem(IDC_FRAME_TOTAL);
	//pProgressBar->SetRange(0, totalFrame);
	//pProgressBar->SetPos(0);
	//CString frameTotal;
	//frameTotal.Format(L"0/%d", totalFrame);
	//pStaticText->SetWindowText(frameTotal);
	//// Step 5: Get a frame from input, change it's color then put it to output
	//
	//unsigned int count = 0;
	//while(1)
	//{
	//	Mat frame;
	//	inputVideo >> frame;
	//	if (frame.empty())
	//	{
	//		break;
	//	}
	//	// Change frame color
	//	ChangeFrameColor(&frame, m_iColor);
	//	// Write frame to output
	//	outputVideo << frame;
	//	count++;
	//	if ((count % 10) == 0)
	//	{
	//		pProgressBar->SetPos(count);
	//	}
	//	frameTotal.Format(L"%d/%d (%0.2f", count, totalFrame, (float)count/totalFrame * 100);
	//	frameTotal += L"%)";
	//	pStaticText->SetWindowText(frameTotal);
	//}
	//AfxMessageBox(L"Done!");
	//inputVideo.release();
	//outputVideo.release();
	if (m_processThread != NULL)
	{
		AfxMessageBox(L"Processing!");
	}
	else
	{
		m_processThread = AfxBeginThread(ProcessingThread, (LPVOID)this, 0, THREAD_PRIORITY_BELOW_NORMAL, CREATE_SUSPENDED);
		if (m_processThread)
		{
			m_processThread->ResumeThread();
		}
	}
}


void CVideoEditorToolDlg::OnBnClickedColorRed()
{
	// TODO: Add your control notification handler code here
	m_iColor = RED;
}


void CVideoEditorToolDlg::OnBnClickedColorGreen()
{
	// TODO: Add your control notification handler code here
	m_iColor = GREEN;
}


void CVideoEditorToolDlg::OnBnClickedColorBlue()
{
	// TODO: Add your control notification handler code here
	m_iColor = BLUE;
}


void CVideoEditorToolDlg::OnBnClickedColorGray()
{
	// TODO: Add your control notification handler code here
	m_iColor = GRAY;
}


void CVideoEditorToolDlg::OnBnClickedColorRaw()
{
	// TODO: Add your control notification handler code here
	m_iColor = RAW;
}


void CVideoEditorToolDlg::OnEnChangeInputPath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* pEditInputPath = (CEdit*) GetDlgItem(IDC_INPUT_PATH);
	pEditInputPath->GetWindowTextW(m_strInputPath);
}


void CVideoEditorToolDlg::OnEnChangeOutputPath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* pEditOutputPath = (CEdit*)GetDlgItem(IDC_OUTPUT_PATH);
	pEditOutputPath->GetWindowTextW(m_strOutputPath);
}


void CVideoEditorToolDlg::OnCbnSelchangeComboFramesize()
{
	// TODO: Add your control notification handler code here
	m_comboBox.GetLBText(m_comboBox.GetCurSel(), m_strComboBoxText);
	UpdateData(FALSE);
	switch (m_comboBox.GetCurSel())
	{
		case 0:
		{
			m_szComboBoxSize.width = 1920;
			m_szComboBoxSize.height = 1080;
		}
		case 1:
		{
			m_szComboBoxSize.width = 1280;
			m_szComboBoxSize.height = 720;
		}
		case 2:
		{
			m_szComboBoxSize.width = 720;
			m_szComboBoxSize.height = 576;
		}
		case 3:
		{
			m_szComboBoxSize.width = 640;
			m_szComboBoxSize.height = 480;
		}
		case 4:
		{
			m_szComboBoxSize.width = 320;
			m_szComboBoxSize.height = 240;
		}
	}
}
