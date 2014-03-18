// -*- C++ -*-
/*!
 * @file  Kinect.h
 * @brief AudioInput Module
 * @date  $Date$
 *
 * $Id$
 */

#ifndef KINECTIMPL_H
#define KINECTIMPL_H

/****** KINECT ****/
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600
#define WINVAR 0x0600

#include <windows.h>
#include <wmcodecdsp.h>
#include <MMDeviceApi.h>
#include <NuiApi.h>
#include <devicetopology.h>
#include <dmo.h>
#include <uuids.h>
#include <mfapi.h>
#include <tchar.h>
#include <strsafe.h>
#include <conio.h>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>


#include "PThread.h"

/**** 
  Static Media Buffer 
  This code come from 'MicArrays_guide.doc' by Microsoft Coorp.
****/

class CStaticMediaBuffer : public IMediaBuffer {
public:
   CStaticMediaBuffer() {}
   CStaticMediaBuffer(BYTE *pData, ULONG ulSize, ULONG ulData) :
      m_pData(pData), m_ulSize(ulSize), m_ulData(ulData), m_cRef(1) {}

   STDMETHODIMP_(ULONG) AddRef() { return 2; }
   STDMETHODIMP_(ULONG) Release() { return 1; }
   STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {
      if (riid == IID_IUnknown) {
         AddRef();
         *ppv = (IUnknown*)this;
         return NOERROR;
      }
      else if (riid == IID_IMediaBuffer) {
         AddRef();
         *ppv = (IMediaBuffer*)this;
         return NOERROR;
      }
      else
         return E_NOINTERFACE;
   }
   STDMETHODIMP SetLength(DWORD ulLength) {m_ulData = ulLength; return NOERROR;}
   STDMETHODIMP GetMaxLength(DWORD *pcbMaxLength) {*pcbMaxLength = m_ulSize; return NOERROR;}
   STDMETHODIMP GetBufferAndLength(BYTE **ppBuffer, DWORD *pcbLength) {
      if (ppBuffer) *ppBuffer = m_pData;
      if (pcbLength) *pcbLength = m_ulData;
      return NOERROR;
   }
   void Init(BYTE *pData, ULONG ulSize, ULONG ulData) {
        m_pData = pData;
        m_ulSize = ulSize;
        m_ulData = ulData;
    }
protected:
   BYTE *m_pData;
   ULONG m_ulSize;
   ULONG m_ulData;
   ULONG m_cRef;
};



/*****
  KINECT Implimentation class:


*******/

class KinectImpl{
public:
	KinectImpl();
	~KinectImpl();

	
	HRESULT initDevice();

	HRESULT initAudio();
	HRESULT captureAudio();
	void finishAudio();

	HRESULT initColorImage();
	void finishColorImage();
	HRESULT initDepthImage();
	void finishDepthImage();

	int initSkeletonTracking();
	int GetSkeleton();

	int readyImageCapture(HANDLE h);
	int imageCapture(HANDLE h, char *Buf, int size);

	int getColorImage();
	int getDepthImage();

	int setTrackingPos(NUI_SKELETON_DATA data, int skelId, int trackingId);
	RGBQUAD Nui_ShortToQuad_Depth( USHORT s );

public:
	HANDLE h_TaskIndex;      // Handler for taskIndex.
	IMediaObject* pMediaObject;   // Media object 
	IPropertyStore* pPropertyStore;  //  PorpertyStore
	INuiAudioBeam* pAudioBeam;   // Audio Bean

	BYTE *pbOutputBuffer;
	CStaticMediaBuffer outputBuffer;
	DMO_OUTPUT_DATA_BUFFER OutputBufferStruct;
	ULONG cbProduced;     // Audio captured length
	
	double dBeamAngle, dAngle;  
	double dConf; 

	///  for capturing image
	HANDLE h_ColorImageEvent;
	HANDLE h_CaptureColorImage;

	///  for capturing image
	HANDLE h_DepthImageEvent;
	HANDLE h_CaptureDepthImage;


	// image buffer 
	char *imageBuffer;
	int img_w, img_h, img_channel, img_size;

	char *imageDepthBuffer;
	unsigned short *imageDepthRawBuffer;
	int depth_img_w, depth_img_h, depth_img_channel, depth_img_size;


	int current_image;
	int max_image_buffers;

	int img_updated;
	int isCapturing;

	NUI_SKELETON_FRAME SkeletonFrame;
	int trackingId;

	short *trackingData;
	int trackingDataSize;

	int nTracking;
	float *skeletonData;

    INuiSensor*    m_pNuiSensor;

};

#endif