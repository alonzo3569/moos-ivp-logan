#--------------------------------------------------------
# The CMakeLists.txt for:                       pStoreSound
# Author(s):                              CTHung
#--------------------------------------------------------

SET(SRC
  StoreSound.cpp
  StoreSound_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pStoreSound ${SRC})

TARGET_LINK_LIBRARIES(pStoreSound
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread
   asound)

