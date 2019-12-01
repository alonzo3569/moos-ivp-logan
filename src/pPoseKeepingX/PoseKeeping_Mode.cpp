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

void Mode::Output(double thrust, double speed, std::string simulation)
{
	if (m_mode == "Keepheading")
	{
		if(simulation == "true")
		{
			m_thrust_l =  thrust;
			m_thrust_r = -thrust;
			return;
		}

		if (thrust > 0)
		{
			/*if (thrust < m_front_thrust_limit)
			{
				m_thrust_l = m_front_thrust_limit;
				m_thrust_r = -m_factor * m_front_thrust_limit;
			}
			else if (thrust >= m_front_thrust_limit && thrust <= 100.0/m_factor)
			{
				m_thrust_l = thrust;
				m_thrust_r = -m_factor * thrust;
			}
			else if (thrust > 100.0/m_factor)
			{
				m_thrust_l = 100.0/m_factor;
				m_thrust_r = -100;
			}*/
			m_thrust_l =  20;
			m_thrust_r = -60;
		}
		else
		{
			/*if (abs(thrust) < m_front_thrust_limit)
			{
				m_thrust_l = -m_factor * m_front_thrust_limit;
				m_thrust_r = m_front_thrust_limit;
			}
			else if (abs(thrust) >= m_front_thrust_limit && thrust <= 100.0/m_factor)
			{
				m_thrust_l = thrust;
				m_thrust_r = -m_factor *thrust;
			}
			else if (abs(thrust) > 100.0/m_factor)
			{
				m_thrust_l = -100;
				m_thrust_r = 100.0/m_factor;
			}*/
			m_thrust_r =  20;
			m_thrust_l = -60;
		}

	}
	else if (m_mode == "Forward")
	{
		m_thrust_l = speed + thrust + 10; // forward shift
		m_thrust_r = speed - thrust + 10;
	}
	else
	{
		m_thrust_l = -speed - thrust - 33; // backward shift
		m_thrust_r = -speed + thrust - 33;
	}
}

//------------------------------------------------------------------------
// Procedure: CheckValue
//   Purpose: Value between -33 to 17 is not acceptable for heron, change it to zero

void Mode::CheckValue()
{
	if(m_thrust_r > m_back_thrust_limit && m_thrust_r < m_front_thrust_limit)
	{
		m_thrust_r = 0;
	}

	if(m_thrust_l > m_back_thrust_limit && m_thrust_l < m_front_thrust_limit)
	{
		m_thrust_l = 0;
	}
}


