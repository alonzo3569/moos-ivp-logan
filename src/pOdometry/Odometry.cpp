/************************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"
#include <cmath>
using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
m_first_reading = false;
m_current_x = 0;
m_current_y = 0;
m_previous_x = 0;
m_previous_y = 0;
m_total_distance = 0;
sum = 0;
currentD = 0;

}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++)
  {
    CMOOSMsg &msg = *p;

    //cout << "get previousX=" << m_previous_x  << endl;
    //cout << "get previousY=" << m_previous_y  << endl;
    string key   = msg.GetKey();
    if(key == "NAV_X")
    {
        m_current_x  = msg.GetDouble();
        cout << "get currentX=" << m_current_x  << endl;
    }
    if(key == "NAV_Y")
    { 
         m_current_y  = msg.GetDouble();
        cout << "get currentY=" << m_current_y  << endl;
    } 
   // cout << "1" << endl;
   // currentD = sqrt(pow(m_current_x,2)+pow(m_current_y,2));
   // cout << "2" << endl;
   // cout << "currentD=" << currentD  << endl;
   // m_total_distance = m_total_distance + currentD;
   // cout << "distance="<< m_total_distance  << endl;
   // Notify("ODOMETRY_DIST",m_total_distance);
   // cout << "3" << endl;
   // m_current_x = m_previous_x;
   // m_current_y = m_previous_y;
        
  
#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
    //cout << "px="<< m_previous_x  << endl;
    //cout << "py="<< m_previous_y  << endl;
    currentD = sqrt(pow(m_current_x-m_previous_x,2)+pow(m_current_y-m_previous_y,2));
    cout << "currentD=" << currentD  << endl;
    m_total_distance = m_total_distance + currentD;
    cout << "distance="<< m_total_distance  << endl;
    Notify("ODOMETRY_DIST",m_total_distance);
    m_previous_x = m_current_x;
    m_previous_y = m_current_y;
    //cout << "px="<< m_previous_x  << endl;
    //cout << "py="<< m_previous_y  << endl;



  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  // Register("FOOBAR", 0);
   Register("NAV_X", 0);
   Register("NAV_Y", 0);
  // Register("DEPLOY", false);
}

