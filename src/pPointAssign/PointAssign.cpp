/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PointAssign.h"

#include <cstdlib>
#include <math.h>
using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  recv_block.clear();
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// function

//void PointAssign::postViewPoint(double x, double y, string label)//, string color)
//{
//  XYPoint point(x, y);
//  point.set_label(label);
//  //point.set_color("vertex", color);
//  point.set_param("vertex_size", "2");
//  string spec = point.get_spec();
//  Notify("VIEW_POINT", spec);
//}


//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
     string key   = msg.GetKey();
     if( key == "VISIT_POINT" )
        {
            string s;
	    Mydata block;
            s  = msg.GetString();
            //cout  << s << endl;
            if(i == 1)
            {
            	block.setfirstpoint();
		recv_block.push_back(block);
		i++;
		//break;
            }

  	    else if(i > 1 && i < 102)
	    {
           	 string a = tokStringParse(s, "x", ',', '=');
  		 string b = tokStringParse(s, "y", ',', '=');
  		 string c = tokStringParse(s, "unique_id", ',', '=');
  		 //cout << "x: [" << a << "]" << endl;
  		 //cout << "y: [" << b << "]" << endl;
  		 //cout << "unique_id: [" << c << "]" << endl;
  		 stringstream q,w,e;
  		 double x,y;
  		 int id; 
		 q << a;
		 q >> x;
		 w << b;
		 w >> y;
		 e << c;
		 e >> id;
		 block.setloc(x,y,id);
		 //postViewPoint(x,y,c);//, string color);
		 recv_block.push_back(block);
		 i++;
	    }
	    else
            {
            	block.setlastpoint();
		recv_block.push_back(block);
		return(true);
            }
             

            //Notify("DEBUG_ONE",x);
        }      
//---------------------------------------------

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble(;
    bool   mstr  = msg.IsString();
#endif
  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
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

bool PointAssign::Iterate()
{
  if(!recv_block.empty())
  {
	Mydata block; 
	//PointAssign a; 
	double x,y;
	int id;
  	block = recv_block.front();
	block.evenodd();					//m_id = 0 
	//block.eastwest(counterx,countery); //ew mode, just switch  this on 
	//Notify("BLOCK_NUM_GILDA",counterx);
	//Notify("BLOCK_NUM_HENRY",countery);
	string vname,str;
	str = block.write_str();
	vname = block.vname();  
	//block.test(); 	
	block.test(x,y,id);
	cout << x << ", " << y << ", " << id << endl;
	//a.postViewPoint(x,y,id, string color)
	//cout << vname << " , " << str << endl;
	if(vname == "HENRY")
	{
		Notify("VISIT_POINT_HENRY",str);		
		cout << "henry" << endl;	
	}
	else if(vname == "GILDA")
	{
		Notify("VISIT_POINT_GILDA",str);
		cout << "gilda" << endl;
	}
	else
	{
		if(str == "firstpoint")
		{
			Notify("VISIT_POINT_HENRY",str);
			Notify("VISIT_POINT_GILDA",str);
		}
		if(str == "lastpoint")
		{
			Notify("VISIT_POINT_HENRY",str);
			Notify("VISIT_POINT_GILDA",str);
			//Notify("BLOCK_NUM_GILDA",counterx);
			//Notify("BLOCK_NUM_HENRY",countery);
			//Notify("BBB","1");
		}
	} 
  	recv_block.pop_front();	
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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

void PointAssign::RegisterVariables()
{
    Register("VISIT_POINT", 0);
    Register("FUCK", 0);
  // Register("FOOBAR", 0);
}
//---------------------------------------------------------
// Procedure: RegisterVariables
//void PointAssign::onIdleState() 
//{
//  postViewPoint(g,l,h);
//}
 
//-----------------------------------------------------------

