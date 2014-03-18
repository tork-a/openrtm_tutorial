// -*- C++ -*-
/*!
 * @file  SampleMotionCaller.cpp
 * @brief SampleMotionCaller image component
 * @date $Date$
 *
 * $Id$
 */

#include "SampleMotionCaller.h"

#include <conio.h>


// Module specification
// <rtc-template block="module_spec">
static const char* SampleMotionCaller_spec[] =
  {
    "implementation_id", "SampleMotionCaller",
    "type_name",         "SampleMotionCaller",
    "description",       "SampleMotionCaller component",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.SampleMotionCallerFileName", "SampleWord.log",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SampleMotionCaller::SampleMotionCaller(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_commandOut("output", m_command)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SampleMotionCaller::~SampleMotionCaller()
{
}


/*!
 * コンポーネント自身の各種初期化処理
 */
RTC::ReturnCode_t SampleMotionCaller::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set OutPort buffers
  addOutPort("command", m_commandOut);
  
 
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  std::cerr << "Help: " << std::endl;
  std::cerr << "  r : move right arm up/down" << std::endl;
  std::cerr << "  l : move left arm up/down" << std::endl;
  std::cerr << " debug mode:" << std::endl;
  std::cerr << "  1 : rightup1" << std::endl;
  std::cerr << "  2 : leftup1" << std::endl;
  std::cerr << "  3 : rightdown1" << std::endl;
  std::cerr << "  4 : leftdown1" << std::endl;
  std::cerr << "  5 : rightup2" << std::endl;
  std::cerr << "  6 : leftup2" << std::endl;
  std::cerr << "  7 : rightup2" << std::endl;
  std::cerr << "  8 : leftup2" << std::endl;

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SampleMotionCaller::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t SampleMotionCaller::onExecute(RTC::UniqueId ec_id)
{
   // 新しい入力のチェック
   if ( _kbhit() ) {
       char c = _getch();
       switch (c) {
       case '1':
           m_command.data = "rightup1";
           break;
       case '2':
           m_command.data = "leftup1";
           break;
       case '3':
           m_command.data = "rightdown1";
           break;
       case '4':
           m_command.data = "leftdown1";
           break;
       case '5':
           m_command.data = "rightup2";
           break;
       case '6':
           m_command.data = "leftup2";
           break;
       case '7':
           m_command.data = "rightdown2";
           break;
       case '8':
           m_command.data = "leftdown2";
           break;
       case 'r':
           if ( strcmp(m_command.data, "rightdown1") == 0 ||
                strcmp(m_command.data, "leftdown1") == 0 ) {
               m_command.data = "rightup1";
           } else if ( strcmp(m_command.data, "rightup1") == 0||
                       strcmp(m_command.data, "leftdown2") == 0) {
               m_command.data = "rightdown1";
           } else if ( strcmp(m_command.data, "leftup1") == 0||
                       strcmp(m_command.data, "rightdown2") == 0) {
               m_command.data = "rightup2";
           } else if ( strcmp(m_command.data, "rightup2") == 0||
                       strcmp(m_command.data, "leftup2") == 0) {
               m_command.data = "rightdown2";
           }
           break;
       case 'l':
           if ( strcmp(m_command.data, "rightdown2") == 0||
                strcmp(m_command.data, "leftdown1") == 0) {
               m_command.data = "leftup1";
           } else if ( strcmp(m_command.data, "leftup1") == 0||
                       strcmp(m_command.data, "rightdown2") == 0) {
               m_command.data = "leftdown1";
           } else if ( strcmp(m_command.data, "rightup1") == 0||
                       strcmp(m_command.data, "leftdown2") == 0) {
               m_command.data = "leftup2";
           } else if ( strcmp(m_command.data, "leftup2") == 0||
                       strcmp(m_command.data, "rightup2") == 0) {
               m_command.data = "leftdown2";
           }
           break;
       }
       std::cerr << "Keyboad input " << c << ", command =  " << m_command.data << std::endl;
       m_commandOut.write();
   }
   return RTC::RTC_OK;
  
}

/*
RTC::ReturnCode_t SampleMotionCaller::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleMotionCaller::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SampleMotionCallerInit(RTC::Manager* manager)
  {
    coil::Properties profile(SampleMotionCaller_spec);
    manager->registerFactory(profile,
                             RTC::Create<SampleMotionCaller>,
                             RTC::Delete<SampleMotionCaller>);
  }
  
};


