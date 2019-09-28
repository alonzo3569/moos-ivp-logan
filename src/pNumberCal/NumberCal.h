/************************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: NumberCal.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef NumberCal_HEADER
#define NumberCal_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class NumberCal : public AppCastingMOOSApp
{
 public:
   NumberCal();
   ~NumberCal();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
