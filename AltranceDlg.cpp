// AltranceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Altrance.h"
#include "AltranceDlg.h"
#include "trasform.h"

#include "const.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAltranceDlg dialog

CAltranceDlg::CAltranceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAltranceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAltranceDlg)
	m_strIN = _T("");
	m_strOUT = _T("");
	m_iTypPrevodu = -1;
	m_statIN = _T("");
	m_statOUT = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	strINBin[0]  = 0;
	strINCpp[0]  = 0;
	strOUTBin[0] = 0;
	strOUTCpp[0] = 0;
	hkPerUser = NULL;
}

CAltranceDlg::~CAltranceDlg()
{
}

void CAltranceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAltranceDlg)
	DDX_Control(pDX, IDC_DATABOX, m_control_pru);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_STATIC_GROUP_PRUBEH, m_btGroupPrubeh);
	DDX_Text(pDX, IDC_EDIT_IN, m_strIN);
	DDX_Text(pDX, IDC_EDIT_OUT, m_strOUT);
	DDX_Radio(pDX, IDC_RADIO_SMER, m_iTypPrevodu);
	DDX_Text(pDX, IDC_STATIC_IN, m_statIN);
	DDX_Text(pDX, IDC_STATIC_OUT, m_statOUT);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAltranceDlg, CDialog)
	//{{AFX_MSG_MAP(CAltranceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_HLEDAT_IN, OnButHledatIN)
	ON_BN_CLICKED(IDC_BUT_HLEDAT_OUT, OnButHledatOUT)
	ON_BN_CLICKED(IDC_RADIO_SMER, OnRadioSmer)
	ON_BN_CLICKED(IDC_PREVOD, OnPrevod)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_RADIO2, OnRadioSmer)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAltranceDlg message handlers

BOOL CAltranceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	//registry
    if (!CreateAppKeys())
    {
		CString strMsg;
		strMsg.LoadString(IDS_ER_REG);
		MessageBox(strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return TRUE;
    }

	//Typ Perveodu
	m_iTypPrevodu = BIN_CPP;

	//nacte z registru cesty a nazvy IN souboru
	if (!GetRegister())
	{
		CString strMsg;
		strMsg.LoadString(IDS_ER_GET_REGFILE);
		MessageBox(strMsg, NULL, MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		m_strIN = strINBin;
		//vypneni editboxu pro OUT
		SetOUT();
	}

	//staticke texty - stringy
	SetString();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAltranceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAltranceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAltranceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAltranceDlg::OnButHledatIN() 
{
	CString strFiltr,strNadpis;

//	UpdateData(TRUE);

	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			strFiltr.LoadString(IDS_BIN_FILTER);//filtr
			strNadpis.LoadString(IDS_BIN);//nadpis
		break;
		case CPP_BIN:
			strFiltr.LoadString(IDS_CPP_FILTER);//filtr
			strNadpis.LoadString(IDS_CPP);//nadpis
		break;
		default: //nejspis nidy nenastane
			strFiltr.LoadString(IDS_BIN_FILTER);//filtr
			strNadpis.LoadString(IDS_BIN);//nadpis
		break;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFiltr);

	dlg.m_ofn.lpstrTitle = strNadpis;

	if (dlg.DoModal() != IDOK)
	{
		return;//zruseni
	}

	//OK
	m_strIN = dlg.GetPathName();

	//dani do promenych
	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			strcpy(strINBin,(LPCTSTR)m_strIN);
		break;
		case CPP_BIN:
			strcpy(strINCpp,(LPCTSTR)m_strIN);
		break;
		default: //nejspis nidy nenastane
			strcpy(strINBin,(LPCTSTR)m_strIN);
		break;
	}

	//vypneni radku (OUT) a dani do promenych OUT
	if (m_strIN != "")
	{
		SetOUT();
	}

	UpdateData(FALSE);
}

