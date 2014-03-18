## OpenCV LIBRARIES
if(WIN32)
  set(OPENCV_INC_DIRS "${OPENRTM_DIR}/OpenCV2.3/include" "${OPENRTM_DIR}/OpenCV2.3/include/opencv")
  set(OPENCV_LIB_DIRS "${OPENRTM_DIR}/OpenCV2.3/lib")
  set(OPENCV_LIBS opencv_core231 opencv_highgui231 opencv_imgproc231)
else()
  set(OPENCV_INC_DIRS "/usr/include/opencv")
  set(OPENCV_LIB_DIRS "/usr/lib /usr/local/lib")
  set(OPENCV_LIBS libcv.so libopencv_core.so libopencv_highgui.so )
  
endif()
