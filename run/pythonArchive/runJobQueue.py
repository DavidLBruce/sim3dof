#!/usr/bin/env python
################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
#
# runJobQueue.py
#
# Author:
#  Dave Bruce
#
# Overview:
#  Execute the jobs in a queue on remote hosts.
#            
# Usage
#  runJobQueue.Execute( jobQueue )
#  
#  jobQueue is a list of jobs
#  Each job is a list of strings comprising
#   [scriptName, outFilePath, testName, argList]
#
#  File paths are relative to simTop/run
#
# Dependencies:
#
################################################################################
import os, sys, subprocess, glob, time

class Machine:
  def __init__( self, _hostName, _maxQtyOfJobs, _statusDir ):
    self.hostName = _hostName
    self.maxJobs = _maxQtyOfJobs
    self.statusDir = _statusDir
    self.full = False
    self.fullLastTime = False

  def CheckIfFull( self ):
    shortName = self.hostName.split('.')[0]
    files = glob.glob( os.path.join(self.statusDir, shortName) + '*' )

    print 'Machine                 = ' + shortName
    print 'Jobs running on machine = ' + str( len(files) )
    print 'Machine capacity        = ' + str( self.maxJobs )
    
    if ( len(files) >= self.maxJobs ):
      self.full = True
    else:
      self.full = False
    return self.full

def SpawnJob( host, job ):

  runDir = os.getcwd()
  scriptFilePath = job[0]
  outDirPath = job[1]
  outFile = job[2]
  outFilePath = os.path.join( outDirPath, outFile )
  jobName = job[3]

  #
  # Remote ssh
  #
  cmd =[ 'ssh', host.hostName ]

  #
  # Script path and arguments
  #
  cmd.append( scriptFilePath )
  cmd.append( runDir )
  cmd.append( outFilePath )
  cmd.append( host.statusDir )
  cmd.append( jobName )

  #
  # Simulation arguments
  #
  for argNo in range(4, len(job) ): 
    cmd.append( job[argNo] )
  cmd.append( '&' )

  #
  # File for stdout/err capture
  #
  scrnFile = 'scrnDump%s.txt' %jobName
  scrnPath = os.path.join( outDirPath, scrnFile )

  fpOut = open( scrnPath, 'w' )
  
  proc = subprocess.Popen( cmd, stdout=fpOut, stderr=fpOut )

  fpOut.close()



#
# Begin.
# External call to run jobQueue
#
def Execute( jobQueue ):

  statusDir = 'runningJobs'
  if ( not os.path.exists(statusDir) ): 
    os.makedirs( statusDir )

  hostList = []
  with open( 'hostList.txt', 'r' ) as fp:
    lines = fp.readlines()

  for line in lines:
    lineList = line.split()

    machineName = lineList[0]
    qtyCores = int( lineList[1] )
      
    hostList.append( Machine(machineName, qtyCores, statusDir) )
   
  hostIdx = 0
  for job in jobQueue:
    done = False
    fullCount = 0
    while ( not done ):
      if ( hostList[hostIdx].CheckIfFull() ):
        print hostList[hostIdx].hostName + ' is full'
        fullCount += 1
        hostList[hostIdx].fullLastTime = True
        if ( fullCount >= len( hostList) ):
          print 'Waiting for available host'
          sys.stdout.flush()
          sys.stderr.flush()
          time.sleep( 0.5 )
      else:  
        SpawnJob( hostList[hostIdx], job );
        print 'Executing job:'
        print job
        done = True        
        if ( hostList[hostIdx].fullLastTime ):
          hostList[hostIdx].fullLastTime = False
          fullCount -= 1

      hostIdx += 1
      if ( hostIdx == len(hostList) ):
        hostIdx = 0

    sys.stdout.flush()
    sys.stderr.flush()
#    time.sleep(0.7)
    time.sleep(0.6)
#    time.sleep(0.5)
#    time.sleep(0.4)
#    time.sleep(0.3)
#    time.sleep(0.2)
        

################################################################################
#                                 UNCLASSIFIED                                 #
################################################################################
