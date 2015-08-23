% contRateVelTau_CEP.m
%
% Overview:
%
%  Contour plots of CEP vs seeker frame rate, missile velocity and missile time
%  constant.
%
% Usage:
%
%  contRateVelTau_CEP( testMatrixFile, statsFile );
%
%  Where the testMatrixFile was created by genTestMatrix.m and the statsFile was
%  created from concatRunSets.sh
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function contRateVelTau_CEP( testMatrixFile, statsFile )

  mat = loadDataFile( testMatrixFile );

  stats = loadDataFile( statsFile );

  qtyRunSets = mat.runSet( end );

  rateVec = unique( mat.eo_frame_rate );
  qtyRate = length(rateVec);

  velVec = unique( mat.vm );
  qtyVel = length( velVec );

  tauVec = unique( mat.tau_af );
  qtyTau = length( tauVec );

  runSet = 0;
  for ( rateIdx = 1 : qtyRate )
    for ( velIdx = 1 : qtyVel )
      for ( tauIdx = 1 : qtyTau )
        runSet = runSet + 1;
        dat( rateIdx ).cep(tauIdx, velIdx) =  stats.med_miss( runSet );
      end;
    end;
  end;

  labels.x = 'Missile Velocity (m/sec)';
  labels.y = 'Missile Time Constant (sec)';

  fig = getFig( 'contRateVelTau_CEP 1' );
    figure(fig);
    clf;
    orient landscape;

    mapJet = colormap('jet');
    [mapRows, mapCols] = size(mapJet);

%    mapInvJet = mapJet([mapRows:-1:1],:);

    for (rateIdx = 1 : qtyRate)
      
     quadLabel = sprintf( 'Seeker Frame Rate = %3.1f Hz', rateVec(rateIdx) );

     labels.title = {'CEP Contours', quadLabel};

     plotContour(rateIdx, velVec, tauVec, ...
                              dat(rateIdx).cep, mapJet, labels);
    end;

  %  end figure

return;

function plotContour(pos, Xvalues, Yvalues, Zvalues, map, labels)

  subplot(2,2,pos);
  hold on;

  colormap(map);

  [contMat, contGroupH] = contourf( Xvalues, Yvalues, Zvalues, ...
                                   [ 0 : 1/256 : 5], ...
                                   'LineStyle', 'none' );

  [contMat, contGroupH] = contour( Xvalues, Yvalues, Zvalues, ...
                                  [ 0.5 : 0.5 : 5.0 ], 'w' );

  th = clabel(contMat, contGroupH, 'FontSize', 10);
  set(th, 'Color', 'w');

  set(gca, 'CLim', [0.5, 5]);

  major;
  grid on;
  xlabel(labels.x);
  ylabel(labels.y);
  title(labels.title);
  
return;
