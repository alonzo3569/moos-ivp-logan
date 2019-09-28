/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_sine.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef sine_HEADER
#define sine_HEADER

#include <string>
#include "IvPBehavior.h"
#include<XYPoint.h>
#include<string>
#include<sstream>

using namespace std;

class BHV_sine : public IvPBehavior {
public:
  BHV_sine(IvPDomain);
  ~BHV_sine() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

  double m_sin_x;
  double m_sin_y;
  double m_start_x;
  double m_start_y;
  double m_w;
  double m_h;
  int m_t;
  int m_k;
  int m_cycle;
  double m_osx;
  double m_osy;
  double m_d;
  double m_resolution;
  double m_arrival_radius;
  int counter;
  double m_timer_end_henry;
  double m_timer_end_gilda;
  string msg;
  XYPoint m_nextpt;
  IvPFunction* buildFunctionWithZAIC();

protected: // Local Utility functions

protected: // Configuration parameters

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_sine(domain);}
}
#endif
