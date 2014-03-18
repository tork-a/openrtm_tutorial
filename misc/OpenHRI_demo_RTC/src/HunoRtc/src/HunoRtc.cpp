// -*- C++ -*-
/*!
 * @file  Huno.cpp
 * @brief Robot Controller for HUNO
 * @date $Date$
 *
 * $Id$

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

#include "HunoRtc.h"

static int Grobot2Huno[20] = {255, 255, 13, 14, 15, 10, 11, 12, 255, 6, 5, 7, 8, 9, 255, 1, 0, 2, 3, 4 };

// Module specification
// <rtc-template block="module_spec">
static const char* hunortc_spec[] =
  {
    "implementation_id", "HunoRtc",
    "type_name",         "HunoRtc",
    "description",       "Robot Controller for HUNO",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Sample",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
	"conf.default.huno_config", "huno.config",
    "conf.default.huno_id", "1",
    "conf.default.motionDir", "motions",
    "conf.default.motion_time", "100",
	"conf.default.servo_flag", "0",
    // Widget
    "conf.__widget__.huno_config", "text",
	"conf.__widget__.huno_id", "text",
    "conf.__widget__.motionDir", "text",
    "conf.__widget__.motion_time", "text",
	"conf.__widget__.servo_flag", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
HunoRtc::HunoRtc(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_commandIn("command", m_command),
	m_targetPoseIn("targetPose", m_targetPose),
    m_qOut("q", m_q)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
HunoRtc::~HunoRtc()
{
}



RTC::ReturnCode_t HunoRtc::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("command", m_commandIn);
  addInPort("targetPose", m_targetPoseIn); 

  // Set OutPort buffer
  addOutPort("q", m_qOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("huno_config", m_huno_config, "huno.config");
  bindParameter("huno_id", m_huno_id, "1");
  bindParameter("motionDir", m_motionDir, ".");
  bindParameter("motion_time", m_motion_time, "100");
  bindParameter("servo_flag", m_servo_flag, "0");
  // </rtc-template>
  
  robot = NULL;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t HunoRtc::onFinalize()
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HunoRtc::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HunoRtc::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t HunoRtc::onActivated(RTC::UniqueId ec_id)
{
  robot = new HUNO(NULL);
  robot->loadConfig((char *)m_huno_config.c_str(), (char *)m_huno_id.c_str());

  recoardMotionFlag=0;
  m_prev_motionTime = m_motionTime;


  if( robot->connect() < 0 ){
	std::cerr << "Error: can't find HUNO: " <<  robot->com->device << std::endl;
	return RTC::RTC_ERROR;
  }
  if(robot->startThread() == 0){
	std::cerr << "Error: fail to create thread" << std::endl;

    return RTC::RTC_ERROR;
  }

  m_q.data.length(robot->joints);
  sentPos = robot->currentPosture->dupPosture();

  robot->setTimeout(m_motionTime);
  robot->initPosition();

  servo_state=0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t HunoRtc::onDeactivated(RTC::UniqueId ec_id)
{

  if(robot){
	robot->terminate();
	delete robot;
	robot = NULL;
  }
  delete sentPos;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t HunoRtc::onExecute(RTC::UniqueId ec_id)
{
  int motion_flag = 1;

  if(servo_state != m_servo_flag){
	if(m_servo_flag == 0){
        robot->setServo(0, 0);
        servo_state = 0;
	}else{
		robot->setServo(1, 0);
		servo_state = 1;
	}
  }

  /* m_commandIn に入力があった場合の処理 */
  if(m_commandIn.isNew() && robot->isMoving() == 0)
  {
	m_commandIn.read();
	std::cout << "Action:" << m_command.data << std::endl;

	if(strcmp(m_command.data, "On")== 0){ /* サーボモータをOnにする　*/
	  robot->setServo(1, 0);
	  //servo_state = 1;
	  m_servo_flag = 1;

	}else if(strcmp(m_command.data, "Off") == 0){ /* サーボモータをOffにする　*/
	  robot->setServo(0,0);
	 // servo_state = 0;
	  m_servo_flag = 0;

	}else if(strcmp(m_command.data, "Break") == 0){ 
	  robot->setServo(2,0);

	}else if(strcmp(m_command.data, "Init") == 0){ /* 初期化姿勢（中腰）になる */
      robot->initPosition();

	}else if(strcmp(m_command.data, "Rec") == 0){
		robot->clearMotion();
		recoardMotionFlag = 1;
		count = 0;

	}else if(strcmp(m_command.data, "End") == 0){
		recoardMotionFlag = 0;
		robot->setMotionDir(m_motionDir.c_str());
		robot->saveMotionToM(NULL);

	}else if(strcmp(m_command.data, "Start") == 0){
		robot->setMotionCount(1);

	}else{ /* 入力された文字列の動作を探して、存在すれば実行する */
	  robot->setMotionDir(m_motionDir.c_str());

	  std::cout << "Motion = "<< m_command.data << std::endl;
	  std::string file(m_command.data);
 
	  if(robot->loadMotion((char *)file.c_str()) > 0){
        robot->setMotionCount(1);
      }
    }
	motion_flag = 0;
  }

  /* m_targetPoseIn に入力があった場合の処理 */
  int motion_flag_2 = 0;
  while(m_targetPoseIn.isNew()){
	m_targetPoseIn.read();
	motion_flag_2 = 1;
  }
  if(motion_flag_2 == 1){
    if(motion_flag == 1 && servo_state == 1){
		robot->clearMotion();

		for(int i(0); i< 20;i++){
			int id = Grobot2Huno[i];
			if(id < 255){
			  robot->setJoint(id+1, m_targetPose.data[i]);
			}
		}
		robot->targetPosture->motionTime=20;
		robot->startMotion();
	}
  }


#if 0

  while(m_qradIn.isNew()){
    m_qradIn.read();
    if(qflag == 0){
      for(int i(0); i< robot->joints;i++){
        robot->setJoint( robot->jointIdToMotorId(i), rad2deg(m_qrad.data[i])*10);
      }
      robot->setMotionTime(0);
      robot->startMotion();
    }
  }
#endif

  /*  現在の姿勢をm_qOutに出力  */
  RobotPosture *cpos = robot->currentPosture->dupPosture();
  if(!sentPos->nearTo(cpos, 5)){
    for(int i(0); i<m_q.data.length(); i++){
	  m_q.data[i] = cpos->getDegree(i+1);
    }
    m_qOut.write();

	if(recoardMotionFlag == 1){
		  if(!cpos->nearTo(robot->getLastPosture(), 100))
		  {
		    robot->appendCurrentPosture(250);
		  }
	  }

	delete sentPos;
	sentPos = cpos;
  }

  if(m_prev_motionTime != m_motionTime){
      m_prev_motionTime = m_motionTime;
	  robot->setDefaultMotionTime(m_motionTime);
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HunoRtc::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HunoRtc::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HunoRtc::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HunoRtc::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HunoRtc::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void HunoRtcInit(RTC::Manager* manager)
  {
    coil::Properties profile(hunortc_spec);
    manager->registerFactory(profile,
                             RTC::Create<HunoRtc>,
                             RTC::Delete<HunoRtc>);
  }
  
};


