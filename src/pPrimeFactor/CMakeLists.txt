#--------------------------------------------------------
# The CMakeLists.txt for:                       pPrimeFactor
# Author(s):                              ual
#--------------------------------------------------------

SET(SRC
  PrimeFactor.cpp
  PrimeFactor_Info.cpp
  main.cpp
  Mydata.cpp
)

ADD_EXECUTABLE(pPrimeFactor ${SRC})

TARGET_LINK_LIBRARIES(pPrimeFactor
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

