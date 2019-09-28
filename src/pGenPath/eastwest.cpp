/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "GenPath.h"
#include <math.h>
#include<string>

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  recv_block.clear();
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}
//---------------------------------------------------------
// function
//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p; 
//  string BBB = msg.GetKey();
//  while(BBB == "BBB"){
	string key   = msg.GetKey();
	//cout << "BBB" << endl;
    	if(key == "BLOCK_NUM")
    	{
    		double bn;
    		bn  = msg.GetDouble();
    		block_number = bn;
		cout << block_number << endl;
    	}
    	if( key == "aaa" )
        {
            	string s;
	   	 Mydata block;
           	 s  = msg.GetString();
//          	  if(i == 1)
//            	{
//            		//block.setfirstpoint();
//			//recv_block.push_back(block);
//			i++;
//			//break;
//            	}	
//
//  	    	else if(i > 1 && i < block_number+2) //<52** i=block number +2 **
//	    	{
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
			 recv_block.push_back(block);
			 i++;
//	    	}	
//	   	 else
//            	{
//            		//block.setlastpoint();
//			//recv_block.push_back(block);
//			return(true);
//            	}
	}
    //--------------------------------------------------
    //get NAV_X,NAV_Y

    	if( key == "NAV_X" )
        	{    
			double X  = msg.GetDouble();
			//cout << "NAV_X" << X << endl;
			start_x = X;		
		}

	if( key == "NAV_Y" )
		{    
			double Y  = msg.GetDouble();
			//cout << "NAV_Y" << Y << endl;	
			start_y = Y;	
		}

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
//   }//while END
   }//for END
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
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

bool GenPath::Iterate()
{
  Mydata block,A,B;
  double Adiff,Bdiff;
  start_x = 0;
  start_y = 0;
//  cout << "bn =" << block_number << endl;
//  cout << "i =" << i << endl;
  if(!recv_block.empty() && i == block_number+2) // == 52** block number+2 **
  {
	recv_block.pop_front();
	recv_block.pop_back();
	for(k=1;k<block_number;k++)//k=1~2//endpoint = block number**
	{

		//cout << "k loop" << k << endl;
		int j = 1;		
		if(j == 1)
		{
			A = recv_block.front();
			Bdiff = A.caldiff(start_x,start_y);//(start_x,start_y)
			B = A;
  			recv_block.pop_front();
			j++;
			//cout << "j==1 +1  " << j << endl;	

		}		
		for(j=2;j<block_number+2-k;j=j+1)//j<(5-k)
		{
			//cout << "j loop " << j << endl;
			A = recv_block.front();
			Adiff = A.caldiff(start_x,start_y);//(start_x,start_y)

			if( Adiff > Bdiff)
			{
				recv_block.pop_front();
				recv_block.push_back(A);
			}
			else//Adiff < Bdiff
			{
				recv_block.push_back(B);
				B = A;
				Bdiff = Adiff;
				recv_block.pop_front();
			}

		}

		des_list.push_back(B);//change start point!
		B.set_startxy(start_x, start_y);

	}

	if(k == block_number)// = block number**
	{
		cout << "k == 4" << endl;
		B = recv_block.front();
		Bdiff = B.caldiff(start_x,start_y);//(start_x,start_y)
		des_list.push_back(B);
	}
		
	int counter=1;
	msg = "polygon=";
	while(!des_list.empty())
	{
	cout << "counter = " << counter << endl;
	//cout << "x " <<start_x << "y " << start_y << endl;	
	cout <<  "---------------" << endl;
	double m_x , m_y;
	Z = des_list.front();
	Z.test2(m_x,m_y);
	stringstream t,u;
	string reportx,reporty;
	t << m_x;
	u << m_y;
	t >> reportx;
	u >> reporty;
	//cout << "(" << m_x << "," << m_y << ")" << endl;
	if(counter < 50)
	{	
		msg += reportx + "," + reporty + ":";
		//cout << msg << endl;
	}
	if(counter == 50)
	{
		msg += reportx + "," + reporty;
	}
	des_list.pop_front();
	counter++;
	}
	//cout << msg << endl;
	Notify("UPDATES_VAR",msg);
	int op=1;
	while(op ==1) {}
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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

void GenPath::RegisterVariables()
{
  // Register("FOOBAR", 0);
     Register("aaa", 0);
     Register("NAV_X", 0);
     Register("NAV_Y", 0);
     //Register("BBB", 0);
     Register("BLOCK_NUM", 0);
}

