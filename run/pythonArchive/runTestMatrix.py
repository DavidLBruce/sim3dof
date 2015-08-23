#!/usr/bin/env python
################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
#
# runTestMatrix.py
#
# Overview:
#  
#  
#  
# 
#
################################################################################

import subprocess, os, sys, shutil, time, glob
import runJobQueue

def PrnHelp():
  print 'python runTestMatrix.py matrixFile inputFile caseDir'


def Exec():
  runDir = os.getcwd()
  scriptName = 'goSsh4.sh'
  scriptPath = os.path.join( runDir, scriptName )

  with open( matrixFile , 'r' ) as fp:
    lines = fp.readlines()

  jobQueue = []
  runSet = 0
  hdrLine = True
  for line in lines:
    if (hdrLine):
      hdrLine = False
      hdrList = line.split()
    else:
      lineList = line.split()
      runSet += 1
      runSetStr = str( runSet ).zfill(3)
      testName = runSetStr
      outFileName = 'out' + runSetStr + '.txt'

      if ( os.path.exists(caseDir) ):
        fileList = glob.glob( caseDir+'/*' )
        for file in fileList:
          os.remove( file )
      else:  
        os.makedirs( caseDir )

      
      argList = ['ex.file='+inFile]
      for idx in range(1, len(hdrList) ):
        arg = hdrList[idx] + '=' + lineList[idx]
        argList.append( arg )

      job = [scriptPath]
      job.append( caseDir )
      job.append( outFileName )
      job.append( testName )
      job.extend( argList )

      jobQueue.append( job )

  runJobQueue.Execute( jobQueue )

#
# Script entry point
#
if ( len(sys.argv) == 4 ):
  matrixFile = sys.argv[1]
  inFile = sys.argv[2]
  caseDir = sys.argv[3]
  
  Exec()

else:
  PrnHelp()








################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
