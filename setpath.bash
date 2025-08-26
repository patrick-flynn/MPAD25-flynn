#!/bin/bash

# Note: run with 
hn=`hostname -s`

if [[ "$hn" ==  "wazoo" ]]; then
 PATH=/opt/llvm-mos/bin:/Users/flynn/Desktop/MPAD25/neo6502-firmware/bin:${PATH}
elif [[ "$hn" == "CSE-FLYNN" ]]; then
 PATH=/opt/llvm-mos-sdk/bin:/Users/flynn/Desktop/MPAD25/neo6502-firmware/bin:${PATH}
else
 echo "Unknown host $hn"
fi

 
