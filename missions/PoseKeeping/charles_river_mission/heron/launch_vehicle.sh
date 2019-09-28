#!/bin/bash -e

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
SHORE_IP=192.168.1.214
SHORE_LISTEN="9300"
TIME_WARP=1
POSITION=50,-50
HEADING=45
VNAME=""
JUST_BUILD="no"

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "\n"
        echo "  --evan,       -e  : Evan vehicle."
        echo "  --felix,      -f  : Felix vehicle."
        echo "  --gus,        -g  : Gus vehicle."
        echo "  --hal,        -H  : Hal vehicle."
        echo "  --ida,        -i  : Ida vehicle."
        echo "  --jing,       -J  : Jing vehicle."
        echo "  --kirk,       -k  : Kirk vehicle."
        echo "  --sim,        -s  : Simulation mode."
        echo "  --just_build, -j  : Build file but don't launch"
        echo "  --timewarp=,  -t= , set up TimeWarp. (Default is $TIME_WARP)"
        echo "  --position=,  -p= , set up destionation. (Default is $POSITION)"
        echo "  --heading= ,  -h= , set up heading. (Default is $HEADING)"
	printf "\n"
	echo "For example: $0 -e $TIME_WARP -p=$POSITION -h=$HEADING"
        echo "             $0 -kirk -t=$TIME_WARP -p=$POSITION -h=$HEADING"
	exit 0;

    elif [ "${ARGI}" = "--evan" -o "${ARGI}" = "-e" ] ; then
        M200_IP=192.168.5.1 #evan
        VNAME="evan"
        VPORT="9005"
        VEHICLE_LISTEN="9305"
    elif [ "${ARGI}" = "--felix" -o "${ARGI}" = "-f" ] ; then
        M200_IP=192.168.6.1 #felix
        VNAME="felix"
        VPORT="9006"
        VEHICLE_LISTEN="9306"
    elif [ "${ARGI}" = "--gus" -o "${ARGI}" = "-g" ] ; then
        M200_IP=192.168.7.1 #gus
        VNAME="gus"
        VPORT="9007"
        VEHICLE_LISTEN="9307"
    elif [ "${ARGI}" = "--hal" -o "${ARGI}" = "-H" ] ; then
        M200_IP=192.168.8.1 #hal
        VNAME="hal"
        VPORT="9008"
        VEHICLE_LISTEN="9308"
    elif [ "${ARGI}" = "--ida" -o "${ARGI}" = "-i" ] ; then
        M200_IP=192.168.9.1 #ida
        VNAME="ida"
        VPORT="9009"
        VEHICLE_LISTEN="9309"
    elif [ "${ARGI}" = "--jing" -o "${ARGI}" = "-J" ] ; then
        M200_IP=192.168.10.1 #jing
        VNAME="jing"
        VPORT="9010"
        VEHICLE_LISTEN="9310"
    elif [ "${ARGI}" = "--kirk" -o "${ARGI}" = "-k" ] ; then
        M200_IP=192.168.11.1 #kirk
        VNAME="kirk"
        VPORT="9011"
        VEHICLE_LISTEN="9311"

    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI:0:11}" = "--timewarp=" ] ; then
        TIME_WARP="${ARGI#--timewarp=*}"
    elif [ "${ARGI:0:3}" = "-t=" ] ; then
        TIME_WARP="${ARGI#-t=*}"

    elif [ "${ARGI:0:11}" = "--position=" ] ; then
        POSITION="${ARGI#--timewarp=*}"
    elif [ "${ARGI:0:3}" = "-p=" ] ; then
        POSITION="${ARGI#-p=*}"


    elif [ "${ARGI:0:10}" = "--heading=" ] ; then
        HEADING="${ARGI#--heading=*}"
    elif [ "${ARGI:0:3}" = "-h=" ] ; then
        HEADING="${ARGI#-h=*}"

    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-J" ] ; then
        JUST_BUILD="yes"
        echo "Just building files; no vehicle launch."
    elif [ "${ARGI}" = "--sim" -o "${ARGI}" = "-s" ] ; then
        SIM="SIM"
        echo "Simulation mode ON."

    else 
	printf "Bad Argument: %s \n" $ARGI
	printf "Please use -h for help."
	exit 0
    fi
done
#-------------------------------------------------------
#  Part 2: Handle Ill-formed command-line arguments
#-------------------------------------------------------

if [ -z $VNAME ]; then
    echo "No vehicle has been selected..."
    echo "Exiting."
    exit 2
fi

#-------------------------------------------------------
#  Part 3: Create the .moos files.
#-------------------------------------------------------

nsplug meta_vehicle.moos targ_${VNAME}.moos -f \
    POS=$POSITION                \
    HEADING=$HEADING             \
    VNAME=$VNAME                 \
    VPORT=$VPORT                 \
    WARP=$TIME_WARP              \
    V_LISTEN=$VEHICLE_LISTEN     \
    SHORE_LISTEN=$SHORE_LISTEN   \
    SHORE_IP=$SHORE_IP           \
    M200_IP=$M200_IP             \
    $SIM


if [ ${JUST_BUILD} = "yes" ] ; then
    echo "Files assembled; vehicle not launched; exiting per request."
    exit 0
fi

if [ ! -e targ_$VNAME.moos ]; then echo "no targ_$VNAME.moos"; exit 1; fi


#-------------------------------------------------------
#  Part 4: Launch the processes
#-------------------------------------------------------

echo "Launching $VNAME MOOS Community "
pAntler targ_${VNAME}.moos >& /dev/null &
uMAC targ_${VNAME}.moos

echo "Killing all processes ..."
kill -- -$$
echo "Done killing processes."

