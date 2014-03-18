/*
  PThread wrapper
  Copyright(C) 2011, Isao HARA, AIST,Japan

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
