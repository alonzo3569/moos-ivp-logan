/***********************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: Mydata.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cmath>
#include "MBUtils.h"
#include "Mydata.h"
//#include <cstdint>
#include <cstdlib>

using namespace std;

//---------------------------------------------------------
// Constructor

Mydata::Mydata()
{
}

//---------------------------------------------------------
// Destructor

Mydata::~Mydata()
{
}

//---------------------------------------------------------

void  Mydata::setfirstpoint()
{
    m_firstpoint = true;
    //cout << m_firstpoint << endl;   
}
//---------------------------------------------------------
void  Mydata::setlastpoint()
{
    m_lastpoint = true;
    //cout << m_lastpoint << endl;     
}
//---------------------------------------------------------
void  Mydata::setloc(double x, double y, int id)
{
    m_x = x;
    m_y = y;
    m_id = id; 
    //cout << m_x << endl;  
    //cout << m_y << endl;  
    //cout << m_id << endl;    
}
//---------------------------------------------------------
void  Mydata::test(double &x, double &y, int &id)
{
    x = m_x;
    y = m_y;
    id= m_id;
    //cout << m_firstpoint << endl;  
    //cout << m_lastpoint << endl; 
    cout << m_x << endl;  
    cout << m_y << endl;  
    cout << m_id << endl;
    //cout << m_new_id << endl;
    //cout << m_name << endl; 
    //cout << m_str << endl; 
}
//---------------------------------------------------------
void  Mydata::evenodd()
{
    if(m_id == 0){return;}//first and last
    if(m_id != 0 )
    {
	if(m_id%2 == 0)
        {
		m_name = "GILDA";
		//cout << "before:" << m_id << endl;
		m_new_id = m_id/2;
		//cout << "after:" << m_id << endl;
        }
	if(m_id%2 == 1)
	{
		m_name = "HENRY";
		//cout << "before:" << m_id << endl;
		m_new_id = (m_id+1)/2;
		//cout << "after:" << m_id << endl;
	}
    }
}
//---------------------------------------------------------
void  Mydata::eastwest(int &counterx, int &countery)
{
    if(m_id == 0){return;}//first and last
    if(m_id != 0 )
    {
	if(m_x < 87.5)
        {
		m_name = "GILDA";
		//cout << "before:" << m_id << endl;
		m_new_id = counterx +1;
		counterx++;
		//cout << "after:" << m_id << endl;
        }
	if(m_x > 87.5)
	{
		m_name = "HENRY";
		//cout << "before:" << m_id << endl;
		m_new_id = countery +1;
		countery++;
		//cout << "after:" << m_id << endl;
	}
    }
}
//---------------------------------------------------------
string  Mydata::write_str()
{
    stringstream ss;  
    if(m_id == 0 && m_firstpoint == true)
    { 
	string f = "firstpoint";	
	m_str    = f; 
    }
    if(m_id == 0 && m_lastpoint == true)
    { 
	string f = "lastpoint";	
	m_str    = f; 
    }
    if(m_id != 0)
    {
	ss << "x=" << m_x <<  ",y=" << m_y << ",unique_id=" << m_new_id << endl;
	//ss << "x=" << m_x << ", y=" << m_y << ", unique_id=" << m_new_id << endl;
        ss >> m_str;
    }
    return(m_str);  
}
//---------------------------------------------------------
string  Mydata::vname()
{
    return(m_name);     
}
//---------------------------------------------------------
//void setcounterxy(int m_counterx,int m_countery)
//{
//	shane guo
//}
