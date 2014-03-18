/*

  Kinect implement class

Copyright (c) 2012, Isao Hara, ISRI, AIST, Japan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  1.Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  2.Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "Kinect.h"

	static int SkeletonPos[] = {
/*
	  NUI_SKELETON_POSITION_HEAD,
	  NUI_SKELETON_POSITION_SHOULDER_LEFT,
	  NUI_SKELETON_POSITION_ELBOW_LEFT,
	  NUI_SKELETON_POSITION_WRIST_LEFT,
	  NUI_SKELETON_POSITION_SHOULDER_RIGHT ,
	  NUI_SKELETON_POSITION_ELBOW_RIGHT,
	  NUI_SKELETON_POSITION_WRIST_RIGHT,
	  NUI_SKELETON_POSITION_SPINE,

	  NUI_SKELETON_POSITION_HAND_LEFT,
	  NUI_SKELETON_POSITION_HAND_RIGHT,

	  NUI_SKELETON_POSITION_HIP_CENTER,
	  NUI_SKELETON_POSITION_HIP_LEFT,
	  NUI_SKELETON_POSITION_HIP_RIGHT,
*/
    NUI_SKELETON_POSITION_HIP_CENTER,
	NUI_SKELETON_POSITION_SPINE,
	NUI_SKELETON_POSITION_SHOULDER_CENTER,
	NUI_SKELETON_POSITION_HEAD,
	NUI_SKELETON_POSITION_SHOULDER_LEFT,
	NUI_SKELETON_POSITION_ELBOW_LEFT,
	NUI_SKELETON_POSITION_WRIST_LEFT,
	NUI_SKELETON_POSITION_HAND_LEFT,
	NUI_SKELETON_POSITION_SHOULDER_RIGHT,
	NUI_SKELETON_POSITION_ELBOW_RIGHT,
	NUI_SKELETON_POSITION_WRIST_RIGHT,
	NUI_SKELETON_POSITION_HAND_RIGHT,
	NUI_SKELETON_POSITION_HIP_LEFT,
	NUI_SKELETON_POSITION_KNEE_LEFT,
	NUI_SKELETON_POSITION_ANKLE_LEFT,
	NUI_SKELETON_POSITION_FOOT_LEFT,
	NUI_SKELETON_POSITION_HIP_RIGHT,
	NUI_SKELETON_POSITION_KNEE_RIGHT,
	NUI_SKELETON_POSITION_ANKLE_RIGHT,
	NUI_SKELETON_POSITION_FOOT_RIGHT,

   };

/*


*/
KinectImpl::KinectImpl(){
    /**** mmHandle ****/
    h_TaskIndex = NULL;
    pMediaObject = NULL;
    pPropertyStore = NULL;
    pAudioBeam = NULL;
    pbOutputBuffer = NULL;

    imageBuffer = NULL;
    imageDepthBuffer = NULL;

    h_CaptureColorImage   = NULL;
    h_CaptureDepthImage   = NULL;

    isCapturing = 1;
    trackingId=0;
    nTracking = sizeof(SkeletonPos)/sizeof(int);
    trackingDataSize = nTracking* 2;
    trackingData = new short[trackingDataSize];

    skeletonData = new float[nTracking * 3];

    max_image_buffers = 4;

    CoInitialize(NULL);
//    CoInitializeEx(NULL, COINIT_MULTITHREADED);


	initDevice();
/*
    NuiInitialize(NUI_INITIALIZE_FLAG_USES_AUDIO
				| NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX
				| NUI_INITIALIZE_FLAG_USES_SKELETON
				| NUI_INITIALIZE_FLAG_USES_COLOR
				);
*/

}


/*


*/
KinectImpl::~KinectImpl()
{
    isCapturing = 0;

    finishAudio();
    finishColorImage();
    finishDepthImage();

	if (m_pNuiSensor)
    {
        m_pNuiSensor->NuiShutdown();
		m_pNuiSensor->Release();
		m_pNuiSensor = NULL;
    }
    // NuiShutdown();
    CoUninitialize();
}

