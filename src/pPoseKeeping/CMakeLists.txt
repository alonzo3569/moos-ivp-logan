#===========================================================================
# FILE:  CMakeLists.txt for pPoseKeeping
# DATE:  2019/11/15
# NAME:  Logan
#===========================================================================

SET(SRC
  PoseKeeping.cpp
  PoseKeeping_Info.cpp
  main.cpp
  Mode.cpp
)

ADD_EXECUTABLE(pPoseKeeping ${SRC})

TARGET_LINK_LIBRARIES(pPoseKeeping
  ${MOOS_LIBRARIES}
  mbutil
  apputil
  m
  pthread
  geometry
)

