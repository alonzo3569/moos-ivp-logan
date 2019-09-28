/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: NTU, Taipei                                          */
/*   FILE: StoreSoundX_Info.cpp                                 */
/*   DATE: Jan 23th, 2019                                       */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "StoreSoundX_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  Need to install libasound2-dev first.                         ");
  blk("  The pStoreSoundX application is used for recording sound.      ");
  blk("  It will create a folder at the pointing path which is set by  ");
  blk("  user. The record file will be saved as a .bin file in the folder");
  blk("  The default path is ~/moos-ivp-cthung/.                       ");
  blk("------------------------------------                            ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pStoreSoundX file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pStoreSoundX with the given process name         ");
  blk("      rather than pStoreSoundX.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pStoreSoundX.        ");
  blk("                                                                ");
  blk("Note: If argv[2] does not otherwise match a known option,       ");
  blk("      then it will be interpreted as a run alias. This is       ");
  blk("      to support pAntler launching conventions.                 ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showExampleConfigAndExit

void showExampleConfigAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pStoreSoundX Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pStoreSoundX                              ");
  blk("{                                                               ");
  blk("  AppTick   = 10                                                ");
  blk("  CommsTick = 10                                                ");
  blk("  PATH = /home/user/wherever                                    ");
  blk("  SAMPLE_RATE = 48000                                           ");
  blk("  SEND_SIZE = 4800                                              ");
  blk("  CHANNELS = 2                                                  ");
  blk("  RECORD_DEVICE = hw:1,0                                        ");
  blk("  FRAMES = 4800                                                 ");
  blk("  BITS = 16                                                     ");
  blk("  RECORD_TIME = 3                                               ");
  blk("  PASS_TIME = 5                                                 ");
  blk("  REPEAT = false                                                ");
  blk("  SAVE_FILE = true                                              ");
  blk("}                                                               ");
  blk("                                                                ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pStoreSoundX INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("  START_RECORD  = true                                           ");
  blk("  SET_PARAMS = true                                             ");
  blk("  START_CHECK = true                                                              ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  Publications are determined by the node message content.      ");
  blk("  SOUND_VOLTAGE_DATA_CH_ONE = string type                       ");
  blk("  SOUND_VOLTAGE_DATA_CH_TWO = string type                       ");
  blk("  START_CHECK = false                                           ");
  blk("  RECORD_FRAMES = 9600, \"string\" type                         ");
  blk("                                                                ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pStoreSoundX", "gpl");
  exit(0);
}

