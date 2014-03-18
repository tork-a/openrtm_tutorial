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

#include "stdafx.h"
#include "HUNO.h"

static  unsigned char HUNO_MoveJointPacket[] = {
      0xff, 0x80, 0x00, 0x7f
};

static unsigned char HUNO_PosturePacket[] = {
	0xff, 0x80, 0x00, 0x7f,
	0xff, 0x81, 0x00, 0x7f,
	0xff, 0x82, 0x00, 0x7f,
	0xff, 0x83, 0x00, 0x7f,
	0xff, 0x84, 0x00, 0x7f,
	0xff, 0x85, 0x00, 0x7f,
	0xff, 0x86, 0x00, 0x7f,
	0xff, 0x87, 0x00, 0x7f,
	0xff, 0x88, 0x00, 0x7f,
	0xff, 0x89, 0x00, 0x7f,
	0xff, 0x8a, 0x00, 0x7f,
	0xff, 0x8b, 0x00, 0x7f,
	0xff, 0x8c, 0x00, 0x7f,
	0xff, 0x8d, 0x00, 0x7f,
	0xff, 0x8e, 0x00, 0x7f,
	0xff, 0x8f, 0x00, 0x7f
};


static unsigned char HUNO_MotorInfoPacket[] = { 
    0xff, 0xa0, 0xa0, 0x00
};


static unsigned char HUNO_ServoPacket[]={
  0xff, 0xe0, 0xfb, 0x01, 0x00, 0x1a
};

static unsigned char HUNO_ServoOff[] ={
  0xff, 0xc0, 0x10, 0x7f

};


static int init_pos[] ={
	 0, -200,  400, 200, 0,
	 0, 200,  -400,  -200, 0,
	 -200, 0, 200,
	 200,  0, -200
};


static short limit_angles[] ={
	-90, 40,
	-90, 5,
	-20, 30,
	-30, 40,
	-80, 80,

	-40, 90,
	-5, 90,
	-30, 20,
	-30, 40,
	-80, 80,

	-65, 180,
	-10, 180,
	-85, 85, 

	-180, 65,
	-180, 10,
	-85, 85
};



static short max_angles[] ={
	40, 5, 30, 40, 80,
	90, 90, 20, 40, 80,
	180, 180, 85, 
	65, 10, 85
};

static short min_angles[] ={
	-90, -90, -20, -30, -80, 
	-40, -5, -30, -30, -80,
	-65, -10, -85, 
	-180, -180, -85
};

static	unsigned char initPosition[] = {
	123, 175, 155, 122, 125,
	122, 66, 96, 126, 118,
	49, 47, 40,
	199, 204, 211
};

static	unsigned char zeroPosition[] = {
	127, 212, 127, 127, 127,
	127,  42, 127, 127, 127,
	 70,  42, 127,
	183, 212, 127
};


static const char jointIdToMotorIdMap[] = {
    255 /* R_HIP_Y */, 6 /* R_HIP_R */,  7 /* R_HIP_P */, 8 /* R_KNEE_P */, 9 /* R_ANKLE_P */, 10 /* R_ANKLE_R */,
    255 /* L_HIP_Y */, 1 /* L_HIP_R */, 2 /* L_HIP_P */, 3 /* L_KNEE_P */, 4 /* L_ANKLE_P */, 5 /* L_ANKLE_R */,
    255 /* CHEST_P */,  255 /* NECK_Y */,
    14 /* R_SHOULDER_P */, 15 /* R_SHOULDER_R */, 16 /* R_ELBOW_P */,
    11 /* L_SHOULDER_P */, 12 /* L_SHOULDER_R */, 13 /* L_ELBOW_P */ };

/**** Method for  HUNO ***/
HUNO::HUNO(char *devname): SerialRobot(devname, N_JOINTS)
{
  Currents = new short[N_JOINTS];

  record = new RobotMotion();
  record->setJoints(N_JOINTS);

  zeroPos = new unsigned char[joints];
  memcpy(zeroPos,zeroPosition, joints);
  initPos = new int[joints];
  memcpy(initPos, init_pos, joints*sizeof(short));
  minPos = new short[joints];
  memcpy(minPos,min_angles, joints*sizeof(short));
  maxPos = new short[joints];
  memcpy(maxPos,max_angles, joints*sizeof(short));

  initPosture->setDegrees(init_pos);
}

