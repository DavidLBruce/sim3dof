#!/usr/bin/env python
################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
#
# checkLogIn.py
#
# Overview:
#  Step through the machines in hostList.txt
#  Execute ping in a loop to find 'live' machines.  Then ssh into them and
#  gather information about them by calling getBoxInfo.sh
# 
#
################################################################################

import subprocess, os, sys, shutil, time


runDir = os.getcwd()
scriptName = 'getBoxInfo.sh'
scriptPath = os.path.join( runDir, scriptName )

outFileName = 'liveBoxData.txt'
outFilePath = os.path.join( runDir, outFileName )

if ( os.path.exists( outFileName) ):
  os.remove( outFileName );


with open( 'hostList.txt', 'r' ) as fp:
  lines = fp.readlines()

for line in lines:
  lineList = line.split()
  machineName = lineList[0]
  print machineName

  test = True
  try:
    subprocess.check_output( ['ping', '-c1', '-w1', machineName] )
  except:
    test = False

  if ( test ):
    print 'live'

    fpNull = open( os.devnull )

    cmd = ['ssh', machineName, scriptPath, outFilePath, ' &'] 
    proc = subprocess.Popen( cmd, cwd=runDir, stdout=fpNull, stderr=fpNull )

    fpNull.close()

    time.sleep(0.2)
    
  else:
    print 'No one home'

 

    
################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
