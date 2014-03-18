/*
   C fnctions for Win32.
   Copyright(C) Isao Hara
*/


#ifndef __ROBOTSIM_CUTIL__
#define __ROBOTSIM_CUTIL__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#ifdef WIN32
#include <sys/types.h>
#include <sys/timeb.h>
#include <process.h>
#include <windows.h>

#else
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <ctype.h>

#endif

/**** Mutex ****/

#ifdef WIN32
#define DEV_STR      "COM%d"
#define SSCANF      sscanf_s
#define SPRINTF      sprintf_s
#define THREAD_FUNC    void
#define H_NULL      NULL
#define THREAD_HANDLE  HANDLE
#define MUTEX_HANDLE  HANDLE
#define StrDup(s)    _strdup(s)

void Pthread_Mutex_Init(HANDLE *hM, void *arg);
int Pthread_Create(HANDLE *hT, void *attr,void (*func)(void *), void *arg);
void Pthread_Detach(HANDLE hT);
void Pthread_Mutex_Destroy(HANDLE *hM);
void Pthread_Join(HANDLE hT, void *thread_return);
void Pthread_Exit(HANDLE hT);

#define PROJECT_DIR "..\\share\\projects\\"

#else
#define HANDLE      int
#define DEV_STR      "/dev/ttyS%d"
#define SSCANF      sscanf
#define SPRINTF      sprintf
#define Sleep(n)    usleep(n*1000)
#define THREAD_FUNC    void *
#define H_NULL      0
#define THREAD_HANDLE  pthread_t
#define MUTEX_HANDLE  pthread_mutex_t
#define StrDup(s)    strdup(s)

#define Pthread_Mutex_Init pthread_mutex_init
#define Pthread_Create  pthread_create
#define Pthread_Detach  pthread_detach
#define Pthread_Mutex_Destroy  pthread_mutex_destroy
#define Pthread_Join  pthread_join
#define Pthread_Exit(x)  pthread_exit((void *)x)

#define PROJECT_DIR "../share/projects/"
#endif


int getCurrentTime();

#ifdef __cplusplus
};
#endif

#endif