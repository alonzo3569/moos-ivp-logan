#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign
# Author(s):                              alonzo
#--------------------------------------------------------

SET(SRC
  PointAssign.cpp
  PointAssign_Info.cpp
  main.cpp
  Mydata.cpp
)

ADD_EXECUTABLE(pPointAssign ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign
   ${MOOS_LIBRARIES}
   mbutil
   m
   geometry
   pthread)

