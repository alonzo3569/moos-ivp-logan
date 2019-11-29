/************************************************************/
/*    NAME: Logan Zhang                                     */
/*    ORGN: MIT                                             */
/*    FILE: Mode.h                                          */
/*    DATE: October  23h, 2019                             */
/************************************************************/

#ifndef Mode_HEADER
#define Mode_HEADER

#include<iostream>
using namespace std;

class Mode
{
private:
    /* data */
    double m_curr_error = 0;
    string m_mode = "NULL";
    double m_thrust_r = 0;
    double m_thrust_l = 0;

    //10.21experiment feature
    const double m_back_thrust_limit  = -33;
    const double m_front_thrust_limit =  17;
    const unsigned int m_factor = 3;

public:
    // constructor & destructor
    Mode();
    ~Mode() {}
    // setter
    void seterror(double error)		{m_curr_error = error;}
    void setmode(string mode)		{m_mode = mode;}
    void setthrustr(double thrust)	{m_thrust_r = thrust;}
    void setthrustl(double thrust)	{m_thrust_l = thrust;}
    void setup(double error, string msg);
    
    // getter
    double geterror() const	{return m_curr_error;}
    string getmode() const	{return m_mode;}
    double getthrustr() const	{return m_thrust_r;}
    double getthrustl() const	{return m_thrust_l;}

    // action
    void CalculateError();
    void Output(double, double, string);
    void CheckValue();
};

#endif 
