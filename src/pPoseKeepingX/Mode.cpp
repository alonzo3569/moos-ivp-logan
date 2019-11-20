#include"Mode.h"

//------------------------------------------------------------------------
// Constructor
Mode::Mode() 
{
    double m_curr_error = 0;
    string m_mode;
    double m_thrust_r = 0;
    double m_thrust_l = 0;
}

//------------------------------------------------------------------------
// Procedure: setup
//   Purpose: setup mode object
void Mode::setup(double error, string msg)
{
	m_curr_error = error;
	m_mode = msg;
}

//------------------------------------------------------------------------
// Procedure: CalculateError
//   Purpose: Transform angle to the graph below and calculate error 
//
//   KeepHeading mode:              SetPoint mode:
//
//                                       forward        forward
//         left      0      right         left     0     right
//                   |                             |
//                   |                    -90      |      90
//         -90 ----- A ----- 90              ----- A -----       
//                   |                    -90      |      90           
//                   |                             |               
//              -180   180                left     0      right    
//                                      backward        backward   

void Mode::CalculateError()
{
	if (m_curr_error < -180)
	{
		m_curr_error += 360;
	}
	else if (m_curr_error > 180)
	{
		m_curr_error -= 360;
	}
	if (m_mode != "Backward")
	return;
	
	if(m_curr_error < 180 && m_curr_error > 90)
	{
		m_curr_error = -m_curr_error+180;
	}
	else if(m_curr_error < -90 && m_curr_error > -180)
	{
		m_curr_error = -m_curr_error-180;
	}

}
//------------------------------------------------------------------------
// Procedure: Output
//   Purpose: Set thrust lft & rgt based on different mode 

void Mode::Output(double thrust, double speed)
{
	if (m_mode == "Keepheading")
	{
		//setthrustl( thrust);
		//setthrustr(-thrust);
		m_thrust_l =  thrust;
		m_thrust_r = -thrust;
	}
	else if (m_mode == "Forward")
	{
		//setthrustl(speed + thrust);
		//setthrustr(speed - thrust);
		m_thrust_l = speed + thrust;
		m_thrust_r = speed - thrust;
	}
	else
	{
		m_thrust_l = -speed - thrust;
		m_thrust_r = -speed + thrust;
	}
}

