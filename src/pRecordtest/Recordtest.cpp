/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: Recordtest.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Recordtest.h"
#include <alsa/asoundlib.h>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

//#include "ACTable.h"
//#include <time.h>
#include <stdio.h> //enable ato lib and fp lib
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <string>
//#include <sstream>

using namespace std;

//---------------------------------------------------------
// Constructor

Recordtest::Recordtest()
{

   //record params:
  m_device = "default";
  m_bits = 32;
  m_channels = 2;
  m_sampleRate = 48000;
  m_frames = 4800;
  m_dir = 0;
  m_period_size = 0;
  //program params
  m_start_record = 0;
  i = 0;
  m_volt_buffer.clear();
  m_tem_buffer.clear();
}

//---------------------------------------------------------
// Destructor

Recordtest::~Recordtest()
{
  
  free(m_period_buffer);
  if(m_fp_save1)
    fclose(m_fp_save1);
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Recordtest::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  AppCastingMOOSApp::OnNewMail(NewMail);
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    string key = msg.GetKey();
    if(key == "START_RECORD")
    {
	m_start_record = 1;
    	string sval = msg.GetString();
    	if(sval == "true")
    	{	
		setRecordParams();
		//m_fp_save1 = fopen("ch1.txt","w");
    	}
    }

   }//for loop end
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Recordtest::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Recordtest::Iterate()
{
  if(m_start_record == 1)
  {
  	capture();
	NotifyVolt();
  }
  Notify("TIC",MOOSTime());
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Recordtest::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();    
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      string orig  = *p;
      
      bool handled = false;
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }

      /* .moos params */
      else if(param == "DEVICE"){
        m_device = value;
        handled = true;
      }
      else if(param == "BITS"){
        m_bits = atoi(value.c_str());
        handled = true;
      }
      else if(param == "CHANNELS"){
        m_channels = atoi(value.c_str());
        handled = true;
      }
      else if(param == "SAMPLE_RATE") {
        m_sampleRate = atoi(value.c_str()); //why atol instead of atoi?
        handled = true;
      }
      else if(param == "FRAMES"){
        m_frames = atoi(value.c_str());
        handled = true;
      }
      if(!handled)
        reportUnhandledConfigWarning(orig);
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Recordtest::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("START_RECORD", 0);
  //Register("FOOBAR", 0);
  //Register("FOOBAR", 0);
}
//---------------------------------------------------------

void Recordtest::setRecordParams(){/*{{{*/
  
  int rc;
  /* Open PCM device for recording. */
  rc = snd_pcm_open(&m_handle, m_device.c_str(), SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0){
    cout <<  "Can't open record device " << endl;
  }else
    cout <<  "Open device success" << endl;

  /* Allcoate a hardware params object. */ 
  snd_pcm_hw_params_alloca(&m_params);
  cout <<  "1.allocate" << endl;

  /* Fill it in with default values. */ 
  snd_pcm_hw_params_any(m_handle, m_params);
  cout <<  "2.fill in" << endl;

  /* Set the params to device. */
  /*Interleaved mode. */
  snd_pcm_hw_params_set_access(m_handle, m_params, SND_PCM_ACCESS_RW_INTERLEAVED);
  cout <<  "3.set access" << endl;

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
  cout <<  "4.decide resolution" << endl;

  /* Set channel. */
  snd_pcm_hw_params_set_channels(m_handle, m_params, m_channels);
  cout <<  "5.set channels" << endl;

  /* Set sample rate. */
  snd_pcm_hw_params_set_rate_near(m_handle, m_params, &m_sampleRate, &m_dir);
  cout <<  "6.set sample rate" << endl;

  /* Set period size to frames. */
  snd_pcm_hw_params_set_period_size_near(m_handle, m_params, &m_frames, &m_dir);
  cout <<  "7.set period size" << endl;

  /* Write the params to the driver. */
  rc = snd_pcm_hw_params(m_handle, m_params);
  if(rc < 0)
  {
    cout <<  "unable to set params" << endl;
  }else{
    cout <<  "set_params() successfully finish" << endl;
  }
  /* Decide the period size and buffer. */
  snd_pcm_hw_params_get_period_size(m_params, &m_frames, &m_dir);
  cout <<  "8.get size of period" << endl;

  /* check m_frames */
  Notify("FRAMES_PER_PERIOD",m_frames);
  m_period_size = m_frames*m_bits*m_channels/8; //16bits // units is byte.
  m_period_buffer = (char *) malloc(m_period_size);
  cout << "setParams success" << endl;
}/*}}}*/
//------------------------------------------------------------/*{{{*/
// Procedure: buildReport()

