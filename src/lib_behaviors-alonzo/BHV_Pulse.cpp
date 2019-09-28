/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Pulse.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include<cmath>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_Pulse.h"
#include "XYRangePulse.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_Pulse::BHV_Pulse(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX");

  // Default values for behavior state variables
  m_osx  = 0;
  m_osy  = 0;
  m_wpt  = 0;
  counter= 0;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_Pulse::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }
  else if (param == "pulse_range") {
    // return(setBooleanOnString(m_my_bool, val));
    m_pulse_range = double_val;
    return(true);
  }
  else if (param == "pulse_duration") {
    m_pulse_duration = double_val;
    return(true);
    // return(setBooleanOnString(m_my_bool, val));
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_Pulse::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_Pulse::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_Pulse::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_Pulse::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_Pulse::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_Pulse::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_Pulse::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_Pulse::onRunState()
{
  // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  bool ok1, ok2, ok3;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }
  toc = getBufferCurrTime();
  m_wpt = getBufferDoubleVal("WPT_INDEX", ok3);
  if(counter != m_wpt)
  {
	tic = getBufferCurrTime();
	counter++;
  }
  if((toc - tic) > 5 && (toc - tic) < 6)
  {
	  XYRangePulse pulse;
	  pulse.set_x(m_osx);
	  pulse.set_y(m_osy);
	  pulse.set_label("bhv_pulse");
	  pulse.set_rad(m_pulse_range);
	  pulse.set_duration(m_pulse_duration);
//	  pulse.set_time(toc);
	  pulse.set_color("edge", "yellow");
	  pulse.set_color("fill", "yellow");

 	  string spec = pulse.get_spec();
  	  postMessage("VIEW_RANGE_PULSE", spec);

  }
  postMessage("WPT", m_wpt);
  postMessage("TIC", tic);
  postMessage("TOC", toc);
  postMessage("COUNTER", counter);
  
  if(counter == 5 && m_wpt == 0){counter = 0;}


  // Part 2: Build the IvP function
  IvPFunction *ipf = 0;



  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

