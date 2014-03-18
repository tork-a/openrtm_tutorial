/*
  Robot control class based on serial commnucation.


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

#ifndef __SERIAL_ROBOT_H__
#define __SERIAL_ROBOT_H__

#include "CUtils.h"
#include "SerialPort.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


// Macro for mutex
#ifdef  WIN32
#define LOCK_COM      WaitForSingleObject(mutex_com, INFINITE);
#define UNLOCK_COM      ReleaseMutex(mutex_com);
#define LOCK_MOTION     WaitForSingleObject(mutex_motion, INFINITE);
#define UNLOCK_MOTION   ReleaseMutex(mutex_motion);

#define FileDelim	"\\"
#else
#define LOCK_COM      pthread_mutex_lock(&mutex_com);
#define UNLOCK_COM      pthread_mutex_unlock(&mutex_com);
#define LOCK_MOTION     pthread_mutex_lock(&mutex_motion);
#define UNLOCK_MOTION    pthread_mutex_unlock(&mutex_motion);

#define FileDelim	"/"
#endif


#ifdef __cplusplus__
extern "C"{
#endif
/*
 * convert radian to degree.
 */
C_DECLSPEC short rad2deg(double d);
/*
 * convert degree  to radian.
 */
C_DECLSPEC double deg2rad(double d);
/*
 * This is a main thread function. 
 */
THREAD_FUNC thread_execution(void *args);
#ifdef __cplusplus__
};
#endif


class SerialRobot;

/*
 This class for controlling a hobby robot which connect to serial
 communiction port.
*/
/*
   Class for a posture of a robot.
*/
class C_DECLSPEC RobotPosture
{
public:
  /*
   * Constractor
   */
  RobotPosture(int n);
  /*
   * Deconstractor
   */
  ~RobotPosture();

  /*
   *  set motion time.
   */
  double setMotionTime(double n);
  /*
   * get angles of the motor with degree.
   *
   */
  int getDegree(int id);

  /*
   * get angles of the motor with radian 
   */
  double getRad(int id);

  /*
   * set angles of the motor with degree.
   */
  void setDegree(int id, int deg);
  bool setDegrees(int *degs);
  /*
   * set angles of the motor with radian.
   */
  void setRad(int id, double rad);

  /*
   *  output posture values on stderr.
   */
  void printPosture();
  /*
   *  output posture values to filestream.
   */
  void printPosture(std::ofstream& o);
  /*
   * copy all values from 'p' .
   */
  void copyPosture(RobotPosture *p);
  /*
   * copy angles from 'p' with offset.
   */
  void copyPosture(RobotPosture *p, int offset);

  /*
   * Create new instance and copy all values to it.
   */
  RobotPosture *dupPosture();

  int *getJointAngles();

  double *getJointAnglesRad();

  bool equalTo(RobotPosture *pos);

  bool nearTo(RobotPosture *pos, int delta);



public:
  int numJoints;          // Number of Joints
  int *jointAngles;       // Motor angles with degree.
  double *jointAnglesRad; // Motor angles with radian.
  double motionTime;      // Time for movement to desired posture.
};

/*
 *  Class for a robot motion. 
 *  I define a robot motion is sequence of RobotPostures...
 */
class  C_DECLSPEC RobotMotion
{
public:
  /*
   * Constructor
   */
  RobotMotion();
  /*
   * Deconstructor
   */
  ~RobotMotion();

  /*
   * set number of joints for the robot
   */
  int setJoints(int n);
  /*
   * return size of postures.
   */
  int getSize();
  /*
   * set revese mode.
   */
  int setReverse(bool f);
  /*
   *  reset current posture.
   */
  int reset();
  /*
   * get next posture.
   */
  RobotPosture *next();

  /*
   * get a nth posture from top.
   */
  RobotPosture *get(int nth);
  /*
   * get a nth posthre from tail. 
   * This function is equal to 'get(motion->size() - nth)'
   */
  RobotPosture *rget(int nth);

  /*
   *  add posture data on the end of motion.
   */
  bool appendPosture(RobotPosture *js);
  /*
   * insert posture date on nth of motuon.
   */
  bool insertPosture(RobotPosture *js, int nth);
  /*
   *  delete posture date on nth of motion
   */
  void deletePosture(int n);

  /*
   * load motion data from file.
   */
  bool loadMotionFromFile(const char *name);
  /*
   * save current motion date to file.
   */
  bool saveMotionToFile(const char *name);

  bool saveMotionToFile(const char *name,const char *dir);

 bool loadMotionFromMseqFile(const char *name, SerialRobot *r);

  /*
   * load motion data from yaml formatted file.
   */
  bool loadMotionFromPseqFile(const char *name, SerialRobot *r);

  /*
   * load motion data from yaml formatted file.
   */
  bool loadMotionFromYamlFile(const char *name, SerialRobot *r);
  
  /*
   * save current motion date to yaml formatted file.
   */
  bool saveMotionToPseqFile(const char *name, SerialRobot *r);


  bool saveMotionToYamlFile(const char *name, SerialRobot *r);
  /*
   * output motion data to stderr.
   */
  void printMotion();
  /*
   * output motion data to filestream.
   */
  void printMotion(std::ofstream& o);
  /*
   * clear all posture data from a current motion.
   */
  void clear();

