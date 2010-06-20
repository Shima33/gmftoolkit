
// GMFGUIDlg.h : header file
//

#pragma once


// CGMFGUIDlg dialog
class CGMFGUIDlg : public CDialogEx
{
// Construction
public:
	CGMFGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GMFGUI_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClickedButtonBrowseI();
	CString m_inputFileName;
	CString m_outputFileName;
	afx_msg void OnClickedButtonBrowseO();
private:
	void getSaveNameForOpen(LPWSTR openFileName);
	WCHAR inputFileName[512];
	WCHAR outputFileName[512];
	bool decompilingMode;
	void setDecompilingMode(bool active);
public:
	afx_msg void OnBnClickedButtonra1();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnBnClickedButtonCompile();
	CButton m_ButtonCompile;
	CButton m_ButtonRA1;
	CListBox m_LogList;
};
