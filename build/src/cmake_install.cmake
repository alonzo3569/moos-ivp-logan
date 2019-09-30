# Install script for directory: /home/ual/moos-ivp-logan/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "None")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ual/moos-ivp-logan/build/src/pPrimeFactor/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pOdometry/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pNumberCal/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pPointAssign/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pGenPath/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/lib_behaviors-alonzo/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pRecordtest/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pStoreSound/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pStoreSoundX/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/uFldHazardMgrX/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pAngleDecision/cmake_install.cmake")
  include("/home/ual/moos-ivp-logan/build/src/pPoseKeeping/cmake_install.cmake")

endif()

