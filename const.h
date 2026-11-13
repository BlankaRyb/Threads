//constanty

/*Registry*/
#define COMPANY_NAME        "Local AppWizard-Generated Applications"
#define APPLICATION_NAME    "Altrance"
#define VERSION_NUMBER      "1.0"

/* Typy zobrazovani dat */
#define BIN_CPP         (__int16)0
#define CPP_BIN         (__int16)1

/*delka stringu*/
#define STR             (__int16)200	//IDS_FILE_IN_BIN,IDS_FILE_IN_CPP, text na radku
#define STRTEXT         (__int16)1000	//delka textu pri dotazu (napr. prepsani souboru)
#define STRINFO         (__int16)5000	//delka textu pro zobrazeni informaci o prubehu

/*pripona souboru*/
#define BIN             (LPCTSTR)".bin"
#define CPP             (LPCTSTR)".cpp"

/*konstanty pro prevod*/
#define CPP_1LINE       (LPCTSTR)"BYTE %s[%li] =\15\12"
#define CPP_2LINE       (LPCTSTR)"{\15\12"
#define CPP_3LINE       (LPCTSTR)"};"
#define CPP_0X          (LPCTSTR)"0x"
#define CPP_TAB         (LPCTSTR)"\11"
#define CPP_CARKA_      (LPCTSTR)", "
#define CPP_ENTER       (LPCTSTR)"\15\12"
#define CPP_ODDELOVACE  (LPCTSTR)"\15\12\11 ,;()[]{}"
#define CPP_BYTE        (LPCTSTR)"BYTE"
#define CPP_ROVNASE     (LPCTSTR)"="
#define CPP_L_ZAV       (LPCTSTR)"{"
