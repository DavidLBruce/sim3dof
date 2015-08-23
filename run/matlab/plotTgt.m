% plotTgt.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function plotTgt( dat )

  r2d = 180/pi;

  colorList = getColorList;

  fig = getFig( 'plotTgt 1' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot(dat.simTime, dat.tgt.ayb, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.ayb (m/sec^2)' );
      title( 'Target Acceleration Y, Body');

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot(dat.simTime, dat.tgt.ayf, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.ayb (m/sec^2)' );
      title( 'Target Acceleration Y, Inertial');

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      plot(dat.simTime, dat.tgt.axf, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.axf (m/sec^2)' );
      title( 'Target Acceleration X, Inertial');

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      theta = dat.tgt.theta;
      idxs = find(theta < 0.0);
      theta(idxs) = 2*pi +  theta(idxs);

      plot(dat.simTime, theta*r2d, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.theta (deg)' );
      title( 'Target Flight Path Angle');

    % end axis


  % end figure

  fig = getFig( 'plotTgt 2' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot(dat.simTime, dat.tgt.vxf, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.vxf (m/sec)' );
      title( 'Target Velocity X, Inertial');

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot(dat.simTime, dat.tgt.vyf, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.vyf (m/sec)' );
      title( 'Target Velocity Y, Inertial');

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      theta = dat.tgt.theta;
      idxs = find(theta < 0.0);
      theta(idxs) = 2*pi +  theta(idxs);

      plot(dat.simTime, theta*r2d, ...
           'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.theta (deg)' );
      title( 'Target Flight Path Angle');

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      plot(dat.tgt.pxf*1e-3, dat.tgt.pyf, ...
           'color', colorList(1,:), 'linewidth', 2 );

      ylim([-0.5e3, 0.5e3]);
      grid on;
      xlabel( 'Tgt.pxf (km)' );
      ylabel( 'Tgt.pyf (m)' );
      title( 'Groung Plane');

    % end axis

  % end figure



return;
