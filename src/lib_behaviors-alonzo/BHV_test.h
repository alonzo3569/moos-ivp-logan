/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_test.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef test_HEADER
#define test_HEADER

#include <string>
#include "IvPBehavior.h"

class BHV_test : public IvPBehavior {
public:
  BHV_test(IvPDomain);
  ~BHV_test() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected: // Local Utility functions

protected: // Configuration parameters

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_test(domain);}
}
#endif
