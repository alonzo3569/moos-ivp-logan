/****************************************************************/
/*   NAME: Logan                                             */
/*   ORGN: MIT                                     */
/*   FILE: PoseKeepingX_Info.cpp                               */
/*   DATE: 2019/11/15                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "PoseKeepingX_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pPoseKeepingX application is used for holding Heron's     ");
  blk("  position and heading as much as possible without using Helm.  ");
  blk("  This is a capability suitable for multi-vehicle testing.      ");
  blk("  With pPoseKeeping application, we can deploy N herons out to  ");
  blk("  station-keeping points just off the dock, putting them into a ");
  blk("  pose-keeping mode (Helm not active), deploying the mission    ");
  blk("  which would halt the pose-keeping mode.                       ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pPoseKeepingX file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pPoseKeepingX with the given process name         ");
  blk("      rather than pPoseKeepingX.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pPoseKeepingX.        ");
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
  blu("pPoseKeepingX Example MOOS Configuration                        ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pPoseKeepingX                                   ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("  position  = 20,-15                                            ");
  blk("  heading   = 180                                               ");
  blk("  Kp        = 1.2                                               ");
  blk("  Kd        = 0.0                                               ");
  blk("  Ki        = 0.0                                               ");
  blk("  tolerance_radius = 5                                          ");
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
  blu("pPoseKeepingX INTERFACE                                         ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("  NAV_HEADING_CPNVG              = [0,359]                      ");
  blk("  NAV_HEADING                    = [0,359]                      ");
  blk("  NAV_X                          = [-inf/+inf]                  ");
  blk("  NAV_Y                          = [-inf/+inf]                  ");
  blk("  THRUST_MODE_DIFFERENTIAL       = [true/false]                 ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  DESIRED_THRUST_L   		= [-100,100]                   ");
  blk("  DESIRED_THRUST_R   		= [-100,100]                   ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pPoseKeepingX", "mit");
  exit(0);
}

