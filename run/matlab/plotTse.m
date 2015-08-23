% plotTse.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function plotTse(dat)

  r2d = 180/pi;

  colorList = getColorList;

  fig = getFig( 'plotTse 1' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcRdr.measAng*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.mscState0*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truState0*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 0 (mRad)' );
      title ( {'Line of Sight Angle' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.mscState1*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truState1*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 1 (mRad/sec)' );
      title ( {'Line of Sight Rate' ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcRdr.measAng - dat.fcTse6.truState0;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTse6.mscState0 - dat.fcTse6.truState0;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTse6.P00 );

      plot( dat.simTime, sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 0 (mRad)' );
      title ( {'LOS Angle Error' ...
               'grn=meas, blu=est, blk=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTse6.mscState1 - dat.fcTse6.truState1;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTse6.P11 );

      plot( dat.simTime, sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 1 (mRad/sec)' );
      title ( {'Line of Sight Rate Error' ...
               'blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotTse 2' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      measInvRng = ones( size(dat.simTime) ) ./  dat.fcRdr.measRng;

      plot( dat.simTime, measInvRng*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.mscState2*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truState2*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 2 x 1e3' );
      title ( {'One Over Range' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      measRdotOvrR = dat.fcRdr.measRdot ./  dat.fcRdr.measRng;

      plot( dat.simTime, measRdotOvrR, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.mscState3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truState3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 3' );
      title ( {'rDot Over Range' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = measInvRng - dat.fcTse6.truState2;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTse6.mscState2 - dat.fcTse6.truState2;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTse6.P22 );

      plot( dat.simTime, sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig*1e3, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 2 Error x 1e3' );
      title ( {'Inv Rng Error' ...
               'grn=meas, blu=est, bld=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = measRdotOvrR - dat.fcTse6.truState3;

      plot( dat.simTime, error, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTse6.mscState3 - dat.fcTse6.truState3;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTse6.P33 );

      plot( dat.simTime, sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 3' );
      title ( {'Rdot Over Rng Error' ...
               'grn=meas, blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotTse 3' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.mscState4*1e6, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truState4*1e6, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 4 * 1e6' );
      title ( {'AccX Over Range' ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.mscState5*1e6, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truState5*1e6, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 5 x 1e6' );
      title ( {'AccY Over Range' ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTse6.mscState4 - dat.fcTse6.truState4;

      plot( dat.simTime, error*1e6, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTse6.P44 );

      plot( dat.simTime, sig*1e6, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig*1e6, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 4 Error x 1e6' );
      title ( {'AccX Over Range Error' ...
               'blu=est, blk=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTse6.mscState5 - dat.fcTse6.truState5;

      plot( dat.simTime, error*1e6, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTse6.P55 );

      plot( dat.simTime, sig*1e6, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig*1e6, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'State 5 Error x 1e6' );
      title ( {'AccY Over Range Error' ...
               'blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotTse 4' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcRdr.measAng*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.estLosAng*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcRdr.truAng*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOS Angle (mRad)' );
      title ( {'Line of Sight Angle' ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.estLosr*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truLosr*1e3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOSR (mRad/sec)' );
      title ( {'Line of Sight Rate', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcRdr.measAng - dat.fcRdr.truAng;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTse6.estLosAng - dat.fcRdr.truAng;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOS Angle Error (mRad)' );
      title ( {'Line of Sight Angle Error' ...
               'grn=meas, blu=est'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTse6.estLosr - dat.fcTse6.truLosr;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'LOSR Error (mRad/sec)' );
      title ( {'Line of Sight Rate Error'} ); ...

    % end axis

  % end figure

  fig = getFig( 'plotTse 5' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcRdr.measRng*1e-3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.estRng*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcRdr.truRng*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Range (km)' );
      title ( {'Range', ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcRdr.measRdot*1e-3, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.estRdot*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcRdr.truRdot*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'rDot (km/sec)' );
      title ( {'Range Rate', ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcRdr.measRng - dat.fcRdr.truRng;

      plot( dat.simTime, error, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTse6.estRng - dat.fcRdr.truRng;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Range Error (m)' );
      title ( {'Range Error', ...
               'grn=meas, blu=est'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcRdr.measRdot - dat.fcRdr.truRdot;

      plot( dat.simTime, error, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTse6.estRdot - dat.fcRdr.truRdot;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Rdot Error (m/sec)' );
      title ( {'Range Rate error Y', ...
               'grn=meas, blu=est'} );

    % end axis

  % end figure

  fig = getFig( 'plotTse 6' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.estAccX, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truAccLosX, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc X (m/sec^2)' );
      title ( {'Acceleration Along LOS', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.estAccY, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTse6.truAccLosY, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc Y (m/sec^2)' );
      title ( {'Acceleration Across LOS', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTse6.estAccX - dat.fcTse6.truAccLosX;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc X Error (m/sec^2)' );
      title ( {'Acceleration Along LOS Error'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTse6.estAccY - dat.fcTse6.truAccLosY;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Acc Y Error (m/sec^2)' );
      title ( {'Acceleration Across LOS Error'} );

    % end axis

  % end figure

  fig = getFig( 'plotTse 7' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.cartState0*1e-3, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, (dat.tgt.pxf - dat.fcRdr.pxf)*1e-3, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position X (km)' );
      title ( {'Target Position X', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTse6.cartState1, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.pyf - dat.fcRdr.pyf, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Y (m)' );
      title ( {'Target Position Y', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      truth = interp1( dat.simTime, (dat.tgt.pxf - dat.fcRdr.pxf), ...
                       dat.fcTse6.timeStamp );

      error = dat.fcTse6.cartState0 - truth;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-5, 5] );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Error X (m)' );
      title ( {'Target Position Error X'});

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      truth = interp1( dat.simTime, (dat.tgt.pyf - dat.fcRdr.pyf), ...
                       dat.fcTse6.timeStamp );

      error = dat.fcTse6.cartState1 - truth;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      ylim( [-5, 5] );
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Error Y (m)' );
      title ( {'Target Position Error Y'});

    % end axis

  % end figure

  fig = getFig( 'plotTse 8' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTseO.measPosY, ...
            'color', colorList(3,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcTseO.estPosY, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.fcRdr.truPosY, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Y (m)' );
      title ( {'Position Crossrange', ...
               'grn=meas, blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTseO.estOmega, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.omega, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim([0, 1]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Omega (rad/sec)' );
      title ( {'Radian Frequency', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTseO.measPosY - dat.fcRdr.truPosY;

      plot( dat.simTime, error, ...
            'color', colorList(3,:), 'linewidth', 2 );

      error = dat.fcTseO.estPosY - dat.fcRdr.truPosY;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTseO.P00 );

      plot( dat.simTime, sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );
            
      plot( dat.simTime, -sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim([-10, 10]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Position Y Error (m)' );
      title ( {'Position Cross Range Error', ...
               'grn=meas, blu=est, blk=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTseO.estOmega - dat.tgt.omega;

      plot( dat.simTime, error*1e3, ...
            'color', colorList(1,:), 'linewidth', 2 );

%       sig = sqrt( sqrt(dat.fcTseO.P22) );
% 
%       plot( dat.simTime, sig*1e3, ...
%             '--', 'color', colorList(15,:), 'linewidth', 2 );
% 
%       plot( dat.simTime, -sig*1e3, ...
%             '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim([-100, 100]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Omega Error (mRad/sec)' );
      title ( {'Radian Frequency Error', ...
               'blu=est, blk=1sig'} );

    % end axis

  % end figure

  fig = getFig( 'plotTse 9' );
    figure(fig);
    clf;
    orient( 'landscape' );

    pos = 1;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTseO.estPosYdot, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.vyf, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Velocity Y (m/sec)' );
      title ( {'Velocity Crossrange', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 2;
      subplot( 2,2,pos )
      hold on;

      plot( dat.simTime, dat.fcTseO.estOmega2, ...
            'color', colorList(1,:), 'linewidth', 2 );

      plot( dat.simTime, dat.tgt.omega.^2, ...
            '--', 'color', colorList(2,:), 'linewidth', 2 );

      ylim([0, 1]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Omega^2 ((rad/sec)^2)' );
      title ( {'Radian Frequency Squared', ...
               'blu=est, red=truth'} );

    % end axis

    pos = 3;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTseO.estPosYdot - dat.tgt.vyf;

      plot( dat.simTime, error, ...
            'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTseO.P11 );

      plot( dat.simTime, sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim([-5, 5]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Velocity Y Error (m/sec)' );
      title ( {'Velocity Crossrange Error', ...
               'blu=est, blk=1sig'} );

    % end axis

    pos = 4;
      subplot( 2,2,pos )
      hold on;

      error = dat.fcTseO.estOmega2 - dat.tgt.omega.^2;

      plot( dat.simTime, error, ...
            '--', 'color', colorList(1,:), 'linewidth', 2 );

      sig = sqrt( dat.fcTseO.P22 );

      plot( dat.simTime, sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      plot( dat.simTime, -sig, ...
            '--', 'color', colorList(15,:), 'linewidth', 2 );

      ylim([-0.1, 0.1]);
      grid on;
      xlabel( 'Time (sec)' );
      ylabel( 'Omega^2 Error ((mRad/sec)^2)' );
      title ( {'Radian Frequency Squared Error', ...
               'blu=est, blk=1sig'} );

    % end axis

  % end figure

return;
