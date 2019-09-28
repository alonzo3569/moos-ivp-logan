#--------------------------------------------------------
# The CMakeLists.txt for:                       pPoseKeeping
# Author(s):                              Logan
#--------------------------------------------------------

SET(SRC
  PoseKeeping.cpp
  PoseKeeping_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPoseKeeping ${SRC})

TARGET_LINK_LIBRARIES(pPoseKeeping
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   geometry
   pthread)

