/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: StoreSound.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef StoreSound_HEADER
#define StoreSound_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <deque>

class StoreSound : public AppCastingMOOSApp
{
 public:
   StoreSound();
   ~StoreSound();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 protected:
   std::string fileTime(char);
   void makeDir();
   void setRecordParams();
   void SendData();
   void Capture();
   void Repeat();

 private: // Configuration variables
   bool m_start_record;
   bool m_start_check;
   bool m_pass_record;
   bool m_dir_flag;
   std::string m_path;
   std::string m_filename;
   std::string m_repeat;
   std::deque<int> m_tem_buffer;
   FILE *m_fp;

 private: // Record params
   std::string m_recordDevice;
   snd_pcm_t *m_handle;
   snd_pcm_hw_params_t *m_params;
   snd_pcm_uframes_t m_frames;
   unsigned int m_sampleRate;
   unsigned int m_channels;
   unsigned int m_recordTime;
   unsigned int m_passTime;
   unsigned int m_loops;
   int m_dir;
   int m_period_size;
   int m_bits;
   int m_send_size;
   char *m_period_buffer;
   
};

#endif 
