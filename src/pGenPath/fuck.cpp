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
  des_list.clear();
  replan_list.clear();
  revisit_list.clear();
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
    string key   = msg.GetKey();
    if( key == "aaa" )
        {
            string s;
	    Mydata block;
            s  = msg.GetString();
            if(i == 1)
            {
            	//block.setfirstpoint();
		//recv_block.push_back(block);
		i++;
		//break;
            }

  	    else if(i > 1 && i < block_number+2) //<52** i=block number +2 **
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
		 recv_block.push_back(block);
		 i++;
	    }
	    else
            {
            	//block.setlastpoint();
		//recv_block.push_back(block);
		return(true);
            }
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
    if( key == "WPT_INDEX" )
	{    
		INDEX   = msg.GetDouble();
		WPT_INDEX   = INDEX;
		//cout << "WPT" << WPT_INDEX << endl;		
	}
    if( key == "GENPATH_REGENERATE" )
	{    
		string sval  = msg.GetString();
		//double dval  = msg.GetDouble();
		//cout << "G_R = " << sval << endl;
		if(sval == "true")		
		{
			//cout << "sval = " << sval << "= TRUE" << endl;
			G_R = true;
		}
//		if(sval =="false")//1)
//		{
//			cout << "sval = " << sval << "= FALSE" << endl;			
//			G_R = false;}	
	}
    if( key == "DEPLOY" )
	{    
		string sval  = msg.GetString();
		deploy   = sval;	
	}
    if( key == "RETURN" )
	{    
		string sval  = msg.GetString();
		r   = sval;	
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
   }
	
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
  //start_x = 0;
  //start_y = 0;
  //int size;
  //size = recv_block.size();
  //cout << size << endl;
  if(!recv_block.empty() && i == block_number+2) // == 52** block number+2 **
  {
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
		B = recv_block.front();
		Bdiff = B.caldiff(start_x,start_y);//(start_x,start_y)
		des_list.push_back(B);
		recv_block.pop_front();
	}//recv_block 	rev_block empty des_list full	
    int counter=1;
    msg = "polygon=";
    while(!des_list.empty())
    {
    	cout << "counter = " << counter << endl;
    	//cout << "x " <<start_x << "y " << start_y << endl;	
    	cout <<  "---------------" << endl;
    	double m_x , m_y;
    	Z = des_list.front();
    	replan_list.push_back(Z);
    	Z.test2(m_x,m_y);
    	stringstream t,u;
    	string reportx,reporty;
    	t << m_x;
    	u << m_y;
    	t >> reportx;
    	u >> reporty;
    	//cout << "(" << m_x << "," << m_y << ")" << endl;
    	if(counter < block_number)//50)
    	{	
    		msg += reportx + "," + reporty + ":";
    		//cout << msg << endl;
    	}
    	if(counter == block_number)//50)
    	{
    		msg += reportx + "," + reporty;
    	}	
    	des_list.pop_front();
    	counter++;
      //cout << msg << endl;
    }// des_list empty replan_list full
    Notify("UPDATES_VAR",msg);			//first TSP planning finish 
    
  }//if end 
  


if(!replan_list.empty() && G_R == true ) //vehicle haven't deployed yet
{
	if(WPT_INDEX == c2)
	{
		Mydata ii;
		ii = replan_list.front();
		ii.test2(replan_x,replan_y); //m_x ,  m_y

		//double diff_x,diff_y;
		if(pow(start_x-replan_x,2) < 16 && pow(start_y-replan_y,2) < 16)
		{
			replan_list.pop_front();
			//cout << "start_x = " << start_x << endl;
			//cout << "start_y = " << start_y << endl;
			//cout << "replan_x = " << replan_x << endl;
			//cout << "replan_y = " << replan_y << endl; 
			//cout << c2 << " == " << WPT_INDEX << endl;
			cout << "same pop" << endl;
			c2++;
			//cout << c2 << " < " << WPT_INDEX << endl;
			return(true);
		}
		else //NAV_X != m_x
		{
			cout << "never equal" << endl;
			return(true);
		}
	}
	else //WPT_INDEX != c2
	{
		if(c2 > WPT_INDEX)
		{
			cout << "c2 > WPT_INDEX" << endl;
			//cout << c2 << " < " << WPT_INDEX << endl;
			return(true);
		}
		else if(c2 < WPT_INDEX)				
		{
			//if(replan_list.size() == 1)
			//{
			//	replan_list.pop_front();
			//	cout << "ya" << endl;
			//}			
			//else 
			//{
			Mydata ii;
			ii = replan_list.front();
			revisit_list.push_back(ii);
			replan_list.pop_front();
			cout << c2 << " < " << WPT_INDEX <<"---------------------------no same next WPT" << endl;
			c2++;
			return(true);
			//}
		}

	}
}////replan empty revisit full , vehicle finish surveying , and vehicle post G_R to MOOSDB




else if(replan_list.empty() && !revisit_list.empty() && G_R == true ) //replan_list empty
{
	G_R = false;
	cout << "G_R = " << G_R << endl;
}//replan = empty, G_R = false

else if(replan_list.empty() && revisit_list.empty() && G_R == true && r == "false")
{
	//Notify("MOOS_MANUAL_OVERRIDE","true");
	Notify("DEPLOY","true");
}


if(!revisit_list.empty() && G_R == false)
{
			cout << "print" << endl;
			c2 = 0;			
			int c1=1;
			msg_replan = "polygon=";
			int size = revisit_list.size();		
			while(!revisit_list.empty())
			{
				Mydata jj;
				jj = revisit_list.front();
				jj.test2(replan_output__x,replan_output__y);
				stringstream t,u;
				string reportx,reporty;
				t << replan_output__x;
				u << replan_output__y;
				t >> reportx;
				u >> reporty;
				if(c1 < size)//50)
				{	
					msg_replan += reportx + "," + reporty + ":";
				}
				if(c1 == size)//50)
				{
					msg_replan += reportx + "," + reporty;
				}	
				cout << msg_replan << endl;
				replan_list.push_back(jj);
				revisit_list.pop_front();
				Notify("UPDATES_VAR",msg_replan);
			}
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
     Register("WPT_INDEX", 0);
     Register("DEPLOY", 0);
     Register("RETURN", 0);
     Register("GENPATH_REGENERATE", 0);
}

