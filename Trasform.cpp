#include "stdafx.h"
#include "trasform.h"
#include "const.h"
#include "resource.h"		// main symbols
#include "AltranceDlg.h"

CAltranceDlg* dlgTh;

UINT TransformThreadProc(LPVOID pParam)
{
	CFile file1, file2; //1-IN 2-OUT
	CFileException e;
	CFileStatus status;
	CString str;
	LPSTR lpszMsg;
	int iYesNo;
//	LPSTR text;
	char ch[50];
	BOOL bTrans; //jestli transformace byla uspesna

	if (dlgTh == NULL)
		return -1;

	//prevod
	bTrans = TRUE;
//	dlgTh->m_control_prubeh.EnableWindow(TRUE);
	dlgTh->m_control_pru.EnableWindow(TRUE);
	dlgTh->m_btGroupPrubeh.EnableWindow(TRUE);
	dlgTh->m_btOK.EnableWindow(FALSE);

	lpszMsg = (char*)calloc (STRTEXT, sizeof(char));

	if (!file1.Open(dlgTh->m_strIN/*strINTh*/, CFile::modeRead, &e))
	{
		str.LoadString(IDS_ER_OPEN_FILE);
		wsprintf(lpszMsg, str, dlgTh->m_strIN);
		MessageBox(NULL, lpszMsg, NULL, MB_ICONERROR| MB_OK);
		dlgTh->m_btGroupPrubeh.EnableWindow(FALSE);
//		dlgTh->m_control_prubeh.EnableWindow(FALSE);
		dlgTh->m_control_pru.EnableWindow(FALSE);
		dlgTh->m_btOK.EnableWindow(TRUE);
		delete(lpszMsg);
		return 1;
	}

	if (file2.Open(dlgTh->m_strOUT, CFile::modeRead, &e))
	{
		//dataz na prepsani OUT souboru
		str.LoadString(IDS_OPEN_FILE);
		wsprintf(lpszMsg, str, dlgTh->m_strOUT);
		iYesNo = MessageBox(NULL, lpszMsg, NULL, MB_ICONQUESTION | MB_YESNO);
		if (iYesNo == IDNO)
		{
			file2.Close();
			file1.Close();
			dlgTh->m_btGroupPrubeh.EnableWindow(FALSE);
//			dlgTh->m_control_prubeh.EnableWindow(FALSE);
			dlgTh->m_control_pru.EnableWindow(FALSE);
			dlgTh->m_btOK.EnableWindow(TRUE);
			delete(lpszMsg);
			return 1;
		}
		file2.Close();
	}
	//YES nebo soubor neexistoval
	if (!file2.Open(dlgTh->m_strOUT, CFile::modeCreate|CFile::modeWrite, &e))
	{
		str.LoadString(IDS_ER_CREATE_FILE);
		wsprintf(lpszMsg, str, dlgTh->m_strOUT);
		MessageBox(NULL, lpszMsg, NULL, MB_ICONERROR| MB_OK);
		dlgTh->m_btGroupPrubeh.EnableWindow(FALSE);
//		dlgTh->m_control_prubeh.EnableWindow(FALSE);
		dlgTh->m_control_pru.EnableWindow(FALSE);
		dlgTh->m_btOK.EnableWindow(TRUE);
		delete(lpszMsg);
		return 1;
	}

//	text = (char*)calloc (STRINFO, sizeof(char));

	//jmeno IN
	str.LoadString(IDS_TEXT_FILE1);
	wsprintf(lpszMsg, str, dlgTh->m_strIN);
//	strcpy(text, lpszMsg);
	dlgTh->AddToListBox(lpszMsg);
	//velikost
	file1.GetStatus(status);
	ltoa(status.m_size,ch,10);
	str.LoadString(IDS_TEXT_SIZE);
	wsprintf(lpszMsg, str, ch);
//	strcat(text, lpszMsg);
	dlgTh->AddToListBox(lpszMsg);
//	dlgTh->m_statPrubeh = text;

	switch(dlgTh->m_iTypPrevodu) 
	{
		case BIN_CPP:
			PrevodDoCpp(&file1, &file2, status);
		break;
		case CPP_BIN:
			bTrans = PrevodDoBin(&file1, &file2, status);
		break;
		default: //nejspis nidy nenastane
			PrevodDoCpp(&file1, &file2, status);
		break;
	}
	//jmeno OUT
	str.LoadString(IDS_TEXT_FILE2);
	wsprintf(lpszMsg, str, dlgTh->m_strOUT);
//	strcat(text, lpszMsg);
	dlgTh->AddToListBox(lpszMsg);
	//velikost
	file2.GetStatus(status);
	ltoa(status.m_size,ch,10);
	str.LoadString(IDS_TEXT_SIZE);
	wsprintf(lpszMsg, str, ch);
//	strcat(text, lpszMsg);
	dlgTh->AddToListBox(lpszMsg);
	if (bTrans)
	{
		str.LoadString(IDS_TEXT_END);
	}
	else
	{
		str.LoadString(IDS_TEXT_END_ER);
	}
//	strcat(text, str);
//	dlgTh->m_statPrubeh = text;
	dlgTh->AddToListBox(str);

	file2.Close();
	file1.Close();

	delete(lpszMsg);
//	delete(text);
	dlgTh->m_btOK.EnableWindow(TRUE);

	return 0;
}