HUNO::~HUNO()
{

  stopThread();

  delete zeroPos;
  delete initPos;
  delete minPos;
  delete maxPos;

  delete Currents;
  delete record;
}

void HUNO::init_packets(){


  PosturePacket  = new unsigned char [ sizeof(HUNO_PosturePacket) ];
  memcpy(PosturePacket, HUNO_PosturePacket, sizeof(HUNO_PosturePacket));

  return;
}

void HUNO::clear_packets(){

  delete PosturePacket;
  return;
}


/*************************************************/

/*
   getCurrent(int id)
*/
short
HUNO::getCurrent(unsigned char id)
{
  return  Currents[id];
}


/*************************/

bool 
HUNO::servo(const char *jname, bool turnon)
{
    std::map<std::string, int>::iterator l;
  if (strcmp(jname, "all") == 0 || strcmp(jname, "ALL") == 0){
        bool ret = true;
        for (int i=0; i<numJoints(); i++){
            ret = ret && servo(i, turnon);
        }
        return ret;
    }else if ((l = Link.find(jname)) != Link.end()) {
        return servo(l->second, turnon);
    }else{
        const std::vector<int> jgroup = m_jointGroups[jname];
        if (jgroup.size() == 0) return false;
        bool ret = true;
        for (unsigned int i=0; i<jgroup.size(); i++){
            ret = ret && servo(jgroup[i], turnon);
            return ret;
        }
    }
    return false;
}

void 
HUNO::readJointAngles(double *o_angles){
  int i;
  for(i=0;i < numJoints(); i++){ 
    o_angles[i] = deg2rad(currentPosture->getDegree(i+1));;
  }
}

void 
HUNO::writeJointCommands(const double *i_commands){
  int i;
  for(i=0;i < numJoints(); i++){
    short deg = rad2deg(i_commands[i+1]);
    setJoint((unsigned int)i, deg);
  }

}

bool 
HUNO::checkEmergency(std::string &o_reason, int &o_id){
  return false;
}

void 
HUNO::oneStep(){
  startMotion();
}

bool 
HUNO::names2ids(const std::vector<std::string> &i_names, 
                            std::vector<int> &o_ids)
{ 
    bool ret = true;
    for (unsigned int i=0; i<i_names.size(); i++){
    std::map<std::string, int>::iterator l = Link.find(i_names[i].c_str());
        if (l == Link.end()){
            std::cout << "joint named [" << i_names[i] << "] not found" 
                      << std::endl;
            ret = false;
        }else{
            o_ids.push_back(l->second);
        }
    }
    return ret;
}

bool 
HUNO::addJointGroup(const char *gname, const std::vector<std::string>& jnames)
{
    std::vector<int> jids;
    bool ret = names2ids(jnames, jids);
    m_jointGroups[gname] = jids;
    return ret;
}

double *
HUNO::getCurrentJointAngles()
{
	RobotPosture *pos = getCurrentPosture();
	return pos->getJointAnglesRad();
}

int 
HUNO::setTargetJointAngles(double *rad)
{
	int i;
	for(i=0;i<joints;i++){
		targetPosture->setDegree(i, rad2deg(rad[i]));
	}
	startMotion();
	return 0;
}


int 
HUNO::readPoseSeq(YamlMapping *m, char *name, unsigned char *pose)
{
  YamlSequence *frames;
  int joints;

  frames = m->getSequence(name);

  if(frames != NULL){
    joints = frames->size();

    for(int k(0) ; k < joints; k++){
	  if(frames->getScalarAt(k) != NULL){
        pose[k] = (unsigned char)( frames->getScalarAt(k)->toInteger() );
	  }
	}
  }
  return joints;
}


int 
HUNO::readPoseSeq(YamlMapping *m, char *name, short *pose)
{
  YamlSequence *frames;
  int joints;

  frames = m->getSequence(name);

  if(frames != NULL){
    joints = frames->size();

    for(int k(0) ; k < joints; k++){
	  if(frames->getScalarAt(k) != NULL){
        pose[k] = (short)( frames->getScalarAt(k)->toInteger() );
	  }
	}
  }
  return joints;
}

int 
HUNO::readPoseSeq(YamlMapping *m, char *name, int *pose)
{
  YamlSequence *frames;
  int joints;

  frames = m->getSequence(name);

  if(frames != NULL){
    joints = frames->size();

    for(int k(0) ; k < joints; k++){
	  if(frames->getScalarAt(k) != NULL){
        pose[k] = (int)( frames->getScalarAt(k)->toInteger() );
	  }
	}
  }
  return joints;
}

