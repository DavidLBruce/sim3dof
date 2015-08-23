% plotMslTse.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function plotMslTse(dat)

  r2d = 180/pi;

  colorList = getColorList;

  idxs = find( dat.skr.measValid );
  idxs = idxs(2:end);

  xLim1 = floor(dat.simTime(idxs(1)) / 5.0 ) * 5;
  xLim2 = ceil(dat.simTime(idxs(end)) / 5.0 ) * 5;
  xLim = [ xLim1, xLim2 ];

  fig = getFig( 'plotMslTse 1' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.skr.measAng(idxs)*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.mscState0(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truState0(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-100, 100] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 0 (mRad)' );
      title ( {'Line of Sight Angle' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.mscState1(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truState1(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-100, 100] );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 1 (mRad/sec)' );
      title ( {'Line of Sight Rate' ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.skr.measAng(idxs) - dat.mslTse6.truState0(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.mslTse6.mscState0(idxs) - dat.mslTse6.truState0(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.mslTse6.P00(idxs) );

      plot( dat.simTime(idxs), sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim( [-2, 2] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 0 (mRad)' );
      title ( {'LOS Angle Error' ...
               'grn=meas, blu=est, blk=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.mscState1(idxs) - dat.mslTse6.truState1(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.mslTse6.P11(idxs) );

      plot( dat.simTime(idxs), sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim( [-15.0, 15.0] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 1 (mRad/sec)' );
      title ( {'Line of Sight Rate Error' ...
               'blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 2' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.measVec1(idxs)*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.mscState2(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truState2(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [0, 4] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 2 x 1e3' );
      title ( {'One Over Range' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.measVec2(idxs), ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.mscState3(idxs), ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truState3(idxs), ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-1.0, 1.0] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 3' );
      title ( {'rDot Over Range' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.measVec1(idxs) - dat.mslTse6.truState2(idxs);

      plot( dat.simTime(idxs), error*1e9, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.mslTse6.mscState2(idxs) - dat.mslTse6.truState2(idxs);

      plot( dat.simTime(idxs), error*1e9, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.mslTse6.P22(idxs) );

      plot( dat.simTime(idxs), sig*1e9, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), -sig*1e9, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim( [-500, 500] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 2 Error x 1e9' );
      title ( {'Inv Rng Error' ...
               'grn=meas, blu=est, bld=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.mscState3(idxs) - dat.mslTse6.truState3(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.mslTse6.P33(idxs) );

      plot( dat.simTime(idxs), sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim( [ -10, 10 ] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 3 * 1e3' );
      title ( {'Rdot Over Rng Error' ...
               'grn=meas, blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 3' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.mscState4(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truState4(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-10, 10]);
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 4 * 1e3' );
      title ( {'AccX Over Range' ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.mscState5(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truState5(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-100, 100] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 5 x 1e6' );
      title ( {'AccY Over Range' ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.mscState4(idxs) - dat.mslTse6.truState4(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.mslTse6.P44(idxs) );

      plot( dat.simTime(idxs), sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim( [-10,10] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 4 Error x 1e3' );
      title ( {'AccX Over Range Error' ...
               'blu=est, blk=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.mscState5(idxs) - dat.mslTse6.truState5(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.mslTse6.P55(idxs) );

      plot( dat.simTime(idxs), sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim( [-20,20] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 5 Error x 1e6' );
      title ( {'AccY Over Range Error' ...
               'blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 4' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.skr.measAng(idxs)*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.estLosAng(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.skr.truAng(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim([-100, 100]);
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOS Angle (mRad)' );
      title ( {'Line of Sight Angle' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estLosr(idxs)*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truLosr(idxs)*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-150.0, 150.0] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOSR (mRad/sec)' );
      title ( {'Line of Sight Rate', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.skr.measAng(idxs) - dat.skr.truAng(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.mslTse6.estLosAng(idxs) - dat.skr.truAng(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-2,2] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOS Angle Error (mRad)' );
      title ( {'Line of Sight Angle Error' ...
               'grn=meas, blu=est'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.estLosr(idxs) - dat.mslTse6.truLosr(idxs);

      plot( dat.simTime(idxs), error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-20.0, 20.0] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOSR Error (mRad/sec)' );
      title ( {'Line of Sight Rate Error'} ); ...

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 5' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.measRng(idxs)*1e-3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.estRng(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truRng(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Range (km)' );
      title ( {'Range', ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.measRdot(idxs)*1e-3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.estRdot(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truRdot(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

%      ylim( [-100, 100]);
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'rDot (km/sec)' );
      title ( {'Range Rate', ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.measRng(idxs) - dat.mslTse6.truRng(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.mslTse6.estRng(idxs) - dat.mslTse6.truRng(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-10, 10] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Range Error (m)' );
      title ( {'Range Error', ...
               'grn=meas, blu=est'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.measRdot(idxs) - dat.mslTse6.truRdot(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.mslTse6.estRdot(idxs) - dat.mslTse6.truRdot(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-10, 10] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Rdot Error (m/sec)' );
      title ( {'Range Rate error Y', ...
               'grn=meas, blu=est'} );

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 6' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estAccX(idxs), ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truAccLosX(idxs), ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-50, 50] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc X (m/sec^2)' );
      title ( {'Acceleration Along LOS', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estAccY(idxs), ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truAccLosY(idxs), ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim( [-500, 500] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc Y (m/sec^2)' );
      title ( {'Acceleration Across LOS', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.estAccX(idxs) - dat.mslTse6.truAccLosX(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-20,20] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc X Error (m/sec^2)' );
      title ( {'Acceleration Along LOS Error'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.estAccY(idxs) - dat.mslTse6.truAccLosY(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-500,500] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc Y Error (m/sec^2)' );
      title ( {'Acceleration Across LOS Error'} );

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 7' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.cartState0(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truRelPxf(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position X (km)' );
      title ( {'Relative Target Position X', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.cartState1(idxs), ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.mslTse6.truRelPyf(idxs), ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Y (m)' );
      title ( {'Relative Target Position Y', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.mslTse6.cartState0(idxs) - dat.mslTse6.truRelPxf(idxs);

      plot( dat.simTime(idxs), error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-10, 10] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Error X (m)' );
      title ( {'Target Relative Position Error X'});

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      truth = interp1( dat.simTime, dat.mslTse6.truRelPyf, ...
                       dat.mslTse6.timeStamp(idxs) );

      error = dat.mslTse6.cartState1(idxs) - truth;

      plot( dat.simTime(idxs), error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-10, 10] );
      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Error Y (m)' );
      title ( {'Target Position Error Y'});

    % end axis

  % end figure

  fig = getFig( 'plotMslTse 8' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estTgtPxf(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.tgt.pxf(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position X (km)' );
      title ( {'Absolute Target Position X', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estTgtPyf(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.tgt.pyf(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Y (km)' );
      title ( {'Absolute Target Position Y', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estTgtVxf(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.tgt.vxf(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Target Velocity X (km/sec)' );
      title ( {'Absolute Target Velocity X', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime(idxs), dat.mslTse6.estTgtVyf(idxs)*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime(idxs), dat.tgt.vyf(idxs)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      xlim( xLim );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Target Velocity Y (km/sec)' );
      title ( {'Absolute Target Velocity Y', ...
               'blu=est, red=truth'} );

    % end axis

  % end figure

return;
