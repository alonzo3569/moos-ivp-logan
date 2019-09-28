/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_sine.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_sine.h"

#include <math.h>/* sin */
#include<XYPoint.h>
#include<sstream> //stringstream
#include "OF_Reflector.h"
#include "AngleUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include<string>

//#include<XYSeglist.h>

#define PI 3.14159265

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_sine::BHV_sine(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("TIMER_END_HENRY","no_warning");
  addInfoVars("TIMER_END_GILDA","no_warning");

  m_sin_x = 0;
  m_sin_y = 0;
  m_start_x = 0;  
  m_start_y = 0;  
  m_t = 0;
  m_osx = 0;
  m_osy = 0;
  m_w = 0;        
  m_h = 0;       
  m_d = 0;
  m_arrival_radius = 0;
  counter = 0;
  m_resolution = 0;
  m_timer_end_henry = 0;
  m_timer_end_gilda = 0;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_sine::setParam(string param, string val)
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

  else if(param == "start_x"){
    m_start_x = double_val;
   return(true);
  }
  else if(param == "start_y"){
    m_start_y = double_val;
    return(true);
  }
  else if(param == "w"){
    m_w = double_val;
    return(true);
  }
  else if (param == "amplitude") {
    m_h = double_val;
    return(true);
  }
  else if (param == "full_length") {
    m_d = double_val;
    return(true);
  }
  else if (param == "arrival_radius") {
    m_arrival_radius = double_val;
    return(true);
  }
  else if (param == "resolution") {
    m_resolution = double_val;
    return(true);
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_sine::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_sine::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_sine::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_sine::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_sine::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_sine::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_sine::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_sine::onRunState()
{

  // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  bool ok1, ok2, ok3, ok4;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  m_timer_end_henry = getBufferDoubleVal("TIMER_END_HENRY", ok3);
  m_timer_end_gilda = getBufferDoubleVal("TIMER_END_GILDA", ok4);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }

  if(m_timer_end_henry == 1)
  {
	msg = "src_node=henry,dest_node=gilda,var_name=NAV_X_HENRY,string_val=";
	stringstream s;
	string x;
	s << m_osx;
	s >> x;
	msg = msg + x;
	postMessage("NODE_MESSAGE_LOCAL",msg);
	postMessage("TIMER_END_HENRY",0); //!!!
  }
  if(m_timer_end_gilda == 1)
  {
	msg = "src_node=gilda,dest_node=henry,var_name=NAV_X_GILDA,string_val=";
	stringstream s;
	string x;
	s << m_osx;
	s >> x;
	msg = msg + x;
	postMessage("NODE_MESSAGE_LOCAL",msg);
	postMessage("TIMER_END_GILDA",0); //!!!
  }

  //postMessage("M_T_HENRY",m_timer_end_henry);  //debug
  //postMessage("M_T_GILDA",m_timer_end_gilda);

  //counter i  m_next_ptx,m_next_pty,r

  if(counter > m_resolution+1){ counter = 1;}
  if(counter == 0)// show circle
  {

	  for(m_t=1;m_t<m_resolution+1;m_t++)
 	 {		
  		m_sin_x = m_start_x + m_d*m_t/m_resolution;
  		m_sin_y = m_start_y + m_h*sin((360*m_w/m_resolution)*m_t*PI/180.0);
		//shift version
  		//m_sin_x = m_start_x + m_d*m_t/m_resolution + m_d*m_t/m_resolution*cos(90*PI/180.0);
  		//m_sin_y = m_start_y + m_h*sin((360*m_w/m_resolution)*m_t*PI/180.0) + m_d*m_t/m_resolution*sin(90*PI/180.0);
		//Mark point on pMarineViewer
		XYPoint point(m_sin_x, m_sin_y);
		stringstream s;
		string label;
		s << m_t;
		s >> label;
		//point.set_label(label);
		point.set_color("vertex", "blue");
		point.set_param("vertex_size", "5");
		string spec = point.get_spec();
		postMessage("VIEW_POINT", spec);
	 }   
	 counter++;
  }
  if(counter >= 0 && counter < m_resolution+1)// counter = 1~360
  {
  	//m_sin_x = m_start_x + m_d*counter/m_resolution;
  	//m_sin_y = m_start_y + m_h*sin((360*m_w/m_resolution)*counter*PI/180.0);
   	m_sin_x = m_start_x + m_d*counter/m_resolution;
  	m_sin_y = m_start_y + m_h*sin((360*m_w/m_resolution)*counter*PI/180.0);
	m_nextpt.set_vx(m_sin_x);
	m_nextpt.set_vy(m_sin_y);
	//counter++
  }

  double dist = pow(m_nextpt.x()-m_osx,2)+pow(m_nextpt.y()-m_osy,2);

  if(dist <= pow(m_arrival_radius,2)) {
    //setComplete();
    counter++;
    postMessage("COUNTER",counter);
    postMessage("DIST",dist);
    //postWMessage("point arrive");
    //postViewPoint(false);
    return(0);
  }

  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  ipf = buildFunctionWithZAIC();



  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}
//---------------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_sine::buildFunctionWithZAIC() 
{
  // Step 1 - Create the IvPDomain, the function's domain
  //IvPDomain domain;
  //domain.addDomain("course", 0, 360, 361);
  double rel_ang_to_wpt = relAng(m_osx, m_osy, m_nextpt.x(), m_nextpt.y());

  // Step 2 - Create the ZAIC_PEAK with the domain and variable name
  ZAIC_PEAK  zaic_peak(m_domain, "course");
  // Step 3 - Configure the ZAIC_LEQ parameters
  zaic_peak.setSummit(rel_ang_to_wpt);
  zaic_peak.setPeakWidth(0);
  zaic_peak.setBaseWidth(180.0);
  zaic_peak.setSummitDelta(0);

  ZAIC_PEAK  spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(5);
  spd_zaic.setPeakWidth(0.5);
  spd_zaic.setBaseWidth(1.0);
  spd_zaic.setSummitDelta(0.8);  


  // Step 4 - Extract the IvP function

  IvPFunction *spd_ipf = zaic_peak.extractIvPFunction();
  IvPFunction *crs_ipf = spd_zaic.extractIvPFunction();

  OF_Coupler coupler;
  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);
  //}
  //else
  //{
	//cout << zaic_peak.getWarnings();
  //}
	return(ivp_function);     

}
