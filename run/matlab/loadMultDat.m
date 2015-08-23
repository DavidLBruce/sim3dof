% loadMultDat.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [dat] = loadMultDat(runVec)

  if (nargin == 1)
    qtyRuns = length(runVec);
    for (runIdx = 1:qtyRuns)
      fileNameVec{runIdx, 1} = sprintf('peet-%05d.dat', runVec(runIdx));
    end;
  else
    dirList = dir('peet*.dat');
    qtyRuns = length(dirList);
    runVec = 1:qtyRuns;
    for (runIdx = runVec)
      fileNameVec{runIdx, 1} = dirList(runIdx).name;
    end;
  end;

  for (runIdx = 1:qtyRuns)
    dat(runIdx,1) = loadDataFile(fileNameVec{runIdx});
  end;

return;
