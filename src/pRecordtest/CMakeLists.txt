#--------------------------------------------------------
# The CMakeLists.txt for:                       pRecordtest
# Author(s):                              alonzo
#--------------------------------------------------------

SET(SRC
  Recordtest.cpp
  Recordtest_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pRecordtest ${SRC})

TARGET_LINK_LIBRARIES(pRecordtest
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   asound
   )

