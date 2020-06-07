#!/bin/bash

columnfirstonoff="ON";
parabolicbowlonoff="OFF";
trapezoidonoff="OFF"
debuglevel=0
meshbuilder_root_path="/workspace/gajanan/adh/shiftx/mesh_builder_adh"
#########################################################################

####################################################################################################################

#    DO NOT EDIT THIS SCRIPT BEYOND THIS LINE!!!!!!!!!!

####################################################################################################################







####################################################################################################################
clear

if [ ! -d $meshbuilder_root_path ]; then
   echo "Meshbuilder directory path '$meshbuilder_root_path' not found. Please open the script and specify the FULL path to the variable 'meshbuilder_root_path' manually";
   echo "Terminating bash run"
   exit -1
fi
cleanstatus=-1
runstatus=-1
cmake_args=" -DUSE_COLUMNFIRST=$columnfirstonoff -DTEST_PARABOLIC_BOWL=$parabolicbowlonoff -DTEST_TRAPEZOID=$trapezoidonoff"

####################################################################################################################
checkpoint(){
if [ $1 -eq -2 ]; then
  echo;echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  echo "Error: Conflicting command line arguments supplied by user. Terminating script."
  echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  exit -1
fi
if [ $1 -eq -3 ]; then
  echo;echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  echo "Error: Directory $2 not found! Terminating script."
  echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  exit -1
fi
if [ $1 -ne 0 ]; then
  echo;echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  echo "Some command was prevented from execution or proceeded incorrectly. Terminating script."
  echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  exit -1
fi
}
shortecho() { echo ""; echo "***********************************************"; }
longecho() { echo ""; echo "*****************************************************************************************************"; }

####################################################################################################################
longecho
for arg in "$@" ;do
  shortecho
  echo "CHECKING ARGUMENT USER ARGUMENT: $arg";echo;
########################################################## For CLEANING BUILD
  if [ "$arg" = "justclean" ] || [ "$arg" = "jc" ]; then
    cleanstatus=2
    echo "'Just clean - don't build option detected. All CMake files will be deleted."
    sleep 0.5
  fi
########################################################## For CLEANING BUILD
  if [ "$arg" = "noclean" ] || [ "$arg" = "nc" ]; then
    cleanstatus=0
    echo "'Don't clean build' option detected."
    sleep 0.5
  fi
########################################################## For CLEANING BUILD
  if [ "$arg" = "compile" ] || [ "$arg" = "co" ]; then
    runstatus=0
    echo "'Only-compile-do-not-run option detected."
    sleep 0.5
  fi
##########################################################

done
####################################################################################################################

sleep 0.5

########################################################## For CLEANING BUILD - DEFAULT
  if [ $cleanstatus -eq -1 ]; then
    cleanstatus=1
    shortecho
    echo "Build cleaning option was not specified by user"
    echo "     - Build directory /CMAKE will be cleaned by default"
    sleep 0.5
  fi
##########################################################

cmake_args="$cmake_args -DBUILD_DEBUG_LEVEL=$debuglevel"

shortecho
longecho
####################################################################################################################

##########################################################
if [ ! -d "$meshbuilder_root_path/CMAKE" ]; then  mkdir "$meshbuilder_root_path/CMAKE"; fi
if [ ! -d "$meshbuilder_root_path/CMAKE" ]; then  checkpoint -3 "$meshbuilder_root_path/CMAKE"; fi
cd "$meshbuilder_root_path/CMAKE/"

if [ $cleanstatus -gt 0 ]; then
  shortecho
  echo "Cleaning previous build, if any..."
  echo "     - Deleting folders"
  rm -r bin/meshbuilder bin/runoutput.txt bin/*.3dm bin/*.bc bin/*.hot bin/superfile.in CMakeFiles main structs builder lib input output  >& /dev/null
  echo "     - Deleting remaining files"
  rm Makefile cmake_install.cmake CMakeCache.txt >& /dev/null
  echo "     - Cleaning complete"
fi
if [ $cleanstatus -eq 2 ]; then
  exit 0;
fi



####################################################################################################################
longecho
cmake $cmake_args ..
checkpoint $?
longecho
make
checkpoint $?
longecho
echo ""
echo "Program 'meshbuilder' compiled successfully in $meshbuilder_root_path/CMAKE/bin/"
if [ $runstatus -eq 0 ]; then
  exit 0;
fi
sleep 1
####################################################################################################################

cd "$meshbuilder_root_path/CMAKE/bin/"
./meshbuilder > runoutput.txt
tail runoutput.txt

longecho
echo ""
echo "Script run completed. Please manually check for errors!."
longecho
