// -*- C++ -*-
/*!
 * @file  CvMonitor.cpp
 * @brief Image Viewer for KinectRTC
 * @date $Date$
 *
 * $Id$
 */


#include "CvMonitor.h"




// Module specification
// <rtc-template block="module_spec">
static const char* cvmonitor_spec[] =
  {
    "implementation_id", "CvMonitor",
    "type_name",         "CvMonitor",
    "description",       "Image Viewer for KinectRTC",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Sample",
    "activity_type",     "PERIODOC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
   "conf.default.image_width", "320",
   "conf.default.image_height", "240",
   "conf.default.image_channel", "3",
   "conf.default.dist", "180",

    // Widget
   "conf.__widget__.image_width", "text",
   "conf.__widget__.image_height", "text",
   "conf.__widget__.image_channel", "text",
   "conf.__widget__.dist", "text",

    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
CvMonitor::CvMonitor(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager)
   , m_rawImageIn("rawImage", m_rawImage)
   , m_pointsIn("points", m_points)


    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
CvMonitor::~CvMonitor()
{
}

/*!
 * @brief initialize
 */
RTC::ReturnCode_t CvMonitor::onInitialize()
{

  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
   addInPort("rawImage", m_rawImageIn);
   addInPort("points", m_pointsIn);
 
  // Set OutPort buffer


  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
    bindParameter("image_width", m_image_width, "320");
    bindParameter("image_height", m_image_height, "240");
    bindParameter("image_channel", m_image_channel, "3");
    bindParameter("dist", m_dist, "180");


  // </rtc-template>

  // OnInitialize[[
  // OnInitialize Procedure of your RTC


  // ]]OnInitialize
  return RTC::RTC_OK;
}

#if ON_FINALIZE
RTC::ReturnCode_t CvMonitor::onFinalize()
{
  // OnFinalize[[
  // OnFinalize Procedure of your RTC

  // ]]OnFinalize
  return RTC::RTC_OK;

}
#endif

#if ON_STARTUP
RTC::ReturnCode_t CvMonitor::onStartup(RTC::UniqueId ec_id)
{
  // OnStartup[[
  // OnStartuo Procedure of your RTC

  // ]]OnStartup
  return RTC::RTC_OK;
}
#endif

#if ON_SHUTDOWN
RTC::ReturnCode_t CvMonitor::onShutdown(RTC::UniqueId ec_id)
{
  // OnShutdown[[
  // OnShutdown Procedure of your RTC

  // ]]OnShutdown
  return RTC::RTC_OK;
}
#endif

#if ON_ACTIVATED
RTC::ReturnCode_t CvMonitor::onActivated(RTC::UniqueId ec_id)
{
  // OnActivated[[
  // OnActivated Procedure of your RTC

// OpenCV[[
//  OnActivated 
   m_img=cvCreateImage(cvSize(m_image_width, m_image_height),IPL_DEPTH_8U, m_image_channel);
   imgSize = m_image_width * m_image_height * m_image_channel;

// ]]OpenCV

  //画像表示用ウィンドウの作成
  cvNamedWindow("CaptureImage", CV_WINDOW_AUTOSIZE);
  
  std::cout << "m_img->nChannels :" << m_img->nChannels << std::endl;
  std::cout << "m_img->width :" << m_img->width << std::endl;
  std::cout << "m_img->height :" << m_img->height << std::endl;

  count = 0;
  tm_pre = coil::gettimeofday();
  // ]]OnActivated
  return RTC::RTC_OK;
}
#endif

#if ON_DEACTIVATED
RTC::ReturnCode_t CvMonitor::onDeactivated(RTC::UniqueId ec_id)
{
  // OnDeactivated[[
  // OnDeactivated Procedure of your RTC

// OpenCV[[
  cvReleaseImage(&m_img);

// ]]OpenCV

  cvDestroyWindow("CaptureImage");
  // ]]OnDeactivated
  return RTC::RTC_OK;
}
#endif

#if ON_EXECUTE
RTC::ReturnCode_t CvMonitor::onExecute(RTC::UniqueId ec_id)
{
  // OnExecute[[
  // OnExecute Procedure of your RTC
  int imgLen;
  int dataLen;
  char *imgBuff;

  int nPoints = 0;
  int Apos = 9999;

  
  if (!m_rawImageIn.isNew()){
      return RTC::RTC_OK;
  }

  m_rawImageIn.read();
  dataLen = m_rawImage.data.length();

  if (dataLen <= 0){
      return RTC::RTC_OK;
  }

  if(m_pointsIn.isNew()){
    m_pointsIn.read();
	nPoints = m_points.data.length()/2;
  }

//  if(m_angleIn.isNew()){
//    m_angleIn.read();
//	Apos = (int)( dist* tan( m_angle.data) +160);
//  }

  if(imgSize != m_image_width*m_image_height*m_image_channel){
	cvReleaseImage(&m_img);
	m_img = cvCreateImage(cvSize(m_image_width, m_image_height),IPL_DEPTH_8U, m_image_channel);
    imgSize = m_image_width*m_image_height*m_image_channel;
	std::cout << "m_img->width :" << m_img->width << std::endl;
    std::cout << "m_img->height :" << m_img->height << std::endl;

	count=0;
  }


  imgBuff = (char *)&(m_rawImage.data[0]);

  if(imgSize > dataLen ){
	imgLen = dataLen;
  }else{
	imgLen = imgSize;
  }

  memcpy(m_img->imageData,(void *)imgBuff, imgLen);

  for(int i=0; i<nPoints; i++){
    CvPoint p;
    p.x = m_points.data[i*2];
    p.y = m_points.data[i*2+1];
  
	//std::cout << " (" << p.x <<"," << p.y << ")" ;
    cvCircle(m_img,p,3,CV_RGB(i*50, 255,0),1);
  }
 // if(nPoints > 0){  std::cout << std::endl ; }

  if(Apos > 0 && Apos < 320){
	CvPoint p1,p2;
    p1.x = p2.x = Apos;
    p1.y = 0; p2.y = 240;
	cvLine(m_img, p1, p2, CV_RGB(255,0,0));
  }
  cvShowImage("CaptureImage", m_img);
  
  cvWaitKey(1);

  if (count > 100)
    {
      count = 0;
      coil::TimeValue tm;
      tm = coil::gettimeofday();
      double sec(tm - tm_pre);
      if (sec > 1.0 && sec < 1000.0)
        {
          std::cout << 100.0/sec << " [FPS]" << std::endl;
        }
      tm_pre = tm;
    }
  ++count;

  while (m_rawImageIn.isNew()){ m_rawImageIn.read(); }
  while (m_pointsIn.isNew()){ m_pointsIn.read(); }

  m_rawImageIn.read();
  // ]]OnExecute
  return RTC::RTC_OK;
}
#endif

#if ON_ABORTING
RTC::ReturnCode_t CvMonitor::onAborting(RTC::UniqueId ec_id)
{
  // OnAborting[[
  // OnAborting Procedure of your RTC

  // ]]OnAborting
  return RTC::RTC_OK;
}
#endif

#if ON_ERROR
RTC::ReturnCode_t CvMonitor::onError(RTC::UniqueId ec_id)
{
  // OnError[[
  // OnError Procedure of your RTC

  // ]]OnError
  return RTC::RTC_OK;
}
#endif

#if ON_RESET
RTC::ReturnCode_t CvMonitor::onReset(RTC::UniqueId ec_id)
{
  // OnReset[[
  // OnReset Procedure of your RTC

  // ]]OnReset
  return RTC::RTC_OK;
}
#endif

#if ON_STATEUPDATE
RTC::ReturnCode_t CvMonitor::onStateUpdate(RTC::UniqueId ec_id)
{
  // OnStateUpdate[[
  // OnStateUpdate Procedure of your RTC

  // ]]OnStateUpdate
  return RTC::RTC_OK;
}
#endif

#if ON_RATECHANGED
RTC::ReturnCode_t CvMonitor::onRateChanged(RTC::UniqueId ec_id)
{
  // OnRateChanged[[
  // OnRateChanged Procedure of your RTC

  // ]]OnRateChanged
  return RTC::RTC_OK;
}
#endif


extern "C"
{
 
  void CvMonitorInit(RTC::Manager* manager)
  {
    coil::Properties profile(cvmonitor_spec);
    manager->registerFactory(profile,
                             RTC::Create<CvMonitor>,
                             RTC::Delete<CvMonitor>);
  }
  
}



