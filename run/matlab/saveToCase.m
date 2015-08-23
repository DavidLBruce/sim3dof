% saveToCase.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function saveToCase( inpFile, caseDirPath )

  figVec = 1:2;
  saveStr = fullfile( caseDirPath, 'plotStd' );
  saveFigs(figVec, saveStr);
  pjpeg(figVec, saveStr);

  figVec = 3:10;
  saveStr = fullfile( caseDirPath, 'plotMslTse' );
  saveFigs(figVec, saveStr);
  pjpeg(figVec, saveStr);

  cpStr = sprintf( 'cp -v %s %s', inpFile, caseDirPath );
  unix(cpStr);

  cpStr = sprintf( 'cp -v %s %s', '../src/Guidance.cpp', caseDirPath );
  unix(cpStr);

  cpStr = sprintf( 'cp -v %s %s', '../src/MslTse6.cpp', caseDirPath );
  unix(cpStr);

  cpStr = sprintf( 'cp -v %s %s', 'out.txt', caseDirPath );
  unix(cpStr);

  cmd = sprintf( 'cd ..; svn info > %s', ...
                      fullfile('run', caseDirPath, 'svnInfo.txt') );  
  unix( cmd );

  cmd = sprintf( 'cd ..; svn st | grep -v .d > %s', ...
                      fullfile('run', caseDirPath, 'svnStatus.txt') );  
  unix( cmd );


return;