HRESULT
KinectImpl::initDevice()
{
    INuiSensor * pNuiSensor;

    int iSensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&iSensorCount);
    if (FAILED(hr))
    {
        return hr;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);
        if (FAILED(hr))
        {
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr)
        {
            m_pNuiSensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

    if (NULL != m_pNuiSensor)
    {
        hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_AUDIO
				| NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX
				| NUI_INITIALIZE_FLAG_USES_SKELETON
				| NUI_INITIALIZE_FLAG_USES_COLOR
				);
    }

	return hr;
}




/*


*/
HRESULT
KinectImpl::initAudio(){
  HRESULT hr = S_OK;

  // Set high priority to avoid getting preempted while capturing sound
#if 0
  DWORD m_TaskIndex;
  h_TaskIndex = AvSetMmThreadCharacteristics("Audio", &m_TaskIndex);

  if (h_TaskIndex == NULL) {
    hr = E_FAIL;
	puts("Failed to set thread priority\n");
//	return hr;
  }
#endif

  // DMO initialization
  INuiAudioBeam* pAudio;
  hr = m_pNuiSensor->NuiGetAudioSource(&pAudio);
  if (FAILED(hr)) {
    puts("Failed to NuiGetAudioSource\n");
	return hr;
  }
 
  hr = pAudio->QueryInterface(IID_IMediaObject, (void**)&pMediaObject);
  if (FAILED(hr)) {
    puts("Fail to call pAudio->QueryInterface(pMediaObject)\n");
	return hr;
  }

  hr = pAudio->QueryInterface(IID_IPropertyStore, (void**)&pPropertyStore);
  if (FAILED(hr)) {
    puts("Fail to call pAudio->QueryInterface(pPropertyStore)\n");
    return hr;
  }
  pAudio->Release();

  // Set AEC-MicArray DMO system mode.
  // This must be set for the DMO to work properly
  PROPVARIANT pvSysMode;
  PropVariantInit(&pvSysMode);
  pvSysMode.vt = VT_I4;
  pvSysMode.lVal = (LONG)(4);
  hr = pPropertyStore->SetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, pvSysMode);
  if (FAILED(hr)) {
    puts("Fail to call pPS->SetValue\n");
    return hr;
  }
  PropVariantClear(&pvSysMode);

  // NOTE: Need to wait 4 seconds for device to be ready right after initialization
 
  DWORD dwWait = 4;
  while (dwWait > 0){
    _tprintf(_T("Device will be ready for recording in %d second(s).\r"), dwWait--);
    Sleep(1000);
  }
  _tprintf(_T("Device is ready.                                                  \n"));

  /*** set output buffer ***/
  memset(&OutputBufferStruct,0, sizeof(OutputBufferStruct));
  OutputBufferStruct.pBuffer = &outputBuffer;

  // Set DMO output format  
  WAVEFORMATEX wfxOut = {WAVE_FORMAT_PCM, 1, 16000, 32000, 2, 16, 0};
  DMO_MEDIA_TYPE mt = {0};

  hr = MoInitMediaType(&mt, sizeof(WAVEFORMATEX));
  if (FAILED(hr)) {
    puts("MoInitMediaType failed\n");
    return hr;
  }
    
  mt.majortype = MEDIATYPE_Audio;
  mt.subtype = MEDIASUBTYPE_PCM;
  mt.lSampleSize = 0;
  mt.bFixedSizeSamples = TRUE;
  mt.bTemporalCompression = FALSE;
  mt.formattype = FORMAT_WaveFormatEx;	
  memcpy(mt.pbFormat, &wfxOut, sizeof(WAVEFORMATEX));
    
  hr = pMediaObject->SetOutputType(0, &mt, 0); 
  if (FAILED(hr)) {
    puts("SetOutputType failed\n");
    return hr;
  }
  MoFreeMediaType(&mt);

  hr = pMediaObject->AllocateStreamingResources();
  if (FAILED(hr)) {
    puts("AllocateStreamingResources failed\n");
    return hr;
  }
    

  int iFrameSize;
  PROPVARIANT pvFrameSize;
  PropVariantInit(&pvFrameSize);
  hr = pPropertyStore->GetValue(MFPKEY_WMAAECMA_FEATR_FRAME_SIZE, &pvFrameSize);
  if (FAILED(hr)) {
    puts("Fail to call pPS->GetValue\n");
    return hr;
  }
  iFrameSize = pvFrameSize.lVal;
  PropVariantClear(&pvFrameSize);

  // allocate output buffer
  DWORD cOutputBufLen = wfxOut.nSamplesPerSec * wfxOut.nBlockAlign;
  pbOutputBuffer = new BYTE[cOutputBufLen];

  if (pbOutputBuffer == NULL) {
    puts("Fail to allocate output buffer.\n");
	return hr;
  }
  
  //  
  hr = pMediaObject->QueryInterface(IID_INuiAudioBeam, (void**)&pAudioBeam);
  if (FAILED(hr)) {
    puts("QueryInterface for IID_INuiAudioBeam failed\n");
    return hr;
  }

  outputBuffer.Init((byte*)pbOutputBuffer, cOutputBufLen, 0);

  return S_OK;
}

