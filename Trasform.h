//Transform.h

class CAltranceDlg;
extern CAltranceDlg* dlgTh;

UINT TransformThreadProc(LPVOID pParam);
BOOL PrevodDoBin(CFile *file1, CFile *file2, CFileStatus status); //prevod z Cpp do Bin
void PrevodDoCpp(CFile *file1, CFile *file2, CFileStatus status); //prevod z Bin do Cpp
LPSTR OdstranPriponuACestu(LPCTSTR file);
LPSTR NactiSlovo(CFile *file1, BOOL *bKonec); //bKonec - TRUE - konec file1; FALSE - neni konec
void UlozByte(LPSTR slovo, CFile *file);
void KonecFile();
