#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SampleMotionCaller" for configuration "MinSizeRel"
set_property(TARGET SampleMotionCaller APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(SampleMotionCaller PROPERTIES
  IMPORTED_IMPLIB_MINSIZEREL "${_IMPORT_PREFIX}/components/lib/SampleMotionCaller.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_MINSIZEREL "RTC110;coil110;omniORB415_rt;omniDynamic415_rt;omnithread34_rt;advapi32;ws2_32;mswsock"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/components/bin/SampleMotionCaller.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SampleMotionCaller )
list(APPEND _IMPORT_CHECK_FILES_FOR_SampleMotionCaller "${_IMPORT_PREFIX}/components/lib/SampleMotionCaller.lib" "${_IMPORT_PREFIX}/components/bin/SampleMotionCaller.dll" )

# Import target "SampleMotionCallerComp" for configuration "MinSizeRel"
set_property(TARGET SampleMotionCallerComp APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(SampleMotionCallerComp PROPERTIES
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/components/bin/SampleMotionCallerComp.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS SampleMotionCallerComp )
list(APPEND _IMPORT_CHECK_FILES_FOR_SampleMotionCallerComp "${_IMPORT_PREFIX}/components/bin/SampleMotionCallerComp.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
