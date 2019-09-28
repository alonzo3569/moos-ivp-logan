/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: Recordtest.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Recordtest_HEADER
#define Recordtest_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <deque>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

using namespace std;

class Recordtest : public AppCastingMOOSApp
{
 public:
   Recordtest();
   ~Recordtest();

   //record params:
   snd_pcm_t *m_handle;
   snd_pcm_hw_params_t *m_params;
   string m_device;
   int m_bits;
   unsigned int m_channels;
   unsigned int m_sampleRate;
   int m_dir;
   snd_pcm_uframes_t m_frames;
   int m_period_size;
   char *m_period_buffer;
   std::deque<int> m_tem_buffer;
   deque<double> m_volt_buffer;

   //program params
   int m_start_record;
   int i;
   FILE *m_fp_save1;
   //std::string m_recordDevice;
   //unsigned int m_recordTime;
   //unsigned int m_passTime;
   //unsigned int m_loops;
   //int m_send_size;


 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();

 protected:
   void RegisterVariables();
   // Iterate function
   void setRecordParams();
   void capture();
   void NotifyVolt();

 private: // Configuration variables

 private: // State variables
};

#endif 
