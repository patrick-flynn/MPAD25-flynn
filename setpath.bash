#!/bin/bash

# Note: run with 
hn=`hostname -s`

if [ -d /opt/llvm-mos/bin ]; then
  PATH=/opt/llvm-mos/bin:${PATH}
  echo "/opt/llvm-mos/bin added to PATH."
else
  echo "/opt/llvm-mos/bin not found - not modifying PATH"
fi

if [ -d ${HOME}/Desktop/MPAD25/forks/neo6502-firmware/bin ]; then
  PATH=${HOME}/Desktop/MPAD25/forks/neo6502-firmware/bin:${PATH}
  echo "${HOME}/Desktop/MPAD25/forks/neo6502-firmware/bin added to PATH."
else
  echo "${HOME}/Desktop/MPAD25/forks/neo6502-firmware/bin not found - not modifying PATH"
fi
