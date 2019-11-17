#include"Mode.h"

void Keepheading::CalculateError()
{
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


void Keepheading::Output(double thrust, double speed)
{
	setthrustl( thrust);
	setthrustr(-thrust);
	//Notify("DESIRED_THRUST_L",  thrust);
	//Notify("DESIRED_THRUST_R", -thrust);
	return;
}

void Forward::CalculateError()
{	

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


void Forward::Output(double thrust, double speed)
{
	setthrustl(speed + thrust);
	setthrustr(speed - thrust);
	//Notify("DESIRED_THRUST_L", speed + thrust);
	//Notify("DESIRED_THRUST_R", speed - thrust);
	return;
}

