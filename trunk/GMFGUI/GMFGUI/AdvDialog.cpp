// AdvDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GMFGUI.h"
#include "AdvDialog.h"
#include "afxdialogex.h"


// CAdvDialog dialog

IMPLEMENT_DYNAMIC(CAdvDialog, CDialogEx)

CAdvDialog::CAdvDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdvDialog::IDD, pParent)
{

}

CAdvDialog::~CAdvDialog()
{
}

void CAdvDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdvDialog, CDialogEx)
END_MESSAGE_MAP()


// CAdvDialog message handlers
