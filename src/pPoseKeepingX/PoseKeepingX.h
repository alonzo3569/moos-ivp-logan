/************************************************************/
/*    NAME: Logan                                           */
/*    ORGN: MIT                                             */
/*    FILE: PoseKeepingX.h                                  */
/*    DATE: 2019/11/15                                      */
/************************************************************/

#ifndef PoseKeepingX_HEADER
#define PoseKeepingX_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "Mode.h"
#include <memory>

class PoseKeepingX : public AppCastingMOOSApp
{
 public:
  PoseKeepingX();
  ~PoseKeepingX() {};

 protected: // Standard MOOSApp functions to overload
  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool Iterate();
  bool OnConnectToServer();
  bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload
  bool buildReport();

 protected:
  void registerVariables();

 protected: // My function
  double  Distance(double, double, double, double);
  void    CheckMode(const Mode);
  double  CheckSpeed(double);
  string  DoubleToString(double);

  //10.21
  void    ShowCompassHeading();
  void    PostPolygons(string mode = "");
  bool    Filter();

 protected: // Mail Callbacks
#if 0 // Keep this as an example for callbacks
  bool onMessageFoo(CMOOSMsg&);
#endif

 private: // Configuration variables
  std::string m_switch_mode;
  double m_previous_time;
  double m_previous_error;
  double m_steady_error;
  double m_kp;
  double m_ki;
  double m_kd;
  double m_upper_speed;
  double m_lower_speed;

 private: // State variables
  double m_nav_heading;
  double m_osx;
  double m_osy;
  double m_desired_x;
  double m_desired_y;
  double m_desired_heading;
  bool   m_active;
  bool   m_keep_heading;
  double m_arrival_radius;
  double m_tolerance_radius; 
  double m_distance;  //for report
  double m_gps_heading; //for report
  Mode   mode;

  // 10.21experiment feature
  double m_pre_heading;

  // 11.25experiment
  std::string m_sim;
};

#endif
