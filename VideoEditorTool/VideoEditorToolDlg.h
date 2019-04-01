
// VideoEditorToolDlg.h : header file
//

#pragma once
#define MIN_SPEED 1			// 0.5x
#define MAX_SPEED 10		// 5x

// CVideoEditorToolDlg dialog
class CVideoEditorToolDlg : public CDialogEx
{
// Construction
public:
	CVideoEditorToolDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOEDITORTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strInputPath;
	CString m_strOutputPath;
	afx_msg void OnBnClickedInputVideo();
	afx_msg void OnBnClickedOutputVideo();
	afx_msg void OnDeltaposSpeedSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
