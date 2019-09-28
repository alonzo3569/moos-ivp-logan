/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: StoreSound.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "StoreSound.h"
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <sstream>
using namespace std;

//---------------------------------------------------------
// Constructor

StoreSound::StoreSound()
{
  m_start_record = 0;
  m_start_check = 0;
  m_dir_flag = 1;
  m_path = "./";
  m_filename = "";
  m_recordDevice = "default";
  m_sampleRate = 48000;
  m_frames = 480;
  m_send_size = 480;
  m_period_size = 0;
  m_channels = 1;
  m_bits = 16;
  m_recordTime = 59*60+50;
  m_passTime = 10;
  m_pass_record = 0;
  m_start_record = 0;
  m_loops = 0;
  m_dir = 0;
  m_repeat = "true";
  m_tem_buffer.clear();
}

//---------------------------------------------------------
// Destructor

StoreSound::~StoreSound()
{
  snd_pcm_drain(m_handle);
  snd_pcm_close(m_handle);
  free(m_period_buffer);
  if(m_fp)
    fclose(m_fp);
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool StoreSound::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    if(key == "START_RECORD"){
      string sval = msg.GetString();
      if(sval == "true"){
        makeDir();
        m_start_record = 1;
        m_start_check = 0;
        m_loops = m_recordTime*m_sampleRate/m_frames;
        m_filename = m_path + "/" + fileTime('f'); //filename = start_record_time.bin
        m_fp = fopen(m_filename.c_str(), "wb");
        if(m_fp){
          reportEvent("Create file sucess: " + m_filename);
          stringstream tt;
          tt << m_recordTime << " sec";
          reportEvent("Recording for " + tt.str());
        }
          else
          reportConfigWarning("Create file fail: " + m_filename);
      }
    }else if(key == "SET_PARAMS"){
      string sval = msg.GetString();
      if(sval == "true")
        setRecordParams();
    }else if(key == "START_CHECK"){
      string sval = msg.GetString();
      if(sval == "true"){
        m_start_check = 1;
        m_start_record = 0;
//        m_loops = 1;
        reportEvent("Checking...");
      }
    }
     else 
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool StoreSound::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool StoreSound::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  if(m_start_check == 1 && m_start_record == 0){
    Capture();
    SendData();
  }else if(m_start_check == 0 && m_start_record == 1){
    Capture();
    SendData();
 //   Record();
    if(m_repeat == "false" && m_loops <= 0){
      fclose(m_fp);
      m_start_record = 0;
      m_start_check = 1;
      reportEvent("Move to next point...");
      Notify("START_CHECK", "false");
    }else if(m_repeat == "true")
      Repeat();
  }

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool StoreSound::OnStartUp()/*{{{*/
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "PATH") {
      m_path = value;
      handled = true;
    }
    else if(param == "SAMPLE_RATE") {
      m_sampleRate = atol(value.c_str());
      handled = true;
    }
    else if(param == "CHANNELS"){
      m_channels = atoi(value.c_str());
      handled = true;
    }
    else if(param == "RECORD_DEVICE"){
      m_recordDevice = value;
      handled = true;
    }
    else if(param == "RECORD_TIME"){
      m_recordTime = atol(value.c_str());
      handled = true;
    }
    else if(param == "PASS_TIME"){
      m_passTime = atoi(value.c_str());
      handled = true;
    }
    else if(param == "FRAMES"){
      m_frames = atoi(value.c_str());
      handled = true;
    }
    else if(param == "REPEAT"){
      m_repeat = value;
      handled = true;
    }else if(param == "BITS"){
      m_bits = atoi(value.c_str());
      handled = true;
    }else if(param == "SEND_SIZE"){
      m_send_size = atoi(value.c_str());
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void StoreSound::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("START_RECORD", 0);
  Register("START_CHECK", 0);
  Register("SET_PARAMS", 0);
}/*}}}*/


//------------------------------------------------------------/*{{{*/
// Procedure: buildReport()

bool StoreSound::buildReport() 
{
  m_msgs << "============================================= \n";
  m_msgs << "File Folder Path :  " << m_path           << "\n";
  m_msgs << "Record Device    :  " << m_recordDevice   << "\n";
  m_msgs << "Sample Rate      :  " << m_sampleRate     << "\n";
  m_msgs << "Bits             :  " << m_bits           << "\n";
  m_msgs << "Channels         :  " << m_channels       << "\n";
  m_msgs << "Frames           :  " << m_frames         << "\n";
  m_msgs << "Send Size        :  " << m_send_size      << "\n";
  m_msgs << "Record File Name :  " << m_filename       << "\n";
  m_msgs << "Record Time      :  " << m_recordTime     << "\n";
  m_msgs << "Repeat           :  " << m_repeat         << "\n";
  m_msgs << "Pass Time        :  " << m_passTime       << "\n";
  m_msgs << "============================================= \n";

  return(true);
}
//-------------------------------------------------------------
// Procedure: fileTime()/*}}}*/

string StoreSound::fileTime(char kind)/*{{{*/
{
  time_t timep;
  struct tm *p;
  int year, mon, day, hour, min, sec;
  stringstream file_time;
  stringstream dir_date;
  time(&timep);
  p = localtime(&timep);
  year = 1900+p->tm_year;
  mon = 1+p->tm_mon;
  day = p->tm_mday;
  hour = p->tm_hour;
  min = p->tm_min;
  sec = p->tm_sec;
  if(kind == 'f'){
    file_time << year << "_" << mon << "_" << day << "_" << hour << "_" << min << "_" << sec << ".bin";
    return(file_time.str());
  }else if(kind == 'd'){
    dir_date << year << "_" << mon << "_" << day;
    return(dir_date.str());
  }
}