void CAltranceDlg::OnButHledatOUT() 
{
	CString strFiltr,strNadpis;

	UpdateData(TRUE);

	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			strFiltr.LoadString(IDS_CPP_FILTER);//filtr
			strNadpis.LoadString(IDS_CPP);//nadpis
		break;
		case CPP_BIN:
			strFiltr.LoadString(IDS_BIN_FILTER);//filtr
			strNadpis.LoadString(IDS_BIN);//nadpis
		break;
		default: //nejspis nidy nenastane
			strFiltr.LoadString(IDS_CPP_FILTER);//filtr
			strNadpis.LoadString(IDS_CPP);//nadpis
		break;
	}

	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFiltr);

	dlg.m_ofn.lpstrTitle = strNadpis;
	dlg.m_ofn.lpstrFile = m_strOUT.GetBuffer(_MAX_PATH);

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	//OK
	m_strOUT = dlg.GetPathName();
	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			strcpy(strOUTCpp,(LPCTSTR)m_strOUT);
		break;
		case CPP_BIN:
			strcpy(strOUTBin,(LPCTSTR)m_strOUT);
		break;
		default: //nejspis nidy nenastane
			strcpy(strOUTCpp,(LPCTSTR)m_strOUT);
		break;
	}

	UpdateData(FALSE);
}

void CAltranceDlg::OnRadioSmer() 
{
	UpdateData(TRUE);
	SetString();
	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			m_strIN  = strINBin;
			m_strOUT = strOUTCpp;
		break;
		case CPP_BIN:
			m_strIN  = strINCpp;
			m_strOUT = strOUTBin;
		break;
		default: //nejspis nidy nenastane
			m_strIN  = strINBin;
			m_strOUT = strOUTCpp;
		break;
	}
	//vypneni editboxu pro OUT
	if (m_strOUT == "")
	{
		SetOUT();
	}
	UpdateData(FALSE);
}

//nasteveni stringu popisujicich IN a OUT soubory
void CAltranceDlg::SetString()
{
	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			m_statIN.LoadString (IDS_STRING_IN_BIN);
			m_statOUT.LoadString(IDS_STRING_OUT_CPP);
		break;
		case CPP_BIN:
			m_statIN.LoadString (IDS_STRING_IN_CPP);
			m_statOUT.LoadString(IDS_STRING_OUT_BIN);
		break;
		default: //nejspis nidy nenastane
			m_statIN.LoadString (IDS_STRING_IN_BIN);
			m_statOUT.LoadString(IDS_STRING_OUT_CPP);
		break;
	}
}

void CAltranceDlg::OnOK() 
{
	if (!SetRegister())
	{
		CString strMsg;
		strMsg.LoadString(IDS_ER_REGFILE);
		MessageBox(strMsg, NULL, MB_ICONINFORMATION | MB_OK);
	}
	
	CDialog::OnOK();
}

//vytvori klic v regiru nebo jej nacte
BOOL CAltranceDlg::CreateAppKeys()
{
	long lResult;
	TCHAR abPathBuffer[MAX_PATH];
	BOOL fSuccess;
	DWORD cbData;

	BOOL  fInstalled= FALSE;

	wsprintf(abPathBuffer, "Software\\%s\\%s\\%s", COMPANY_NAME, APPLICATION_NAME, VERSION_NUMBER);

	lResult= ~ERROR_SUCCESS;
	while (lResult != ERROR_SUCCESS)
	{
		lResult= RegOpenKeyEx(HKEY_CURRENT_USER, abPathBuffer, 0, KEY_ALL_ACCESS, &hkPerUser);

		cbData= sizeof(fInstalled);

		if ( ERROR_SUCCESS != lResult)
		{

			hkPerUser= NULL;

			//klic neni zde se vytvori
			fSuccess= InitUser(abPathBuffer);

			if (!fSuccess)
			{
				CString strMsg;
				strMsg.LoadString(IDS_ER_REG);
				MessageBox(strMsg, NULL, MB_ICONINFORMATION | MB_OK);
				return fSuccess;
			}
		}
	}

	return TRUE;
}

