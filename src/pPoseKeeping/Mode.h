/************************************************************/
/*    NAME: Logan Zhang                                     */
/*    ORGN: MIT                                             */
/*    FILE: Mode.h                                          */
/*    DATE: October  23h, 2019                             */
/************************************************************/

#ifndef Mode_HEADER
#define Mode_HEADER

#include<iostream>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "AngleUtils.h"
using namespace std;

class Mode
{
private:
    /* data */
    double m_curr_heading = 0;
    double m_curr_error = 0;
    double m_curr_distance = 0;
    string m_mode = "NULL";
    double m_thrust = 0;
    double m_thrust_r = 0;
    double m_thrust_l = 0;
    double m_speed = 0;
    // static member (PID var)
    static double m_kp, m_ki, m_kd;
    static double m_previous_time;//
    static double m_previous_error;
    static double m_steady_error;//
    static string m_switch_mode;
    static double m_curr_time;
    static double m_delta_time;//

public:
    // constructor & destructor
    Mode(/* args */) {}
    Mode(double heading, double error){
        m_curr_heading = heading;
	m_curr_error = error;
	cout << "Base con" << endl;
    }
    Mode(double heading, double error, double distance){
	m_curr_heading = heading;
	m_curr_error = error;
	m_curr_distance = distance;
	cout << "Base con" << endl;
    }
    ~Mode() { cout << "Base de" << endl;}
    // setter
    void setheading(double heading){ m_curr_heading = heading;}
    void seterror(double error){m_curr_error = error;}
    void setdistance(double distance){m_curr_distance = distance;}
    void setmode(string mode){m_mode = mode;}
    void setthrust(double thrust){m_thrust = thrust;}
    void setthrustr(double r){m_thrust_r = r;}
    void setthrustl(double l){m_thrust_l = l;}
    void setspeed(double speed){m_speed = speed;}
    
    static void setkp(double kp){m_kp = kp;}
    static void setki(double ki){m_ki = ki;}
    static void setkd(double kd){m_kd = kd;}
    static void setpretime(double time){m_previous_time = time;}
 /*
    static void setpreerror(double error){m_previous_error = error;}
    static void setsteadyerror(double error){m_steady_error = error;}
    static void setstaticmode(string mode){m_switch_mode = mode;}
*/
    // getter
    double getheading(){return m_curr_heading;}
    double geterror(){return m_curr_error;}
    double getdistance(){return m_curr_distance;}
    string getmode(){return m_mode;}
    double getthrust(){return m_thrust;}
    double getthrustr(){return m_thrust_r;}
    double getthrustl(){return m_thrust_l;}
    double getspeed(){return m_speed;}

    static double getkp(){return m_kp;}
    static double getki(){return m_ki;}
    static double getkd(){return m_kd;}

    static double getpretime(){return m_previous_time;}//
    static double getpreerror(){return m_previous_error;}
    static double getsteadyerror(){return m_steady_error;}
    static string getstaticmode(){return m_switch_mode;}//
    static double getdeltatime(){return m_delta_time;}

    // reuse functions
    void CheckMode();
    void SetParams(); 
    void SaveParams();
    //void Thrust();

    // absturact functions
    virtual void CalculateError() = 0;
    virtual void ThrustSpeed() = 0;
    virtual void Output() = 0;
    //friend ostream& operator<<(ostream& os, Mode &M);
};

class Keepheading: public Mode
{
private:
    /* data */
public:
    Keepheading(double heading, double error): Mode(heading, error) {
        setmode("keepheading");
	cout << "Keep con" << endl;
    }
    ~Keepheading() {cout << "Keep de" << endl;}
    void CalculateError();
    void ThrustSpeed();
    void Output();
    //string whichmode(){return "keepheading";}

};

class Front: public Mode
{
private:
    /* data */
public:
    Front(double heading, double error, double distance): Mode(heading, error, distance) {
        setmode("front");
	cout << "Front con" << endl;
    }
    ~Front() {cout << "Front de" << endl;}
    void CalculateError();
    void ThrustSpeed();
    void Output();
    //bool IsFront();

};

#endif 
