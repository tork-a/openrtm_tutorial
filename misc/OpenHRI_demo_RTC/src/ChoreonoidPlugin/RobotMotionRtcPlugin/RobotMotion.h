// -*- C++ -*-
/*!
 * @file  RobotMotion.h
 * @brief ModuleDescription
 * @date  $Date$
 *
 * $Id$
 */

#ifndef ROBOTSIM_H
#define ROBOTSIM_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include "MotionPlayer.h"

using namespace RTC;

/*!
 * @class RobotMotion
 * @brief ModuleDescription
 *
 */
class RobotMotion
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  RobotMotion(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~RobotMotion();

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
   /*** private member functions ***/
   void  setMotionPlayer(MotionPlayer *mp);
   MotionPlayer*  getMotionPlayer();

 protected:
  /*!
   * 
   * - Name:  project_dir
   * - DefaultValue: "."
   */
  std::string m_project_dir;

  std::vector<int> m_jointAngleMask;
  /*!
   */
  TimedString m_command;
  InPort<TimedString> m_commandIn;
  
  TimedDoubleSeq m_targetPose;
  InPort<TimedDoubleSeq> m_targetPoseIn;

    
  TimedFloatSeq m_skeleton;
  InPort<TimedFloatSeq> m_skeletonIn;

 private:
  MotionPlayer *m_player;
  cnoid::MessageView *mv;

  int kinect_ctrl;
  double spine_y, spine_z;
};


extern "C"
{
  DLL_EXPORT void RobotMotionInit(RTC::Manager* manager);
};

#endif // ROBOTSIM_H