//prevod z Cpp do Bin
//perdpoklada, ze obsahuje stejne stringy jako cpp vytvoreny timto programem
BOOL PrevodDoBin(CFile *file1, CFile *file2, CFileStatus status)
{
	BOOL bNavrat;
	CString str;
	char ch[2];
	LPSTR slovo,file_naz;
	int iPorov;
	BOOL bKonec;
	UINT nBytesRead;
	DWORD dwPosition;

	bNavrat = TRUE;
	ch[1] = 0;

	//BYTE
	do
	{
		slovo = NactiSlovo(file1, &bKonec);
		iPorov = strcmp(slovo,CPP_BYTE);
	} while((iPorov != 0)&&(!bKonec));

	//konec souboru, rozhodne pokud tam BYTE je, tak za nim neni nic a neni co prevadet
	if(bKonec)
	{
		KonecFile();
		delete(slovo);
		return FALSE;
	}

	//za BYTE je nazev
	slovo = NactiSlovo(file1, &bKonec);
	//kontorla jestli nazev v souboru je shodny s nazvem OUT souboru,
	//pokud tomu tak neni jen upozorni na neshodnost
	file_naz = OdstranPriponuACestu((LPCTSTR)dlgTh->m_strIN);
	iPorov = stricmp(slovo,file_naz);
	if(iPorov!=0)
	{
		str.LoadString(IDS_INFO_FILE);
		MessageBox(NULL, str, NULL, MB_ICONINFORMATION| MB_OK);
	}

	//=
	do
	{
		slovo = NactiSlovo(file1, &bKonec);
		iPorov = strcmp(slovo,CPP_ROVNASE);
	} while((iPorov != 0)&&(!bKonec));
	if(bKonec)
	{
		KonecFile();
		delete(file_naz);
		delete(slovo);
		return FALSE;
	}

	//{ - zacatek dat
	do
	{
		nBytesRead = file1->Read(ch,1);
		iPorov = strcmp(ch,CPP_L_ZAV);
	}while((iPorov != 0)&&(nBytesRead>0));
	if(nBytesRead == 0)
	{
		KonecFile();
		delete(file_naz);
		delete(slovo);
		return FALSE;
	}

	//nacitani jednotlivych bytu
	do
	{
		slovo = NactiSlovo(file1, &bKonec);
		if (!bKonec)
		{
			UlozByte(slovo, file2);
		}
		delete(slovo);
		dwPosition = file1->GetPosition();
		dlgTh->m_progress.SetPos((int)((__int64)dwPosition * 100 / status.m_size));//progressbar
	} while(!bKonec);

	delete(file_naz);
//	delete(slovo);

	dlgTh->m_progress.SetPos(0);//progressbar

	return bNavrat;
}

