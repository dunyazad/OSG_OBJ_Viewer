
// OSG_OBJ_ViewerDlg.h : header file
//

#pragma once

#include "OpenSceneGraph.h"


// OpenSceneGraph_OBJ_ViewerDlg dialog
class OpenSceneGraph_OBJ_ViewerDlg : public CDialogEx
{
// Construction
public:
	OpenSceneGraph_OBJ_ViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OSG_OBJ_VIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	OpenSceneGraph* m_pOSG;
	CRenderingThread* mThreadHandle;

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};
