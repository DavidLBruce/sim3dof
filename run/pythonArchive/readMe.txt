readMe.txt
==========

> cd run

> matlab &
>> setuppath;
>> genTestMatrix( matFileName );

#
# Check the status of the machines in hostList.txt
#
> checkHostList.py

#
# Edit hostList.txt based on liveBoxData.txt deleting machines someone is logged
# into.  Also remove machines that are indicated by "No one home" in std out
# dump.
#

> runTestMatrix.py study001.txt hostList.txt in001.inp case001

#
# Collect the results
#
> concatRunSets.sh case001 out.txt

//
// Generate contour plots
//
>> contRngRateTau( 'study001.txt', 'out.txt' );

