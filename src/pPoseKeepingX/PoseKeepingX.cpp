/************************************************************/
/*    NAME: Logan                                           */
/*    ORGN: MIT                                             */
/*    FILE: PoseKeepingX.cpp                                */
/*    DATE: 2019/11/15                                      */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PoseKeepingX.h"

//logan
#include "AngleUtils.h"
#include "ColorPack.cpp"

using namespace std;

//---------------------------------------------------------
// Constructor

PoseKeepingX::PoseKeepingX()
{
  m_nav_heading = 0;
  m_osx = 0;
  m_osy = 0;
  m_desired_x = 0;
  m_desired_y = 0;
  m_desired_heading = 0;
  m_active = false;
  m_keep_heading = false;
  m_arrival_radius = 10;
  m_tolerance_radius = 0;

  m_previous_time = 0;
  m_previous_error = 0;
  m_steady_error = 0;
  m_switch_mode = "NULL";
  m_kp = 0;
  m_ki = 0;
  m_kd = 0;
  m_upper_speed = 100;
  m_lower_speed = 10;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PoseKeepingX::OnNewMail(MOOSMSG_LIST &NewMail)
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

  return UpdateMOOSVariables(NewMail);  // Automatically updates registered MOOS Vars
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PoseKeepingX::OnConnectToServer()
{
#if 0 // Keep this for messages that do not require a callback but just a simple read
  AddMOOSVariable("bar_msg", "BAR_IN", "BAR_OUT", 0);
  // incoming BAR_IN is automatically updated thru UpdateMOOSVariables();
  // to get the latest value of BAR_OUT, call :
  // double d = GetMOOSVar("bar_msg")->GetDoubleVal(); // if value is double
  // string s = GetMOOSVar("bar_msg")->GetStringVal(); // if value is string
#endif

#if 0 // Keep this as an example for callbacks
  AddMOOSVariable("foo_msg", "FOO_IN", "FOO_OUT", 0); // foo_msg is a local name
  AddActiveQueue("foo_callback", this, &PoseKeepingX::onMessageFoo);
  AddMessageRouteToActiveQueue("foo_callback", "FOO_IN");
#endif

  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PoseKeepingX::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
     Register("NAV_HEADING", 0);
     Register("NAV_X", 0);
     Register("NAV_Y", 0);
     Register("THRUST_MODE_DIFFERENTIAL",0 );

  RegisterMOOSVariables();
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PoseKeepingX::Iterate()
{
  AppCastingMOOSApp::Iterate();

  // Wait until active
  if(!m_active){
    return(false);}

  // Config
  double distance = Distance(m_osx, m_osy, m_desired_x, m_desired_y);
  double keepheading_error = m_desired_heading - m_nav_heading;
  double setpoint_error = relAng(m_osx, m_osy, m_desired_x, m_desired_y) - m_nav_heading;

  // Behavior
  if(distance < m_arrival_radius){
    m_keep_heading = true;}
  // If vehicle outside KeepHeading region, KeepHeading mode Off (will become SetPoint mode)
  if(distance > m_tolerance_radius){
    m_keep_heading = false;}

  // Decide Mode
  unique_ptr<Mode> p;
///*  
  if(m_keep_heading)
   p.reset(new Keepheading(keepheading_error));
  else if((setpoint_error < 180 && setpoint_error > 90 || setpoint_error < -90 && setpoint_error > -180) && distance < m_tolerance_radius+10)
   p.reset(new Forward(setpoint_error));
  else
   p.reset(new Forward(setpoint_error));
//*/
  // Go go!
  p->CalculateError();
  CheckMode(p.get());

  double curr_time = MOOSTime();
  double delta_time = curr_time - m_previous_time;

  double thrust = m_kp*(p->geterror()) + m_kd*((p->geterror() - m_previous_error)/delta_time) + m_ki*m_steady_error;
  double speed = m_kp*distance;
  speed = CheckSpeed(speed);

  p->Output(thrust, speed);

  Notify("DESIRED_THRUST_L", p->getthrustl());
  Notify("DESIRED_THRUST_R", p->getthrustr());

  m_previous_error = p->geterror();
  m_previous_time = curr_time;

  PublishFreshMOOSVariables();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PoseKeepingX::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());
  else {
    STRING_LIST::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string orig  = *p;
      string line  = *p;
      string param = toupper(biteStringX(line, '='));
      string value = line;

      bool handled = false;
      if(param == "FOO") {
        handled = true;
      }
      else if(param == "BAR") {
        handled = true;
      }
      // logan
      else if(param == "POSITION") {
        value = stripQuotes(value);
        string x = biteString(value, ',');
        string y = value;
        m_desired_x = atof(x.c_str());
        m_desired_y = atof(y.c_str());
        handled = true;
      }

      else if(param == "HEADING") {
        m_desired_heading = atof(value.c_str());
        handled = true;
      }

      else if(param == "KP") {
        m_kp = atof(value.c_str());
        handled = true;
      }

      else if(param == "KI") {
        m_ki = atof(value.c_str());
        handled = true;
      }

      else if(param == "KD") {
        m_kd = atof(value.c_str());
        handled = true;
      }

      else if(param == "TOLERANCE_RADIUS") {
        m_tolerance_radius = atof(value.c_str());
        handled = true;
      }
      if(!handled)
        reportUnhandledConfigWarning(orig);
    }

  }

  registerVariables();
  return(true);
}

#if 0
//------------------------------------------------------------
// Procedure: onMessageFoo() callback

bool PoseKeepingX:onMessageFoo(CMOOSMsg& foo)
{
  // do something with foo

  // update outgoing message (FOO_OUT in this case)
  // SetMOOSVar("foo_msg", new_value, m_curr_time);

  return(true);
}
#endif

//------------------------------------------------------------
// Procedure: buildReport()

bool PoseKeepingX::buildReport()
{
  m_msgs << "pPoseKeepingX Report\n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

//---------------------------------------------------------------
// Procedure: Distance
//   Purpose: Calculate distance between vehicle and destination

double PoseKeepingX::Distance(double current_x, double current_y, double destination_x, double destination_y)
{
  double distance = sqrt(pow(current_x-destination_x,2)+pow(current_y-destination_y,2));
  return(distance);
}

//---------------------------------------------------------------
// Procedure: CheckMode
//   Purpose: If the mode changed, reset PID variables

void PoseKeepingX::CheckMode(Mode* ptr)
{
	if(ptr->getmode() != m_switch_mode)
	{
		m_previous_time = MOOSTime();
		m_previous_error = 0;
		m_steady_error = 0;
		m_switch_mode = ptr->getmode();
	}
	return;
}

//---------------------------------------------------------------
// Procedure: CheckMode
//   Purpose: If the mode changed, reset PID variables

double PoseKeepingX::CheckSpeed(double speed)
{
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
//------------------------------------------------------------
// Procedure: postPolygons
//   Purpose: Post KeepHeading region on pMarineViewer

void PoseKeepingX::postPolygons()
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
string PoseKeepingX::DoubleToString(double input)
{
    stringstream msg;
    string output;
    msg << input;
    msg >> output;
    return(output);
}