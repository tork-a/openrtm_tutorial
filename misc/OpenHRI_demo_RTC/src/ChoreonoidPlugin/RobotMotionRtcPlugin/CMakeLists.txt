
option(BUILD_ROBOT_MOTION_RTC "Building a Simple RT-Component for Robot plugin \"RobotMotionRtcPlugin\"" OFF)


if(BUILD_ROBOT_MOTION_RTC)
# check OpenRTM installed
  find_package(OpenRTM REQUIRED)

  list(APPEND CFLAGS ${OMNIORB_CFLAGS} ${OPENRTM_CFLAGS})
  list(APPEND INCLUDE_DIRS ${OMNIORB_INCLUDE_DIRS} ${OPENRTM_INCLUDE_DIRS})
  list(APPEND LDFLAGS ${OMNIORB_LDFLAGS} ${OPENRTM_LDFLAGS})
  list(APPEND LIBRARY_DIRS ${OMNIORB_LIBRARY_DIRS} ${OPENRTM_LIBRARY_DIRS})
  list(APPEND LIBRARIES ${OMNIORB_LIBRARIES} ${OPENRTM_LIBRARIES})

  set(target CnoidRobotMotionRtcPlugin)
  set(srcdir ${PROJECT_SOURCE_DIR}/extplugin/RobotMotionRtcPlugin)
  set(srcfiles 	${srcdir}/RobotMotionRtcPlugin.cpp
		${srcdir}/RobotMotionRtcItem.cpp 
		${srcdir}/RobotMotionRtcItem.h
		${srcdir}/RobotMotion.cpp
		${srcdir}/RobotMotion.h
		${srcdir}/MotionPlayer.cpp 
		${srcdir}/MotionPlayer.h 
		${srcdir}/cutils.cpp 
		${srcdir}/cutils.h 
		)
  list(APPEND INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/extplugin/RtcManagerPlugin)

  include_directories(${INCLUDE_DIRS})
  link_directories(${LIBRARY_DIRS})
  add_definitions(${CFLAGS})

  add_library(${target} SHARED ${srcfiles})
  target_link_libraries(${target} CnoidBodyPlugin CnoidRtcManagerPlugin ${LIBRARIES})
  apply_common_setting_for_plugin(${target})

endif()

