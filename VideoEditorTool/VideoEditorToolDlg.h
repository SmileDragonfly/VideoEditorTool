
// VideoEditorToolDlg.h : header file
//

#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "CProgressBar.h"
#define MIN_SPEED 1			// 0.5x
#define MAX_SPEED 10		// 5x
enum ColorSelection
{
	RED,
	GREEN,
	BLUE,
	GRAY,
	RAW
};

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
	ColorSelection m_iColor;
	float m_fSpeed;
	CWinThread* m_processThread;
	static UINT ProcessingThread(LPVOID param);
	void ChangeFrameColor(cv::Mat* frame, ColorSelection color);
	afx_msg void OnBnClickedInputVideo();
	afx_msg void OnBnClickedOutputVideo();
	afx_msg void OnDeltaposSpeedSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedColorRed();
	afx_msg void OnBnClickedColorGreen();
	afx_msg void OnBnClickedColorBlue();
	afx_msg void OnBnClickedColorGray();
	afx_msg void OnBnClickedColorRaw();
	afx_msg void OnEnChangeInputPath();
	afx_msg void OnEnChangeOutputPath();
	CComboBox m_comboBox;
	CString m_strComboBoxText;
	cv::Size m_szComboBoxSize;
	cv::Size m_szRawInputSize;
	afx_msg void OnCbnSelchangeComboFramesize();
};
