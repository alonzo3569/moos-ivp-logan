/************************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
//using namespace std;
#include<deque>
#include"Mydata.h"
class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
 //int x;   
 //std::string  x ;
   std::deque<uint64_t> m_buff;

};

#endif 
