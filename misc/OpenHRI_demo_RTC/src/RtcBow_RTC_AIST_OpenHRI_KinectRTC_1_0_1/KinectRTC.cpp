// -*- C++ -*-
/*!
 * @file  KinectRTC.cpp
 * @brief Kinect component for KINECT SDK 1.6
 * @date $Date$
 *
 * $Id$
 */


#include "KinectRTC.h"

// Module specification
// 
static const char* kinectrtc_spec[] =
  {
    "implementation_id", "KinectRTC",
    "type_name",         "KinectRTC",
    "description",       "Kinect component for KINECT SDK 1.6",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "OpenHRI",
    "activity_type",     "PERIODOC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
   "conf.default.max_angle", "0.3",
   "conf.default.min_angle", "-0.3",
   "conf.default.capture_mode", "2",
   "conf.default.intval", "100",

    // Widget
   "conf.__widget__.max_angle", "text",
   "conf.__widget__.min_angle", "text",
   "conf.__widget__.capture_mode", "text",
   "conf.__widget__.intval", "text",

    // Constraints
    ""
  };
// 

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
KinectRTC::KinectRTC(RTC::Manager* manager)
    // 
  : RTC::DataFlowComponentBase(manager)
   , m_commandIn("command", m_command)
   , m_audio_signalOut("audio_signal", m_audio_signal)
   , m_sound_src_angleOut("sound_src_angle", m_sound_src_angle)
   , m_rawImageOut("rawImage", m_rawImage)
   , m_trackingOut("tracking", m_tracking)
   , m_skeletonOut("skeleton", m_skeleton)


    // 
{
}

/*!
 * @brief destructor
 */
KinectRTC::~KinectRTC()
{
}

/*!
 * @brief initialize
 */
RTC::ReturnCode_t KinectRTC::onInitialize()
{

  // Registration: InPort/OutPort/Service
  // 
  // Set InPort buffers
   addInPort("command", m_commandIn);
 
  // Set OutPort buffer
   addOutPort("audio_signal", m_audio_signalOut);
   addOutPort("sound_src_angle", m_sound_src_angleOut);
   addOutPort("rawImage", m_rawImageOut);
   addOutPort("tracking", m_trackingOut);
   addOutPort("skeleton", m_skeletonOut);


  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports
  
  // 

  // 
  // Bind variables and configuration variable
    bindParameter("max_angle", m_max_angle, "0.3");
    bindParameter("min_angle", m_min_angle, "-0.3");
    bindParameter("capture_mode", m_capture_mode, "2");
    bindParameter("intval", m_intval, "100");


  // 

  // OnInitialize[[
  // OnInitialize Procedure of your RTC

  kinect = new KinectImpl();

  if(kinect && kinect->m_pNuiSensor != NULL){
    kinect->initAudio();
    kinect->initColorImage();
    kinect->initDepthImage();

    m_rawImage.data.length(kinect->img_size);
  }
  trackingId=0;
  // ]]OnInitialize
  return RTC::RTC_OK;
}

#if ON_FINALIZE
RTC::ReturnCode_t KinectRTC::onFinalize()
{
  // OnFinalize[[
  // OnFinalize Procedure of your RTC
 delete kinect;

  // ]]OnFinalize
  return RTC::RTC_OK;

}
#endif

#if ON_STARTUP
RTC::ReturnCode_t KinectRTC::onStartup(RTC::UniqueId ec_id)
{
  // OnStartup[[
  // OnStartuo Procedure of your RTC

  // ]]OnStartup
  return RTC::RTC_OK;
}
#endif

#if ON_SHUTDOWN
RTC::ReturnCode_t KinectRTC::onShutdown(RTC::UniqueId ec_id)
{
  // OnShutdown[[
  // OnShutdown Procedure of your RTC

  // ]]OnShutdown
  return RTC::RTC_OK;
}
#endif

#if ON_ACTIVATED
RTC::ReturnCode_t KinectRTC::onActivated(RTC::UniqueId ec_id)
{
  // OnActivated[[
  // OnActivated Procedure of your RTC

  count = 0;
  // ]]OnActivated
  return RTC::RTC_OK;
}
#endif

