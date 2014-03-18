// -*- C++ -*-
/*!
 * @file  Skelton2JointAngle.cpp
 * @brief Data convert from the skenton data to joint angles for GR001
 * @date $Date$
 *
 * $Id$
 */


#include "Skelton2JointAngle.h"




// Module specification
// <rtc-template block="module_spec">
static const char* skelton2jointangle_spec[] =
  {
    "implementation_id", "Skelton2JointAngle",
    "type_name",         "Skelton2JointAngle",
    "description",       "Data convert from the skenton data to joint angles for GR001",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Robot",
    "activity_type",     "PERIODOC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
   "conf.default.mode", "0",

    // Widget
   "conf.__widget__.mode", "text",

    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Skelton2JointAngle::Skelton2JointAngle(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager)
   , m_skeletonIn("skeleton", m_skeleton)
   , m_joint_angleOut("joint_angle", m_joint_angle)
   , m_commandIn("command", m_command)


    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Skelton2JointAngle::~Skelton2JointAngle()
{
}

/*!
 * @brief initialize
 */
RTC::ReturnCode_t Skelton2JointAngle::onInitialize()
{

  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
   addInPort("skeleton", m_skeletonIn);
   addInPort("command", m_commandIn);
 
  // Set OutPort buffer
   addOutPort("joint_angle", m_joint_angleOut);


  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
    bindParameter("mode", m_mode, "0");


  // </rtc-template>

  // OnInitialize[[
  // OnInitialize Procedure of your RTC
  m_joint_angle.data.length(21);

  // ]]OnInitialize
  return RTC::RTC_OK;
}

#if ON_FINALIZE
RTC::ReturnCode_t Skelton2JointAngle::onFinalize()
{
  // OnFinalize[[
  // OnFinalize Procedure of your RTC

  // ]]OnFinalize
  return RTC::RTC_OK;

}
#endif

#if ON_STARTUP
RTC::ReturnCode_t Skelton2JointAngle::onStartup(RTC::UniqueId ec_id)
{
  // OnStartup[[
  // OnStartuo Procedure of your RTC

  // ]]OnStartup
  return RTC::RTC_OK;
}
#endif

#if ON_SHUTDOWN
RTC::ReturnCode_t Skelton2JointAngle::onShutdown(RTC::UniqueId ec_id)
{
  // OnShutdown[[
  // OnShutdown Procedure of your RTC

  // ]]OnShutdown
  return RTC::RTC_OK;
}
#endif

#if ON_ACTIVATED
RTC::ReturnCode_t Skelton2JointAngle::onActivated(RTC::UniqueId ec_id)
{
  // OnActivated[[
  // OnActivated Procedure of your RTC

  // ]]OnActivated
  return RTC::RTC_OK;
}
#endif

#if ON_DEACTIVATED
RTC::ReturnCode_t Skelton2JointAngle::onDeactivated(RTC::UniqueId ec_id)
{
  // OnDeactivated[[
  // OnDeactivated Procedure of your RTC

  // ]]OnDeactivated
  return RTC::RTC_OK;
}
#endif

#if ON_EXECUTE
RTC::ReturnCode_t Skelton2JointAngle::onExecute(RTC::UniqueId ec_id)
{
  // OnExecute[[
  // OnExecute Procedure of your RTC

  if(m_commandIn.isNew()){
    m_commandIn.read();

    if( strcmp( m_command.data, "stop") == 0){
      m_mode = 1;
	}else  if( strcmp(m_command.data, "start") == 0){
      m_mode = 0;
    }
	std::cout << m_command.data << std::endl;
  }

  if(m_skeletonIn.isNew()){

    while(m_skeletonIn.isNew()) {
      m_skeletonIn.read();

      if(m_skeleton.data[SPINE+2] < 1.1 || m_mode == 1) { 
        kinect_ctrl=0;
        spine_y=0;
        return RTC::RTC_OK;

      }else if(m_skeleton.data[SPINE+2] > 1.2 && m_skeleton.data[SPINE+2] < 2.0) {
        if(kinect_ctrl == 0){
          spine_y = m_skeleton.data[SPINE+1];
        }
        kinect_ctrl = 1;
      }

      if(kinect_ctrl == 1){
        if(spine_y == 0){ spine_y = m_skeleton.data[SPINE+1]; }

        int len = m_skeleton.data.length() / 3;

         m_JointAngle[ ELBOW_LEFT_JOINT]        = calcJointAngle(m_skeleton, ELBOW_LEFT_JOINT);
         m_JointAngle[ SHOULDER_R_LEFT_JOINT]   = calcJointAngle(m_skeleton, SHOULDER_R_LEFT_JOINT);
         m_JointAngle[ SHOULDER_P_LEFT_JOINT]   = calcJointAngle(m_skeleton, SHOULDER_P_LEFT_JOINT);
         m_JointAngle[ ELBOW_RIGHT_JOINT]       = calcJointAngle(m_skeleton, ELBOW_RIGHT_JOINT);
         m_JointAngle[ SHOULDER_R_RIGHT_JOINT ] = calcJointAngle(m_skeleton, SHOULDER_R_RIGHT_JOINT);
         m_JointAngle[ SHOULDER_P_RIGHT_JOINT ] = calcJointAngle(m_skeleton, SHOULDER_P_RIGHT_JOINT);
         m_JointAngle[ CHEST_JOINT]             = calcJointAngle(m_skeleton, CHEST_JOINT);

        double rot = calcJointAngle(m_skeleton, ROT_JOINT);

         m_JointAngle[ HIP_R_LEFT ] = rot;
         m_JointAngle[ HIP_R_RIGHT ] = rot;

         m_JointAngle[ HEIGHT] = spine_y - m_skeleton.data[SPINE+1];
  
        for(int i=0; i< 21; i++){
            m_joint_angle.data[i] = m_JointAngle[i];
        }
         m_joint_angleOut.write();
      }
    }

  }
  // ]]OnExecute
  return RTC::RTC_OK;
}
#endif

#if ON_ABORTING
RTC::ReturnCode_t Skelton2JointAngle::onAborting(RTC::UniqueId ec_id)
{
  // OnAborting[[
  // OnAborting Procedure of your RTC

  // ]]OnAborting
  return RTC::RTC_OK;
}
#endif

#if ON_ERROR
RTC::ReturnCode_t Skelton2JointAngle::onError(RTC::UniqueId ec_id)
{
  // OnError[[
  // OnError Procedure of your RTC

  // ]]OnError
  return RTC::RTC_OK;
}
#endif

#if ON_RESET
RTC::ReturnCode_t Skelton2JointAngle::onReset(RTC::UniqueId ec_id)
{
  // OnReset[[
  // OnReset Procedure of your RTC

  // ]]OnReset
  return RTC::RTC_OK;
}
#endif

#if ON_STATEUPDATE
RTC::ReturnCode_t Skelton2JointAngle::onStateUpdate(RTC::UniqueId ec_id)
{
  // OnStateUpdate[[
  // OnStateUpdate Procedure of your RTC

  // ]]OnStateUpdate
  return RTC::RTC_OK;
}
#endif

#if ON_RATECHANGED
RTC::ReturnCode_t Skelton2JointAngle::onRateChanged(RTC::UniqueId ec_id)
{
  // OnRateChanged[[
  // OnRateChanged Procedure of your RTC

  // ]]OnRateChanged
  return RTC::RTC_OK;
}
#endif


extern "C"
{
 
  void Skelton2JointAngleInit(RTC::Manager* manager)
  {
    coil::Properties profile(skelton2jointangle_spec);
    manager->registerFactory(profile,
                             RTC::Create<Skelton2JointAngle>,
                             RTC::Delete<Skelton2JointAngle>);
  }
  
}



