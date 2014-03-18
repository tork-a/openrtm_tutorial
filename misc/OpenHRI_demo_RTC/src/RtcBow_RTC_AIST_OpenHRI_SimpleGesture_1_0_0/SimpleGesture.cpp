// -*- C++ -*-
/*!
 * @file  SimpleGesture.cpp
 * @brief Simple geasture component using KinectRTC
 * @date $Date$
 *
 * $Id$
 */


#include "SimpleGesture.h"




// Module specification
// <rtc-template block="module_spec">
static const char* simplegesture_spec[] =
  {
    "implementation_id", "SimpleGesture",
    "type_name",         "SimpleGesture",
    "description",       "Simple geasture component using KinectRTC",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "OpenHRI",
    "activity_type",     "PERIODOC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables

    // Widget

    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SimpleGesture::SimpleGesture(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager)
   , m_skeletonIn("skeleton", m_skeleton)
   , m_commandOut("command", m_command)
   , m_left_handOut("left_hand", m_left_hand)
   , m_right_handOut("right_hand", m_right_hand)
   , m_left_hand_posOut("left_hand_pos", m_left_hand_pos)
   , m_right_hand_posOut("right_hand_pos", m_right_hand_pos)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SimpleGesture::~SimpleGesture()
{
}

/*!
 * @brief initialize
 */
RTC::ReturnCode_t SimpleGesture::onInitialize()
{

  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
   addInPort("skeleton", m_skeletonIn);
 
  // Set OutPort buffer
   addOutPort("command", m_commandOut);
   addOutPort("left_hand", m_left_handOut);
   addOutPort("right_hand", m_right_handOut);
   addOutPort("left_hand_pos", m_left_hand_posOut);
   addOutPort("right_hand_pos", m_right_hand_posOut);
  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable


  // </rtc-template>

  // OnInitialize[[
  // OnInitialize Procedure of your RTC


  // ]]OnInitialize
  return RTC::RTC_OK;
}

#if ON_FINALIZE
RTC::ReturnCode_t SimpleGesture::onFinalize()
{
  // OnFinalize[[
  // OnFinalize Procedure of your RTC

  // ]]OnFinalize
  return RTC::RTC_OK;

}
#endif

#if ON_STARTUP
RTC::ReturnCode_t SimpleGesture::onStartup(RTC::UniqueId ec_id)
{
  // OnStartup[[
  // OnStartuo Procedure of your RTC

  // ]]OnStartup
  return RTC::RTC_OK;
}
#endif

#if ON_SHUTDOWN
RTC::ReturnCode_t SimpleGesture::onShutdown(RTC::UniqueId ec_id)
{
  // OnShutdown[[
  // OnShutdown Procedure of your RTC

  // ]]OnShutdown
  return RTC::RTC_OK;
}
#endif

#if ON_ACTIVATED
RTC::ReturnCode_t SimpleGesture::onActivated(RTC::UniqueId ec_id)
{
  // OnActivated[[
  // OnActivated Procedure of your RTC

  gesture_state = GESTURE_DISABLE;

  gesture = new KinectSimpleGesture();

  // ]]OnActivated
  return RTC::RTC_OK;
}
#endif

#if ON_DEACTIVATED
RTC::ReturnCode_t SimpleGesture::onDeactivated(RTC::UniqueId ec_id)
{
  // OnDeactivated[[
  // OnDeactivated Procedure of your RTC

  // ]]OnDeactivated
  return RTC::RTC_OK;
}
#endif

#if ON_EXECUTE
RTC::ReturnCode_t SimpleGesture::onExecute(RTC::UniqueId ec_id)
{
  // OnExecute[[
  // OnExecute Procedure of your RTC

  if(m_skeletonIn.isNew()){
    while(m_skeletonIn.isNew()) {
      m_skeletonIn.read();

      gesture->checkGesture(m_skeleton);

	  if(gesture->gesture_state != gesture_state){
		  if(gesture->gesture_state == GESTURE_ENABLE){
			  std::cout << " ]]] Start Gesture mode  " << std::endl;
			  m_command.data = "start_gesture";
			  m_commandOut.write();
		  }else{
			  m_command.data = "stop_gesture";
			  m_commandOut.write();
			  std::cout << "     Stop Gesture mode  [[[" << std::endl;
		  }
		  gesture_state = gesture->gesture_state;
	  }

      if(gesture->motion_r_h != NO_ACTION || gesture->motion_l_h != NO_ACTION) {
        switch(gesture->motion_l_h){
          case SWIPE_LEFT:
			m_left_hand.data = "swipe_left";
            std::cout << " <- Hand_L  ";
            break;
          case SWIPE_LEFT_FAST:
			m_left_hand.data = "swipe_left_fast";
            std::cout << " <---- Hand_L  ";
            break;
          case SWIPE_RIGHT:
			m_left_hand.data = "swipe_right";
            std::cout << " Hand_L ->  ";
            break;
          case SWIPE_RIGHT_FAST:
			m_left_hand.data = "swipe_right_fast";
            std::cout << " Hand_L ----->  ";
            break;
          default:
			m_left_hand.data = "";
            break;
        }

        switch(gesture->motion_r_h){
          case SWIPE_LEFT:
			m_right_hand.data = "swipe_left";
            std::cout << " <- Hand_R  ";
            break;
          case SWIPE_LEFT_FAST:
			m_right_hand.data = "swipe_left_fast";
            std::cout << " <---- Hand_R  ";
            break;
          case SWIPE_RIGHT:
			m_right_hand.data = "swipe_right";
            std::cout << " Hand_R ->  ";
            break;
          case SWIPE_RIGHT_FAST:
			m_right_hand.data = "swipe_right_fast";
            std::cout << " Hand_R ----->  ";
            break;
          default:
			m_right_hand.data = ""; 
            break;
        }
		m_right_handOut.write();
		m_left_handOut.write();
        std::cout << std::endl;
      }


      if(gesture->motion_r_v != NO_ACTION || gesture->motion_l_v != NO_ACTION) {
        switch(gesture->motion_l_v){
          case SWIPE_UP:
			m_left_hand.data = "swipe_up";
            std::cout << "  <UP> Hand_L  ";
            break;
          case SWIPE_UP_FAST:
			m_left_hand.data = "swipe_up_fast";
            std::cout << " <UP_FAST> Hand_L  ";
            break;
          case SWIPE_DOWN:
			m_left_hand.data = "swipe_down";
            std::cout << " Hand_L <DOWN>  ";
            break;
          case SWIPE_DOWN_FAST:
			m_left_hand.data = "swipe_down_fast";
            std::cout << " Hand_L <DOWN_FAST>  ";
            break;
          default:
            break;
        }

        switch(gesture->motion_r_v){
          case SWIPE_UP:
			m_right_hand.data = "swipe_up";
			std::cout << " <UP> Hand_R  ";
            break;
          case SWIPE_UP_FAST:
			m_right_hand.data = "swipe_up_fast";
            std::cout << " <UP_FAST> Hand_R  ";
            break;
          case SWIPE_DOWN:
			m_right_hand.data = "swipe_down";   
            std::cout << " Hand_R <DOWN>  ";
            break;
          case SWIPE_DOWN_FAST:
		    m_right_hand.data = "swipe_down_fast";   
            std::cout << " Hand_R <DOWN_FAST>  ";
            break;
          default:
		   
            break;
        }
		m_right_handOut.write();
		m_left_handOut.write();
        std::cout << std::endl;
      }

      if(gesture->state_changed){
        switch(gesture->current_state){
          case BOTH_HANDS_UP:
			m_command.data = "both_up";
            std::cout << " Both Hands Up  " << std::endl;
            break;
          case LEFT_HAND_UP:
			m_command.data = "left_up";
            std::cout << " LEFT Hand Up  " << std::endl;
            break;
          case RIGHT_HAND_UP:
			m_command.data = "right_up";
            std::cout << " Right Hand Up  " << std::endl;
            break;
          case BOTH_HANDS_DOWN:
			m_command.data = "both_down";
            std::cout << " Both Hands Down  " << std::endl;
            break;
          default:
			m_command.data = "";
            break;
        }

		m_commandOut.write();

      }else{
		if( gesture->motion_r == 2 && gesture->motion_l == 2){
		  m_command.data = "both_byebye";
		  std::cout << " RightHand ByeBye  " << std::endl;
		  m_commandOut.write();

		}else if( gesture->motion_r == 2 ){
		  m_command.data = "right_byebye";
		  std::cout << " RightHand ByeBye  " << std::endl;
		  m_commandOut.write();

		}else  if( gesture->motion_l == 2){
		  m_command.data = "left_byebye";
		  std::cout << " LeftHand ByeBye  " << std::endl;
		  m_commandOut.write();
	    }

	    if( gesture->motion_r == 1 || gesture->motion_r == 3){
		  m_right_hand_pos.data = gesture->hand_pos_r;
		  m_right_hand_posOut.write();
		  std::cout << " RightHand Pos   " <<  gesture->hand_pos_r << std::endl;
	    }
	    if( gesture->motion_l == 1 || gesture->motion_l == 3){
		  m_left_hand_pos.data = gesture->hand_pos_l;
		  m_left_hand_posOut.write();
		  std::cout << " LeftHand Pos  " << gesture->hand_pos_l  <<std::endl;
	    }
	  }
    }
  }

  // ]]OnExecute
  return RTC::RTC_OK;
}
#endif

#if ON_ABORTING
RTC::ReturnCode_t SimpleGesture::onAborting(RTC::UniqueId ec_id)
{
  // OnAborting[[
  // OnAborting Procedure of your RTC

  // ]]OnAborting
  return RTC::RTC_OK;
}
#endif

#if ON_ERROR
RTC::ReturnCode_t SimpleGesture::onError(RTC::UniqueId ec_id)
{
  // OnError[[
  // OnError Procedure of your RTC

  // ]]OnError
  return RTC::RTC_OK;
}
#endif

#if ON_RESET
RTC::ReturnCode_t SimpleGesture::onReset(RTC::UniqueId ec_id)
{
  // OnReset[[
  // OnReset Procedure of your RTC

  // ]]OnReset
  return RTC::RTC_OK;
}
#endif

#if ON_STATEUPDATE
RTC::ReturnCode_t SimpleGesture::onStateUpdate(RTC::UniqueId ec_id)
{
  // OnStateUpdate[[
  // OnStateUpdate Procedure of your RTC

  // ]]OnStateUpdate
  return RTC::RTC_OK;
}
#endif

#if ON_RATECHANGED
RTC::ReturnCode_t SimpleGesture::onRateChanged(RTC::UniqueId ec_id)
{
  // OnRateChanged[[
  // OnRateChanged Procedure of your RTC

  // ]]OnRateChanged
  return RTC::RTC_OK;
}
#endif


extern "C"
{
 
  void SimpleGestureInit(RTC::Manager* manager)
  {
    coil::Properties profile(simplegesture_spec);
    manager->registerFactory(profile,
                             RTC::Create<SimpleGesture>,
                             RTC::Delete<SimpleGesture>);
  }
  
}



