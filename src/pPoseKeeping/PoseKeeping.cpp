/************************************************************/
/*    NAME: Logan                                           */
/*    ORGN: MIT                                             */
/*    FILE: PoseKeeping.cpp                                 */
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
	m_arrival_radius = 1;
	m_upper_speed = 100;
	m_lower_speed = 10;
	m_active = false;
	// version 4
	m_previous_error_x = 0;
	m_switch = '\0';
	m_switch_temp = false;
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
    string sval  = msg.GetString();

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

     else if(key == "THRUST_MODE_DIFFERENTIAL")
     {
	if(sval == "true")
	{
		m_active = true; 
		m_previous_time = MOOSTime();
		postPolygons();
	}
	else
	{
		m_active = false;
		postPolygons();
	}
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

	//Constructor
	Notify("1","1");
	Data block(m_nav_heading,Distance(m_osx, m_osy, m_desired_x, m_desired_y));

	//HEADING CONTROL PID
	//Calculate azimuth of destination
	double rel_ang_to_wpt = relAng(m_osx, m_osy, m_desired_x, m_desired_y);
	Notify("2",rel_ang_to_wpt);

	//Get time difference for PID
	double current_time = MOOSTime();
	double delta_time = current_time - m_previous_time;

	//Calculate error for PID
	block.m_mode = "KeepHeading";
	CalculateError(block, rel_ang_to_wpt); //**
	double rel_ang_error = block.m_curr_error;
	CalculateError(block, m_desired_heading);
	Notify("3",block.m_curr_error);
	Notify("ERROR",block.m_curr_error);
	Notify("D_T",1.0/delta_time);
	Notify("E_X",(abs(m_desired_x - m_osx) - m_previous_error_x));

	//Calculate thrust
	//double thrust = 300*(abs(m_desired_x - m_osx) - m_previous_error_x); //100,1000*delta_time,300  **
	double thrust = 1*block.m_curr_error;
	//double thrust = 300*(abs(m_desired_x - m_osx) - m_previous_error_x) + 1*block.m_curr_error; //100,1000*delta_time,300  **
	Notify("4",thrust);
	//Calculate speed
	double speed = 20*abs(m_desired_x - m_osx); //50 ** +35
	//double speed = 2*abs(m_desired_x - m_osx) + 300*(abs(m_desired_x - m_osx) - m_previous_error_x);///delta_time;//100*(abs(m_desired_x - m_osx) - m_previous_error_x)/delta_time;
	Notify("5",speed);

	//Output Thruster
///*
	if (rel_ang_error > 0 && rel_ang_error <= 90)
	{
		//Notify("DESIRED_THRUST_R",(speed - thrust));
		//Notify("DESIRED_THRUST_L",(speed + thrust));
		//Notify("DESIRED_THRUST_R",-speed + thrust);
		//Notify("DESIRED_THRUST_L",-speed - thrust);

		Notify("DESIRED_THRUST_R",(-speed - thrust));
		Notify("DESIRED_THRUST_L",(-speed + thrust));
	}
	else if (rel_ang_error > 90 && rel_ang_error < 180)
	{
		//Notify("DESIRED_THRUST_R",speed - thrust);
		//Notify("DESIRED_THRUST_L",speed + thrust);
		//Notify("DESIRED_THRUST_R",-speed + thrust);
		//Notify("DESIRED_THRUST_L",-speed - thrust);

		Notify("DESIRED_THRUST_R",(-speed - thrust));
		Notify("DESIRED_THRUST_L",(-speed + thrust));
	}
//*/

/*
	if (((block.m_curr_error > 0 && block.m_curr_error <= 90) || (block.m_curr_error > 90 && block.m_curr_error < 180)) && m_switch_temp == false)
	{
		m_switch_temp = true;
		m_switch = "backward";
	}
	if (m_switch == "foward")
	{
		Notify("DESIRED_THRUST_R",(speed - thrust));
		Notify("DESIRED_THRUST_L",(speed + thrust));
	}
	if (m_switch == "backward")
	{
		Notify("DESIRED_THRUST_R",-(speed+10) + thrust);
		Notify("DESIRED_THRUST_L",-(speed+10) - thrust);
	}

	if (m_desired_y - m_osy > m_tolerance_radius-5)//-5
	{
		m_switch = "backward";
		//SetPoint();
	}
	if (m_desired_y - m_osy < -m_tolerance_radius)
	{
		m_switch = "foward";
	}
*/
	//Save previous data
	m_previous_error_x = abs(m_desired_x - m_osx);
	m_previous_time = current_time; 
/* 
  // Wait until active
  if(!m_active)
    return(false);

  // Behavior
  // If vehicle reach destination, KeepHeading mode On
  if(Distance(m_osx, m_osy, m_desired_x, m_desired_y) < m_arrival_radius)
    m_keep_heading = true;
  // If vehicle outside KeepHeading region, KeepHeading mode Off (will become SetPoint mode)
  if(Distance(m_osx, m_osy, m_desired_x, m_desired_y) > m_tolerance_radius)
    m_keep_heading = false;
	
  if(m_keep_heading)
    KeepHeading();
  else
    SetPoint();
*/

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
     Register("THRUST_MODE_DIFFERENTIAL",0 );
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
// Procedure: KeepHeeding
// Procedure: Keep Vehicle's heading while vehicle is inside the KeepHeading region

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
	double thrust = m_kp*block.m_curr_error + m_kd*(block.m_curr_error - m_previous_error) + m_ki*m_steady_error;

	//Output Thruster
	OutputThruster(block, thrust, 0);

	//Save previous data
	m_previous_error = block.m_curr_error;
	m_previous_time = current_time; 
	
}
//------------------------------------------------------------
// Procedure: CalculateError
//   Purpose: Transform angle to the graph below and calculate error 
//
//   KeepHeading mode:              SetPoint mode:
//
//                                       foward         foward
//         left      0      right         left     0     right
//                   |                             |
//                   |                    -90      |      90
//         -90 ----- A ----- 90              ----- A -----       
//                   |                    -90      |      90           
//                   |                             |               
//              -180   180                left     0      right    
//                                      backward        backward   

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

	//Quit if this is keep heading mode
	if(block.m_mode == "KeepHeading")
	   return;

	//Keep going if this is set_point mode
	if(error < 180 && error > 90 && block.m_curr_distance < m_tolerance_radius+10)
	{
		error = -error+180;
		block.m_mode = "Backward";
	}
	else if(error < -90 && error > -180 && block.m_curr_distance < m_tolerance_radius+10)
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
// Procedure: OutputThruster
//   Purpose: Notify thruster to MOOSDB based on different mode 

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
// Procedure: SetPoint
//   Purpose: Go to destination

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

	//Get time difference for PID
	double current_time = MOOSTime();
	double delta_time = current_time - m_previous_time;

	//Calculate steady state error
	m_steady_error = m_steady_error + block.m_curr_error*delta_time;

	//Calculate thrust
	double thrust = m_kp*block.m_curr_error + m_kd*(block.m_curr_error - m_previous_error)/delta_time + m_ki*m_steady_error;

	//SPEED CONTROL
	double speed = Speed(block);

	//Output Thruster
	OutputThruster(block, thrust, speed);

	//Save previous data
	m_previous_error = block.m_curr_error;
	m_previous_time = current_time; 
}
//-------------------------------------------------------------
// Procedure: relAng
//   Purpose: Calculate angle between destination and vehicle (from lib-geometry) 

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
//   Purpose: Convert angle to be strictly in the rang [0, 360). (from lib-geometry)

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
//   Purpose: Change rad to degree (from lib-geometry)

