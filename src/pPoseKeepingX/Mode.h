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

public:
    // constructor & destructor
    //Mode(/* args */) {}
    Mode(double error){
	m_curr_error = error;
	cout << "Base con" << endl;
    }
    virtual ~Mode() { cout << "Base de" << endl;}
    // setter
    void seterror(double error){m_curr_error = error;}
    void setmode(string mode){m_mode = mode;}
    void setthrustr(double thrust){m_thrust_r = thrust;}
    void setthrustl(double thrust){m_thrust_l = thrust;}
    
    // getter
    double geterror(){return m_curr_error;}
    string getmode(){return m_mode;}
    double getthrustr(){return m_thrust_r;}
    double getthrustl(){return m_thrust_l;}

    // absturact functions
    virtual void CalculateError() = 0;
    virtual void Output(double, double) = 0;
};

class Keepheading: public Mode
{
private:
    /* data */
public:
    Keepheading(double error): Mode(error) {
        setmode("keepheading");
	cout << "Keep con" << endl;
    }
    ~Keepheading() {cout << "Keep de" << endl;}
    void CalculateError();
    void Output(double, double);
};

class Forward: public Mode
{
private:
    /* data */
public:
    Forward(double error): Mode(error) {
        setmode("forward");
	cout << "Forward con" << endl;
    }
    ~Forward() {cout << "Forward de" << endl;}
    void CalculateError();
    void Output(double, double);
};

#endif 
