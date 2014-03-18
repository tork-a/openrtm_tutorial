/*
  Serial port commnucation  Class.

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
#include "SerialPort.h"
#include <iostream>
#include <tchar.h>

/**

*/
SerialCom::SerialCom():device(NULL),handle(H_NULL),mode(0),baudrate(B115200)
{
}
/**

*/
SerialCom::SerialCom(char *devname):handle(H_NULL),mode(0),baudrate(B115200)
{
  setDevPort(devname);
}

/**
  Deconstructor
*/
SerialCom::~SerialCom()
{
  closePort();
  delete device;
  handle = H_NULL;
}

/**
  set device name
*/
void SerialCom::setDevPort(char *devname){
  device = (char *)StrDup((const char *)devname);
}

/**
  open serial port
    byte size : 8
    parity    :  None
    stop bits : 1
    baudrate    :  115200 (default)
*/
int 
SerialCom::openPort()
{
#ifdef WIN32
  DCB dcb;

  if(mode == 1){
	  handle = CreateFile(device,
      GENERIC_READ | GENERIC_WRITE, 0, NULL,
      OPEN_EXISTING, 
      FILE_FLAG_OVERLAPPED,
      NULL);
  }else{
	  handle = CreateFile(device,
      GENERIC_READ | GENERIC_WRITE, 0, NULL,
      OPEN_EXISTING, 
      FILE_ATTRIBUTE_NORMAL,
      NULL);
  }

  if (handle == INVALID_HANDLE_VALUE){
    char dev[32];
	sprintf_s(dev, "\\\\.\\%s",device);

    if(mode == 1){
	  handle = CreateFile(dev,
      GENERIC_READ | GENERIC_WRITE, 0, NULL,
      OPEN_EXISTING, 
      FILE_FLAG_OVERLAPPED,
      NULL);
    }else{
	  handle = CreateFile(dev,
      GENERIC_READ | GENERIC_WRITE, 0, NULL,
      OPEN_EXISTING, 
      FILE_ATTRIBUTE_NORMAL,
      NULL);
    }

  }

  if (handle != INVALID_HANDLE_VALUE){
    GetCommState(handle, &dcb);

    dcb.BaudRate = baudrate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.fParity = FALSE;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(handle, &dcb);
  }else{
     handle = H_NULL;
   return -1;
  }
#else
  struct termios tio;

  if(mode == 1){
    handle = open(device, O_RDWR|O_NONBLOCK);
  }else{
    handle = open(device, O_RDWR);
  }
  if(handle < 0){
    handle = H_NULL;
    return -1;
  }

  memset(&tio, 0, sizeof(tio));
  tio.c_cflag = CS8|CREAD|CLOCAL;
  tio.c_cc[VMIN] = 1;
  cfsetospeed(&tio, baudrate);
  cfsetispeed(&tio, baudrate);
  tcsetattr(handle, TCSANOW, &tio);

#endif

  return (int)handle;
}

/*
 *  Change baudrate
 */
int
SerialCom::setBaudrate(int b)
{
  baudrate = b;
  if(handle != H_NULL){
#ifdef WIN32
    DCB dcb;

	GetCommState(handle, &dcb); 
	dcb.BaudRate = baudrate;
	SetCommState(handle, &dcb);
#else
    struct termios tio;
    tcgetattr(handle, &tio);
    cfsetospeed(&tio, b);
    cfsetispeed(&tio, b);
    tcsetattr(handle, TCSANOW, &tio);
#endif
  }
  return b;
}

/**
  close serial port
*/
void SerialCom::closePort(){
  if(this->handle != H_NULL){
#ifdef WIN32
    CloseHandle(this->handle);
#else
	close(this->handle);
#endif
    this->handle = H_NULL;
  }
}

/**
  recieve data from serial port (low level)
 */
