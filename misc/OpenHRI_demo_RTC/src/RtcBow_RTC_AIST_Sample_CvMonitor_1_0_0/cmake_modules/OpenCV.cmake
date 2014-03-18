## OpenCV LIBRARIES
if(WIN32)
  set(EXTRA_INC_DIRS "${OPENRTM_DIR}/OpenCV2.3/include" "${OPENRTM_DIR}/OpenCV2.3/include/opencv")
  set(EXTRA_LIB_DIRS "${OPENRTM_DIR}/OpenCV2.3/lib")
  set(EXTRA_LIBS opencv_core231 opencv_highgui231 opencv_imgproc231)
else()
  set(EXTRA_INC_DIRS "/usr/include/opencv")
  set(EXTRA_LIB_DIRS "/usr/lib /usr/local/lib")
  set(EXTRA_LIBS libcv.so libopencv_core.so libopencv_highgui.so )
  
endif()
