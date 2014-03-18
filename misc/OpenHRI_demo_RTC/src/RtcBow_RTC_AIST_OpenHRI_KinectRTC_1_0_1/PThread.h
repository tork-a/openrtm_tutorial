/*
  Pthread wrapper
  Copyright(C) 2011, Isao HARA, AIST,Japan

*/
#ifndef __C_UTILS_H__
#define __C_UTILS_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/timeb.h>
#include <process.h>
#include <windows.h>

#ifdef CFUNC_EXPORTS
#define C_DECLSPEC	__declspec(dllexport)
#else
#define C_DECLSPEC  __declspec(dllimport)
#endif


/**** Mutex ****/

#define DEV_STR      "COM%d"
#define SSCANF      sscanf_s
#define SPRINTF      sprintf_s
#define THREAD_FUNC    void
#define H_NULL      NULL
#define THREAD_HANDLE  HANDLE
#define MUTEX_HANDLE  HANDLE
#define StrDup(s)    _strdup(s)


C_DECLSPEC int getCurrentTime();

void Pthread_Mutex_Init(HANDLE *hM, void *arg);
int Pthread_Create(HANDLE *hT, void *attr,void (*func)(void *), void *arg);
void Pthread_Detach(HANDLE hT);
void Pthread_Mutex_Destroy(HANDLE *hM);
void Pthread_Join(HANDLE hT, void *thread_return);
void Pthread_Exit(HANDLE hT);


#ifdef __cplusplus
};
#endif

#endif
