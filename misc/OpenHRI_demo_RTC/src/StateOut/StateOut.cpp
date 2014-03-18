// -*- C++ -*-
/*!
 * @file  SateOut.cpp
 * @brief Display State for GRobotRTC
 * @date $Date$
 *
 * $Id$
 */

#include "StateOut.h"

// Module specification
// <rtc-template block="module_spec">
static const char* stateout_spec[] =
  {
    "implementation_id", "StateOut",
    "type_name",         "StateOut",
    "description",       "Display State for GRobotRTC",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Sample",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
StateOut::StateOut(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_qRefIn("qRef", m_qRef)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
StateOut::~StateOut()
{
}



RTC::ReturnCode_t StateOut::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("qRef", m_qRefIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StateOut::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t StateOut::onExecute(RTC::UniqueId ec_id)
{
	if(m_qRefIn.isNew()){
		m_qRefIn.read();
		int len = m_qRef.data.length();
		std::cout << "State: ";
		for(int i(0); i< len; i++){
			std::cout << m_qRef.data[i] << " ";
		}
		std::cout << std::endl;
	}
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StateOut::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StateOut::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void StateOutInit(RTC::Manager* manager)
  {
    coil::Properties profile(stateout_spec);
    manager->registerFactory(profile,
                             RTC::Create<StateOut>,
                             RTC::Delete<StateOut>);
  }
  
};


