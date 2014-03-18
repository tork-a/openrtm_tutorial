
option(BUILD_RTC_MANAGER "Building  RTC Manager plugin \"RtcManagerPlugin\"" OFF)


if(BUILD_RTC_MANAGER)
# check OpenRTM installed
  find_package(OpenRTM REQUIRED)

  list(APPEND CFLAGS ${OMNIORB_CFLAGS} ${OPENRTM_CFLAGS})
  list(APPEND INCLUDE_DIRS ${OMNIORB_INCLUDE_DIRS} ${OPENRTM_INCLUDE_DIRS})
  list(APPEND LDFLAGS ${OMNIORB_LDFLAGS} ${OPENRTM_LDFLAGS})
  list(APPEND LIBRARY_DIRS ${OMNIORB_LIBRARY_DIRS} ${OPENRTM_LIBRARY_DIRS})
  list(APPEND LIBRARIES ${OMNIORB_LIBRARIES} ${OPENRTM_LIBRARIES})

  set(target CnoidRtcManagerPlugin)
  set(srcdir ${PROJECT_SOURCE_DIR}/extplugin/RtcManagerPlugin)
  set(srcfiles 	${srcdir}/RtcManagerPlugin.cpp
		${srcdir}/RtcManager.cpp 
		${srcdir}/RtcManager.h 
		)

  include_directories(${INCLUDE_DIRS})
  link_directories(${LIBRARY_DIRS})
  add_definitions(${CFLAGS})

  add_library(${target} SHARED ${srcfiles})
  target_link_libraries(${target} CnoidBase ${LIBRARIES})
  apply_common_setting_for_plugin(${target})

endif()

