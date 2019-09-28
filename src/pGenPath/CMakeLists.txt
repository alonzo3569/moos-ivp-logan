#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenPath
# Author(s):                              alonzo
#--------------------------------------------------------

SET(SRC
  GenPath.cpp
  GenPath_Info.cpp
  main.cpp
  Mydata.cpp
)

ADD_EXECUTABLE(pGenPath ${SRC})

TARGET_LINK_LIBRARIES(pGenPath
   ${MOOS_LIBRARIES}
   mbutil
   m
   geometry
   pthread)

