7#!/bin/bash 
#-----------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-----------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-----------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-----------------------------------------------------------
VNAME1="GILDA"  
VNAME2="HENRY"  
START_POS1="0,0"    
START_POS2="80,0"   

CIRCLE_X1="150" ##"-25"
CIRCLE_Y1="-50" ##"-100"
SINE_X1="-75"   #"100"
SINE_Y1="-100"  #"-50"
#CIRCLE_SINE_V2="true"
#CIRCLE_SINE_V1="false"
#SINE_V1="true"

CIRCLE_X2="-25" ##
CIRCLE_Y2="-100" ##
SINE_X2="100" ##"-75"
SINE_Y2="-50" ##"-100"
#CIRCLE_SINE_V2="true"
#SINE_V2="false"


SHORE_LISTEN="9300"

nsplug meta_vehicle.moos targ_$VNAME1.moos -f WARP=$TIME_WARP \
    VNAME=$VNAME1      START_POS=$START_POS1                  \
    VPORT="9001"       SHARE_LISTEN="9301"                    \
    VTYPE="kayak"      SHORE_LISTEN=$SHORE_LISTEN             \
    CIRCLE_X=$CIRCLE_X1 CIRCLE_Y=$CIRCLE_Y1\
    SINE_X=$SINE_X1 SINE_Y=$SINE_Y1\
    #CIRCLE_SINE=$CIRCLE_SINE_V1 SINE=$SINE_V1

nsplug meta_vehicle.moos targ_$VNAME2.moos -f WARP=$TIME_WARP \
    VNAME=$VNAME2      START_POS=$START_POS2                  \
    VPORT="9002"       SHARE_LISTEN="9302"                    \
    VTYPE="kayak"      SHORE_LISTEN=$SHORE_LISTEN             \
    CIRCLE_X=$CIRCLE_X2 CIRCLE_Y=$CIRCLE_Y2\
    SINE_X=$SINE_X2 SINE_Y=$SINE_Y2\
    #CIRCLE_SINE=$CIRCLE_SINE_V2 SINE=$SINE_V2

nsplug meta_vehicle.bhv targ_$VNAME1.bhv -f VNAME=$VNAME1     \
    START_POS=$START_POS1 LOITER_POS=$LOITER_POS1\
    CIRCLE_X=$CIRCLE_X1 CIRCLE_Y=$CIRCLE_Y1\
    SINE_X=$SINE_X1 SINE_Y=$SINE_Y1\
    #CIRCLE_SINE=$CIRCLE_SINE_V1 SINE=$SINE_V1        

nsplug meta_vehicle.bhv targ_$VNAME2.bhv -f VNAME=$VNAME2     \
    START_POS=$START_POS2 LOITER_POS=$LOITER_POS2\
    CIRCLE_X=$CIRCLE_X2 CIRCLE_Y=$CIRCLE_Y2\
    SINE_X=$SINE_X2 SINE_Y=$SINE_Y2\
    #CIRCLE_SINE=$CIRCLE_SINE_V2 SINE=$SINE_V2       

nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
   VNAME="shoreside"  SHARE_LISTEN=$SHORE_LISTEN  VPORT="9000"     
        
if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-----------------------------------------------------------
#  Part 3: Launch the processes
#-----------------------------------------------------------
printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME1.moos >& /dev/null &
printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME2.moos >& /dev/null &
printf "Done \n"

#-----------------------------------------------------------
#  Part 4: Launch uMAC and kill everything upon exiting uMAC
#-----------------------------------------------------------
uMAC targ_shoreside.moos
printf "Killing all processes ... \n"
kill %1 %2 %3 
printf "Done killing processes.   \n"
