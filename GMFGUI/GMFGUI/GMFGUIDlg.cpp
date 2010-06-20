
// GMFGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GMFGUI.h"
#include "GMFGUIDlg.h"
#include "AdvDialog.h"
#include "afxdialogex.h"
#include "decompilerMain.h"
#include "compilerMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CListBox* theLog; 

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAboutDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGMFGUIDlg dialog




CGMFGUIDlg::CGMFGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGMFGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_inputFileName = _T("");
	m_outputFileName = _T("");
}

void CGMFGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITINPUT, m_inputFileName);
	DDV_MaxChars(pDX, m_inputFileName, 512);
	DDX_Text(pDX, IDC_EDITOUTPUT, m_outputFileName);
	DDV_MaxChars(pDX, m_outputFileName, 512);
	DDX_Control(pDX, IDC_BUTTONCOMPILE, m_ButtonCompile);
	DDX_Control(pDX, IDC_BUTTONRA1, m_ButtonRA1);
	DDX_Control(pDX, IDC_LOG, m_LogList);
}

BEGIN_MESSAGE_MAP(CGMFGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGMFGUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONABOUT, &CGMFGUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTONBROWSEI, &CGMFGUIDlg::OnClickedButtonBrowseI)
	ON_BN_CLICKED(IDC_BUTTONBROWSEO, &CGMFGUIDlg::OnClickedButtonBrowseO)
	ON_BN_CLICKED(IDC_BUTTONRA1, &CGMFGUIDlg::OnBnClickedButtonra1)
	ON_COMMAND(IDC_RADIO1, &CGMFGUIDlg::OnRadio1)
	ON_COMMAND(IDC_RADIO2, &CGMFGUIDlg::OnRadio2)
	ON_BN_CLICKED(IDC_BUTTONCOMPILE, &CGMFGUIDlg::OnBnClickedButtonCompile)
END_MESSAGE_MAP()


// CGMFGUIDlg message handlers

BOOL CGMFGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	setDecompilingMode(false);
	theLog = &m_LogList;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGMFGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGMFGUIDlg::OnPaint()
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
HCURSOR CGMFGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGMFGUIDlg::OnBnClickedOk()
{
	MessageBox(L"This is some fun stuff!", L"HELL YEAH", NULL);
}


void CGMFGUIDlg::OnBnClickedButton3()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CGMFGUIDlg::getSaveNameForOpen(LPWSTR openFileName)
{
	int length = wcslen(openFileName);
	if (!decompilingMode)
	{
		if (!wcsncmp(openFileName + length - 8, L"_txt.gmf", 8))
		{
			LPWSTR substring = (LPWSTR)malloc(sizeof(WCHAR) * length - 7);
			wcsncpy_s(substring, length-7, openFileName, length-8);
			swprintf_s(outputFileName, 512, L"%s.gmf", substring);
			m_outputFileName = outputFileName;
			free(substring);
		}
	}
	else
	{
		if (!wcsncmp(openFileName + length - 4, L".gmf", 4))
		{
			LPWSTR substring = (LPWSTR)malloc(sizeof(WCHAR) * length - 3);
			wcsncpy_s(substring, length-3, openFileName, length-4);
			swprintf_s(outputFileName, 512, L"%s_txt.gmf", substring);
			m_outputFileName = outputFileName;
			free(substring);
		}
	}
}

void CGMFGUIDlg::OnClickedButtonBrowseI()
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrFile = inputFileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(inputFileName);
	ofn.lpstrFilter = L"All Files (*.*)\0*.*\0GMF Files (*.gmf)\0*.gmf\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
		m_inputFileName = inputFileName;

	getSaveNameForOpen(inputFileName);

	UpdateData(FALSE);
}


void CGMFGUIDlg::OnClickedButtonBrowseO()
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrFile = outputFileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(outputFileName);
	ofn.lpstrFilter = L"All Files (*.*)\0*.*\0GMF Files (*.gmf)\0*.gmf\0\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = 0;
	ofn.lpstrDefExt = L"gmf";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
		m_outputFileName = outputFileName;
	UpdateData(FALSE);
}

void CGMFGUIDlg::setDecompilingMode(bool active)
{
	if (active)
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		m_ButtonCompile.SetWindowTextW(L"Decompile");
		m_ButtonRA1.ShowWindow(SW_SHOW);
	}
	else
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		m_ButtonCompile.SetWindowTextW(L"Compile");
		m_ButtonRA1.ShowWindow(SW_HIDE);
	}
	
	//swap boxes
	if(decompilingMode != active)
	{
		CString tempOutput = m_outputFileName;
		m_outputFileName = m_inputFileName;
		m_inputFileName = tempOutput;
		UpdateData(FALSE);
	}

	decompilingMode = active;
}

void CAboutDlg::OnBnClickedButton1()
{
	ShellExecute(NULL, L"open", L"http://code.google.com/p/gmftoolkit/", NULL, NULL, SW_SHOW);
}


void CGMFGUIDlg::OnBnClickedButtonra1()
{
	CAdvDialog advancedWindow;
	advancedWindow.DoModal();
}

void CGMFGUIDlg::OnRadio1()
{
	setDecompilingMode(false);
}


void CGMFGUIDlg::OnRadio2()
{
	setDecompilingMode(true);
}


void CGMFGUIDlg::OnBnClickedButtonCompile()
{
	m_ButtonCompile.EnableWindow(false);

	if (decompilingMode)
		decompile(GetSafeHwnd(), (LPWSTR)m_inputFileName.GetString(), (LPWSTR)m_outputFileName.GetString());
	else
		compile(GetSafeHwnd(), (LPWSTR)m_inputFileName.GetString(), (LPWSTR)m_outputFileName.GetString());

	UpdateData(false);

	m_ButtonCompile.EnableWindow(true);
}


void CAboutDlg::OnBnClickedButton2()
{
	ShellExecute(NULL, L"open", L"http://q3k.org/donate/", NULL, NULL, SW_SHOW);
}
