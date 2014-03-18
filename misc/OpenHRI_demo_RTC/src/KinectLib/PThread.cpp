/*
  PThread wrapper
  Copyright(C) 2011, Isao HARA, AIST,Japan

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  1.Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  2.Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/

#include "stdafx.h"
#include "PThread.h"

#ifdef __cplusplus
extern "C"{
#endif


int getCurrentTime()
{
#ifdef WIN32
   _timeb tv;
   _ftime_s(&tv);

   return (int)(tv.time * 1000 + tv.millitm);
#else
  struct timeval tv={0,0};
  struct timezone tz={0,9};

  gettimeofday(&tv, &tz);

  return (int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

/******
  PThread Functions
*****/

void Pthread_Mutex_Init(HANDLE *hM, void *arg)
{
  *hM = CreateMutex(NULL, FALSE, NULL);
  return;
}

int Pthread_Create(HANDLE *hT, void *attr,void (*func)(void *), void *arg)
{
  uintptr_t tp;
  tp = _beginthread(func, NULL, arg);
  if(tp == -1L){
     *hT = (HANDLE)NULL;
     return -1;
  }
  *hT = (HANDLE)tp;
  return 0;
}

void Pthread_Detach(HANDLE hT){
  return;
}

void Pthread_Mutex_Destroy(HANDLE *hM)
{
  CloseHandle(*hM);
  return;
}

void Pthread_Join(HANDLE hT, void *thread_return)
{
  WaitForSingleObject(hT,INFINITE);
  return;
}

void Pthread_Exit(HANDLE hT)
{
  CloseHandle(hT);
}

#ifdef __cplusplus
};
#endif