/*************** FIXED ****************/

/*
  Set torque of a motor
*/
int 
HUNO::setTorque( unsigned char id, char val)
{
  unsigned char ServoPacket[4];

  ServoPacket[0] = 0xff;

  if(val){
	ServoPacket[1] = 0x80| id;
    ServoPacket[2] = maxmin_unit(id, Currents[id] / 10);

  }else{
	memcpy(ServoPacket, HUNO_ServoOff, 4);
	ServoPacket[1] = 0xc0| id;
    ServoPacket[2] = 0x10;
  }
  ServoPacket[3] = ( ServoPacket[1] ^ ServoPacket[2] ) & 0x7f;

  if(sendCommand(ServoPacket, 4) < 0){
    std::cerr<< " Fail to setTorque: id=" << std::dec << (int)id <<std::endl;
    return -1;
  }
  Sleep(10);
  clearComBuffer();
  return 0;
}


/*
   Turn on/off  all servos.
*/
void 
HUNO::setServo(char on, int jointId)
{
  int i;
  int len=16;
  unsigned char ALL[N_JOINTS]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  unsigned char r_arm[]={R_ARM0, R_ARM1, R_ARM2};
  unsigned char l_arm[]={L_ARM0, L_ARM1, L_ARM2};
  unsigned char arms[]={L_ARM0, L_ARM1, L_ARM2, R_ARM0, R_ARM1, R_ARM2};
  unsigned char r_leg[]={R_LEG1, R_LEG2, R_KNEE, R_FOOT0, R_FOOT1};
  unsigned char l_leg[]={L_LEG1, L_LEG2, L_KNEE, L_FOOT0, L_FOOT1};
  unsigned char legs[]={R_LEG1, R_LEG2, R_KNEE, R_FOOT0, R_FOOT1, L_LEG1, L_LEG2, L_KNEE, L_FOOT0, L_FOOT1};

  unsigned char *Target;

  Target = ALL;

  switch(jointId){
	 case ALL_JOINTS:
      Target = ALL;
      len = N_JOINTS;
      break;
    case R_ARMS:
      Target = r_arm;
      len = sizeof(r_arm);
      break;
    case L_ARMS:
      Target = l_arm;
      len = sizeof(l_arm);
      break;
    case ALL_ARMS:
      Target = arms;
      len = sizeof(arms);
      break;
    case R_LEGS:
      Target = r_leg;
      len = sizeof(r_leg);
      break;
    case L_LEGS:
      Target = l_leg;
      len = sizeof(l_leg);
      break;
    case ALL_LEGS:
      Target = legs;
      len = sizeof(legs);
      break;
    default:
      break;
  }
  for(i=0; i < len; i++){
	  if(Target[i] < N_JOINTS){
		setTorque( Target[i], on);
		setServoState(Target[i], on);
	  }
  }
}

bool 
HUNO::servo(int jid, bool turnon)
{
  if(turnon){
    setServo(1, jid);
  }else{
    setServo(0, jid);
  }
  return true;
}


/*
  BreakMode
*/
void
HUNO::setBreakMode()
{
  unsigned char cmd[4] = { 0xff, 0xdf, 0x20, 0x7f};
  LOCK_COM
    com->sendData(cmd, 4);
    com->clearBuffer();
  UNLOCK_COM
  return;

}

/*
  print joint angles on a console.
*/
void 
HUNO::printPosture()
{
  printCurrents();
}

void 
HUNO::printCurrents()
{
  fprintf(stderr,"R_ARM:%d %d %d\n", Currents[R_ARM0],Currents[R_ARM1],Currents[R_ARM2]);
  fprintf(stderr,"L_ARM:%d %d %d\n", Currents[L_ARM0],Currents[L_ARM1],Currents[L_ARM2]);
  fprintf(stderr,"R_LEG:%d %d %d %d %d\n",
	  Currents[R_LEG1],Currents[R_LEG2],Currents[R_KNEE],Currents[R_FOOT0],Currents[R_FOOT1]);
  fprintf(stderr,"L_LEG:%d %d %d %d %d %d\n",
	  Currents[L_LEG1],Currents[L_LEG2],Currents[L_KNEE],Currents[L_FOOT0],Currents[L_FOOT1]);
  fprintf(stderr,"\n");
}

