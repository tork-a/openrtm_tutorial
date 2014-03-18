# Install script for directory: C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/OpenRTM-aist/1.1")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Debug/SampleMotionCaller.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Release/SampleMotionCaller.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/MinSizeRel/SampleMotionCaller.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/RelWithDebInfo/SampleMotionCaller.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "applications")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE SHARED_LIBRARY FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Debug/SampleMotionCaller.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE SHARED_LIBRARY FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Release/SampleMotionCaller.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE SHARED_LIBRARY FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/MinSizeRel/SampleMotionCaller.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE SHARED_LIBRARY FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/RelWithDebInfo/SampleMotionCaller.dll")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "applications")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "applications")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE EXECUTABLE FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Debug/SampleMotionCallerComp.exe")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE EXECUTABLE FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Release/SampleMotionCallerComp.exe")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE EXECUTABLE FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/MinSizeRel/SampleMotionCallerComp.exe")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE EXECUTABLE FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/RelWithDebInfo/SampleMotionCallerComp.exe")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "applications")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "applications")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE FILE FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/RTC.xml")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "applications")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "documents")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0" TYPE DIRECTORY FILES "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/doc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "documents")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "sources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0/src" TYPE FILE FILES
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/SampleMotionCaller.cpp"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/SampleMotionCaller.h"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/SampleMotionCallerComp.cpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "sources")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "sources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0/src" TYPE FILE FILES
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/CMakeLists.txt"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/Doxyfile.in"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/RTC.xml"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/SampleMotionCaller.conf"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/rtc.conf"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "sources")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "sources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/components/c++/samplemotioncaller1.0.0/src" TYPE DIRECTORY FILES
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/cmake_modules"
    "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/cpack_resources"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "sources")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
