/************************************************************/
/*    NAME: Logan Zhang                                     */
/*    ORGN: MIT                                             */
/*    FILE: PoseKeeping.h                                   */
/*    DATE: September 15th, 2019                            */
/************************************************************/

#ifndef PoseKeeping_HEADER
#define PoseKeeping_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class PoseKeeping : public AppCastingMOOSApp
{
 public:
   PoseKeeping();
   ~PoseKeeping();

   class Data
   {
    public:
      double m_curr_heading;
      double m_curr_error;
      double m_curr_distance;
      std::string m_mode;

      //keep heading mode data block constructor
      Data(double heading):m_curr_heading(heading)
      {
	m_curr_error = 0;
	m_curr_distance = 0;
	m_mode = "KeepHeading";
      }

      //set point mode data block
      Data(double heading, double distance):m_curr_heading(heading),m_curr_distance(distance)
      {
	m_curr_error = 0;
	m_mode = '\0';
      }
   };

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 protected: //My Function
   void KeepHeading();
   void CalculateError(Data &block, double desired_angle);
   void OutputThruster(Data block, double thrust, double speed);
   void SetPoint();
   double relAng(double xa, double ya, double xb, double yb);
   double angle360(double degval);
   double radToDegrees(double radval);
   double Distance(double current_x, double current_y, double destination_x, double destination_y);
   double Speed(Data &block);
   void CheckMode(Data &block);
   void postPolygons();
   std::string DoubleToString(double input);


 private: // Configuration variables
   double m_desired_heading;
   double m_kp;
   double m_ki;
   double m_kd;
   double m_tolerance_radius;

 private: // State variables
   double m_nav_heading;
   double m_osx;
   double m_osy;
   double m_desired_x;
   double m_desired_y;
   double m_previous_time;
   double m_previous_error;
   double m_steady_error;
   std::string m_switch_mode;
   bool m_keep_heading;
   double m_arrival_radius;
   int m_upper_speed;
   int m_lower_speed;   
   bool m_active;
   //version4
   double m_previous_error_x;
   std::string m_switch;
   bool m_switch_temp;
};

#endif 
