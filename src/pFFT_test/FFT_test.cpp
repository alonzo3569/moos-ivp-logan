/************************************************************/
/*    NAME: alonzo                                              */
/*    ORGN: MIT                                             */
/*    FILE: FFT_test.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "FFT_test.h"

#include <fftw3.h>

using namespace std;

//---------------------------------------------------------
// Constructor

FFT_test::FFT_test()
{
	m_volt_buffer.clear();
	m_start_calculate_fft = 0;
}

//---------------------------------------------------------
// Destructor

FFT_test::~FFT_test()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool FFT_test::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     if(key == "MSG")
     {
    	string sval = msg.GetString();
	VoltToVector(sval);
	Notify("TIME_RECIEVED",MOOSTime());
     }

     if(key == "START_CALCULATE_FFT")
     {
	m_start_calculate_fft = 1;
	m_volt_buffer.clear();// clear accumulated volts
     }

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }//for end
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool FFT_test::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool FFT_test::Iterate()
{
  AppCastingMOOSApp::Iterate();

  // Do your thing here!
  if(m_start_calculate_fft == 1)
  {
	cout << "Size of m_volt_buffer = " << m_volt_buffer.size() << endl;// delay
	Notify("SIZE_AFTER",m_volt_buffer.size());
	Notify("ITERATE_CLEAR_BUFFER",MOOSTime());
	m_volt_buffer.clear();
  }

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool FFT_test::OnStartUp()
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
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
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

void FFT_test::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("MSG", 0);
  Register("START_CALCULATE_FFT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

//bool FFT_test::buildReport() 
//{
  //m_msgs << "============================================ \n";
  //m_msgs << "File:                                        \n";
  //m_msgs << "============================================ \n";

  //ACTable actab(4);
  //actab << "Alpha | Bravo | Charlie | Delta";
  //actab.addHeaderLines();
  //actab << "one" << "two" << "three" << "four";
  //m_msgs << actab.getFormattedString();

  //return(true);
//}
//------------------------------------------------------------
// Procedure: VoltToVector()

void FFT_test::VoltToVector(string input)
{
    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++)
    {
      double volt = atof(voltages[i].c_str());
      m_volt_buffer.push_back(volt);
    }
}
//------------------------------------------------------------
// Procedure: AL_FFT()
void FFT_test::AL_FFT(int window_type,unsigned int N)
{
	fft_p plan;
	fft complex *in 
//in = new double[N];

}


