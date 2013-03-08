#!/bin/bash
#
# Modify overscan on the fly.
# By Russell "ukscone" Davis using RPi mailbox code from Broadcom & Dom Cobley
# 2013-01-05
#
# There is very little, ok no error/sanity checking. I've left that as an exercise
# for the reader :D This is a very simplistic script but it works and i'm sure someone
# will take it and make it all flashy and cool looking.

# Check that overscan is enabled.
if [ `vcgencmd get_config disable_overscan | awk -F '=' '{print $2}'` -eq "1" ]; then
  echo -ne "Overscan is currently disabled. Please add the line\n\noverscan_disabled=0\n\nto the bottom of the config.txt file in the /boot\ndirectory, reboot & then rerun this script.\n"
        exit 1
fi

# Get current overscan values from GPU
TEMP=`./overscan`
GPU_OVERSCAN_TOP=`echo $TEMP | awk -F ' ' '{print $1}'`
GPU_OVERSCAN_BOTTOM=`echo $TEMP | awk -F ' ' '{print $2}'`
GPU_OVERSCAN_LEFT=`echo $TEMP | awk -F ' ' '{print $3}'`
GPU_OVERSCAN_RIGHT=`echo $TEMP | awk -F ' ' '{print $4}'`

# Set overscan top
LOOP=1
echo -ne "Setting overscan top. Press + key to increase, - key to decrease & q key to finish.\n"
while [ $LOOP -eq 1 ]; do
     read -s -r -d "" -N 1 CHAR
     case "$CHAR" in
	+)
    		((GPU_OVERSCAN_TOP++))
    		;;
	-)
		((GPU_OVERSCAN_TOP--))
    		;;
	q)
		let LOOP=0 
    		;;
	esac
	./overscan $GPU_OVERSCAN_TOP $GPU_OVERSCAN_BOTTOM $GPU_OVERSCAN_LEFT $GPU_OVERSCAN_RIGHT
done

# Set overscan bottom
LOOP=1
echo -ne "Setting overscan bottom. Press + key to increase, - key to decrease & q key to finish.\n"
while [ $LOOP -eq 1 ]; do
     read -s -r -d "" -N 1 CHAR
     case "$CHAR" in
        +)
                ((GPU_OVERSCAN_BOTTOM++))
                ;;
        -)
                ((GPU_OVERSCAN_BOTTOM--))
                ;;
        q)
                let LOOP=0
                ;;
        esac
        ./overscan $GPU_OVERSCAN_TOP $GPU_OVERSCAN_BOTTOM $GPU_OVERSCAN_LEFT $GPU_OVERSCAN_RIGHT

done

# Set overscan left
LOOP=1
echo -ne "Setting overscan left. Press + key to increase, - key to decrease & q key to finish.\n"
while [ $LOOP -eq 1 ]; do
     read -s -r -d "" -N 1 CHAR
     case "$CHAR" in
        +)
                ((GPU_OVERSCAN_LEFT++))
                ;;
        -)
                ((GPU_OVERSCAN_LEFT--))
                ;;
        q)
                let LOOP=0
                ;;
        esac
        ./overscan $GPU_OVERSCAN_TOP $GPU_OVERSCAN_BOTTOM $GPU_OVERSCAN_LEFT $GPU_OVERSCAN_RIGHT

done

# Set overscan right
LOOP=1
echo -ne "Setting overscan right. Press + key to increase, - key to decrease & q key to finish.\n"
while [ $LOOP -eq 1 ]; do
     read -s -r -d "" -N 1 CHAR
     case "$CHAR" in
        +)
                ((GPU_OVERSCAN_RIGHT++))
                ;;
        -)
                ((GPU_OVERSCAN_RIGHT--))
                ;;
        q)
                let LOOP=0
                ;;
        esac
        ./overscan $GPU_OVERSCAN_TOP $GPU_OVERSCAN_BOTTOM $GPU_OVERSCAN_LEFT $GPU_OVERSCAN_RIGHT

done

# Finished.
echo -ne "The current settings are temporary. If you wish to make them perminant add the\n\
following lines to the bottom of your /boot/config.txt file.\n\n\
disable_overscan=0\noverscan_top=$GPU_OVERSCAN_TOP\noverscan_bottom=$GPU_OVERSCAN_BOTTOM\noverscan_left=$GPU_OVERSCAN_LEFT\noverscan_right=$GPU_OVERSCAN_RIGHT\n"
