/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include "XYRangePulse.h"
#include "ZAIC_PEAK.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course");//"heading"

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX");
  addInfoVars("DESIRED_HEADING");

  // Default values for behavior state variables
  m_osx  = 0;
  m_osy  = 0;
  m_wpt  = 0;
  counter= 0;
  m_zig_angle= 0;
  m_zig_duration= 0;
  m_heading  = 0;
  m_zig_heading = 0;
  i = 0;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
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
  else if (param == "zig_duration") {
    // return(setBooleanOnString(m_my_bool, val));
    m_zig_duration = double_val;
    return(true);
  }
  else if (param == "zig_angle") {
    m_zig_angle = double_val;
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

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
{
  // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  bool ok1, ok2, ok3, ok4;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }
  m_heading = getBufferDoubleVal("DESIRED_HEADING", ok4);
  m_wpt = getBufferDoubleVal("WPT_INDEX", ok3);
  toc = getBufferCurrTime();
  if(counter != m_wpt)
  {
	tic = getBufferCurrTime();// getBufferLocalTime();
	counter++;
	i=0;
  }

  postMessage("WPT", m_wpt);
  postMessage("TIC", tic);
  postMessage("TOC", toc);
  postMessage("COUNTER", counter);
  postMessage("HEADING", m_heading);
  
  if(counter == 5 && m_wpt == 0){counter = 0;}


  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  //if((toc - tic) > 8 && (toc - tic) < 8.25) //one z_heading output 0.25sec
  if((toc - tic) > 6 && (toc - tic) < 6 + m_zig_duration) //heading == 225 135 27+45=72 334+45=379-360=19
  {
	if(i == 0)
	{	
		m_zig_heading = m_heading+m_zig_angle;
		i++;
	}	
	else
	{
		ipf = buildFunctionWithZAIC();
		postMessage("HEADING", m_zig_heading);
	}
  }
  if(ipf == 0) 
  {
	postWMessage("Problem Creating the IvP Function");
  }

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}
//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC() 
{
  // Step 1 - Create the IvPDomain, the function's domain
  IvPDomain domain;
  domain.addDomain("course", 0, 360, 361);

  // Step 2 - Create the ZAIC_PEAK with the domain and variable name
  ZAIC_PEAK  zaic_peak(m_domain, "course");
  // Step 3 - Configure the ZAIC_LEQ parameters
  zaic_peak.setSummit(m_zig_heading);
  postMessage("Z_HEADING", m_zig_heading);
  zaic_peak.setMinMaxUtil(20, 120);
  zaic_peak.setBaseWidth(10);

  // Step 4 - Extract the IvP function
  IvPFunction *ivp_function = 0;
  //if(zaic_peak.stateOK())//zaic_leq?
  //{
	ivp_function = zaic_peak.extractIvPFunction();
  //}
  //else
  //{
	//cout << zaic_peak.getWarnings();
  //}
	return(ivp_function);     

}
