/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER
#include "Mydata.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <deque>

#include "XYPoint.h"
#include "IvPBehavior.h"
using namespace std;

class PointAssign : public CMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();
   int i = 1;
   double x = 0.0;//temp
   double y = 0.0;
   int counterx = 0;
   int countery = 0;
   //void postViewPoint(double,double,string);

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   //void onIdleState();
   //void postViewPoint(double,double,string,string);

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
 deque<Mydata>recv_block;
 deque<Mydata>notify_block;
};

#endif 
