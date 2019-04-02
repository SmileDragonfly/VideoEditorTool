#pragma once


// CProgressBar dialog
#define UPDATE_PROGRESS_BAR  (WM_USER + 100)
#define CLOSE_PROGRESS_BAR   (WM_USER + 101)

class CProgressBar : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressBar)

public:
	CProgressBar(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProgressBar();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_BAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_progressBar;
protected:
	afx_msg LRESULT OnUpdateProgressBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseProgressBar(WPARAM wParam, LPARAM lParam);
	HICON m_hIcon;
};
