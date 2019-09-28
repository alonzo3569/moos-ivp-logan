/***********************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cmath>
#include "MBUtils.h"
#include "PrimeFactor.h"
//#include <cstdint>
#include <cstdlib>
#include <string>
#include <stdlib.h>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
   // x =0;
   m_buff.clear();
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    
    string key   = msg.GetKey();
    if( key == "NUM_VALUE" )
        {
            // x  = msg.GetDouble();
            // if (x%2 == 1){ cout << x << " is odd" <<  endl;} 
            // else { cout << x << " is even" << endl;} 
            stringstream y;//將NUM_VALUE由字串轉整數
            cout  << "input number = " << msg.GetString() << endl; 
            uint64_t input_num = 0;
            //string input_str = msg.GetString();
            //y << msg.GetString();
            //y >> input_num;
            m_buff.push_back(atoll(msg.GetString().c_str()));
            //cout << "num = " << input_num << endl;
            Notify("DEBUG_ONE",input_num);
                  
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
   } 
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//----------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
   uint64_t x;       
   double tic,toc;
   if(!m_buff.empty())
   {
     x =  m_buff.front();
     cout << m_buff[0] << endl;
     //cout << x << endl;
     
     Mydata fuck;
     fuck.setori(x);
     tic = MOOSTime();
     cout << "start" << x << endl;
     fuck.primefactor();
     toc = MOOSTime();
     fuck.gettimespend(toc-tic);
     string result = fuck.buildreport();
     Notify("RESULT",result);
     cout << "finish" << endl;
     m_buff.pop_front(); 
   }
   // cout << x << endl;
          // if (x%2 == 1){ cout << x << " is odd" <<  endl;} 
           //else { cout << x << " is even" << endl;}

   

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
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

void PrimeFactor::RegisterVariables()
{
  // Register("FOOBAR", 0);
   Register("NUM_VALUE", 0);
}