/*

*/
void 
KinectImpl::finishAudio()
{
	if (pMediaObject != NULL) {
		pMediaObject->Release();
		pMediaObject = NULL;
	}

	if (pPropertyStore != NULL) {
		pPropertyStore->Release();
		pPropertyStore = NULL;
	}

	if( pbOutputBuffer != NULL){
		delete pbOutputBuffer;
	}

	if(h_TaskIndex != NULL){
      AvRevertMmThreadCharacteristics(h_TaskIndex);
	  h_TaskIndex = NULL;
	}

}

/*


*/
HRESULT 
KinectImpl::captureAudio(){

  HRESULT hr = S_OK;
  DWORD dwStatus;


  outputBuffer.SetLength(0);
  OutputBufferStruct.dwStatus = 0;
  cbProduced = 0;

  hr = pMediaObject->ProcessOutput(0, 1, &OutputBufferStruct, &dwStatus);
  if (FAILED(hr)) {
    puts("ProcessOutput failed.\n");
    return hr;
  }

  if(hr != S_FALSE){
	hr = outputBuffer.GetBufferAndLength(NULL, &cbProduced);
	if (FAILED(hr)) {
      puts("GetBufferAndLength failed\n");
      return hr;
    }
	hr = pAudioBeam->GetBeam(&dBeamAngle);
    hr = pAudioBeam->GetPosition(&dAngle, &dConf);
  }

  return hr;
}

/*

*/
HRESULT
KinectImpl::initColorImage()
{
  HRESULT hr = S_OK;

  // Event Handler
  h_ColorImageEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
  h_CaptureColorImage   = NULL;

  img_w = 640; img_h = 480; img_channel = 4;
  img_size = img_w * img_h * img_channel; 
 
  //  Image Stream
  hr = m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,
	                      NUI_IMAGE_RESOLUTION_640x480,
						  0, 2, h_ColorImageEvent, &h_CaptureColorImage);
  if (FAILED(hr)) {
    puts("NuiImageStreamOpen failed.\n");
    return hr;
  }

  imageBuffer = new char[img_size * max_image_buffers];

  return S_OK;
}

/*

*/
void 
KinectImpl::finishColorImage()
{
  if (imageBuffer != NULL) {
    delete imageBuffer;
    imageBuffer = NULL;
  }
  h_CaptureColorImage   = NULL;

}

