
set(OMNIORB_FOUND FALSE)
set(OPENRTM_FOUND FALSE)


if(UNIX)
    include(FindPkgConfig)

    # omniORB
    pkg_check_modules(OMNIORB REQUIRED "omniORB4")
    if(NOT OMNIORB_DIR)
        if(OMNIORB_FOUND)
            set(OMNIORB_DIR "${OMNIORB_PREFIX}")
        endif()
        set(OMNIORB_DIR "${OMNIORB_DIR}" CACHE PATH "omniORB root directory")
    endif()

    set(OMNIORB_CFLAGS ${OMNIORB_CFLAGS_OTHER})
    set(OMNIORB_LDFLAGS ${OMNIORB_LDFLAGS_OTHER})

    # OpenRTM-aist
    pkg_check_modules(OPENRTM REQUIRED "openrtm-aist")
    if(NOT OPENRTM_DIR)
        if(OPENRTM_FOUND)
            set(OPENRTM_DIR "${OPENRTM_PREFIX}")
        endif()
        set(OPENRTM_DIR "${OPENRTM_DIR}" CACHE PATH "OpenRTM-aist root directory")
    endif()

    set(OPENRTM_CFLAGS ${OPENRTM_CFLAGS_OTHER})
    set(OPENRTM_LDFLAGS ${OPENRTM_LDFLAGS_OTHER})

    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1"
           OPENRTM_VERSION_MAJOR "${OPENRTM_VERSION}")
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2"
           OPENRTM_VERSION_MINOR "${OPENRTM_VERSION}")
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3"
           OPENRTM_VERSION_PATCH "${OPENRTM_VERSION}")

    # IDL Compiler
    set(OPENRTM_IDLC "")
    set(OPENRTM_IDLFLAGS "")

    execute_process(COMMAND rtm-config --idlc
                    RESULT_VARIABLE result_val
                    OUTPUT_VARIABLE output_val
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(result_val EQUAL 0)
        set(OPENRTM_IDLC "${output_val}")
    endif()

    execute_process(COMMAND rtm-config --idlflags
                    RESULT_VARIABLE result_val
                    OUTPUT_VARIABLE output_val
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(result_val EQUAL 0)
        string(REPLACE " " ";" output_val ${output_val})
        set(OPENRTM_IDLFLAGS ${output_val} "-I${OPENRTM_DIR}/include/rtm/idl")
    endif()

    # IDL Skelton Wrapper
    set(OPENRTM_IDL_WRAPPER "rtm-skelwrapper")
    set(OPENRTM_IDL_WRAPPER_FLAGS --include-dir="" --skel-suffix=Skel --stub-suffix=Stub)

endif(UNIX)

if(WIN32)
    # omniORB
    if(NOT OMNIORB_DIR)
        if(NOT $ENV{OMNI_ROOT} STREQUAL "")
            set(OMNIORB_DIR "$ENV{OMNI_ROOT}")
            set(OMNIORB_FOUND TRUE)
        endif()
        set(OMNIORB_DIR "${OMNIORB_DIR}" CACHE PATH "omniORB root directory")
        if(NOT OMNIORB_FOUND)
            message(FATAL_ERROR "omniORB not found.")
        endif()
    endif()

    set(OMNIORB_INCLUDE_DIRS "${OMNIORB_DIR}/include")
    set(OMNIORB_LIBRARY_DIRS "${OMNIORB_DIR}/lib/x86_win32")

    # omniORB version
    file(GLOB _vers RELATIVE "${OMNIORB_DIR}" "${OMNIORB_DIR}/THIS_IS_OMNIORB*")
    if("${_vers}" STREQUAL "")
        message(FATAL_ERROR "omniORB version file not found.")
    endif()

    set(OMNIORB_VERSION "${_vers}")
    string(REGEX REPLACE "THIS_IS_OMNIORB_" ""
           OMNIORB_VERSION "${OMNIORB_VERSION}")
    string(REGEX REPLACE "[_]" "."
           OMNIORB_VERSION "${OMNIORB_VERSION}")
    string(REGEX REPLACE "[.]" ""
           OMNIORB_VERSION_NUM "${OMNIORB_VERSION}")

    # omnithread version
    file(GLOB _vers RELATIVE "${OMNIORB_LIBRARY_DIRS}" "${OMNIORB_LIBRARY_DIRS}/omnithread*")
    if("${_vers}" STREQUAL "")
        message(FATAL_ERROR "omnithread not found.")
    endif()
    string(REGEX REPLACE ".*omnithread([0-9]+)_rt\\.lib.*" "\\1"
           OMNIORB_THREAD_NUM "${_vers}")

    set(OMNIORB_CFLAGS -D__WIN32__;-D__x86__;-D__NT__;-D__OSVERSION__=4;-D_CRT_SECURE_NO_DEPRECATE)
    if(${OMNIORB_VERSION_NUM} MATCHES "^40")
      set(OMNIORB_CFLAGS ${OMNIORB_CFLAGS};-D_WIN32_WINNT=0x0400)
    else()
      set(OMNIORB_CFLAGS ${OMNIORB_CFLAGS};-D_WIN32_WINNT=0x0500;-DRTC_CORBA_CXXMAPPING11)
    endif()

    foreach(library "omniORB${OMNIORB_VERSION_NUM}_rt"
                    "omniDynamic${OMNIORB_VERSION_NUM}_rt"
                    "omnithread${OMNIORB_THREAD_NUM}_rt")
        list(APPEND OMNIORB_LIBRARIES optimized "${library}" debug "${library}d")
    endforeach()

    # OpenRTM-aist
    if(NOT OPENRTM_DIR)
        if(NOT $ENV{RTM_ROOT} STREQUAL "")
            set(OPENRTM_DIR "$ENV{RTM_ROOT}")
            set(OPENRTM_FOUND TRUE)
        endif()
        set(OPENRTM_DIR "${OPENRTM_DIR}" CACHE PATH "OpenRTM-aist root directory")
        if(NOT OPENRTM_FOUND)
            message(FATAL_ERROR "OpenRTM-aist not found.")
        endif()
    endif()

    # OpenRTM-aist version
    set(OPENRTM_VERSION "${OPENRTM_DIR}")
    string(REGEX REPLACE ".*OpenRTM-aist/" "" OPENRTM_VERSION "${OPENRTM_VERSION}")
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)" "\\1" OPENRTM_VERSION_MAJOR "${OPENRTM_VERSION}")
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)" "\\2" OPENRTM_VERSION_MINOR "${OPENRTM_VERSION}")
    set(OPENRTM_VERSION_PATCH "0")
    set(OPENRTM_VERSION "${OPENRTM_VERSION_MAJOR}.${OPENRTM_VERSION_MINOR}.${OPENRTM_VERSION_PATCH}")
    string(REGEX REPLACE "[.]" ""
           OPENRTM_VERSION_NUM "${OPENRTM_VERSION}")

    set(OPENRTM_INCLUDE_DIRS "${OPENRTM_DIR}")
    set(OPENRTM_LIBRARY_DIRS "${OPENRTM_DIR}/bin")
    list(APPEND OPENRTM_INCLUDE_DIRS "${OPENRTM_DIR}/rtm/idl")

    set(OPENRTM_CFLAGS "-DUSE_stub_in_nt_dll")

    foreach(library "RTC${OPENRTM_VERSION_NUM}" "coil${OPENRTM_VERSION_NUM}")
        list(APPEND OPENRTM_LIBRARIES optimized "${library}" debug "${library}d")
    endforeach()
    foreach(library "ws2_32" "mswsock")
        list(APPEND OPENRTM_LIBRARIES optimized "${library}" debug "${library}")
    endforeach()

    # IDL Compiler
    set(OPENRTM_IDLC "omniidl")
    set(OPENRTM_IDLFLAGS -bcxx -Wba -nf)

    # IDL Skelton Wrapper
    set(OPENRTM_IDL_WRAPPER "rtm-skelwrapper.py")
    set(OPENRTM_IDL_WRAPPER_FLAGS --include-dir="" --skel-suffix=Skel --stub-suffix=Stub)