bool Recordtest::buildReport() 
{
  m_msgs << "============================================= \n";
  //m_msgs << "File Folder Path :  " << m_path           << "\n";
  //m_msgs << "Record Device    :  " << m_recordDevice   << "\n";
  //m_msgs << "Sample Rate      :  " << m_sampleRate     << "\n";
  //m_msgs << "Bits             :  " << m_bits           << "\n";
  //m_msgs << "Channels         :  " << m_channels       << "\n";
  //m_msgs << "Frames           :  " << m_frames         << "\n";
  //m_msgs << "Send Size        :  " << m_send_size      << "\n";
  //m_msgs << "Record File Name :  " << m_filename       << "\n";
  //m_msgs << "Record Time      :  " << m_recordTime     << "\n";
  //m_msgs << "Repeat           :  " << m_repeat         << "\n";
  //m_msgs << "Pass Time        :  " << m_passTime       << "\n";
  m_msgs << "============================================= \n";

  return(true);
}/*}}}*/

//------------------------------------------------------------/*{{{*/
// Procedure: capture()

void Recordtest::capture()
{
  int rc;
//  if(m_loops > 0){
    //m_loops--;
    m_fp_save1 = fopen("ch1.txt","w");
    rc = snd_pcm_readi(m_handle, m_period_buffer, m_frames);
    if(rc == -EPIPE){
      cout << "Overrun occurred." << endl;
      snd_pcm_prepare(m_handle);
    }else if(rc < 0){
      string error = snd_strerror(rc);
      cout << "Error from reading" << endl;
    }else if(rc != (int)m_frames){
      cout << "Short read, read wrong frames" << endl;
    }
    //cout << "open success" << endl;
    //Notify("OPEN_SUCCESS","TRUE");
    
    double volt;
    switch(m_bits){
      case 16:
        for(int i=0; i<m_period_size-3; i=i+4){
          int sum = (unsigned char)m_period_buffer[i]+256*m_period_buffer[i+1];
	  //Notify("SUM",sum);//debug
          //m_tem_buffer.push_back(sum);
          if(sum <= 0){
            volt = (double)sum/32768.0;
          }
          else{
            volt = (double)sum/32767.0;
          }
	  //Notify("VOLT",volt);//debug
          //m_volt_buffer.push_back(volt);
	  fprintf(m_fp_save1,"%f %s",volt,"\n");
        }//for end
        break;

      case 24:
        for(int i=0; i<m_period_size-5; i=i+6){
          int sum = (unsigned char)m_period_buffer[i]+256*(unsigned char)m_period_buffer[i+1]+256*256*m_period_buffer[i+2];
	  //Notify("SUM",sum);//debug
          //m_tem_buffer.push_back(sum);
          if(sum <= 0){
            volt = (double)sum/8388608.0;
          }
          else{
            volt = (double)sum/8388607.0;
          }
	  //Notify("VOLT",volt);//debug
          //m_volt_buffer.push_back(volt);
	  fprintf(m_fp_save1,"%f %s",volt,"\n");
        }//for end
        break;

      case 32:
        for(int i=0; i<m_period_size-7; i=i+8){
          int sum = (unsigned char)m_period_buffer[i]+256*(unsigned char)m_period_buffer[i+1]+256*256*(unsigned char)m_period_buffer[i+2]+256*256*256*m_period_buffer[i+3];
	  //Notify("SUM",sum);//debug
          //m_tem_buffer.push_back(sum);
          if(sum <= 0){
            volt = (double)sum/2147483648.0;
          }
          else{
            volt = (double)sum/2147483647.0;
          }
	  //Notify("VOLT",volt);//debug
          m_volt_buffer.push_back(volt);
	  fprintf(m_fp_save1,"%lf\n",volt);
        }//for end
	fclose(m_fp_save1);
	//cout << "output success" << endl;
	//Notify("OUTPUT_SUCCESS","TRUE");
        break;
    }
//  }
}/*}}}*/

//-------------------------------------------------------------/*{{{*/
void Recordtest::NotifyVolt()
{
  if(!m_volt_buffer.empty())
  {
	stringstream msg;
	int volt_buffer_size = m_volt_buffer.size(); //bug!!! cautious!!
	Notify("SIZE_BEFORE",volt_buffer_size); //should be equal to frame size
	for(int i=0; i<volt_buffer_size;i++)
	{
		if(i == volt_buffer_size-1)
		{
			double value = m_volt_buffer.front(); //last one
			m_volt_buffer.pop_front();
			msg << value;

		}
		else
		{
			double value = m_volt_buffer.front();
			m_volt_buffer.pop_front();
			msg << value << ",";
		}

	}
	Notify("MSG",msg.str());
	Notify("TIME_SEND",MOOSTime());
  }
}/*}}}*/

//-------------------------------------------------------------/*{{{*/
