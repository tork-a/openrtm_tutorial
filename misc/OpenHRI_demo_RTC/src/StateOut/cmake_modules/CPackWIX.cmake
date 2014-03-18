#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)
#
# Fixed for OpenRTM
#
IF(CMAKE_BINARY_DIR)
  MESSAGE(FATAL_ERROR "CPackWIX.cmake may only be used by CPack internally.")
ENDIF(CMAKE_BINARY_DIR)

IF(NOT CPACK_WIX_XSL OR NOT CPACK_PACKAGE_FILE_NAME OR NOT CPACK_RESOURCE_FILE_LICENSE OR NOT CPACK_PACKAGE_INSTALL_DIRECTORY)
    MESSAGE(FATAL_ERROR "Variables required CPACK_WIX_XSL, CPACK_PACKAGE_FILE_NAME, CPACK_RESOURCE_FILE_LICENSE, CPACK_PACKAGE_INSTALL_DIRECTORY")
ENDIF()

IF(NOT CPACK_WIX_LANG)
    SET(CPACK_WIX_LANG "en-us")
ENDIF()

# use the package defaults if none exists.
# for the community.
#IF ( NOT CPACK_WIX_XSL )
#	
#ENDIF( NOT CPACK_WIX_XSL )
#

IF ( CPACK_WIX_WXS_FILES )
	# this could probably be a single expression but I'm no regex guru.
    string(REGEX MATCH "[^/]*$" wix_temp "${CPACK_WIX_WXS_FILES}")
	string(REGEX REPLACE "[.]wxs" ".wixobj" CPACK_WIX_OBJS "${wix_temp}" )
ENDIF( CPACK_WIX_WXS_FILES )

FIND_PROGRAM(WIX_EXE_HEAT heat PATHS "$ENV{ProgramFiles}/Windows Installer XML v3/bin")
FIND_PROGRAM(WIX_EXE_CNDL candle PATHS "$ENV{ProgramFiles}/Windows Installer XML v3/bin")
FIND_PROGRAM(WIX_EXE_LGHT light PATHS "$ENV{ProgramFiles}/Windows Installer XML v3/bin")

IF( WIX_EXE_HEAT AND WIX_EXE_CNDL AND WIX_EXE_LGHT )
  SET(CPACK_WIX_HEAT_COMMAND "\"${WIX_EXE_HEAT}\" dir ${CPACK_PACKAGE_FILE_NAME} -ke -g1 -gg -var var.Source -t:${CPACK_WIX_XSL} -out ${CPACK_PACKAGE_FILE_NAME}.wxs")
  SET(CPACK_WIX_CNDL_COMMAND "\"${WIX_EXE_CNDL}\" -ext WixFirewallExtension -dSource=${CPACK_PACKAGE_FILE_NAME} ${CPACK_PACKAGE_FILE_NAME}.wxs ${CPACK_WIX_WXS_FILES}")
  SET(CPACK_WIX_LGHT_COMMAND "\"${WIX_EXE_LGHT}\" -ext WixFirewallExtension -ext WixUIExtension -cultures:${CPACK_WIX_LANG} -out ${CPACK_PACKAGE_FILE_NAME}.msi ${CPACK_PACKAGE_FILE_NAME}.wixobj ${CPACK_WIX_OBJS}")
ELSE()
  MESSAGE(FATAL_ERROR "Can not find Wix Executables.")
ENDIF()