endif(WIN32)

message(STATUS "FindOpenRTM setup done.")

message(STATUS "  OMNIORB_DIR=${OMNIORB_DIR}")
message(STATUS "  OMNIORB_VERSION=${OMNIORB_VERSION}")
message(STATUS "  OMNIORB_CFLAGS=${OMNIORB_CFLAGS}")
message(STATUS "  OMNIORB_INCLUDE_DIRS=${OMNIORB_INCLUDE_DIRS}")
message(STATUS "  OMNIORB_LDFLAGS=${OMNIORB_LDFLAGS}")
message(STATUS "  OMNIORB_LIBRARY_DIRS=${OMNIORB_LIBRARY_DIRS}")
message(STATUS "  OMNIORB_LIBRARIES=${OMNIORB_LIBRARIES}")

message(STATUS "  OPENRTM_DIR=${OPENRTM_DIR}")
message(STATUS "  OPENRTM_VERSION=${OPENRTM_VERSION}")
message(STATUS "  OPENRTM_VERSION_MAJOR=${OPENRTM_VERSION_MAJOR}")
message(STATUS "  OPENRTM_VERSION_MINOR=${OPENRTM_VERSION_MINOR}")
message(STATUS "  OPENRTM_VERSION_PATCH=${OPENRTM_VERSION_PATCH}")
message(STATUS "  OPENRTM_CFLAGS=${OPENRTM_CFLAGS}")
message(STATUS "  OPENRTM_INCLUDE_DIRS=${OPENRTM_INCLUDE_DIRS}")
message(STATUS "  OPENRTM_LDFLAGS=${OPENRTM_LDFLAGS}")
message(STATUS "  OPENRTM_LIBRARY_DIRS=${OPENRTM_LIBRARY_DIRS}")
message(STATUS "  OPENRTM_LIBRARIES=${OPENRTM_LIBRARIES}")

message(STATUS "  OPENRTM_IDLC=${OPENRTM_IDLC}")
message(STATUS "  OPENRTM_IDLFLAGS=${OPENRTM_IDLFLAGS}")
message(STATUS "  OPENRTM_IDL_WRAPPER=${OPENRTM_IDL_WRAPPER}")
message(STATUS "  OPENRTM_IDL_WRAPPER_FLAGS=${OPENRTM_IDL_WRAPPER_FLAGS}")
