% plotStd.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function plotStd(dat)

  r2d = 180/pi;

  colorList = getColorList;

  xLim = [dat.msl.launchTime(1), dat.simTime(end)];

  fig = getFig( 'plotStd 1' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.msl.pxf*1e-3, dat.msl.pyf, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.tgt.pxf*1e-3, dat.tgt.pyf, ...
            'color', colorList(2,:), 'linewidth', 2 );

      ylim([-200, 200]);
      grid on;
      xlabel( 'Position X (km)' );
      ylabel( 'Position Y (m)' );
      title ( {'Ground Plane Projection' ...
               'blu=msl, red=tgt'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.msl.pyf, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.pyf, ...
            'color', colorList(2,:), 'linewidth', 2 );

      ylim([-200, 200]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Y (m)' );
      title ( {'Cross Range Positon', ...
               'blu=msl, red=tgt'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.msl.vyf, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.vyf, ...
            'color', colorList(2,:), 'linewidth', 2 );

      ylim([-200, 200]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Velocity Y (m/sec)' );
      title ( {'Cross Range Velocity', ...
               'blu=msl, red=tgt'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.msl.ayf, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.ayf, ...
            'color', colorList(2,:), 'linewidth', 2 );

%      ylim([-200, 200]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Accelerataion Y (m/sec^2)' );
      title ( {'Cross Range Acceleration', ...
               'blu=msl, red=tgt'} );

    % end axis

  % end figure

  fig = getFig( 'plotStd 2' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.ap.ayb, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.msl.ayb, ...
            '--', 'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.gdn.aCmdYb, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-200, 200] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Guidance Command (m/sec^2)' );
      title ( {'Guidance Command' ...
               'blu=achieved, grn=ap, red=cmd'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.ap.aCmdYbLim, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.gdn.aCmdYb, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim([-250, 250]);
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Guidance Command (m/sec^2)' );
      title ( {'Guidance Command' ...
               'grn=ap.cmdLim, red=cmd'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.msl.vxb, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'msl.vxb (m/sec)' );
      title ( {'Projectile Axial Velocity'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.tgt.vxb, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'tgt.vxb (m/sec)' );
      title ( {'ASCM Axial Velocity'} );

    % end axis

  % end figure


return