//prevod z Bin do Cpp
void PrevodDoCpp(CFile *file1, CFile *file2, CFileStatus status)
{
	LPSTR lpsz,file_naz;
	long i;
	int j;
	char ch[1];
	char chZnak[3];
	char text[STR];

	lpsz = (char*)calloc (STRTEXT, sizeof(char));

	file_naz = OdstranPriponuACestu((LPCTSTR)dlgTh->m_strIN);

	//1. a 2. radek
	wsprintf(lpsz, CPP_1LINE, file_naz, status.m_size);
	file2->Write(lpsz,strlen(lpsz));
	file2->Write(CPP_2LINE,strlen(CPP_2LINE));
	//stred - data
	chZnak[2] = 0; //kvuli problemum se zakoncenim stringu kdy se na prvni misto vklada 0
	for (i = 0; i < status.m_size; )
	{
		strncpy(text,"",STR);
		strcat(text, CPP_TAB);
		for (j = 0; (j < 16)&&(i < status.m_size); i++,j++)
		{
			file1->Read(ch,1);
			itoa(ch[0]&0xff,chZnak,16);//&0xff - aby sebyla zaporna cisla
			strcat(text, CPP_0X);
			//pridani 0 pred kratky char
			if (strlen(chZnak)==1)
			{
				chZnak[1] = chZnak[0];
				chZnak[0] = '0';
			}
			strcat(text, chZnak);
			if (i < status.m_size-1)
			{
				strcat(text, CPP_CARKA_);
			}
		}
		strcat(text, CPP_ENTER);
		file2->Write(text,strlen(text));
		dlgTh->m_progress.SetPos((int)((__int64)i * 100 / status.m_size));//progressbar
	}
	//zakonceni
	file2->Write(CPP_3LINE,strlen(CPP_3LINE));

	dlgTh->m_progress.SetPos(0);//progressbar

	delete(lpsz);
	delete(file_naz);
}

//pri odvodi nazev pro soubor OUT
LPSTR OdstranPriponuACestu(LPCTSTR file)
{
	LPSTR str;
	int i,i_tec,i_lom;
	int len;

	str = (char*)calloc (MAX_PATH, sizeof(char));
	len = strlen(file);

	for (i = len-1; i >= 0; i--)
	{
		if (file[i] == '.')
		{
			i_tec = i;
		}
		if (file[i] == 92) // \otocene lomitko
		{
			i_lom = i;
			break;
		}
	}

	strxfrm(str,file,i_tec);

	for (i = 0; i < (i_tec-(i_lom+1)); i++)
	{
		str[i] = str[i+i_lom+1];
	}
	str[i] = 0;

	return str;
}

//nacte ze souboru jedno slovo, oddelene oddelovaci nebo zacatkem a koncem souboru
//bKonec - TRUE - konec file1; FALSE - neni konec
LPSTR NactiSlovo(CFile *file1, BOOL *bKonec)
{
	char ch[2];
	UINT nBytesRead;
//	char text[STR];
	LPSTR str;
	char *test;
	int i;

	str = (char*)calloc (STR, sizeof(char)); //dalka 200 znaku

	ch[1] = 0;
	i = 0;
	do
	{
		nBytesRead = file1->Read(ch,1);
		test = strpbrk(ch,CPP_ODDELOVACE);//vrati ty znaky ktere jsou zaroven v ch a v CPP_ODDELOVACE
		if ( (test == 0)&&(nBytesRead>0) )
		{
			str[i] = ch[0];
			i++;
		}
	}while((nBytesRead>0) && ( ((i==0)&&(test!=0)) || ((i!=0)&&(test==0)) ) );

	*bKonec = FALSE;
	if (nBytesRead==0)
	{
		*bKonec = TRUE;
	}

	return str;
}

//ulozi Byte
void UlozByte(LPSTR slovo, CFile *file)
{
	int i, len;
	char *stopstring;
	long l;

	len = strlen(slovo); //melo by byt 4

	for (i=0; i<=(len-2); i++)
	{
		slovo[i] = slovo[i+2];
	}
	l = strtol(slovo, &stopstring, 16);

	file->Write(&l,1);
}

//vypise hlaseni ze se doslo na konec souboru a trasformace z toho duvodu neni uspesna
void KonecFile()
{
	CString str;

	str.LoadString(IDS_ER_DATA);
	MessageBox(NULL, str, NULL, MB_ICONINFORMATION| MB_OK);
}
