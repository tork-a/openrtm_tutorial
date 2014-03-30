# SampleMotionCaller CMake config file
#
# This file sets the following variables:
# SampleMotionCaller_FOUND - Always TRUE.
# SampleMotionCaller_INCLUDE_DIRS - Directories containing the SampleMotionCaller include files.
# SampleMotionCaller_IDL_DIRS - Directories containing the SampleMotionCaller IDL files.
# SampleMotionCaller_LIBRARIES - Libraries needed to use SampleMotionCaller.
# SampleMotionCaller_DEFINITIONS - Compiler flags for SampleMotionCaller.
# SampleMotionCaller_VERSION - The version of SampleMotionCaller found.
# SampleMotionCaller_VERSION_MAJOR - The major version of SampleMotionCaller found.
# SampleMotionCaller_VERSION_MINOR - The minor version of SampleMotionCaller found.
# SampleMotionCaller_VERSION_REVISION - The revision version of SampleMotionCaller found.
# SampleMotionCaller_VERSION_CANDIDATE - The candidate version of SampleMotionCaller found.

message(STATUS "Found SampleMotionCaller-0.0.1")
set(SampleMotionCaller_FOUND TRUE)

find_package(<dependency> REQUIRED)

#set(SampleMotionCaller_INCLUDE_DIRS
#    "C:/Program Files (x86)/SampleMotionCaller/include/samplemotioncaller-0"
#    ${<dependency>_INCLUDE_DIRS}
#    )
#
#set(SampleMotionCaller_IDL_DIRS
#    "C:/Program Files (x86)/SampleMotionCaller/include/samplemotioncaller-0/idl")
set(SampleMotionCaller_INCLUDE_DIRS
    "C:/Program Files (x86)/SampleMotionCaller/include/"
    ${<dependency>_INCLUDE_DIRS}
    )
set(SampleMotionCaller_IDL_DIRS
    "C:/Program Files (x86)/SampleMotionCaller/include//idl")


if(WIN32)
    set(SampleMotionCaller_LIBRARIES
        "C:/Program Files (x86)/SampleMotionCaller/components/lib/samplemotioncaller.lib"
        ${<dependency>_LIBRARIES}
        )
else(WIN32)
    set(SampleMotionCaller_LIBRARIES
        "C:/Program Files (x86)/SampleMotionCaller/components/lib/samplemotioncaller.dll"
        ${<dependency>_LIBRARIES}
        )
endif(WIN32)

set(SampleMotionCaller_DEFINITIONS ${<dependency>_DEFINITIONS})

set(SampleMotionCaller_VERSION 0.0.1)
set(SampleMotionCaller_VERSION_MAJOR 0)
set(SampleMotionCaller_VERSION_MINOR 0)
set(SampleMotionCaller_VERSION_REVISION 1)
set(SampleMotionCaller_VERSION_CANDIDATE )

