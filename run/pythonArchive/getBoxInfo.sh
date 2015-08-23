#! /bin/bash
################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
#
# getBoxInfo.sh
#
# Overview:
#  Get user and machine info
# 
#
################################################################################

outFile=$1

touch $outFile

machineName=`uname -n | cut -d. -f1`

qtyProcs=`grep -c processor /proc/cpuinfo`

model=`sysinfo mb | cut -d: -f2 | head -2 | grep Workstation`

user=`who | cut -d' ' -f1 | head -1 | grep -v rat9100`

echo "$machineName $qtyProcs $model $user" >> $outFile

################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
