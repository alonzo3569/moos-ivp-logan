/*****************************************************************/
/*    NAME: Michael Benjamin                                     */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: HazardMgr.h                                          */
/*    DATE: Oct 26th 2012                                        */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#ifndef UFLD_HAZARD_MGR_HEADER
#define UFLD_HAZARD_MGR_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYHazardSet.h"
#include "XYPolygon.h"
using namespace std;

class HazardMgr : public AppCastingMOOSApp
{
 public:
   HazardMgr();
   ~HazardMgr() {}

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport(); //change

 protected: // Registration, Configuration, Mail handling utils
   void registerVariables();
   bool handleMailSensorConfigAck(std::string);
   bool handleMailSensorOptionsSummary(std::string) {return(true);}
   bool handleMailDetectionReport(std::string);
   bool handleMailHazardReport(std::string) {return(true);}
   string handleMailReportRequest();//*change
   void handleMailMissionParams(std::string);
   //^_^
   void buildNodeMessage(string a);
   string stringtrasform(string b);
   string reversetransform(string c);
   string removename(string d);

 protected: 
   void postSensorConfigRequest();
   void postSensorInfoRequest();
   void postHazardSetReport();
   
 private: // Configuration variables
   double      m_swath_width_desired;
   double      m_pd_desired;
   std::string m_report_name;

 private: // State variables
   bool   m_sensor_config_requested;
   bool   m_sensor_config_set;

   unsigned int m_sensor_config_reqs;
   unsigned int m_sensor_config_acks;

   unsigned int m_sensor_report_reqs;
   unsigned int m_detection_reports;

   unsigned int m_summary_reports;

   double m_swath_width_granted;
   double m_pd_granted;

   XYHazardSet m_hazard_set;
   XYPolygon   m_search_region;
   
   double      m_transit_path_width;
   //^_^
   string m_hazardreport_jake;
   string m_hazardreport_kasper;
};

#endif 








