
// OSG_OBJ_ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OSG_OBJ_Viewer.h"
#include "OSG_OBJ_ViewerDlg.h"
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
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// OpenSceneGraph_OBJ_ViewerDlg dialog




OpenSceneGraph_OBJ_ViewerDlg::OpenSceneGraph_OBJ_ViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(OpenSceneGraph_OBJ_ViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void OpenSceneGraph_OBJ_ViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(OpenSceneGraph_OBJ_ViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()


// OpenSceneGraph_OBJ_ViewerDlg message handlers

BOOL OpenSceneGraph_OBJ_ViewerDlg::OnInitDialog()
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
	m_pOSG = new OpenSceneGraph(m_hWnd);
	m_pOSG->Initialize();

	mThreadHandle = new CRenderingThread(m_pOSG);
    mThreadHandle->start();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void OpenSceneGraph_OBJ_ViewerDlg::OnDestroy()
{
	delete mThreadHandle;
    if(m_pOSG != 0) delete m_pOSG;
}

void OpenSceneGraph_OBJ_ViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void OpenSceneGraph_OBJ_ViewerDlg::OnPaint()
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
HCURSOR OpenSceneGraph_OBJ_ViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnKeyDown(nChar, nRepCnt, nFlags);

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnKeyUp(nChar, nRepCnt, nFlags);

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnLButtonDown(nFlags, point);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnLButtonUp(nFlags, point);

	CDialogEx::OnLButtonUp(nFlags, point);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnMButtonDown(nFlags, point);

	CDialogEx::OnMButtonDown(nFlags, point);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnMButtonUp(nFlags, point);

	CDialogEx::OnMButtonUp(nFlags, point);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnRButtonDown(nFlags, point);

	CDialogEx::OnRButtonDown(nFlags, point);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnRButtonUp(nFlags, point);

	CDialogEx::OnRButtonUp(nFlags, point);
}


void OpenSceneGraph_OBJ_ViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnMouseMove(nFlags, point);

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL OpenSceneGraph_OBJ_ViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	m_pOSG->OnMouseWheel(nFlags, zDelta, pt);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
