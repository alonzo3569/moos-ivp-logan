#--------------------------------------------------------
# The CMakeLists.txt for:                       pFFT_test
# Author(s):                              alonzo
#--------------------------------------------------------

SET(SRC
  FFT_test.cpp
  FFT_test_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pFFT_test ${SRC})

TARGET_LINK_LIBRARIES(pFFT_test
   ${MOOS_LIBRARIES}
   apputil
   fftw3
   mbutil
   m
   pthread)

