% runTestMatrix.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function runTestMatrix( inFileName, ...
                        outDirPath, ...
                        qtyRuns, ...
                        runSetVec, ...
                        statusDirPath )

  dat = loadDataFile( inFileName );

  runDirPath = pwd;

  switch ( nargin )
    case {1}
      error( 'Not enough inputs' );
    case {2}
      qtyRuns = 100;
      runSetVec = dat.runSet';
      statusDirPath = fullfile(runDirPath, 'runningJobs');
    case {3}
      runSetVec = dat.runSet';
      statusDirPath = fullfile(runDirPath, 'runningJobs');
    case {4}
      statusDirPath = fullfile(runDirPath, 'runningJobs');
    case {5}
      % noop
    otherwise
      error( 'Too many inputs' );
  end;

  test = ismember( runSetVec, dat.runSet );

  if ( sum(test) < length(runSetVec) )
    error( 'runSet argument contains runSets that are outside the test matrix');    
  end;

  fields = fieldnames( dat );
  sshScript = sprintf( 'goSsh%-d.sh', length(fields) );
  sshScriptPath = fullfile( runDirPath, sshScript );

  idx = 0;
  for ( runSet = runSetVec )
    idx = idx + 1;
    argList = [];
    for (argIdx = 2 : length(fields) )
      argList = [ argList, sprintf( '%s=%-e ', fields{argIdx}, ...
                  dat.( fields{argIdx} )(runSet) ) ];
    end;
    argList = sprintf('%s runs=%-d', argList, qtyRuns);

    cmd = sprintf( 'mkdir -p %s ', outDirPath);
    unix( cmd );

    outFile = sprintf('runSet%03d.txt', runSet);

    cmd = sprintf('%s %s %s %s %s %s ', ...
                sshScriptPath, ...
                  argList, ...
                    runDirPath, ...
                      outDirPath, ...
                        outFile, ...
                          statusDirPath );
                       
    disp(cmd);
    cmdVec{idx, 1} = cmd;
  end;

  spawnSshJobs( cmdVec );

return;
