/************************************************************/
/*    NAME: Logan                                              */
/*    ORGN: MIT                                             */
/*    FILE: PoseKeeping.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PoseKeeping.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

//---------------------------------------------------------
// Constructor

PoseKeeping::PoseKeeping()
{
	m_nav_heading = 0;
	m_osx = 0;
	m_osy = 0;
	m_desired_x = 0;
	m_desired_y = 0;
	m_desired_heading = 0;
	m_kp = 0;
	m_ki = 0;
	m_kd = 0;
	m_tolerance_radius = 0;
	m_previous_time = MOOSTime();
	m_previous_error = 0;
	m_steady_error = 0;
	m_switch_mode = '\0';
	m_keep_heading = false;
}

//---------------------------------------------------------
// Destructor

PoseKeeping::~PoseKeeping()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PoseKeeping::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    double dval  = msg.GetDouble();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key == "NAV_HEADING")
     {
	m_nav_heading = dval; 
     }
     else if(key == "NAV_X") 
     {
	m_osx = dval; 
     }
     else if(key == "NAV_Y") 
     {
	m_osy = dval; 
     }


     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PoseKeeping::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PoseKeeping::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  
  //KeepHeading();
  //SetPoint();

  // Mark Destination on pMarineViewer 
  postPolygons();

  // Behavior
  if(Distance(m_osx, m_osy, m_desired_x, m_desired_y) < 1)
    m_keep_heading = true;
  if(Distance(m_osx, m_osy, m_desired_x, m_desired_y) > m_tolerance_radius)
    m_keep_heading = false;
	
  if(m_keep_heading)
    KeepHeading();
  else
    SetPoint();


  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PoseKeeping::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    else if(param == "position") {
      value = stripQuotes(value);
      string x = biteString(value, ',');
      string y = value;
      m_desired_x = atof(x.c_str());
      m_desired_y = atof(y.c_str());
      Notify("X",m_desired_x);
      Notify("Y",m_desired_y);
      handled = true;
    }

    else if(param == "heading") {
      m_desired_heading = atof(value.c_str());
      handled = true;
    }

    else if(param == "kp") {
      m_kp = atof(value.c_str());
      handled = true;
    }

    else if(param == "ki") {
      m_ki = atof(value.c_str());
      handled = true;
    }

    else if(param == "kd") {
      m_kd = atof(value.c_str());
      handled = true;
    }

    else if(param == "tolerance_radius") {
      m_tolerance_radius = atof(value.c_str());
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PoseKeeping::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
     Register("NAV_HEADING", 0);
     Register("NAV_X", 0);
     Register("NAV_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PoseKeeping::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

//------------------------------------------------------------
// Procedure: Keep Vehicle's heading

void PoseKeeping::KeepHeading() 
{
	//Constructor
	Data block(m_nav_heading);

	//HEADING CONTROL PID
	//Calculate error for PID(difference between current heading and desired heading)
	CalculateError(block, m_desired_heading);

	//Check mode, if the mode changed, reset PID variables
	CheckMode(block);

	//Get time difference for PID
	double current_time = MOOSTime();
	double delta_time = current_time - m_previous_time;
	
	//Calculate steady state error
	m_steady_error = m_steady_error + block.m_curr_error*delta_time;

	//Calculate thrust
	Notify("K_B_H",block.m_curr_error);
	Notify("KP",m_kp);
	double thrust = m_kp*block.m_curr_error + m_kd*(block.m_curr_error - m_previous_error) + m_ki*m_steady_error;

	//Output Thruster
	Notify("THRUST",thrust);
	OutputThruster(block, thrust, 0);

	//Save previous data
	m_previous_error = block.m_curr_error;
	m_previous_time = current_time; 
	
}
//------------------------------------------------------------
// Procedure: Calculate error

void PoseKeeping::CalculateError(Data &block, double desired_angle)
{

	double error = desired_angle - block.m_curr_heading;
	if (error < -180)
	{
		error += 360;
	}
	else if (error > 180)
	{
		error -= 360;
	}
	block.m_curr_error = error;
	Notify("D_H",error);

	//Quit if this is keep heading mode
	if(block.m_mode == "KeepHeading")
	   return;

	//Keep going if this is set_point mode
	if(error < 180 && error > 90)
	{
		error = -error+180;
		block.m_mode = "Backward";
	}
	else if(error < -90 && error > -180)
	{
		error = -error-180;
		block.m_mode = "Backward";
	}
	else
	{
		block.m_mode = "Foward";
	}

	//This is set point mode
	block.m_curr_error = error;
}
//------------------------------------------------------------
// Procedure: Output Thruster

void PoseKeeping::OutputThruster(Data block, double thrust, double speed)
{

	if (block.m_mode == "KeepHeading")
	{
		Notify("DESIRED_THRUST_R",-thrust);
		Notify("DESIRED_THRUST_L",thrust);
	}
	else if (block.m_mode == "Foward")
	{
		Notify("DESIRED_THRUST_R",speed - thrust);
		Notify("DESIRED_THRUST_L",speed + thrust);
	}
	else if (block.m_mode == "Backward")
	{
		Notify("DESIRED_THRUST_R",-speed + thrust);
		Notify("DESIRED_THRUST_L",-speed - thrust);
	}

}
//------------------------------------------------------------
// Procedure: SetPoint() 

void PoseKeeping::SetPoint() 
{
	//Constructor
	Data block(m_nav_heading,Distance(m_osx, m_osy, m_desired_x, m_desired_y));

	//HEADING CONTROL PID
	//Calculate azimuth of destination
	double rel_ang_to_wpt = relAng(m_osx, m_osy, m_desired_x, m_desired_y);

	//Calculate error for PID
	CalculateError(block, rel_ang_to_wpt);

	//Check mode, if the mode changed, reset PID variables
	CheckMode(block);
	Notify("TIME",m_previous_time);

	//Get time difference for PID
	double current_time = MOOSTime();
	double delta_time = current_time - m_previous_time;
	Notify("D_TIME",delta_time);

	//Calculate steady state error
	m_steady_error = m_steady_error + block.m_curr_error*delta_time;
	Notify("SSE",m_steady_error);

	//Calculate thrust
	Notify("K_B_H",block.m_curr_error);
	Notify("KP",m_kp);
	double thrust = m_kp*block.m_curr_error + m_kd*(block.m_curr_error - m_previous_error)/delta_time + m_ki*m_steady_error;
	Notify("KI",m_ki);
	Notify("KD",m_kd);
	Notify("1",m_kp*block.m_curr_error);
	Notify("2",m_kd*(block.m_curr_error - m_previous_error)/delta_time);
	Notify("3",m_ki*m_steady_error);

	//SPEED CONTROL
	double speed = Speed(block);

	//Output Thruster
	Notify("THRUST",thrust);
	OutputThruster(block, thrust, speed);

	//Save previous data
	m_previous_error = block.m_curr_error;
	m_previous_time = current_time; 
}
//-------------------------------------------------------------
// Procedure: relAng

double PoseKeeping::relAng(double xa, double ya, double xb, double yb)
{ 
  if((xa==xb)&&(ya==yb))
    return(0);

  double w   = 0;
  double sop = 0;

  if(xa < xb) {
    if(ya==yb)  
      return(90.0);
    else
      w = 90.0;
  }
  else if(xa > xb) {
    if(ya==yb)  
      return(270.0);
    else
      w = 270.0;
  }

  if(ya < yb) {
    if(xa == xb) 
      return(0.0);
    if(xb > xa) 
      sop = -1.0;
    else 
      sop =  1.0;
  }
  else if(yb < ya) {
    if(xa == xb) 
      return(180);
    if(xb >  xa) 
      sop =  1.0;
    else 
      sop = -1.0;
  }

  double ydiff = yb-ya;
  double xdiff = xb-xa;
  if(ydiff<0) ydiff = ydiff * -1.0;
  if(xdiff<0) xdiff = xdiff * -1.0;

  double avalPI = atan(ydiff/xdiff);
  double avalDG = radToDegrees(avalPI);
  double retVal = (avalDG * sop) + w;

  retVal = angle360(retVal);

  return(retVal);
}
//---------------------------------------------------------------
// Procedure: angle360
//   Purpose: Convert angle to be strictly in the rang [0, 360).

double PoseKeeping::angle360(double degval)
{
  while(degval >= 360.0)
    degval -= 360.0;
  while(degval < 0.0)
    degval += 360.0;
  return(degval);
}
//---------------------------------------------------------------
// Procedure: radToDegrees

double PoseKeeping::radToDegrees(double radval)
{
  return((radval / M_PI) * 180);
}

//---------------------------------------------------------------
// Procedure: Distance

double PoseKeeping::Distance(double current_x, double current_y, double destination_x, double destination_y)
{
  double distance = sqrt(pow(current_x-destination_x,2)+pow(current_y-destination_y,2));
  return(distance);
}
//---------------------------------------------------------------
// Procedure: Speed

double PoseKeeping::Speed(Data &block)
{
	double speed = m_kp*block.m_curr_distance;

	if(speed > 100)
	{
		speed = 100;
	}	
	if(speed < 10)
	{
		speed = 10;
	}
	return(speed);	
}
//---------------------------------------------------------------
// Procedure: CheckMode

void PoseKeeping::CheckMode(Data &block)
{
	if(block.m_mode != m_switch_mode)
	{
		m_previous_time = MOOSTime();
		m_previous_error = 0;
		m_steady_error = 0;
		m_switch_mode = block.m_mode;
		Notify("MODE",m_switch_mode);
	}
}

//------------------------------------------------------------
// Procedure: postPolygons

void PoseKeeping::postPolygons()
{
    string spec = "format=radial, label=destination_point,edge_color=blue,vertex_color=blue,fill_color=grey90,vertex_size=0,edge_size=1";
    spec += ",x=" + DoubleToString(m_desired_x);
    spec += ",y=" + DoubleToString(m_desired_y);
    spec += ",radius=" + DoubleToString(m_tolerance_radius);
    spec += ",pts=24, snap=1";
    Notify("VIEW_POLYGON", spec);
}

//------------------------------------------------------------
// Procedure: DoubleToString

string PoseKeeping::DoubleToString(double input)
{
    stringstream msg;
    string output;
    msg << input;
    msg >> output;
    return(output);
}






