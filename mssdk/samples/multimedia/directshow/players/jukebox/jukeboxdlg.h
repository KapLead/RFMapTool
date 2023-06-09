//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1998 - 2000  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;
// JukeboxDlg.h : header file
//

#if !defined(AFX_JUKEBOXDLG_H__04AD8433_DF22_4491_9611_260EDAE17B96__INCLUDED_)
#define AFX_JUKEBOXDLG_H__04AD8433_DF22_4491_9611_260EDAE17B96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dshow.h>

/////////////////////////////////////////////////////////////////////////////
// CJukeboxDlg dialog

class CJukeboxDlg : public CDialog
{
// Construction
public:
	CJukeboxDlg(CWnd* pParent = NULL);	// standard constructor
    void FillFileList(LPTSTR pszCmdLine);

    HRESULT PrepareMedia(LPTSTR lpszMovie);
    BOOL DisplayFileInfo(LPTSTR szFile);
    HRESULT DisplayFileDuration(void);
    Say(LPTSTR szText);
    TCHAR* CJukeboxDlg::GetDXSDKMediaPath(void);

    HRESULT InitDirectShow(void);
    HRESULT FreeDirectShow(void);
    HRESULT HandleGraphEvent(void);
    void ResetDirectShow(void);
    void DisplayECEvent(long lEventCode, long lParam1, long lParam2);
    void CenterVideo(void);
    void PlayNextFile(void);
    void PlayPreviousFile(void);
    void PlaySelectedFile(void);
    void ShowState(void);
    BOOL CanStep(void);
    HRESULT StepFrame(void);
    HRESULT EnumFilters(void);
    HRESULT EnumPins(IBaseFilter *pFilter, PIN_DIRECTION PinDir, CListBox& Listbox);
    IBaseFilter * FindFilterFromName(LPTSTR szName);
    BOOL SupportsPropertyPage(IBaseFilter *pFilter);

    // Dialog Data
	//{{AFX_DATA(CJukeboxDlg)
	enum { IDD = IDD_JUKEBOX_DIALOG };
	CStatic	m_StrDuration;
	CEdit	m_EditMediaDir;
	CSpinButtonCtrl	m_SpinFiles;
	CButton	m_ButtonFrameStep;
	CListBox	m_ListEvents;
	CButton	m_CheckEvents;
	CButton	m_ButtonProperties;
	CStatic	m_StrMediaPath;
	CButton	m_CheckMute;
	CButton	m_ButtonStop;
	CButton	m_ButtonPlay;
	CButton	m_ButtonPause;
	CButton	m_CheckPlaythrough;
	CButton	m_CheckLoop;
	CStatic	m_StrFileDate;
	CStatic	m_StrFileSize;
	CListBox	m_ListPinsOutput;
	CListBox	m_ListPinsInput;
	CStatic	m_StrFileList;
	CStatic	m_Status;
	CStatic	m_Screen;
	CListBox	m_ListInfo;
	CListBox	m_ListFilters;
	CListBox	m_ListFiles;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJukeboxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    int   m_nCurrentFileSelection;

	// Generated message map functions
	//{{AFX_MSG(CJukeboxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSelectFile();
	afx_msg void OnPause();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnButtonMute();
	afx_msg void OnCheckMute();
	afx_msg void OnCheckLoop();
	afx_msg void OnCheckPlaythrough();
	afx_msg void OnSelchangeListFilters();
	afx_msg void OnDblclkListFilters();
	afx_msg void OnButtonProppage();
	afx_msg void OnCheckEvents();
	afx_msg void OnButtonFramestep();
	afx_msg void OnButtonClearEvents();
	afx_msg void OnDblclkListFiles();
	afx_msg void OnDeltaposSpinFiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSetMediadir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JUKEBOXDLG_H__04AD8433_DF22_4491_9611_260EDAE17B96__INCLUDED_)
