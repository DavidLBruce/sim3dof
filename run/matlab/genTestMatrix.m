% genTestMatrix.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function genTestMatrix( fileName )

  tgtVelVec = [ 250, 500, 750, 1000 ];        % m/sec 4 elements 

  dlIntVec = [ 1, 2 : 2 : 16, 32, 64 ] / 64;  % sec   datalink uplink interval
                                              %       11 elements

  tauVec = [ 10,  20 : 20: 200 ]*1e-3;        % sec   11 elements

  fp = fopen( fileName, 'w');

  fprintf( fp, '%20s', 'runSet', 'tgt.initVxb', 'gdn.datalinkStep', 'ap.tau' );
  fprintf( fp, '\n' );

  runSet = 0;
  for ( velIdx = 1 : length(tgtVelVec) )
    for ( dlIdx = 1 : length(dlIntVec) )
      for ( tauIdx = 1 : length(tauVec) )
        runSet = runSet + 1;

        fprintf( fp, '%20d', runSet ); 
        fprintf( fp, '%20.6e', tgtVelVec(velIdx), dlIntVec(dlIdx), ...
                                                            tauVec(tauIdx));
        fprintf( fp, '\n' );

      end;
    end;
  end;

  fclose( fp );

return;
