/*
   CUtils.cpp

   Copyright(C) 2011, Isao HARA, AIST,Japan

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
	are permitted provided that the following conditions are met:

      Redistributions of source code must retain the above copyright notice, 
	    this list of conditions and the following disclaimer.
      Redistributions in binary form must reproduce the above copyright notice, 
	    this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.
      Neither the name of the AIST,Japan nor the names of its contributors may be
	    used to endorse or promote products derived from this software without
		specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
	OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "stdafx.h"
#include "CUtils.h"

#ifdef __cplusplus
extern "C"{
#endif

/*************
   Packet Operation
*************/
/*
  convert char to short.
*/
short convC2S(unsigned char *data)
{
  union{
    short s;
    unsigned char c[2];
  } conv;

  conv.c[0]  = data[0];
  conv.c[1]  = data[1] ;

  return conv.s;
}

/*
   Calculate check sum for servo-motor.
*/
char calcSum(char *data, int len)
{
  int i;
  char res;
 
  if(len < 1) return -1;
  res = data[0];
  for(i=1;i<len;i++){
    res = res ^ data[i];
  }
  return res;
}


/***
    get number from string.
**/
char *getNumber(char *str, short *val)
{
  int isVal = 0;
  short v = 0;

  while(*str){
     if(isspace(*str)){
       if(isVal){
         *val = v * isVal;
         return str;
       }
       str++;
       continue;
     }
     if(*str == '-'){
       if(isVal){
         return str;
       }
       isVal = -1;
       str++;
       continue;
     }
     if(isdigit(*str) ){
       if(isVal == 0) isVal=1;
        v = v *10 + (*str -0x30); 
     }
     str++;
  }
  return NULL;
}


char *getDoubleNumber(char *str, double *val)
{
  int isVal = 0;
  int decimalPoint = 0;
  double v = 0;

  while(*str){
    if(isspace(*str)){
       str++;
       continue;
  }else if(strchr("0123456789-" , (int)(*str))){
    break;
  }else{
    fprintf(stderr,  "Invalid format: %s\n", str);
    return str+1;
  }
  }

  while(*str)
  {
     if(isspace(*str))
   {
       if(isVal){
         *val = v * isVal;
         return str;
       }
       str++;
       continue;
     }

     if(*str == '-')
   {
       if(isVal){
         return str;
       }
       isVal = -1;
       str++;
       continue;
     }

   if(*str == '.')
   {
       if(isVal && decimalPoint)
     {
         return str;
       }
       decimalPoint = 1;
       str++;
       continue;
     }

     if(isdigit(*str) )
   {
     if(isVal == 0){
     isVal=1;
     }
     if(decimalPoint == 0){
         v = v *10 + (*str -0x30);
     }else{
       v = v + (*str -0x30) * 0.1 * decimalPoint;
     decimalPoint++;
     }
     }
     str++;
  }
  return NULL;
}


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


bool FileExists(const char* fname, const char *ext)
{
	FILE *fp;
	bool res = false;
	char filename[256];

	strcpy_s(filename,fname);
	strcat_s(filename, ".");
	strcat_s(filename, ext);

	if((fp = fopen(filename, "r")) != 0){
		fclose(fp);
		res = true;
	}

   return res;	
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
  int val = WaitForSingleObject(hT, INFINITE);

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