/*


*/
HRESULT
KinectImpl::initDepthImage()
{
  HRESULT hr = S_OK;

  // Event Handler
  h_DepthImageEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
  h_CaptureDepthImage   = NULL;

  depth_img_w = 320; depth_img_h = 240;  depth_img_channel = 1;
  depth_img_size = depth_img_w * depth_img_h * depth_img_channel; 
 
  //  Image Stream
  hr = m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
	                      NUI_IMAGE_RESOLUTION_320x240,
						  0, 2, h_DepthImageEvent, &h_CaptureDepthImage);
  if (FAILED(hr)) {
    puts("NuiImageStreamOpen failed.\n");
    return hr;
  }
  imageDepthBuffer = new char[depth_img_size * 3 * max_image_buffers];
  imageDepthRawBuffer = new unsigned short[depth_img_size * max_image_buffers];
  return S_OK;
}

void 
KinectImpl::finishDepthImage()
{
  if (imageDepthBuffer != NULL) {
    delete imageDepthBuffer;
    imageDepthBuffer = NULL;
  }

  h_CaptureDepthImage   = NULL;
}


int
KinectImpl::initSkeletonTracking()
{
  HRESULT hr;

  if(h_CaptureDepthImage == NULL){
    hr = initDepthImage();
  }
  hr = m_pNuiSensor->NuiSkeletonTrackingEnable( h_DepthImageEvent, 0 );

  if( FAILED( hr ) ) return -1;
 return 1;
}

/*


*/
int 
KinectImpl::GetSkeleton()
{

  HRESULT hr = m_pNuiSensor->NuiSkeletonGetNextFrame(0,&SkeletonFrame);
  if(FAILED(hr)){
	  return -1;
  }

  trackingId=0;

  for(int i=0;i<NUI_SKELETON_COUNT;i++){
    if(SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
	  trackingId=i+1;

      NUI_SKELETON_DATA data = SkeletonFrame.SkeletonData[i];

	  for(int j=0; j< nTracking; j++){

	    int res = setTrackingPos(data, SkeletonPos[j] , j);
		if(res < 0) return -1;
	  }

	  return 1;
	}
  }
  return -1;
}


int 
KinectImpl::setTrackingPos(NUI_SKELETON_DATA data, int skelId, int trackingId)
{
  float x,y;

  if(data.eSkeletonPositionTrackingState[skelId] == NUI_SKELETON_POSITION_NOT_TRACKED){
	  return -1;
  }
  if( data.SkeletonPositions[skelId].x ==  0 &&
	   data.SkeletonPositions[skelId].y == 0 &&
	    data.SkeletonPositions[skelId].z == 0) return -1;


  NuiTransformSkeletonToDepthImage(data.SkeletonPositions[skelId], &x, &y, NUI_IMAGE_RESOLUTION_320x240 );
  trackingData[trackingId*2] = (short)x;
  trackingData[trackingId*2+1] = (short)y;

 //  std::cout << " (" << trackingData[trackingId*2] <<"," << trackingData[trackingId*2+1] << ")" ;

  skeletonData[trackingId*3] = data.SkeletonPositions[skelId].x;
  skeletonData[trackingId*3+1] = data.SkeletonPositions[skelId].y;
  skeletonData[trackingId*3+2] = data.SkeletonPositions[skelId].z;

  return 1;
}


/*

*/
int
KinectImpl::readyImageCapture(HANDLE h){
  DWORD rec;
  rec = WaitForSingleObject(h, 0);

  if(rec == WAIT_OBJECT_0){
    return 1;
  }else{
    return 0;
  }
}

