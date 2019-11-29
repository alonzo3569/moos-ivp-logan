#===========================================================================
# FILE:  CMakeLists.txt for pPoseKeepingX
# DATE:  2019/11/15
# NAME:  Logan
#===========================================================================

SET(SRC
  PoseKeepingX.cpp
  PoseKeepingX_Info.cpp
  main.cpp
  Mode.cpp
)

ADD_EXECUTABLE(pPoseKeepingX ${SRC})

TARGET_LINK_LIBRARIES(pPoseKeepingX
  ${MOOS_LIBRARIES}
  geometry
  mbutil
  apputil
  m
  pthread
)

