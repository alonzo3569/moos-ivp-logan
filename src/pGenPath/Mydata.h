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
#include <math.h>
#include<deque>
#include<string>
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
   int m_new_id        = 0;
   string  m_name;
   string  m_str;
   void setfirstpoint();
   void setlastpoint();
   void setloc(double,double,int);
   //void test();
   //void test(double&,double&,int&);
   void test(double &x, double &y, int &id);
   void evenodd();
   string write_str();
   string vname();

   double m_diff;
   void test2(double &x, double &y);
   double caldiff(double , double);
   void set_startxy(double &x, double &y);
 protected:

 private: // Configuration variables

 private: // State variables
};

#endif 