void StoreSound::makeDir(){
  if(m_dir_flag == 1){
    m_path = m_path + "record";
    if(access(m_path.c_str(), 0) == -1){
      mkdir(m_path.c_str(), 0777);
      reportEvent("Create folder sucess: " + m_path);
    }
    m_path = m_path + "/" + fileTime('d');
    if(access(m_path.c_str(), 0) == -1){
      mkdir(m_path.c_str(), 0777);
      reportEvent("Create folder sucess: " + m_path);
    }
    m_dir_flag = 0;
  }
}/*}}}*/

void StoreSound::setRecordParams(){/*{{{*/
  int rc;
  /* Open PCM device for recording. */
  rc = snd_pcm_open(&m_handle, m_recordDevice.c_str(), SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0){
    reportRunWarning("Can't open record device: " + m_recordDevice);
  }else
    reportEvent("Open device sucess: " + m_recordDevice);
  
  /* Allcoate a hardware params object. */ 
  snd_pcm_hw_params_alloca(&m_params);

  /* Fill it in with default values. */ 
  snd_pcm_hw_params_any(m_handle, m_params);

  /* Set the params to device. */
  /*Interleaved mode. */
  snd_pcm_hw_params_set_access(m_handle, m_params, SND_PCM_ACCESS_RW_INTERLEAVED);
  
  /* Signed 16-bit little-endian format. */
  switch(m_bits){
    case 16:
      snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S16_LE);
      break;
    case 24:
      snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S24_LE);
      break;
    case 32:
      snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S32_LE);
      break;
  }
  /* Set channel. */
  snd_pcm_hw_params_set_channels(m_handle, m_params, m_channels);

  /* Set sample rate. */
  snd_pcm_hw_params_set_rate_near(m_handle, m_params, &m_sampleRate, &m_dir);
  
  /* Set period size to frames. */
  snd_pcm_hw_params_set_period_size_near(m_handle, m_params, &m_frames, &m_dir);

  /* Write the params to the driver. */
  rc = snd_pcm_hw_params(m_handle, m_params);
  if(rc < 0)
    reportRunWarning("Unable to set hw params. ");
  else
    reportEvent("Set params sucess. ");
    
  /* Decide the period size and buffer. */
  snd_pcm_hw_params_get_period_size(m_params, &m_frames, &m_dir);
  m_period_size = m_frames*m_bits*m_channels/8; //16bits // units is byte.
  m_period_buffer = (char *) malloc(m_period_size);
}/*}}}*/


void StoreSound::Capture()
{
  int rc;
//  if(m_loops > 0){
    m_loops--;
    rc = snd_pcm_readi(m_handle, m_period_buffer, m_frames);
    if(rc == -EPIPE){
      reportConfigWarning("Overrun occurred.");
      snd_pcm_prepare(m_handle);
    }else if(rc < 0){
      string error = snd_strerror(rc);
      reportRunWarning("Error from read: " + error);
    }else if(rc != (int)m_frames){
      reportConfigWarning("Short read, read wrong frames: " + rc);
    }

    if(m_pass_record == 0 && m_start_record == 1){
      fwrite(m_period_buffer, 1, m_period_size, m_fp);
    }

    switch(m_bits){
      case 16:
        for(int i=0; i<m_period_size-3; i=i+4){
          int sum = (unsigned char)m_period_buffer[i]+256*(unsigned char)m_period_buffer[i+1];
          m_tem_buffer.push_back(sum);
        }
        break;

      case 24:
        for(int i=0; i<m_period_size-5; i=i+6){
          int sum = (unsigned char)m_period_buffer[i]+256*(unsigned char)m_period_buffer[i+1]+256*256*(unsigned char)m_period_buffer[i+2];
          m_tem_buffer.push_back(sum);
        }
        break;

      case 32:
        for(int i=0; i<m_period_size-7; i=i+8){
          int sum = (unsigned char)m_period_buffer[i]+256*(unsigned char)m_period_buffer[i+1]+256*256*(unsigned char)m_period_buffer[i+2]+256*256*256*m_period_buffer[i+3];
          m_tem_buffer.push_back(sum);
        }
        break;
    }
//  }
}

void StoreSound::SendData()
{
  if(!m_tem_buffer.empty()){
    stringstream msg;
    if(m_tem_buffer.size() >= m_send_size){
      for(int i=0; i<m_send_size; i++){
        int value = m_tem_buffer.front();
        m_tem_buffer.pop_front();
        msg << value << ",";
      }
    }else{
      for(int i=0; i<m_tem_buffer.size(); i++){
        int value = m_tem_buffer.front();
        m_tem_buffer.pop_front();
        msg << value << ",";
      }
    }
  Notify("Sound_Data", msg.str());
  }
}


void StoreSound::Repeat()
{
  if(m_loops <= 0 && m_start_record == 1 && m_pass_record == 0){
    fclose(m_fp);
  //  m_start_record = 0;
    m_pass_record = 1;
    m_loops = m_passTime*m_sampleRate/m_frames;
    stringstream tt;
    tt << m_passTime << " sec";
    reportEvent("Waiting for " + tt.str());
    Notify("START_CHECK", "false");
    
  }else if(m_loops <= 0 && m_start_record == 1 && m_pass_record == 1){
    m_pass_record = 0;
  //  m_start_record = 1;
    m_loops = m_recordTime*m_sampleRate/m_frames;
    m_filename = m_path + "/" + fileTime('f'); //filename = start_record_time.bin
    m_fp = fopen(m_filename.c_str(), "wb"); //Open file
    if(m_fp){
      reportEvent("Create file sucess: " + m_filename);
      stringstream tt;
      tt << m_recordTime << " sec";
      reportEvent("Recording for " + tt.str());
    }
    else
      reportConfigWarning("Create file fail: " + m_filename);
  }
}