#if ON_DEACTIVATED
RTC::ReturnCode_t KinectRTC::onDeactivated(RTC::UniqueId ec_id)
{
  // OnDeactivated[[
  // OnDeactivated Procedure of your RTC

  // ]]OnDeactivated
  return RTC::RTC_OK;
}
#endif

#if ON_EXECUTE
RTC::ReturnCode_t KinectRTC::onExecute(RTC::UniqueId ec_id)
{
  // OnExecute[[
  // OnExecute Procedure of your RTC

  HRESULT hr = S_OK;
  
  hr = kinect->captureAudio();
  if(SUCCEEDED(hr)){

    if(kinect->dConf>0.9){
     // _tprintf(_T("Position: %f\tConfidence: %f\tBeam Angle = %f\r\n"), kinect->dAngle, kinect->dConf, kinect->dBeamAngle);
      m_sound_src_angle.data = kinect->dAngle;
      m_sound_src_angleOut.write();
    }
    if(kinect->cbProduced > 0 && (kinect->dBeamAngle > m_min_angle && kinect->dBeamAngle < m_max_angle)){
            m_audio_signal.data.length(kinect->cbProduced);
            memcpy(m_audio_signal.data.NP_data(), kinect->pbOutputBuffer, kinect->cbProduced);
            m_audio_signalOut.write();
	}
  }
  
  if(m_capture_mode == 1){

	if(kinect->isCapturing == 0){ 
	  if( kinect->getColorImage() > 0){
            memcpy((void *)&(m_rawImage.data[0]), kinect->imageBuffer, kinect->img_size);
            m_rawImageOut.write();;
	  }
        }
	if(m_intval < 1) { m_intval=1; }
        kinect->isCapturing =  (kinect->isCapturing +1 ) % m_intval;

  }else if(m_capture_mode == 2){

	if(kinect->isCapturing == 0){ 

	  if( kinect->getDepthImage() > 0){
	    int res = kinect->GetSkeleton();
	      if(count == 0){
		    memcpy((void *)&(m_rawImage.data[0]), kinect->imageDepthBuffer, kinect->depth_img_size * 3);
		    m_rawImageOut.write();
             }
             // count = (count+1) % 3;
          
		  if(kinect->trackingId > 0 && res > 0){
		    m_tracking.data.length(kinect->trackingDataSize);
		    memcpy((void *)&(m_tracking.data[0]), kinect->trackingData, kinect->trackingDataSize *2);
		    m_trackingOut.write();

		    m_skeleton.data.length(kinect->nTracking * 3);
		    memcpy((void *)&(m_skeleton.data[0]), kinect->skeletonData, kinect->nTracking * 12);
		    m_skeletonOut.write();
		  }
		
	  }
	}

	if(m_intval < 1) { m_intval=1; }
	kinect->isCapturing =  (kinect->isCapturing +1 ) % m_intval;
  }
  // ]]OnExecute
  return RTC::RTC_OK;
}
#endif

#if ON_ABORTING
RTC::ReturnCode_t KinectRTC::onAborting(RTC::UniqueId ec_id)
{
  // OnAborting[[
  // OnAborting Procedure of your RTC

  // ]]OnAborting
  return RTC::RTC_OK;
}
#endif

#if ON_ERROR
RTC::ReturnCode_t KinectRTC::onError(RTC::UniqueId ec_id)
{
  // OnError[[
  // OnError Procedure of your RTC

  // ]]OnError
  return RTC::RTC_OK;
}
#endif

#if ON_RESET
RTC::ReturnCode_t KinectRTC::onReset(RTC::UniqueId ec_id)
{
  // OnReset[[
  // OnReset Procedure of your RTC

  // ]]OnReset
  return RTC::RTC_OK;
}
#endif

#if ON_STATEUPDATE
RTC::ReturnCode_t KinectRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  // OnStateUpdate[[
  // OnStateUpdate Procedure of your RTC

  // ]]OnStateUpdate
  return RTC::RTC_OK;
}
#endif

#if ON_RATECHANGED
RTC::ReturnCode_t KinectRTC::onRateChanged(RTC::UniqueId ec_id)
{
  // OnRateChanged[[
  // OnRateChanged Procedure of your RTC

  // ]]OnRateChanged
  return RTC::RTC_OK;
}
#endif


extern "C"
{
 
  void KinectRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(kinectrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<KinectRTC>,
                             RTC::Delete<KinectRTC>);
  }
  
}



