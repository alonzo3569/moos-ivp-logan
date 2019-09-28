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
#include <math.h>
//#include <cstdint>
#include <cstdlib>
#include<string>

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
void  Mydata::test2(double &x, double &y)
{
    x = m_x;    
    y = m_y;
    //cout << m_firstpoint << endl;  
    //cout << m_lastpoint << endl; 
    //cout << m_x << endl;  
    //cout << m_y << endl;  
    //cout << m_id << endl;
    //cout << m_diff << endl; 
    //cout << m_new_id << endl;
    //cout << m_name << endl; 
    //cout << m_str << endl; 
}
//---------------------------------------------------------
double  Mydata::caldiff(double x, double y)
{
	m_diff = pow(m_x-x,2)+pow(m_y-y,2);
	return(m_diff);
}
//---------------------------------------------------------
void  Mydata::set_startxy(double &x, double &y)
{
	x = m_x;
	y = m_y;
}
//---------------------------------------------------------
//void  Mydata::buildreport()
//{
//	x = m_x;
//	y = m_y;
//}
