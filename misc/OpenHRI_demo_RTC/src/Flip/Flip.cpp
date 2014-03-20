// -*- C++ -*-
/*!
 * @file  Flip.cpp
 * @brief Flip image component
 * @date $Date$
 *
 * $Id$
 */

#include "Flip.h"

// Module specification
// <rtc-template block="module_spec">
static const char* flip_spec[] =
  {
    "implementation_id", "Flip",
    "type_name",         "Flip",
    "description",       "Flip image component",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.flipMode", "0",
    // Widget
    "conf.__widget__.flipMode", "radio",
    // Constraints
    "conf.__constraints__.flipMode", "(-1,0,1)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Flip::Flip(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_originalImageIn("originalImage", m_originalImage),
    m_flippedImageOut("flippedImage", m_flippedImage)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Flip::~Flip()
{
}


/*!
 * コンポーネント自身の各種初期化処理
 */
RTC::ReturnCode_t Flip::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("originalImage", m_originalImageIn);
  
  // Set OutPort buffer
  addOutPort("flippedImage", m_flippedImageOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("flipMode", m_flipMode, "0");
  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Flip::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Flip::onActivated(RTC::UniqueId ec_id)
{
  // イメージ用メモリの初期化
  m_imageBuff = NULL;
  m_flipImageBuff = NULL;
 
  // OutPortの画面サイズの初期化
  m_flippedImage.width = 0;
  m_flippedImage.height = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Flip::onDeactivated(RTC::UniqueId ec_id)
{
  if(m_imageBuff != NULL)
  {
    // イメージ用メモリの解放
    cvReleaseImage(&m_imageBuff);
    cvReleaseImage(&m_flipImageBuff);
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Flip::onExecute(RTC::UniqueId ec_id)
{
   // 新しいデータのチェック
   if (m_originalImageIn.isNew()) {
     // InPortデータの読み込み
     m_originalImageIn.read();
 
     // InPortとOutPortの画面サイズ処理およびイメージ用メモリの確保
     if( m_originalImage.width != m_flippedImage.width || m_originalImage.height != m_flippedImage.height)
       {
     m_flippedImage.width = m_originalImage.width;
     m_flippedImage.height = m_originalImage.height;
 
     // InPortのイメージサイズが変更された場合
     if(m_imageBuff != NULL)
       {
         cvReleaseImage(&m_imageBuff);
         cvReleaseImage(&m_flipImageBuff);
       }
 
     // イメージ用メモリの確保
     m_imageBuff = cvCreateImage(cvSize(m_originalImage.width, m_originalImage.height), IPL_DEPTH_8U, 3);
     m_flipImageBuff = cvCreateImage(cvSize(m_originalImage.width, m_originalImage.height), IPL_DEPTH_8U, 3);
       }
 
     // InPortの画像データをIplImageのimageDataにコピー
     memcpy(m_imageBuff->imageData,(void *)&(m_originalImage.pixels[0]),m_originalImage.pixels.length());
 
     // InPortからの画像データを反転する。 m_flipMode 0: X軸周り, 1: Y軸周り, -1: 両方の軸周り
     cvFlip(m_imageBuff, m_flipImageBuff, m_flipMode);
 
     // 画像データのサイズ取得
     int len = m_flipImageBuff->nChannels * m_flipImageBuff->width * m_flipImageBuff->height;
     m_flippedImage.pixels.length(len);
 
     // 反転した画像データをOutPortにコピー
     memcpy((void *)&(m_flippedImage.pixels[0]),m_flipImageBuff->imageData,len);
 
     // 反転した画像データをOutPortから出力する。
     m_flippedImageOut.write();
   }
 
   return RTC::RTC_OK;
  
}

/*
RTC::ReturnCode_t Flip::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Flip::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void FlipInit(RTC::Manager* manager)
  {
    coil::Properties profile(flip_spec);
    manager->registerFactory(profile,
                             RTC::Create<Flip>,
                             RTC::Delete<Flip>);
  }
  
};


