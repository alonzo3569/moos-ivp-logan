/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: FFT_test.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef FFT_test_HEADER
#define FFT_test_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>
#include <stdio.h>//fprintf
using namespace std;
const float pi = 3.1415926;

class FFT_test : public AppCastingMOOSApp
{
 public:
   FFT_test();
   ~FFT_test();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   //bool buildReport();

   // Iterate function
   void VoltToVector(string input);
   void AL_FFT(int window_type,unsigned int N);


 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables

 //subscribe params
 vector<double> m_volt_buffer;
 //AL_FFT params
 int m_start_calculate_fft;
};

#endif 
