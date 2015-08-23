% contRdrRateTau.m
%
% Overview:
%
%  Contour plots of pHit vs Radar angle noise, datalink uplink rate, and
%  missile time constant.
%
% Usage:
%
%  contRdrRateTau( testMatrixFile, statsFile );
%
%  Where the testMatrixFile was created by genTestMatrix.m and the statsFile was
%  created from concatRunSets.sh
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function contRdrRateTau( testMatrixFile, statsFile )

  mat = loadDataFile( testMatrixFile );

  stats = loadDataFile( statsFile );

  qtyRunSets = mat.runSet( end );

  rdrVec = unique( mat.fcRdr.rinAng );
  qtyRdr = length( rdrVec );

  rateVec = unique( mat.gdn.datalinkStep );
  qtyRate = length(rateVec);

  tauVec = unique( mat.ap.tau );
  qtyTau = length( tauVec );

  runSet = 0;
  for ( rdrIdx = 1 : qtyRdr )
    for ( rateIdx = 1 : qtyRate )
      for ( tauIdx = 1 : qtyTau )
        runSet = runSet + 1;
        dat( rdrIdx ).pHit(tauIdx, rateIdx) =  stats.pHit( runSet );
      end;
    end;
  end;

  labels.x = 'Datalink Interval (sec)';
  labels.y = 'Missile Time Constant (sec)';

  fig = getFig( 'contRdrRateTau 1' );
    figure(fig);
    clf;
    orient landscape;

    mapJet = colormap('jet');
    [mapRows, mapCols] = size(mapJet);

    mapInvJet = mapJet([mapRows:-1:1],:);

    for (rdrIdx = 1 : qtyRdr)
      
      quadLabel = sprintf( 'Radar Angle Noise = %6.1f uRad/sec', ...
                                                        rdrVec(rdrIdx)*1e6 );

      labels.title = {'1 m pHit Contours', quadLabel};

      plotContour( rdrIdx, rateVec, tauVec, ...
                              dat(rdrIdx).pHit, mapInvJet, labels );
    end;

  %  end figure

return;

function plotContour(pos, Xvalues, Yvalues, Zvalues, map, labels)

  subplot(2,2,pos);
  hold on;

  colormap(map);

%   [contMat, contGroupH] = contourf( Xvalues, Yvalues, Zvalues, ...
%                                    [ 0.5 : 1/256 : 1], ...
%                                    'LineStyle', 'none' );

  [contMat, contGroupH] = contourf( Xvalues, Yvalues, Zvalues, ...
                                   [ 0.0 : 1/256 : 1], ...
                                   'LineStyle', 'none' );

  [contMat, contGroupH] = contour( Xvalues, Yvalues, Zvalues, ...
                                  [ 0.5 : 0.1 : 1.0 ], 'k' );

  th = clabel(contMat, contGroupH, 'FontSize', 10);
  set(th, 'Color', 'k');

  set(gca, 'CLim', [0.5, 1]);

  xlabel(labels.x);
  ylabel(labels.y);
  title(labels.title);
  
return;
