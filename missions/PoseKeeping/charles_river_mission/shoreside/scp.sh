#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Initial value
#-------------------------------------------------------
REMOTE_USR=student2680
REMOTE_IP=192.168.6.100 #192.168.6.100
DATE=11.25.2019

#-------------------------------------------------------
#  Part 2: Check for and handle command-line arguments
#-------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        echo " --usr=   ,-u=  , Remote device user name. (Default is $REMOTE_USR)"
        echo " --ip=    ,-i=  , Ip of remote device. (Default is $REMOTE_IP)"
        echo " --date=  ,-d=  , Date. (Default is $DATE)"
	printf "\n"
	echo "For example: $0 -u=$REMOTE_USR -ip=$REMOTE_IP"
	exit 0;

    #elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        #TIME_WARP=$ARGI
    elif [ "${ARGI:0:6}" = "--usr=" ] ; then
        REMOTE_USR="${ARGI#--usr=*}"
    elif [ "${ARGI:0:3}" = "-u=" ] ; then
        REMOTE_USR="${ARGI#-u=*}"

    elif [ "${ARGI:0:5}" = "--ip=" ] ; then
        REMOTE_IP="${ARGI#--ip=*}"
    elif [ "${ARGI:0:3}" = "-i=" ] ; then
        REMOTE_IP="${ARGI#-i=*}"

    elif [ "${ARGI:0:7}" = "--date=" ] ; then
        DATE="${ARGI#--d=*}"
    elif [ "${ARGI:0:3}" = "-d=" ] ; then
        DATE="${ARGI#-d=*}"
    else 
	printf "Bad Argument: %s \n" $ARGI
	printf "Usage: %s -u=%s -i=%s -d=%s\n" $0 $REMOTE_USR $REMOTE_IP $DATE
	exit 0
    fi
done
echo "--Configuration done"

#-------------------------------------------------------
#  Part 3: Make directory for saving files
#-------------------------------------------------------

echo "--Checking files"

if [ -d "/home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/" ]; then
    echo "[ 20%] Directory $DATE already exists."
else
    mkdir ~/moos-ivp-logan/data/PoseKeeping/$DATE/
    echo "[ 20%] Create Directory $DATE."
fi

if [ -d "/home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/" ]; then
    echo "[ 35%] Directory pPoseKeeping already exists."
else
    mkdir ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/
    echo "[ 35%] Create Directory pPoseKeeping."
fi

if [ -d "/home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/heron" ]; then
    echo "[ 50%] Directory heron already exists."
else
    mkdir ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/heron
    echo "[ 50%] Create Directory heron."
fi

if [ -d "/home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/shoreside" ]; then
    echo "[ 65%] Directory shoreside already exists."
else
    mkdir ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/shoreside
    echo "[ 65%] Create Directory shoreside."
fi

if [ -d "/home/ual/Desktop/pPoseKeeping_logfile/$DATE" ]; then
    echo "[ 70%] Desktop directory already exists."
else
    mkdir ~/Desktop/pPoseKeeping_logfile/$DATE
    echo "[ 70%] Create Directory shoreside."
fi


#-------------------------------------------------------
#  Part 4: Copy files to target directory
#-------------------------------------------------------

echo "[ 80%] Scp from remote device"
echo "================================================================================================"
scp -r $REMOTE_USR@$REMOTE_IP:/home/$REMOTE_USR/moos-ivp-logan/missions/PoseKeeping/charles_river_mission/heron/LOG_* /home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/heron/
echo "================================================================================================"
echo "[ 95%] Copy shoreside files to data directory"
cp -rp ~/moos-ivp-logan/missions/PoseKeeping/charles_river_mission/shoreside/LOG_* /home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/shoreside/

echo "[100%] Backup files to desktop directory"
cp -rp /home/ual/moos-ivp-logan/data/PoseKeeping/$DATE/* ~/Desktop/pPoseKeeping_logfile/$DATE/

echo "--Scp successfully finished!"

#-------------------------------------------------------
#  Part 5: Print outcome
#-------------------------------------------------------
echo "Show Directory and files? (yes/no)"
read bool
if [ $bool == 'yes' ]; then

	echo ""
	echo "================================================================================================"
	echo "Shoreside logfile copy to ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/shoreside/"
	echo "================================================================================================"
	ls -lat ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/shoreside/
	echo ""
	echo "================================================================================================"
	echo "Vehicle logfile copy to ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/heron/"
	echo "================================================================================================"
	ls -lat ~/moos-ivp-logan/data/PoseKeeping/$DATE/pPoseKeeping/heron/
	echo "================================================================================================"
else
	exit 0
fi





