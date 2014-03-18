// -*- C++ -*-
/*!
 * @file  CvMonitor.h
 * @brief AudioInput Module
 * @date  $Date$
 *
 * $Id$
 */

#ifndef __RTC_CVMONITOR_H__
#define __RTC_CVMONITOR_H__


// Headers[[
// Entry Headers

// OpenCV[[
#include <opencv/cv.h>
#include <opencv/highgui.h>

// ]]OpenCV
// ]]Headers

/*********************************/

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>


/*****  Define Event Handler *****/

#define  ON_FINALIZE	1
#define  ON_STARTUP	0
#define  ON_SHUTDOWN	0
#define  ON_ACTIVATED	1
#define  ON_DEACTIVATED	1
#define  ON_EXECUTE	1
#define  ON_ABORTING	0
#define  ON_ERROR	0
#define  ON_RESET	0
#define  ON_STATEUPDATE	0
#define  ON_RATECHANGED	0

/********************************/

using namespace RTC;

/*!
 * @class CvMonitor
 * @brief Image Viewer for KinectRTC
 *
 */
class CvMonitor
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  CvMonitor(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~CvMonitor();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

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

#if ON_FINALIZE
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
#endif

#if ON_STARTUP
  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);
#endif

#if ON_SHUTDOWN
  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);
#endif

#if ON_ACTIVATED
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
#endif

#if ON_DEACTIVATED
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
#endif

#if ON_EXECUTE
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
#endif

#if ON_ABORTING
  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);
#endif

#if ON_ERROR
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
   virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);
#endif

#if ON_RESET
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
   virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
#endif

#if ON_STATEUPDATE
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);
#endif

#if ON_RATECHANGED
  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);
#endif

 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
   int m_image_width;
   int m_image_height;
   int m_image_channel;
   int m_dist;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
   RTC::TimedOctetSeq m_rawImage;
   InPort<RTC::TimedOctetSeq> m_rawImageIn;
   RTC::TimedShortSeq m_points;
   InPort<RTC::TimedShortSeq> m_pointsIn;

  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">


  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">

  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">

  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">

  // PrivateAttrs[[
  // Entry private attributes

// OpenCV[[
  IplImage* m_img;
  int imgSize;

// ]]OpenCV

  coil::TimeValue tm_pre;
  int count;
  // ]]PrivateAttrs
  // </rtc-template>

  // <rtc-template block="private_operation">
  // PrivateOperation[[
  // Entry private operation

  // ]]PrivateOperation
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void CvMonitorInit(RTC::Manager* manager);
};

#endif // __RTC_CVMONITOR_H__