/*
  

*/
int
KinectImpl::imageCapture(HANDLE h, char *Buf, int size){

  HRESULT hr;

  // imageFrame
  const NUI_IMAGE_FRAME * pImageFrame = NULL;
  NUI_IMAGE_FRAME ImageFrame;

  hr = m_pNuiSensor->NuiImageStreamGetNextFrame(h, 30, &ImageFrame );
  if( FAILED( hr ) ){
	  return -1;
  }
 
  //フレームから画像データの取得
  // Locked Rectangle
  INuiFrameTexture * pTexture = ImageFrame.pFrameTexture;
  NUI_LOCKED_RECT LockedRect;
  //pImageFrame->pFrameTexture->LockRect( 0, &LockedRect, NULL, 0 );
  pTexture->LockRect( 0, &LockedRect, NULL, 0 );

  if( LockedRect.Pitch != 0 ){
   //copy image buffer

    //memcpy(Buf, LockedRect.pBits, size);
	memcpy(Buf, static_cast<BYTE *>(LockedRect.pBits), LockedRect.size );

  }
  pTexture->UnlockRect( 0 );
  // Release imageFrame
  hr = m_pNuiSensor->NuiImageStreamReleaseFrame( h, &ImageFrame );
  if( FAILED( hr ) ){
	  return -1;
  }
  
  return 1;
}

int
KinectImpl::getColorImage()
{
  if(  readyImageCapture( h_ColorImageEvent) > 0){
    return  imageCapture( h_CaptureColorImage,  imageBuffer, img_size);
  }
  return 0;
}

int
KinectImpl::getDepthImage()
{

  if( readyImageCapture(h_DepthImageEvent) > 0){
    imageCapture(h_CaptureDepthImage, (char *)imageDepthRawBuffer, depth_img_size * 2);

    int i=0;
    for(int y=0; y<240; y++){
	  for(int x=0;x<320;x++){
	    RGBQUAD quad = Nui_ShortToQuad_Depth( imageDepthRawBuffer[i] );
		imageDepthBuffer[i*3] = quad.rgbRed;
		imageDepthBuffer[i*3+1] = quad.rgbGreen;
		imageDepthBuffer[i*3+2] = quad.rgbBlue;
		i++;
	  }
    }
	return 1;
  }
  return 0;
}

/*


*/
RGBQUAD 
KinectImpl::Nui_ShortToQuad_Depth( USHORT s )
{
  USHORT RealDepth = (s & 0xfff8) >> 3;
  USHORT Player = s & 7;
 
 // transform 13-bit depth information into an 8-bit intensity appropriate
 // for display (we disregard information in most significant bit)
  BYTE l = 255 - (BYTE)(256*RealDepth/0x0fff);
  
  RGBQUAD q;
  q.rgbRed = q.rgbBlue = q.rgbGreen = 0;
  
  switch( Player ){
    case 0:
      q.rgbRed = l / 2;
      q.rgbBlue = l / 2;
      q.rgbGreen = l / 2;
      break;

    case 1:   // Red
      q.rgbRed = l;
      break;

    case 2:  // Green
      q.rgbGreen = l;
      break;

    case 3:  // 
      q.rgbRed = l / 4;
      q.rgbGreen = l;
      q.rgbBlue = l;
      break;
    case 4:
      q.rgbRed = l;
      q.rgbGreen = l;
      q.rgbBlue = l / 4;
      break;

    case 5:
      q.rgbRed = l;
      q.rgbGreen = l / 4;
      q.rgbBlue = l;
      break;

    case 6:
      q.rgbRed = l / 2;
      q.rgbGreen = l / 2;
      q.rgbBlue = l;
      break;

    case 7:
      q.rgbRed = 255 - ( l / 2 );
      q.rgbGreen = 255 - ( l / 2 );
      q.rgbBlue = 255 - ( l / 2 );
  }
  return q;
}


#if 0

/******** Thread Function *****/
#ifdef __cplusplus__
extern "C" {
#endif


THREAD_FUNC thread_execution(void *args){

  KinectImpl *kinect = (KinectImpl *)args;

  while(kinect->isCapturing){
    if(kinect->readyImageCapture(kinect->m_hNVFE) > 0){
      if(kinect->imageCapture(kinect->m_pVSH, kinect->imageBuffer, kinect->img_size) > 0){
		  kinect->img_updated = 1;
	  }
	}
  }

  std::cerr << "Thread terminated." << std::endl;

  return;
}


#ifdef __cplusplus__
};
#endif

#endif