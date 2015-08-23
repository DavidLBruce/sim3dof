#! /bin/bash
#
# goSsh4.sh
#
################################################################################

runDir=$1
outFilePath=$2
statusDir=$3
jobName=$4

echo ""
echo "runDir = "$runDir
echo "outFilePath = "$outFilePath
echo "statusDir = "$statusDir
echo "jobName = "$jobName

arg1=$5
arg2=$6
arg3=$7
arg4=$8

echo ""
echo "arg1 = "$arg1
echo "arg2 = "$arg2
echo "arg3 = "$arg3
echo "arg4 = "$arg4

machine=`uname -n`
machine=`echo $machine | cut -d. -f1`
runningFile=$machine.$jobName

echo ""
echo "Running status file:"
echo $runningFile

echo ""
echo "Initial directory:"
pwd

cd $runDir

echo "Run directory:"
pwd

#
# Indicate running status by creating a file in the $statusDir directory
#
touch $statusDir/$runningFile

echo ""
echo "Simulation command line:"
echo "./peet $arg1 $arg2 $arg3 $arg4 > $outFilePath"

./peet $arg1 $arg2 $arg3 $arg4 > $outFilePath

#./peet $arg1 $arg2 $arg3 $arg4 | tee $outFilePath

rm $statusDir/$runningFile