int SerialCom::Read(char *data, int len){
#ifdef WIN32
  DWORD dwRead;

  if(mode == 1){
    OVERLAPPED lpo;
    ZeroMemory(&lpo, sizeof(lpo));
    lpo.hEvent = NULL;
    ReadFile(handle, data, len, &dwRead, &lpo);
    if(GetLastError() != ERROR_IO_PENDING){
      return -1;
    }
  }else{
    if(ReadFile(handle, data, len, &dwRead, NULL) == FALSE){
      return -1;
    }
  }
  return dwRead;
#else
  return read(handle, data, len);
#endif
}

/**
 * recieve data from serial port. this function is blocked until all date recieved. (high level)
 */
int SerialCom::recieveData(unsigned char *data, int len){
  int i,c, recieved;

  recieved = 0;

  /*  Waiting for a moment  */
  for(i = 0; i <100; i++){ 
    if(this->chkBuffer() >= len){
      break;
    }
    Sleep(1);
  }

  if(i >= 100) {
    this->clearBuffer();
    return -1;
  }
  do{
    c = Read((char *)(data+recieved), len-recieved);
    if(c < 0){
      std::cerr << "ERROR in read" << std::endl;
      return -1;
    }
  recieved += c;
  } while(recieved < len);

  return len;
}

/**
 send data to serial port (low level)
*/
int SerialCom::Write(char *data, int len){
#ifdef WIN32
  DWORD dwWrite;
  if(mode == 1){
    OVERLAPPED lpo;
    ZeroMemory(&lpo, sizeof(lpo));
    lpo.hEvent = NULL;
    WriteFile(handle, data, len, &dwWrite, &lpo);
    if(GetLastError() != ERROR_IO_PENDING){
      return -1;
    }
  }else{
    if(WriteFile(handle, data, len, &dwWrite, NULL) == FALSE){
    return -1;
    }
  }
  return dwWrite;
#else
  return write(handle, data, len);
#endif
}

/**
 *  send date to serial port. this function block until all data sent.
 */
int SerialCom::sendData(unsigned char *data, int len){
  int c, sent=0;

  do{
    c = Write((char *)(data+sent), len-sent);
    if(c < 0){
    std::cerr << "ERROR in SerialCom::sendData" << std::endl;
    return -1;
    }
    sent += c;
  }while(sent < len);

  return sent;
}

/**
  check buffer and return size of data.
*/
int SerialCom::chkBuffer(){
  int n;

#ifdef WIN32
  DWORD lpErrors;
  COMSTAT lpStat;
  ClearCommError(this->handle, &lpErrors, &lpStat);

  n = lpStat.cbInQue;
#else
#ifndef TIOCINQ
#define TIOCINQ FIONREAD
#endif

  ioctl(this->handle, TIOCINQ, &n);
#endif

  return n;
}

/*
 *  clear date in the buufer of commnuication port.
 */
int SerialCom::clearBuffer(){
  char data[1024];

  int n;
  
  while(1){
    n = this->chkBuffer();
	if(n== 0){ break; }

    if(n > 1024) { n=1024; }
    Read(data, n);
  }

  return n;
}

/**
   print packets for debugging
*/
void SerialCom::printPacket(unsigned char *data, int len){
  int i;
  for(i=0; i<len; i++){
    fprintf(stderr, " %02x", data[i]);
  }
  fprintf(stderr, "\n");
  return;
}

/*
 *  return braurate.
 */
int SerialCom::getBaudrate(){
  return baudrate;
}

/*
 *  return current commnication mode.
 */
int SerialCom::getMode(){
  return mode;
}

/*
 *  set commnication mode. if the commication port is already opened, this function will fail.
 */
int SerialCom::setMode(int mode){
  if(handle != H_NULL){
    std::cerr << "Fail to chandge mode" << std::endl;
  }else{
    mode = mode;
  }
  return mode;
}

int SerialCom::isConnected(){
  if(handle == H_NULL){
    return -1;
  }else{
    return 1;
  }
}
