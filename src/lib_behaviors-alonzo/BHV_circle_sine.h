/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_circle_sine.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef circle_sine_HEADER
#define circle_sine_HEADER

#include <string>
#include "IvPBehavior.h"
#include<XYPoint.h>

class BHV_circle_sine : public IvPBehavior {
public:
  BHV_circle_sine(IvPDomain);
  ~BHV_circle_sine() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

  double m_circle_x;
  double m_circle_y;
  double m_center_x;
  double m_center_y;
  double m_r;
  int i;
  double m_osx;
  double m_osy;
  double m_arrival_radius;
  int counter;
  double m_resolution;
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
  {return new BHV_circle_sine(domain);}
}
#endif
