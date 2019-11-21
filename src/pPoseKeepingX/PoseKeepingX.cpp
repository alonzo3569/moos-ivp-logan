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
//10.21
#include "XYVector.h"

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
  m_arrival_radius = 3;
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

  //10.21experiment feature
  m_pre_heading = -1;
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

     else if(key == "NAV_HEADING_CPNVG") //CPNVG
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
	}
	else
	{
		m_active = false;
		PostPolygons();
		ShowCompassHeading();
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
     Register("NAV_HEADING_CPNVG", 0); //CPNVG
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
    PublishFreshMOOSVariables();
    AppCastingMOOSApp::PostReport();
    return(true);}

  // Show Compass Heading when active
  ShowCompassHeading();

  // Filter: CheckCompassHeading
  bool result = Filter();
  if(!result){
    PublishFreshMOOSVariables();
    AppCastingMOOSApp::PostReport();
    return(true);}

  // Calculate Params for deciding mode
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
  if(m_keep_heading)
   mode.setup(keepheading_error, "Keepheading");
  else if(((setpoint_error < 270 && setpoint_error > 90) || (setpoint_error < -90 && setpoint_error > -270)) && distance < m_tolerance_radius+10)
   mode.setup(setpoint_error, "Backward");
  else
   mode.setup(setpoint_error, "Forward");

  // Show PoseKeeping region
  PostPolygons(mode.getmode());

  //Calculate error for PID
  mode.CalculateError();

  // Check if mode change, reset PID params
  CheckMode(mode);

  // Params for PID
  double curr_time = MOOSTime();
  double delta_time = curr_time - m_previous_time;
  m_steady_error = m_steady_error + mode.geterror()*delta_time;

  // Calculate speed and check speed value
  double thrust = m_kp*(mode.geterror()) + m_kd*((mode.geterror() - m_previous_error)/delta_time) + m_ki*m_steady_error;
  double speed = m_kp*distance;
  speed = CheckSpeed(speed);

  // Calculate thrust left & right
  mode.Output(thrust, speed);

  // Check thrust value
  mode.CheckValue();

  // Notify
  Notify("DESIRED_THRUST_L", mode.getthrustl());
  Notify("DESIRED_THRUST_R", mode.getthrustr());

  // Save PID params
  m_previous_error = mode.geterror();
  m_previous_time = curr_time;

  // Filter: Save m_nav_heading to m_pre_heading
  m_pre_heading = m_nav_heading; 

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
  m_msgs << actab.getFormattedString() << endl;
  m_msgs <<  " m_curr_error: " << mode.geterror() << endl;
  m_msgs <<  "       m_mode: " << mode.getmode() << endl;
  m_msgs <<  "   m_thrust_l: "  << mode.getthrustl() << endl;
  m_msgs <<  "   m_thrust_r: "  << mode.getthrustr() << endl;
  m_msgs << "============================================ \n";
  m_msgs <<  "m_switch_mode   : "  << m_switch_mode << endl;
  m_msgs <<  "m_previous_time : "  << m_previous_time << endl;
  m_msgs <<  "m_previous_error: "  << m_previous_error << endl;
  m_msgs <<  "m_steady_error  : "  << m_steady_error << endl;

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

void PoseKeepingX::CheckMode(const Mode mode)
{
	if(mode.getmode() != m_switch_mode)
	{
		m_previous_error = 0;
		m_steady_error = 0;
		m_switch_mode = mode.getmode();
	}
	return;
}

//---------------------------------------------------------------
// Procedure: CheckSpeed
//   Purpose: Decide Upper and lower limit of the speed

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

//------------------------------------------------------------
// Procedure: ShowCompassHeading
//   Purpose: Using VIEW_VECTOR to show compass heading on pMarineViewer

void PoseKeepingX::ShowCompassHeading()
{
	XYVector vector(m_osx, m_osy, 5, m_nav_heading);
	vector.set_active(m_active);
	vector.set_label("hdg");
	//vector.set_color("fill", "orange");
	vector.set_vertex_size(5);
	vector.set_edge_size(5);
	vector.set_edge_color("green");
	vector.setHeadSize(2);
	string str = vector.get_spec();
	m_Comms.Notify("VIEW_VECTOR", str);
}

//------------------------------------------------------------
// Procedure: PostPolygons
//   Purpose: Post KeepHeading region on pMarineViewer

void PoseKeepingX::PostPolygons(string mode)
{
	string spec = "format=radial,label=destination,vertex_color=blue,fill_color=grey90,vertex_size=0,edge_size=1,pts=24,snap=1";
	spec += ",x=" + DoubleToString(m_desired_x);
	spec += ",y=" + DoubleToString(m_desired_y);
	spec += ",radius=" + DoubleToString(m_tolerance_radius);

	if(!m_active)
	{
		spec += ",active=false";
		Notify("VIEW_POLYGON",spec);
		return;
	}

	spec += ",active=true";
	if(mode == "Keepheading")
	{
		
		spec += ",edge_color=blue";
		Notify("VIEW_POLYGON",spec);
		return;
	}

	spec += ",edge_color=red";
	Notify("VIEW_POLYGON",spec);
}

//------------------------------------------------------------
// Procedure: Filter
//   Purpose: Remove unreasonable heading provided by compass

bool PoseKeepingX::Filter()
{
  if(m_pre_heading == -1){}
  else
  {
	if(abs(m_nav_heading - m_pre_heading) > 120 && abs(m_nav_heading - m_pre_heading) < 340) // from logfile data result
	{
		return(false);
	}
  }
  return(true);
}

