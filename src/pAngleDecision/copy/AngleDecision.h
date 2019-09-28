/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: AngleDecision.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef AngleDecision_HEADER
#define AngleDecision_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

//^_^
#include<XYPoint.h>
#include<string>
#include<sstream>
#include<vector>
#include<iostream>
#include <string>
#include <math.h>/* sin */
using namespace std;

class AngleDecision : public AppCastingMOOSApp
{
 public:
   AngleDecision();
   ~AngleDecision();

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

   //^_^ variables
   string m_vname1,m_vname2;
   struct Data {
     double theta;
     double x1;
     double y1;
     double x2;
     double y2;
     double dis;
   };
   vector<struct Data> angle_point; // it has to be placed after the struct, if not, error!
   vector<struct Data>::iterator p; //test

  //function
  void GetData(string a);
  void FormReport1(double x, double y, double theta, string name, double distance);
  void FormReport2(double x, double y, double theta, string name, double distance);
  void Average();

  //template <class T> 
  string ConvertToString(double value);


};

#endif 
