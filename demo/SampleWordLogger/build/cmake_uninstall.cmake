if(NOT EXISTS "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleWordLogger/build/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleWordLogger/build/install_manifest.txt\"")
endif()

file(READ "C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleWordLogger/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    if(EXISTS "$ENV{DESTDIR}${file}")
        exec_program("C:/Program Files (x86)/CMake 2.8/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                     OUTPUT_VARIABLE rm_out
                     RETURN_VALUE rm_retval)
        if(NOT "${rm_retval}" STREQUAL 0)
            message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
        endif()
    else()
        message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
    endif()
endforeach()
