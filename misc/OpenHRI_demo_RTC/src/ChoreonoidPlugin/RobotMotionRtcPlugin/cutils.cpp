/*
   C fnctions for Win32.
   Copyright(C) Isao Hara
*/

#include "cutils.h"


#ifdef __cplusplus
extern "C" {
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

#ifdef WIN32

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

#else
#define Pthread_Mutex_Init pthread_mutex_init
#define Pthread_Create  pthread_create
#define Pthread_Detach  pthread_detach
#define Pthread_Mutex_Destroy  pthread_mutex_destroy
#define Pthread_Join  pthread_join
//#define Pthread_Exit  pthread_exit

#endif

#ifdef __cplusplus
};
#endif
