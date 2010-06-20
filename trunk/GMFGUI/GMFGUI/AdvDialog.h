#pragma once


// CAdvDialog dialog

class CAdvDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAdvDialog)

public:
	CAdvDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdvDialog();

// Dialog Data
	enum { IDD = IDD_ADVBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
