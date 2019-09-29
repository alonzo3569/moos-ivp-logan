/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Lawnmower.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_Lawnmower.h"

#include <math.h>/* sin */
#include<XYPoint.h>
#include<sstream> //stringstream
#include "OF_Reflector.h"
#include "AngleUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include<string>
#include<vector>

#define PI 3.14159265

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_Lawnmower::BHV_Lawnmower(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  //^_^ Register
  addInfoVars("A","no_warning");
  addInfoVars("NODE_REPORT","no_warning");

  m_start_x = 0;  
  m_start_y = 0;
  m_h = 0;       
  m_d = 0;
  m_resolution = 0;
  m_osx = 0;
  m_osy = 0;
  m_theta = 0;
  m_w = 0;

  counter = 0;//for switch
  //p= view_point.begin();//test
  m_arrival_radius = 0;

  view_point.clear();

}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_Lawnmower::setParam(string param, string val)
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

  else if (param == "amplitude") {
    m_h = double_val;
    return(true);
  }
  else if (param == "resolution") {
    m_resolution = double_val;
    return(true);
  }
  else if(param == "w"){
    m_w = double_val;
    return(true);
  }
  else if (param == "arrival_radius") {
    m_arrival_radius = double_val;
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

void BHV_Lawnmower::onSetParamComplete()
{

}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_Lawnmower::onHelmStart()
{

}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_Lawnmower::onIdleState()
{

}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_Lawnmower::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_Lawnmower::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_Lawnmower::onIdleToRunState()
{

}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_Lawnmower::onRunToIdleState()
{

}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_Lawnmower::onRunState()
{
	bool ok1, ok2;
	m_osx = getBufferDoubleVal("NAV_X", ok1);//OnNewMail
	m_osy = getBufferDoubleVal("NAV_Y", ok2);

	if(counter==0)
	{
		setting_path();
		counter++;
	}
	else
	{
		//start behavior  
		postMessage("START_SIZE", view_point.size());
		if(!view_point.empty())
		{  
  		  	struct Data block = view_point.back();
			m_nextpt.set_vx(block.x);
			m_nextpt.set_vy(block.y);
			calculate_distance();

		}
		else
		{
			counter = 0;
		}
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
// Procedure: MarkpMarine()
//   Purpose: Mark points on pMarineViewer

void BHV_Lawnmower::MarkpMarine(double x, double y)
{
	XYPoint point(x,y);
	//point.set_label(label);
	point.set_color("vertex", "yellow");
	point.set_param("vertex_size", "3");
	string spec = point.get_spec();
	postMessage("VIEW_POINT", spec);
}
//---------------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_Lawnmower::buildFunctionWithZAIC() 
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
//---------------------------------------------------------------
// Procedure: setting_params()
//   Purpose: set parameters

void BHV_Lawnmower::setting_params(string msg)
{
      m_name        =      tokStringParse(msg , "name" , ',' , '=' );
      m_theta       = StringToDouble(tokStringParse(msg , "theta" , ',' , '=' ));
      m_start_x     = StringToDouble(tokStringParse(msg , "x" , ',' , '=' ));
      m_start_y     = StringToDouble(tokStringParse(msg , "y" , ',' , '=' ));
      m_distance    = StringToDouble(tokStringParse(msg , "dis" , ',' , '=' ));
}
//---------------------------------------------------------------
// Procedure: setting_path()
//   Purpose: set path

void BHV_Lawnmower::setting_path()
{
      //Before starting
      //Get information from shoreside
      bool ok1;
      m_information = getBufferStringVal("A", ok1);
      setting_params(m_information);
      
      //Rotation Sine
      //Step1. Rotation Metrix
      double rotation_metrix[2][2] = { {cos(m_theta*PI/180), -sin(m_theta*PI/180)}, {sin(m_theta*PI/180), cos(m_theta*PI/180)} };
     
      //Step2. make sin x,y
      vector< vector<double> > Metrix_row;
      for(int i=1;i<m_resolution+1;i++)
      {
    	double m_sin_x = m_distance*i/m_resolution; //dis
    	double m_sin_y = m_h*sin((360*m_w/m_resolution)*i*PI/180.0);
    	vector<double> Metrix_column;
    	Metrix_column.push_back(m_sin_x);
    	Metrix_column.push_back(m_sin_y);
    	Metrix_row.push_back(Metrix_column);
      }
      //postMessage("VECTOR_SIZE",Metrix_row.size());

      //Step3. Multiply!!!
      for(int i=0; i<Metrix_row.size(); i++)
      {
    	struct Data block;
    	block.x = rotation_metrix[0][0] * Metrix_row[i][0] + rotation_metrix[0][1] * Metrix_row[i][1] + m_start_x; //x term
    	block.y = rotation_metrix[1][0] * Metrix_row[i][0] + rotation_metrix[1][1] * Metrix_row[i][1] + m_start_y; //y term
    	view_point.push_back(block);
      }

      //Step4. print out
      //for(int i=0; i<view_point.size(); i++)
      postMessage("VECTOR_SIZE",view_point.size());
      vector<struct Data>::iterator iter = view_point.begin();
      for(iter; iter != view_point.end(); ++iter)
      {
	//if(-220* (*iter).x +39600<0 || -80* (*iter).y +1600<0 || 60* (*iter).x -150* (*iter).y-3000<0 || -35* (*iter).x +50* (*iter).y+250>0 || -125* (*iter).x -50* (*iter).y-16250>0 || -230* (*iter).y -46000>0)
	if(   0*(*iter).x	+60*(*iter).y	+0	>0 || 
	    -50*(*iter).x	+140*(*iter).y	+5000	>0 || 
	    -30*(*iter).x	+45*(*iter).y	+1050	>0 || 
	   -105*(*iter).x	-40*(*iter).y	-12125	>0 ||
	      0*(*iter).x	-210*(*iter).y	-38850	>0 ||
	    185*(*iter).x       +65*(*iter).y	-29600	>0)
	{iter = view_point.erase(iter); iter--;  continue;}//iter--;
    	MarkpMarine(iter->x , iter->y);
    	postMessage("SIZE", view_point.size());
    	postMessage("X", iter->x);
    	postMessage("Y", iter->y);
      } 
      postMessage("VECTOR_SIZE_AFTER",view_point.size());
}
//---------------------------------------------------------------
// Procedure: calculate_distance()
//   Purpose: calculate_distance

void BHV_Lawnmower::calculate_distance()
{
	double dist = pow(m_nextpt.x()-m_osx,2)+pow(m_nextpt.y()-m_osy,2);
	if(dist <= pow(m_arrival_radius,2))
	{
		view_point.pop_back();
		//view_point.erase(view_point.end());
	}
}
//---------------------------------------------------------------
// Procedure: StringToDouble()
//   Purpose: StringToDouble

double BHV_Lawnmower::StringToDouble(string msg)
{
	stringstream s;
	double num;
	s << msg;
	s >> num;
	return(num);
}
//---------------------------------------------------------------
