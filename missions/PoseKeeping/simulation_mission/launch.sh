#!/bin/bash -e

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
POSITION=50,-50
HEADING=45
COMMUNITY="logan"


for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        echo " --timewarp=,-t=  , set up TimeWarp. (Default is $TIME_WARP)"
        echo " --position=,-p=  , set up destionation. (Default is $POSITION)"
        echo " --heading= ,-h=  , set up heading. (Default is $HEADING)"
	printf "\n"
	echo "For example: $0 $TIME_WARP -p=$POSITION -h=$HEADING"
        echo "             $0 -t=$TIME_WARP -p=$POSITION -h=$HEADING"
	exit 0;

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

    else 
	printf "Bad Argument: %s \n" $ARGI
	printf "Usage: %s %s -p=%s -h=%s\n" $0 $TIME_WARP $POSITION $HEADING
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 2: Create the Shoreside MOOS file
#-------------------------------------------------------
nsplug logan.moos targ_$COMMUNITY.moos -f WARP=$TIME_WARP \
       POS=$POSITION  HEADING=$HEADING COMMUNITY=$COMMUNITY

if [ ! -e targ_$COMMUNITY.moos ]; then echo "no targ_$COMMUNITY.moos"; exit 1; fi


#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
#printf "Launching the %s MOOS Community (WARP=%s) \n"  $COMMUNITY $TIME_WARP
#pAntler targ_$COMMUNITY.moos >& /dev/null &
printf "Launching $COMMUNITY MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_$COMMUNITY.moos >& /dev/null &

uMAC targ_$COMMUNITY.moos

printf "Killing all processes ... \n"
kill -- -$$
printf "Done killing processes.   \n"





