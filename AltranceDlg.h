// AltranceDlg.h : header file
//

#if !defined(AFX_ALTRANCEDLG_H__2C0EFEEE_769A_4A39_A14E_41FF60968A26__INCLUDED_)
#define AFX_ALTRANCEDLG_H__2C0EFEEE_769A_4A39_A14E_41FF60968A26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAltranceDlg dialog

class CAltranceDlg : public CDialog
{
// Construction
public:
	CAltranceDlg(CWnd* pParent = NULL);	// standard constructor
	~CAltranceDlg();

// Dialog Data
	//{{AFX_DATA(CAltranceDlg)
	enum { IDD = IDD_ALTRANCE_DIALOG };
	CListBox	m_control_pru;
	CButton	m_btOK;
	CProgressCtrl	m_progress;
	CButton	m_btGroupPrubeh;
	CString	m_strIN;
	CString	m_strOUT;
	int		m_iTypPrevodu;
	CString	m_statIN;
	CString	m_statOUT;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAltranceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void AddToListBox(LPCTSTR szBuffer);

// Implementation
protected:
	HICON m_hIcon;
	char strINBin[MAX_PATH];
	char strINCpp[MAX_PATH];
	char strOUTBin[MAX_PATH];
	char strOUTCpp[MAX_PATH];
	HKEY hkPerUser;  // Klic od registru

	// Generated message map functions
	//{{AFX_MSG(CAltranceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButHledatIN();
	afx_msg void OnButHledatOUT();
	afx_msg void OnRadioSmer();
	virtual void OnOK();
	afx_msg void OnPrevod();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL CreateAppKeys(); //vytvori klic v regiru
	void SetString(); //nasteveni stringu popisujicich IN a OUT soubory
	BOOL InitUser(PSZ pszPathBuff); //Vytvoreni klice
	BOOL GetRegister(); //nacte z registru cesty a nazvy IN souboru (Bin a Cpp)
	BOOL SetRegister(); //da do registru cesty a nazvy IN souboru (Bin a Cpp)
	void SetOUT(); //nastaveni hodnti v EditBoxu pro OUT
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALTRANCEDLG_H__2C0EFEEE_769A_4A39_A14E_41FF60968A26__INCLUDED_)
