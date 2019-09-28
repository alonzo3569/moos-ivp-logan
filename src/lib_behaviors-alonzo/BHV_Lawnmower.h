/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Lawnmower.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef Lawnmower_HEADER
#define Lawnmower_HEADER

#include <string>
#include "IvPBehavior.h"

//^_^
#include<XYPoint.h>
#include<string>
#include<sstream>
#include<vector>
#include<iostream>
#include <string>

using namespace std;

class BHV_Lawnmower : public IvPBehavior {
public:
  BHV_Lawnmower(IvPDomain);
  ~BHV_Lawnmower() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

  //^_^  variables

  double m_start_x;  
  double m_start_y;
  double m_h;       
  double m_d;
  double m_resolution;
  double m_osx;
  double m_osy;
  double m_theta;
  double m_w;
  int counter;//for case
  double m_arrival_radius;
  XYPoint m_nextpt;

  struct Data {
    double x;
    double y;
  };
  vector<struct Data> view_point; // it has to be placed after the struct, if not, error!
  vector<struct Data>::iterator p; //test

  string m_name;
  string m_information;
  double m_distance;

  IvPFunction* buildFunctionWithZAIC();

  //function
  void MarkpMarine(double x, double y);
  void setting_params(string msg);
  void setting_path();
  void calculate_distance();
  bool check_opregion(double x, double y);

protected: // Local Utility functions

protected: // Configuration parameters

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_Lawnmower(domain);}
}
#endif