  bool appendMotion(RobotMotion *rm);

  RobotMotion *dupMotion();

public:
  std::vector<RobotPosture *> motion;  //  current motion data
  int numJoints;                       // Number of joints
  int current;                         // current position in the motion
  bool reverse;                        // reverve mode or not.
};

/*
 * Serial Robot control class.
 */
class  C_DECLSPEC SerialRobot
{
public:
  /**
    Constructor: require 2 arguments, 
    first argument is a device name and second one is a number of robot's joints.
  */
  SerialRobot(char *devname, int n);
  /**
    Deconstructor: stop a backgroud thread and close a serial port.
  */
  ~SerialRobot();

  /**
    Open serial port: This method just calls com->opne().
  */
    int openPort();
  /**
    Close serial port: This method just calls com->close().
   */
  void closePort();
  /**
    Set serial device name: This method just calls com->setDevPort().
  */
     void setDevice(char *devname);

  /*
    start current motion which stored in 'motion'.
    In this function, 'commandCount = 1'.
  */
  void startMotion();

  /*
    set targetPosture to initPosture. 
  */
  int initPosition();
  
  /*
    Append a current posture on the end of 'motion'.
  */
  int appendCurrentPosture();
  int appendCurrentPosture(double mtime);
  /*
           Load a motion from the file
  */
  int loadMotion(char *name);

  int loadMotionFromM(char *fname);

  int loadMotionFromYaml(char *fname);

  int loadMotionFromPseq(char *fname);

  int loadMotionFromMseq(char *fname);
  /*
   * save a current motion to the file.
   */
   int saveMotionToM(char *fname);

   int saveMotionToYaml(char *fname);

   int saveMotionToPseq(char *fname);

  /*
   *  set target joint angle.
   */
  int setJoint(unsigned char id, short deg);
  int setMotionTime(int tm);
  /*
           clear current motion.
  */
  void clearMotion();

  /*
   *  set default motion time which means the time of movement to the targetPosture.
   */
  int setDefaultMotionTime(int sval);

  /*
   * return 'motioTime'
   */
  int getDefaultMotionTime();

  /*
   *  set 'timeout' value to val.
   */
  int setTimeout(int val);

  /*
   *  return 'timeout'.
   */
  int getTimeout();

  /*
   * set command packet to 'commandBuf'
   */
      int setCommand(unsigned char *packet, int len);

  /*
   *  recieve date from the robot.
   */
  int recieveData(unsigned char *data, int len);

  /*
   *  send 'data' to the robot.
   */
  int sendCommand(unsigned char *data, int len);

  /*
   *  start the thread which send motion commands.
   */
  int startThread();

  /*
   *  stop the thread.
   */
  int stopThread();

  /*
   * check the thread is active or not.
   */
     int isActive();

	RobotPosture *getNthPosture(int n);
	int doNthMotion(int n);
	RobotPosture *getTargetPosture();
	RobotPosture *getCurrentPosture();

	RobotPosture *getFirstPosture();
	RobotPosture *getLastPosture();

	int isMoving();

  /*
   *  set motion counter.
   */
  int  setMotionCount(int count);

  /*
   *  return a number of joints.
  */
  int numJoints();


  char *getServoState();
  int setServoState(int id, int state);


  /*
   *  check connection.
   */
  virtual  int checkConnection();


  /*
   *   get angle of the joint.
   */
  virtual short getAngle(unsigned char id);


  /*
   *  connect to the robot
   */
     int connect();

  /*
   *  get current posture from the robot.
   */
  virtual void getPosture();

  /*
   *  main thread function. this is similar to onExecution.... 
   */
  virtual void svc();

  /*
   *  convert a porture to a command packet to the robot
   */
  virtual void postureToCommand(RobotPosture *pos) = 0;

  virtual int jointIdToMotorId(int jid) = 0;
  virtual int motorIdToJointId(int mid) = 0;

  virtual int stabilizer() = 0;

  void setMotionDir(const char *dir);
  void clearComBuffer();

  virtual void terminate() = 0;
  virtual void init_packets()=0;
  virtual void clear_packets()=0;

  int convertJointId(int n);
  void setSenseTime(int tm);

public:
  char *name;
  int joints;                    // number of joints

  char *servoState;

  RobotPosture *initPosture;     //  initial posture
  RobotPosture *currentPosture;  //  current posture
  RobotPosture *targetPosture;   //  target posture

  std::string motionDir;
  
  RobotMotion *motion;         // current motion
  int motionTime;              // default movement time.
  int senseTime;               // default sensing interval time. 
  int timeout;                 // timeout 

private:

  int threadLoop;              // flag of thread loop. if the 'threadLoop' equals to 1, the control thread is active.

  int repeatCount;             // repeat count of excute the current motion.
  int reverseFlag;             // reverse flag. if this value is true. current motion evaluates reverse order.

  bool executeMotion;          // excute motion flag

  int commandCount;            // number of the commands  to send

  unsigned char *commandBuf;            // command buffer
  int commandSize;             // size of a command

public:
  THREAD_HANDLE hThread;       // thread handler
  MUTEX_HANDLE mutex_com;      // mutex for commnication
  MUTEX_HANDLE mutex_motion;   // mutex for motion.

 
  SerialCom *com;               // serial port to control a robot.
};


#endif

