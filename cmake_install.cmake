# Install script for directory: /home/aghozlane/workspace/MetaboFlux/metaboflux

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe"
         RPATH "")
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/aghozlane/workspace/MetaboFlux/metaboflux/bin/MetaBoFlux.exe")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe")
    FILE(RPATH_REMOVE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MetaBoFlux.exe")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libCompute.py~"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libCompute.pyc"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libAnalysis.pyc"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libXml.py"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libAnalysis.py"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libXml.pyc"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libCompute.py"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libGetfiles.pyc"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/libGetfiles.py"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/.svn/entries"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/.svn/all-wcprops"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/.svn/text-base/libAnalysis.py.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/.svn/text-base/libGetfiles.py.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/.svn/text-base/libXml.py.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/lib/.svn/text-base/libCompute.py.svn-base"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "/home/aghozlane/workspace/MetaboFlux/metaboflux/./bin/MetaBoFlux.py")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "/home/aghozlane/workspace/MetaboFlux/metaboflux/./bin/MetaBoFlux_stat.R")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src" TYPE FILE FILES
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/simulation.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/especes.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/mpi_load.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/gsl_min.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/xml_parameter.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/gsl_recuit.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/MetaBoFlux.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/entries"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/all-wcprops"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/data_parameters.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/xml_parameter.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/gsl_min.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/gsl_recuit.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/mpi_load.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/gsl_sd.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/simulation.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/gsl_mod.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/especes.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/equations.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/.svn/text-base/MetaBoFlux.c.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/equations.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/gsl_mod.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/gsl_sd.c"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/src/data_parameters.c"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/xml_parameter.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/all-wcprops"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/gsl_mod.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/especes.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/equations.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/gsl_recuit.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/xml_parameter.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/mpi_load.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/gsl_sd.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/data_parameters.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/simulation.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/text-base/gsl_min.h.svn-base"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/.svn/entries"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/gsl_recuit.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/gsl_mod.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/equations.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/gsl_min.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/mpi_load.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/simulation.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/especes.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/data_parameters.h"
    "/home/aghozlane/workspace/MetaboFlux/metaboflux/include/gsl_sd.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/aghozlane/workspace/MetaboFlux/metaboflux/doc/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/aghozlane/workspace/MetaboFlux/metaboflux/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/aghozlane/workspace/MetaboFlux/metaboflux/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
