/************************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: Mydata.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Mydata_HEADER
#define Mydata_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include<iostream>
#include<deque>
using namespace std;
class Mydata 
{
 public:
   Mydata();
   ~Mydata();
   bool m_firstpoint = false;
   bool m_lastpoint  = false;
   double m_x      = 0; 
   double m_y      = 0; 
   int m_id        = 0;
   int m_new_id    = 0;
   string  m_name;
   string  m_str;
   int counterx    = 0;
   int countery    = 0;
   void setfirstpoint();
   void setlastpoint();
   void setloc(double,double,int);
   //void test();
   //void test(double&,double&,int&);
   void test(double &x, double &y, int &id);
   void evenodd();
   void eastwest(int &counterx, int &countery);
   string write_str();
   string vname();
 protected:

 private: // Configuration variables

 private: // State variables
};

#endif 
