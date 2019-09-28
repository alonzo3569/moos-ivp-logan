/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "Mydata.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <deque>
#include <math.h>
#include<string>
using namespace std;
class GenPath : public CMOOSApp
{
 public:
   GenPath();
   ~GenPath();
   int i = 1;
   int k = 1;
   double start_x,replan_x,replan_output__x;
   double start_y,replan_y,replan_output__y;
   int block_number = 50; //evenodd
   //int block_number;        //eastwest
   double INDEX,WPT_INDEX;
   Mydata Z;
   deque<Mydata>recv_block;
   deque<Mydata>des_list;
   deque<Mydata>replan_list;
   deque<Mydata>revisit_list;
   std::string msg;
   std::string msg_replan;
   bool G_R = false;
   string deploy;
   int c2 = 0;
   string fuck;
   string r;

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
