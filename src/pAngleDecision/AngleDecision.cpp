/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: AngleDecision.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "AngleDecision.h"
#include<XYPoint.h>//pMarineViewer


using namespace std;

//---------------------------------------------------------
// Constructor

AngleDecision::AngleDecision()
{
	angle_point.clear();
}

//---------------------------------------------------------
// Destructor

AngleDecision::~AngleDecision()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool AngleDecision::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    string sval  = msg.GetString();//change

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

     //^_^
     else if(key == "ANGLE_REPORT") //should be placed before unhandled mail
     {
	cout << "get angle report" << endl;
	GetData(sval);
     }

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);

   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool AngleDecision::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool AngleDecision::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  PostRegion();
  //if(!angle_point.empty())
  if(angle_point.size() == 2)
  {
	  Average();
	  struct Data block = angle_point.back();
	  if(block.x1 < block.x2)
  	  {
		FormReport1(block.x1, block.y1, block.theta, m_vname1, block.dis);
		FormReport2(block.x2, block.y2, block.theta+180, m_vname2, block.dis);
	  }
	  else
  	  {
		FormReport1(block.x2, block.y2, block.theta, m_vname1, block.dis);
		FormReport2(block.x1, block.y1, block.theta+180, m_vname2, block.dis);
	  }
  }
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool AngleDecision::OnStartUp()
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

    else if(param == "vname1") {
      value = stripQuotes(value);
      m_vname1 = value;
      handled = true;
    }

    else if(param == "vname2") {
      value = stripQuotes(value);
      m_vname2 = value;
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

void AngleDecision::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
     Register("ANGLE_REPORT", 0);
     //Register("VIEW_POLYGON", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool AngleDecision::buildReport() 
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
// Procedure: GetData()
//   Purpose: Get ANGLE_REORT from MOOSDB
void AngleDecision::GetData(string a)
{
	struct Data block;
	block.theta = stod(tokStringParse(a , "theta" , ',' , '=' ));
	block.x1    = stod(tokStringParse(a , "x1" , ',' , '=' ));
	block.y1    = stod(tokStringParse(a , "y1" , ',' , '=' ));
	block.x2    = stod(tokStringParse(a , "x2" , ',' , '=' ));
	block.y2    = stod(tokStringParse(a , "y2" , ',' , '=' ));
	block.dis   = 0.5*sqrt(pow(block.x1-block.x2,2)+pow(block.y1-block.y2,2));
	angle_point.push_back(block); 
	Notify("CHECK", block.dis);
}

//------------------------------------------------------------
// Procedure: ConvertToString()
//   Purpose: ConvertToString 
string AngleDecision::ConvertToString(double value)
{
	string msg;
	stringstream ss;
	ss << value;
	ss >> msg;
	return(msg);
}
//------------------------------------------------------------
// Procedure: FormReport()
//   Purpose: make theta and start point report for each vehicle
void AngleDecision::FormReport1(double x, double y, double theta, string name, double distance)
{
	string msg;
	msg = "name="  + name +
 	      ",theta="+ ConvertToString(theta) + 
	      ",x="    + ConvertToString(x) +
	      ",y="    + ConvertToString(y) +
	      ",dis="  + ConvertToString(distance);
	//MarkpMarine(x,y,name);
	Notify("A_ARCHIE",msg);//==============================CHANGE NAME
	//Notify("A_GILDA",msg);//==============================CHANGE NAME
}
//------------------------------------------------------------
// Procedure: FormReport()
//   Purpose: make theta and start point report for each vehicle
void AngleDecision::FormReport2(double x, double y, double theta, string name, double distance)
{
	string msg;
	msg = "name="  + name +
 	      ",theta="+ ConvertToString(theta) + 
	      ",x="    + ConvertToString(x) +
	      ",y="    + ConvertToString(y) +
	      ",dis="  + ConvertToString(distance);
	//MarkpMarine(x,y,name);
	Notify("A_BETTY",msg);//==============================CHANGE NAME
	//Notify("A_HENRY",msg);//==============================CHANGE NAME
}
//------------------------------------------------------------
// Procedure: Average()
//   Purpose: average of two Data block
void AngleDecision::Average()
{
	struct Data block;
        vector<struct Data>::iterator iter = angle_point.begin();
	block.x1 = 0.5*((*iter).x1 + (*(iter+1)).x1);
	block.x2 = 0.5*((*iter).x2 + (*(iter+1)).x2);
	block.y1 = 0.5*((*iter).y1 + (*(iter+1)).y1);
	block.y2 = 0.5*((*iter).y2 + (*(iter+1)).y2);
	block.dis = 0.5*((*iter).dis + (*(iter+1)).dis);
	block.theta = atan((block.y2 - block.y1)/(block.x2 - block.x1))*180/PI; //0.5*((*iter).theta + (*(iter+1)).theta);
	angle_point.push_back(block);
	//string msg = ConvertToString(block.x1) + ConvertToString(block.y1)+ ConvertToString(block.x2)+ ConvertToString(block.y2)+ ConvertToString(block.theta)+ ConvertToString(block.dis);
	//Notify("BLOCK_AVG",msg);
}

//------------------------------------------------------------
// Procedure: PostRegion()
//   Purpose: Post Region
void AngleDecision::PostRegion()
{
	string msg = "label=logan, pts={160,0:100,0:-40,-50:-85,-80:-45,-185:165,-185},edge_color=red,vertex_color=brown,vertex_size=2,edge_size=1";
	Notify("VIEW_POLYGON", msg);
}

