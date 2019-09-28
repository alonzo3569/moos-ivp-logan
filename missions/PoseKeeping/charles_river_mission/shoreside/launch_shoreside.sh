#!/bin/bash -e

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
SHORE_IP="localhost"
SHORE_LISTEN="9300"

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        echo "  --timewarp=, -t= , set up TimeWarp. (Default is $TIME_WARP)"
        echo "  --shore-port=    , set up a shore listening port. (Default is $SHORE_LISTEN)"
        echo "  --shore-ip=      , set up a shore listening IP. (Default is $SHORE_IP)"
        echo "  --just_make, -j    "
        echo "  --help, -h         "
	exit 0;

    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI:0:11}" = "--timewarp=" ] ; then
        TIME_WARP="${ARGI#--timewarp=*}"
    elif [ "${ARGI:0:3}" = "-t=" ] ; then
        TIME_WARP="${ARGI#-t=*}"
    elif [ "${ARGI}" = "--just_make" -o "${ARGI}" = "-j" ] ; then
        JUST_MAKE="yes"
    elif [ "${ARGI:0:11}" = "--shore-ip=" ] ; then
        SHORE_IP="${ARGI#--shore-ip=*}"
    elif [ "${ARGI:0:13}" = "--shore-port=" ] ; then
        SHORE_LISTEN=${ARGI#--shore-port=*}

    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 2: Create the Shoreside MOOS file
#-------------------------------------------------------
nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
       SNAME="shoreside"  SHARE_LISTEN=$SHORE_LISTEN  SPORT="9000"\
       SHORE_IP=$SHORE_IP

if [ ! -e targ_shoreside.moos ]; then echo "no targ_shoreside.moos"; exit 1; fi

#-------------------------------------------------------
#  Part 3: Possibly exit now if we're just building targ files
#-------------------------------------------------------

if [ ${JUST_MAKE} = "yes" ] ; then
    echo "Shoreside targ files built. Nothing launched."
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the Shoreside
#-------------------------------------------------------

echo "Launching $SNAME MOOS Community (WARP=$TIME_WARP)"
pAntler targ_shoreside.moos >& /dev/null &
echo "Done Launching Shoreside "

uMAC targ_shoreside.moos

sleep .2 # Give them a chance to exit with grace
echo "Killing all processes ... "
kill -- -$$
echo "Done killing processes.   "


