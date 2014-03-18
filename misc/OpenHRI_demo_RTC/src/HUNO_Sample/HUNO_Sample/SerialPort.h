/*
  Serial Port Communication Class

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

#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#include "CUtils.h"

/*
 * Baudrate 
 */
#ifdef WIN32
#define B0  0
#define B2400  2400
#define B4800  4800
#define B9600  9600
#define B19200  19200
#define B38400  38400
#define B7200  7200
#define B14400  14400
#define B28800  28800
#define B57600  57600
#define B76800  76800
#define B115200  115200
#define B230400  230400
#endif

/*
 *  Serial Port Class
 */
class SerialCom
{
public:
  /*
   * Constrauctor
   */
  SerialCom();
  SerialCom(char *devname);
  /*
   * Deconstructor
   */
  ~SerialCom();

  /*
   *  set device name.
   */
  void setDevPort(char*devname);
  /*
   * open commnication port. if it is sucess, this function return positive value.
   * if  it is failed, return nagative value (-1).
   */
  int openPort();
  /*
   *  close commnication port.
   */
  void closePort();

  /*
   * Low level communication
   */
  /*
   * Read : this function is compatible with 'read' function in POSIX-C
   */
  int Read(char *data, int len);
  /*
   * Write: this function is compatible with 'write' function in POSIX-C
   */
  int Write(char *data, int len);

  /*
   * High level commnucation
   */
  /*
   * This function is blocked until all data will be recieved.
   */
  int recieveData(unsigned char *data, int len);
  /*
   * This function is blcoked until all data will be sent.
   */
  int sendData(unsigned char *data, int len);
  /*
   * Check a buffer and returns size of data.
   */
  int chkBuffer();
  /*
   * Clear a buffer
   */
  int clearBuffer();
  /*
   *  print packet data for debugging commnication packets
   */
  void printPacket(unsigned char *data, int len);
  /*
   *  return current baudrate.
   */
  int getBaudrate();
  /*
   *  change baurate.
   */
  int setBaudrate(int baud_rate);
  /*
   * return commnication mode: 1 -> non blocking mode, other blocking mode.
   */
  int getMode();
  /*
   * set commnication mode. if the commnication port already opend, this function fail to set mode.
   */
  int setMode(int mode);

  int isConnected();

public:
  char *device;  // device name. e.g. /dev/ttyS0 for Unix, COM1 for Windows
  int baudrate;   // Baudrate 
  int mode;      // Communication mode. mode=1 -> Non Blocking mode, other Blocking mode

  HANDLE handle; // Handle for serial communication
};

#endif

