
// testLogitechDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testLogitech.h"
#include "testLogitechDlg.h"
#include "afxdialogex.h"
#include "LogitechSteeringWheelLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(lib, "LogitechSteeringWheelLib.lib")
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


// CtestLogitechDlg dialog



CtestLogitechDlg::CtestLogitechDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTLOGITECH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestLogitechDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListInfo);
}

BEGIN_MESSAGE_MAP(CtestLogitechDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestLogitechDlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CtestLogitechDlg message handlers

BOOL CtestLogitechDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	if (!LogiSteeringInitializeWithWindow(true, m_hWnd))
	{
		MessageBox(_T("初始化Logitech窗口失败!"), _T("警告"), MB_OK | MB_ICONQUESTION);
	}
	SetTimer(1,10, NULL);

	CString cstr = _T("");
	for (size_t i = 0; i < 10; i++) {
		m_ListInfo.DeleteString(i);
		m_ListInfo.InsertString(i, cstr);
		m_ListInfo.SetCurSel(i);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CtestLogitechDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestLogitechDlg::OnPaint()
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
HCURSOR CtestLogitechDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestLogitechDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

}


void CtestLogitechDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
	static int i=0;
	char sBuffer_[100];
	bool temp;
	i++;

	//a = LogiSteeringInitializeWithWindow(true, m_hWnd);

	if (!LogiUpdate())
	{
		temp = LogiSteeringInitializeWithWindow(true, m_hWnd);
		return;
	}
	if (!LogiIsConnected(0))
	{
		return;
	}
	//GetState
	DIJOYSTATE2 *m_DIJoyState2Device[1];
	m_DIJoyState2Device[0] = LogiGetState(0);

	int steerInput, brakeInput, throttleInput, clutchInput;
	steerInput = m_DIJoyState2Device[0]->lX;
	brakeInput = m_DIJoyState2Device[0]->lY;
	throttleInput = m_DIJoyState2Device[0]->rglSlider[0];
	clutchInput = m_DIJoyState2Device[0]->lRz;

	steerInput = steerInput > 32767 ? 32767 : steerInput;
	brakeInput = brakeInput > 32767 ? 32767 : brakeInput;
	throttleInput = throttleInput > 32767 ? 32767 : throttleInput;
	clutchInput = clutchInput < -32768 ? -32768 : clutchInput;

	sprintf_s(sBuffer_, "steer:%d,brake:%d,throttle:%d,clutch:%d", steerInput, brakeInput, throttleInput, clutchInput);
	CString cstr2;
	cstr2.Format(_T("%s"), CString(sBuffer_));
	m_ListInfo.DeleteString(2);
	m_ListInfo.InsertString(2, cstr2);
	UpdateData(false);
}