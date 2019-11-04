#include"Mode.h"

// initialize static member
double Mode::m_previous_time = 0;
double Mode::m_previous_error = 0;
double Mode::m_steady_error = 0;
string Mode::m_switch_mode = "NULL";
double Mode::m_curr_time = 0;
double Mode::m_delta_time = 0;
double Mode::m_kp = 0;
double Mode::m_ki = 0;
double Mode::m_kd = 0;

/*
ostream& operator<<(ostream& os, Mode& M){
    os << "Keepheading class" << endl
       << "=========================" << endl 
       << "m_curr_heading = "<< M.getheading() << endl
       << "m_curr_error = " << M.geterror() << endl
       << "m_curr_distance = " << M.getdistance() << endl
       << "m_mode = " << M.getmode() << endl
       << "Static Member:" << endl

       << "m_previous_time = " << Mode::getpretime() << endl
       << "m_previous_error = " << Mode::getpreerror() << endl
       << "m_steady_error = " << Mode::getsteadyerror() << endl
       << "m_switch_mode = " << Mode::getstaticmode() << endl;

       << "m_previous_time = " << Mode::m_previous_time << endl
       << "m_previous_error = " << Mode::m_previous_error << endl
       << "m_steady_error = " << Mode::m_steady_error << endl
       << "m_switch_mode = " << Mode::m_switch_mode << endl
       << "Output:" << endl
       << "m_thrust = " << M.getthrust() << endl;

    return os;
}
*/

void Keepheading::CalculateError(){

    double error = geterror();

	if (error < -180)
	{
		error += 360;
	}
	else if (error > 180)
	{
		error -= 360;
	}
    seterror(error);

   return;
}

void Mode::CheckMode(){

    //if (getmode() != getstaticmode()){
    if (getmode() != m_switch_mode){
        m_previous_time = MOOSTime(); // MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();
        m_previous_error = 0;
        m_steady_error = 0;
        m_switch_mode = getmode();
        /*
        setpretime(100);
        setpreerror(0);
        setsteadyerror(0);
        setstaticmode(getmode());
        */
        // Notify("STATIC_MODE",getstaticmode());
    }
}

void Mode::SetParams(){
	//Get time difference for PID
    m_curr_time = MOOSTime(); //MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();MOOSTime();
    m_delta_time = m_curr_time - m_previous_time;
   	//Calculate steady state error
    m_steady_error = m_steady_error + m_curr_error*m_delta_time;
}

void Keepheading::ThrustSpeed(){
    //Calculate thrust 
    setthrust(Mode::getkp()*geterror() + getkd()*(geterror() - getpreerror())/getdeltatime() + getki()*getsteadyerror());
}

void Keepheading::Output(){
	//Notify thrust to MOOSDB
	setthrustr(-getthrust());
	setthrustl( getthrust());
}

void Mode::SaveParams(){
    //Saving pid params
    m_previous_error = m_curr_error;
    m_previous_time = m_curr_time;
}

void Front::CalculateError(){
	
	double error = geterror();
	if (error < -180)
	{
		error += 360;
	}
	else if (error > 180)
	{
		error -= 360;
	}
	seterror(error);
}

void Front::ThrustSpeed(){
	setthrust(Mode::getkp()*geterror() + Mode::getkd()*(geterror() - Mode::getpreerror())/getdeltatime() + Mode::getki()*getsteadyerror());
	double speed = Mode::getkp()*getdistance();
	if(speed > 100) //m_upper_speed
	{
		speed = 100;
	}	
	if(speed < 10)//m_lower_speed
	{
		speed = 10;
	}
	setspeed(speed);
}

void Front::Output(){
	setthrustr(getspeed() - getthrust());
	setthrustl(getspeed() + getthrust());
}

