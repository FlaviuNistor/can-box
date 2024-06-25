#!/usr/bin/ bash

#help stage
if [ "$1" == "-h" ]; then
    echo "Usage description for: `basename $0`"
    echo "  [param 1 = can_interface --> specify the CAN interface you want to test]"
    echo "  [example call: bash `basename $0` can0]"
    exit 0
fi

#check nr. of parameter function
if [[ $# != 1 ]]; then  
    echo "Wrong number of parameters. Expected one parameter."
    echo "Parameter1: can interface"
    exit 1
fi

# Path to the log file
can_interface=$1
can_bitrate=1000000
can_dbitrate=5000000
restart_time=100

# make sure that all can interfaces are down in order to have a clean start
ip link set $can_interface down
var=$?   # used not to get overwritten by echo exit code
if [[ var -eq 0 ]]; then
    echo "$can_interface down: preparing for initialization"
else
    echo "Stoping the script due to socketCAN error: $(echo $var)"
    exit 1  # use different than 0 to reflect error
fi

# configure CANx 
ip link set $can_interface type can bitrate $can_bitrate dbitrate $can_dbitrate fd on berr-reporting on restart-ms $restart_time
var=$?   # used not to get overwritten by echo exit code
if [[ var -eq 0 ]]; then
    echo "$can_interface succesfully configured"
else
    echo "Stoping the script due to socketCAN error: $(echo $var)"
    exit 3  # use different than 0 to reflect error
fi

# enable CANx
ip link set $can_interface up
var=$?   # used not to get overwritten by echo exit code
if [[ var -eq 0 ]]; then
    echo "$can_interface succesfully bring up"
else
    echo "Stoping the script due to socketCAN error: $(echo $var)"
    exit 4  # use different than 0 to reflect error
fi

# in order not to get error from cangen: No buffer space
# by default this is set to 10
ifconfig $can_interface txqueuelen 1000
var=$?   # used not to get overwritten by echo exit code
if [[ var -eq 0 ]]; then
    echo 'txqueue succesfully reconfigured'
else
    echo "Stoping the script due to ifconfig error: $(echo $var) when setting txqueuelen"
    exit 7  # use different than 0 to reflect error
fi

# echo needed messages
echo 'CAN is configured as CAN-FD with the next parameters:' 
echo "Arbitration baud rate: $can_bitrate"
echo "Data baud rate: $can_dbitrate"

# exit with no error
exit 0
