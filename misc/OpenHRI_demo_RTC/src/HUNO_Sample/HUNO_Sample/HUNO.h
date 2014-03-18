/*
  Sample program to control Huno
 
    Copyright(C) 2013, Isao HARA, AIST,Japan

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
#ifndef __HUNO_H__
#define __HUNO_H__


#include "SerialRobot.h"
#include "YamlParser.h"
#include <string>
#include <vector>
#include <map>

#define N_JOINTS  16
#define MAX_CMD_LEN  127

/** Moter IDs */
#define HEAD 255
#define BODY 255

#define R_ARMS (N_JOINTS + 3)
#define R_ARM0 13
#define R_ARM1 14
#define R_ARM2 15

#define L_ARMS (N_JOINTS + 4)
#define L_ARM0 10
#define L_ARM1 11
#define L_ARM2 12

#define R_LEGS (N_JOINTS + 5)
#define R_LEG0 255
#define R_LEG1 5
#define R_LEG2 6

#define R_KNEE 7

#define R_FOOT0 8
#define R_FOOT1 9

#define L_LEGS (N_JOINTS + 6)
#define L_LEG0 255
#define L_LEG1 0
#define L_LEG2 1

#define L_KNEE 2

#define L_FOOT0 3
#define L_FOOT1 4

#define ALL_ARMS (N_JOINTS + 7)
#define ALL_LEGS (N_JOINTS + 8)

#define ALL_JOINTS (N_JOINTS + 9)

/*
 *  Control class for HUNO
 * 
 */
class __declspec(dllexport) HUNO: public SerialRobot
{
public:
  /*
   *Constructor
   */
  HUNO(char *devname);
  /*
   * Deconstructor
   */
  ~HUNO();

  /*
   * connect to the robot
   */
  int connect();
  /*
   *  check connection
   */
  int checkConnection();

  /*
   *  output the current posture.
   */
  void printPosture();
  void printCurrents();

  /*
   * set servo
   */
  int setTorque(unsigned char id, char val);
  void setServo(char on, int c);

  /*
   *  get the angle of the motor.
   */
  short getAngle(unsigned char id);

  short getCurrent(unsigned char id);


  /*
   * move a joint.
   */
  int moveJoint(unsigned char id, short deg, unsigned short cs);

  /*
   * set joint angles and motionTime to the 'targetPosture'.
   */
  int setJoints(short *deg, int cs);
  /*
   * set joint angles to the 'targetPosture' from a string.
   */
  int setJointsFromString(char *str);

  /*
   *  convert a RobotPosture to a command packet.  
   */
  void postureToCommand(RobotPosture *pos);

  double *getCurrentJointAngles();

  int setTargetJointAngles(double *rad);

  int clearRecord();

  int recordCurrentPosture();

  int doRecord();

  void getPosture();

       /**  for hrpsys **/
  bool servo(int jid, bool turnon);
  bool servo(const char *jname, bool turnon);

  void setBreakMode();

  void readJointAngles(double *o_angles);
  void writeJointCommands(const double *i_commands);
  bool checkEmergency(std::string &o_reason, int &o_id);
  void oneStep();
  bool addJointGroup(const char *gname, const std::vector<std::string>& jnames);


  int jointIdToMotorId(int jid);
  int motorIdToJointId(int mid);
  int stabilizer();

  int setFreeMotion(int x);

  int readPoseSeq(YamlMapping *m, char *name, unsigned char *pose);
  int readPoseSeq(YamlMapping *m, char *name, short *pose);
  int readPoseSeq(YamlMapping *m, char *name, int *pose);

  bool loadConfig(char *name, char *id);

  void terminate();

  void init_packets();
  void clear_packets();

  unsigned char Deg2Unit(int id, short deg );
  unsigned char maxmin_unit(int id, short deg );

private:
  bool names2ids(const std::vector<std::string> &i_names, std::vector<int> &o_ids);

public:
  RobotMotion *record;


private:
  short *Currents;

  unsigned char *PosturePacket;       // a command packet template to move joints.

  unsigned char *zeroPos;
  int *initPos;
  short *minPos;
  short *maxPos;

  std::map<std::string, std::vector<int> > m_jointGroups;  // table of joint group.
  std::map<std::string, int> Link;    // table of links.

  int freeMotion;

};

#endif