/*
  Ask motor infomation, and set current position and voltages. 
*/
short
HUNO::getAngle(unsigned char id)
{
  unsigned char rbuf[2];
  unsigned char buf[4];

  buf[0] = 0xff;
  buf[1] = 0xa0 | id;
  buf[2] = 0xa0 | id;
  buf[3] = 0x00;

  clearComBuffer();

  com->Write((char *)buf, 4);
  com->recieveData(rbuf, 2);

  if(rbuf != NULL){
	int deg = (int)(((rbuf[1] - zeroPos[id]) * 10.55) + 0.5);
	if(id==12 || id==15){ deg = -deg; }

    currentPosture->setDegree(id+1, deg);

  }else{
	std::cerr<< "ERROR :getAngle" <<std::endl;
	return -10000;
  }

  short res = currentPosture->getDegree(id+1);

  return res;
 
}

void 
HUNO::getPosture()
{
  int i;
  short val;

  unsigned char IDS[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  int max_ids = 16;
  /*
  for(i = 0 ; i < max_ids ; i++){
	unsigned char id = IDS[i];
    if((val = getAngle(id)) == -10000){
       LOCK_COM
	   if(com->clearBuffer() < 0){
		   com->closePort();
		   UNLOCK_COM
		   return;
	   }
       UNLOCK_COM
	}
  }
  */
  return; 
}

/*
  Move a joint
*/
int 
HUNO::moveJoint(unsigned char id, short deg, unsigned short cs)
{
  //unsigned char maxmin_unit(int id, short deg );
  unsigned char buf[4];

  Currents[id] += deg;
  short posture =  Currents[id] / 10;

  buf[0] = 0xff;
  buf[1] = 0x80 | id;
  buf[2] = maxmin_unit(id, posture);
  buf[3] = (buf[1] ^ buf[2]) & 0x7f;

  if(sendCommand(buf, 4) < 0){
    std::cerr << " :moveJoint" << std::endl;
  }

  clearComBuffer();
  
  return 0;
}

/****
  Set joint angles.
 ***/

int 
HUNO::setJoints( short *deg, int cs)
{

  if(cs > 0){
    targetPosture->setMotionTime((double)cs * 10);
  }else{
    targetPosture->setMotionTime(0);
  }
  for(int i=0; i < numJoints() ; i++){
    targetPosture->setDegree(i+1, (int)deg[i]);
  }

  return 0;
}


int 
HUNO::setJointsFromString(char *str)
{
  char *tp;
  short val;
  int i;

  tp = str;

  targetPosture->setMotionTime(500);

  getNumber(tp, &val);
  if(val > 0 && val < 1000){
    targetPosture->setMotionTime((double)val * 10);
  }

  for(i=0;i<joints;i++){
    tp = getNumber(tp, &val);
    if(tp == NULL) break;

     targetPosture->setDegree(i+1, (int)val);
  }
  return 0;
}


/*


*/
unsigned char 
HUNO::Deg2Unit(int id, short deg )
{
	unsigned char val = 127;
	int minval = 5;
	int maxval = 200;

	if(id == 12 || id == 15){
	  val = zeroPos[id] - (int)(deg/1.055 + 0.5);
	}else{
	  val = zeroPos[id] + (int)(deg/1.055 + 0.5);
	}

	if(val < minval) { val = minval;}
	else if(val > maxval) { val = maxval;}
	return val;
}

unsigned char 
HUNO::maxmin_unit(int id, short deg )
{
	unsigned char val = 127;

    int minval = minPos[id];
	int maxval = maxPos[id];

	if(deg < minval) { deg = minval;}
	else if(deg > maxval) { deg = maxval;}


	if(id == 12 || id == 15){
	  val = zeroPos[id] - (int)(deg / 1.08 + 0.5);
	}else{
	  val = zeroPos[id] + (int)(deg / 1.08 + 0.5);
	}

	return val;
}

void 
HUNO::postureToCommand(RobotPosture *pos)
{

  int mt = (int)targetPosture->motionTime;

  if(mt > 0 && mt < 10000){
	setTimeout( (int)targetPosture->motionTime );
  }else{
    setTimeout(getDefaultMotionTime());
  }


  for(int i=0; i<joints; i++){
    short posture = (short)pos->getDegree(i+1)/10;
	Currents[i] = posture * 10;
	short mtm = (short)getTimeout()/10;

	int p = i*4;
	PosturePacket[p+2] = maxmin_unit(i, posture);
	PosturePacket[p+3] = (PosturePacket[p+1] ^ PosturePacket[p+2]) & 0x7f;
  }

  setCommand(PosturePacket,  sizeof(HUNO_PosturePacket) );

  return;
}

/*
  Connect to Robot
*/
int
HUNO::connect(){
 int res;
 res = SerialRobot::connect();

 return res;

}

/*
  Check the connection to the HUNO, this member function is called in 'SerialRobot::connect()'.
  We should run the initialize for connection in this procedure.
*/
int 
HUNO::checkConnection()
{
	unsigned char init1[6]     = { 0xff, 0xe0, 0xfb, 0x01, 0x00, 0x1a };
	unsigned char init2[16]    = { 0xff, 0xff, 0xaa, 0x55, 0xaa, 0x55, 0x37, 0xba, 0x12, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01 };
	unsigned char initRes1[17] = { 0xff, 0xff, 0xaa, 0x55, 0xaa, 0x55, 0x37, 0xba, 0x12, 0x65, 0x00, 0x00, 0x00, 0x02, 0x64, 0x24, 0x40 };
	unsigned char init3[22]    = { 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0xff, 0xff, 0xaa, 0x55, 0xaa, 0x55, 0x37, 0xba, 0x10, 0x65, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01};
	unsigned char initRes2[16] = { 0xff, 0xff, 0xaa, 0x55, 0xaa, 0x55, 0x37, 0xba, 0x10, 0x65, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01 };
	int res;
	unsigned char rbuf[256];

	// Wait for setup...
	Sleep(65);

	/// Send Initialize Message
	res = com->sendData(init1, 6);
	res = com->sendData(init2, 16);
	Sleep(100);

	res = com->recieveData(rbuf, 17);

	if(res < 17){ // Clear buffer
		Sleep(1000);
		com->clearBuffer();
	}

	res = com->sendData(init3, 22);
	Sleep(100);

	res = com->recieveData(rbuf, 16);
	
	if(res < 16){ // Clear buffer
		Sleep(1000);
		com->clearBuffer();
	}

  return 0;
}

int 
HUNO::jointIdToMotorId(int jid)
{
	return jointIdToMotorIdMap[jid];
}


int 
HUNO::motorIdToJointId(int mid)
{
	for(int i(0); i< N_JOINTS; i++){
		if (jointIdToMotorIdMap[i] == mid){
			return i;
		}
	}
	return 0;
}



int 
HUNO::stabilizer()
{
	return 0;
}

/*

test...

*/

int 
HUNO::setFreeMotion(int x)
{
	freeMotion=x;
	return freeMotion;
}


int 
HUNO::clearRecord()
{
    record->clear();
	return 0;
}

int 
HUNO::recordCurrentPosture()
{
	record->appendPosture(currentPosture->dupPosture());
	return 0;
}

int 
HUNO::doRecord()
{
	clearMotion();
	motion->appendMotion(record);
	setMotionCount(1);
	return 0;
}

bool HUNO::loadConfig(char *name, char *id)
{
  YamlDocument doc;
  YamlMapping *topMap;
  YamlSequence *comps;

  if(!doc.load(name)){
	  std::cerr << "Fail to Configuration File" << std::endl;
	  return false;
  }

  if(doc.empty() ){
    return false;
  }

  topMap = doc.root->childNodes[0]->toMapping();

  if( topMap == NULL  
	|| !topMap->equalTo("type", "RQ_HUNO")
	|| (comps = topMap->getSequence("configurations")) == NULL )
  {
    std::cerr << "Invalid Yaml format : " << name << std::endl;
    return false; 
  }
 
  for( int i(0); i<comps->size() ;i++){
	YamlMapping *m = comps->at(i)->toMapping();

	if(m == NULL) continue;
	if( m->equalTo("robotID", id) && m->equalTo("numJoints", "16") )
	{
	  if(m->getScalar("portName") == NULL ){
		 setDevice( "COM1");
	  }else{
         setDevice((char *)(m->getScalar("portName")->value.c_str()));
	  }

	  readPoseSeq(m, "initPose", initPos);
	  readPoseSeq(m, "zeroPose", zeroPos);
	  readPoseSeq(m, "maxPose", maxPos);
	  readPoseSeq(m, "minPose", minPos);

	  break;
	}else{
	}
  }
  return true;
}

void HUNO::terminate()
{
  unsigned char servo_off[6] = { 0xff, 0xe0, 0xfb, 0x01, 0x00, 0x1a};

  com->sendData(servo_off, 6);

}