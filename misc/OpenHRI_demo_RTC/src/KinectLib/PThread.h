/*
  Pthread wrapper
  Copyright(C) 2011, Isao HARA, AIST,Japan

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  1.Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  2.Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
