# concatRunSets.sh
#
#
################################################################################

datDir=$1
outFile=$2

peet | head -1 >$outFile

datFiles=`ls $datDir/out*.txt`

for datFile in $datFiles; do
  echo $datFile
  tail -2 $datFile >>$outFile
done


