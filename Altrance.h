// Altrance.h : main header file for the ALTRANCE application
//

#if !defined(AFX_ALTRANCE_H__54DCCEF3_DFAE_4C0F_9410_D980D281E533__INCLUDED_)
#define AFX_ALTRANCE_H__54DCCEF3_DFAE_4C0F_9410_D980D281E533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAltranceApp:
// See Altrance.cpp for the implementation of this class
//

class CAltranceApp : public CWinApp
{
public:
	CAltranceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAltranceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAltranceApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALTRANCE_H__54DCCEF3_DFAE_4C0F_9410_D980D281E533__INCLUDED_)