double PoseKeeping::radToDegrees(double radval)
{
  return((radval / M_PI) * 180);
}

//---------------------------------------------------------------
// Procedure: Distance
//   Purpose: Calculate distance between vehicle and destination

double PoseKeeping::Distance(double current_x, double current_y, double destination_x, double destination_y)
{
  double distance = sqrt(pow(current_x-destination_x,2)+pow(current_y-destination_y,2));
  return(distance);
}
//---------------------------------------------------------------
// Procedure: Speed
//   Purpose: Decide Upper and lower limit of the speed  

double PoseKeeping::Speed(Data &block)
{
	double speed = m_kp*block.m_curr_distance;

	if(speed > m_upper_speed)
	{
		speed = m_upper_speed;
	}	
	if(speed < m_lower_speed)
	{
		speed = m_lower_speed;
	}
	return(speed);	
}
//---------------------------------------------------------------
// Procedure: CheckMode
//   Purpose: If the mode changed, reset PID variables

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
//   Purpose: Post KeepHeading region on pMarineViewer

void PoseKeeping::postPolygons()
{
    string spec = "format=radial,label=destination_point,edge_color=blue,vertex_color=blue,fill_color=grey90,vertex_size=0,edge_size=1";
    spec += ",x=" + DoubleToString(m_desired_x);
    spec += ",y=" + DoubleToString(m_desired_y);
    spec += ",radius=" + DoubleToString(m_tolerance_radius);
    spec += ",pts=24, snap=1";
    if(m_active)
    {
	spec += ",active=true";
    }
    else
    {
	spec += ",active=false";
    }
    Notify("VIEW_POLYGON", spec);
}

//------------------------------------------------------------
// Procedure: DoubleToString
//   Purpose: Change double to string
string PoseKeeping::DoubleToString(double input)
{
    stringstream msg;
    string output;
    msg << input;
    msg >> output;
    return(output);
}






