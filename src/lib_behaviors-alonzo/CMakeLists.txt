#--------------------------------------------------------
# The CMakeLists.txt for:              lib_behaviors-test
# Author(s):                                
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})


MACRO(ADD_BHV BHV_NAME)
  ADD_LIBRARY(${BHV_NAME} SHARED ${BHV_NAME}.cpp)
  TARGET_LINK_LIBRARIES(${BHV_NAME} 
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )

ENDMACRO(ADD_BHV)


#--------------------------------------------------------
#                                      BHV_SimpleWaypoint
#--------------------------------------------------------
#ADD_LIBRARY(BHV_SimpleWaypoint SHARED 
#   BHV_SimpleWaypoint.cpp AOF_SimpleWaypoint.cpp)
#TARGET_LINK_LIBRARIES(BHV_SimpleWaypoint
#   helmivp
#   behaviors 
#   ivpbuild 
#   logic 
#   ivpcore 
#   bhvutil 
#   mbutil 
#   geometry 
#   ${SYSTEM_LIBS} )
#--------------------------------------------------------
#                                      test
#--------------------------------------------------------
  ADD_LIBRARY(BHV_test SHARED BHV_test.cpp)
  TARGET_LINK_LIBRARIES(BHV_test
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry
    ${SYSTEM_LIBS} ) 
#--------------------------------------------------------
#                                      Pulse
#--------------------------------------------------------
  ADD_LIBRARY(BHV_Pulse SHARED BHV_Pulse.cpp)
  TARGET_LINK_LIBRARIES(BHV_Pulse
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )       
#--------------------------------------------------------
#                                      ZigLeg
#--------------------------------------------------------
  ADD_LIBRARY(BHV_ZigLeg SHARED BHV_ZigLeg.cpp)
  TARGET_LINK_LIBRARIES(BHV_ZigLeg
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )       
#--------------------------------------------------------
#                                      circle_sine
#--------------------------------------------------------
  ADD_LIBRARY(BHV_circle_sine SHARED BHV_circle_sine.cpp)
  TARGET_LINK_LIBRARIES(BHV_circle_sine
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )       
#--------------------------------------------------------
#                                      sine
#--------------------------------------------------------
  ADD_LIBRARY(BHV_sine SHARED BHV_sine.cpp)
  TARGET_LINK_LIBRARIES(BHV_sine
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} ) 
#--------------------------------------------------------
#                                      Lawnmower
#--------------------------------------------------------
ADD_LIBRARY(BHV_Lawnmower SHARED BHV_Lawnmower.cpp)
  TARGET_LINK_LIBRARIES(BHV_Lawnmower
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )  
#--------------------------------------------------------
#                                      rotate_lawnmower
#--------------------------------------------------------
ADD_LIBRARY(BHV_rotate_lawnmower SHARED BHV_rotate_lawnmower.cpp)
  TARGET_LINK_LIBRARIES(BHV_rotate_lawnmower
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )  



















