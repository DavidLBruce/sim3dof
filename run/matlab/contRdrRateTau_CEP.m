% contRdrRateTau_CEP.m
%
% Overview:
%
%  Contour plots of CEP vs target velocity, datalink uplink rate, and
%  missile time constant.
%
% Usage:
%
%  contRdrRateTau_CEP( testMatrixFile, statsFile );
%
%  Where the testMatrixFile was created by genTestMatrix.m and the statsFile was
%  created from concatRunSets.sh
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [dat] = contRdrRateTau_CEP( testMatrixFile, statsFile )

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
        dat( rdrIdx ).cep(tauIdx, rateIdx) =  stats.medMiss( runSet );
      end;
    end;
  end;

  labels.x = 'Datalink Interval (sec)';
  labels.y = 'Missile Time Constant (sec)';

  fig = getFig( 'contRdrRateTau_CEP 1' );
    figure(fig);
    clf;
    orient landscape;

    mapJet = colormap('jet');
    [mapRows, mapCols] = size(mapJet);

%    mapInvJet = mapJet([mapRows:-1:1],:);

    for (rdrIdx = 1 : qtyRdr)
      
      quadLabel = sprintf( 'Radar Angle Noise = %6.1f uRad', ...
                            rdrVec(rdrIdx)*1e6 );

      labels.title = {'CEP Contours', quadLabel};

      plotContour( rdrIdx, rateVec, tauVec, ...
                              dat(rdrIdx).cep, mapJet, labels);
    end;

  %  end figure

return;

function plotContour(pos, Xvalues, Yvalues, Zvalues, map, labels)

  subplot(2,2,pos);
  hold on;

  colormap(map);

  [contMat, contGroupH] = contourf( Xvalues, Yvalues, Zvalues, ...
                                   [ 0 : 1/256 : 1], ...
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
