#--------------------------------------------------------
# The CMakeLists.txt for:                       pAngleDecision
# Author(s):                              alonzo
#--------------------------------------------------------

SET(SRC
  AngleDecision.cpp
  AngleDecision_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pAngleDecision ${SRC})

TARGET_LINK_LIBRARIES(pAngleDecision
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   geometry
   pthread)