BOOL CAltranceDlg::InitUser(PSZ pszPathBuff)
{
	HKEY hkPerUser  = NULL;

	LONG lResult;
	DWORD dwDisposition;

	lResult= RegCreateKeyEx(HKEY_CURRENT_USER, pszPathBuff, NULL, "", REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hkPerUser, &dwDisposition);

	if (lResult != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

//nacte z registru, cesty a nazvy souboru pro radek IN, jedno pro Cpp a jedno pro Bin
BOOL CAltranceDlg::GetRegister()
{
	LONG  lResult;
	DWORD dwTypeBin, cbDataBin, dwTypeCpp, cbDataCpp;
    CHAR  chBuffer[STR];

	//BIN
	LoadString(NULL,IDS_FILE_IN_BIN,chBuffer,sizeof(chBuffer));
	lResult= RegQueryValueEx(hkPerUser, chBuffer, NULL, &dwTypeBin,
			(LPBYTE) strINBin, &cbDataBin);

	if (lResult != ERROR_SUCCESS)
		return FALSE;

	//CPP
	LoadString(NULL,IDS_FILE_IN_CPP,chBuffer,sizeof(chBuffer));
	lResult= RegQueryValueEx(hkPerUser, chBuffer, NULL, &dwTypeCpp,
			(LPBYTE) strINCpp, &cbDataCpp);

	return (lResult == ERROR_SUCCESS)? TRUE : FALSE;
}

//ulozeni do registru, ulozi cesty a nazvy souboru z radku IN jedno pro Cpp a jedno pro Bin
BOOL CAltranceDlg::SetRegister()
{
	LONG  lResult;
	LONG  cbData;
    CHAR  chBuffer[STR];

	//BIN
	cbData = strlen(strINBin);
	LoadString(NULL,IDS_FILE_IN_BIN,chBuffer,sizeof(chBuffer));
	lResult= RegSetValueEx(hkPerUser, chBuffer, 0, REG_SZ,
			(LPBYTE) strINBin, cbData);

	if (lResult != ERROR_SUCCESS)
		return FALSE;

	//CPP
	cbData = strlen(strINCpp);
	LoadString(NULL,IDS_FILE_IN_CPP,chBuffer,sizeof(chBuffer));
	lResult= RegSetValueEx(hkPerUser, chBuffer, 0, REG_SZ,
			(LPBYTE) strINCpp, cbData);

	return (lResult == ERROR_SUCCESS)? TRUE : FALSE;
}

//odvodi nazev pro soubor OUT
void CAltranceDlg::SetOUT()
{
	LPCTSTR szFile;
	int i;
	int len;

	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			szFile = strINBin;
		break;
		case CPP_BIN:
			szFile = strINCpp;
		break;
		default: //nejspis nidy nenastane
			szFile = strINBin;
		break;
	}

	len = strlen(szFile);

	//kontrola jestli u aktualniho IN souboru neni jen ""
	if (len <= 0)
	{
		return;
	}

	for (i = len-1; i >= 0; i--)
	{
		if (szFile[i] == '.')
		{
			break;
		}
	}
	switch(m_iTypPrevodu) 
	{
		case BIN_CPP:
			strxfrm(strOUTCpp,szFile,i);
			strOUTCpp[i] = 0;
			strcat(strOUTCpp,CPP);
			m_strOUT = strOUTCpp;
		break;
		case CPP_BIN:
			strxfrm(strOUTBin,szFile,i);
			strOUTBin[i] = 0;
			strcat(strOUTBin,BIN);
			m_strOUT = strOUTBin;
		break;
		default: //nejspis nidy nenastane
			strxfrm(strOUTCpp,szFile,i);
			strOUTCpp[i+1] = 0;
			m_strOUT = strOUTCpp;
		break;
	}
}

void CAltranceDlg::OnPrevod() 
{
	CWinThread* Thread;

	UpdateData(TRUE);
	dlgTh = this;
	Thread = AfxBeginThread(TransformThreadProc, GetSafeHwnd(), THREFAD_PRIORITY_NORMAL);
	Thread->ResumeThread();
}

void CAltranceDlg::AddToListBox(LPCTSTR szBuffer)
{
	CListBox* pBox = (CListBox*) GetDlgItem(IDC_DATABOX);
	ASSERT(pBox != NULL);

	if (pBox != NULL)
	{
		int x = pBox->AddString(szBuffer);
		pBox->SetCurSel(x);
	}
}

BOOL CAltranceDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//	m_control_prubeh.UpdateData(FALSE);
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
