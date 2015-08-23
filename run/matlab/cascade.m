% cascade1.m
%
% Overview:
%
%   Cascades figure placement on the screen.  Does not resize figures.
%
% Usage:
%
%  cascade1(figVec, pos, screen);
%
%   figVec   - Is a vector of figure handles.  Default is all open figures.
%
%   pos      - Is 'left' or 'right'.  Default is 'right'.
%
%   screen   - Is the monitor number in dual monitor systems.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function cascade(figs, pos, screen)

  mon = get(0, 'MonitorPositions');
  [qtyMons,dummy] = size(mon);

  switch nargin
    case {0}
      figs = curFigs;
      pos = 'right';
      screen = 2;
    case {1}
      if (isstr(figs))
        pos = figs;
        figs = curFigs;
      else
        pos = 'right';
      end;
      screen = 2;
    case {2}
      screen = 2;
    case {3}
      % noop
    otherwise
      help cascade;
      error('cascade argument error');
  end;

  %
  % Get the figure size from the first figure.  Assumes all figures are the same
  % size.
  %
  set(figs(1), 'units', 'normalized');

  posVec1 = get(figs(1), 'position');

  if (qtyMons == 1)
    sizeFigs = [posVec1(3), posVec1(4)];
  else
    %
    % Normilization is wrt 2 screens
    %
    sizeFigs = [2*posVec1(3), posVec1(4)];
  end;
  sizeStep = [0.01, 0.02, 0, 0];

  if (~strcmp('right', lower(pos)))
    llPosh = 1-2*sizeFigs(1)-0.01;
  else
    llPosh = 1-sizeFigs(1)-0.005;
  end;
  llPosv = 1-sizeFigs(2) - 0.08;

  llPos = [llPosh, llPosv];

  qtyFigs = length(figs);

  posSet = [llPos, sizeFigs];

  if (qtyMons == 1)

    for (idx = qtyFigs:-1:1)

      fig = figs(idx);

      set(fig, 'units', 'normalized');

      set(fig, 'position', posSet);

      posSet = posSet - sizeStep;

      set(fig, 'units', 'pixels');

    end;

  else

    for (idx = qtyFigs:-1:1)

      fig = figs(idx);

      set(fig, 'units', 'pixels');

      posSetPxl(1) = mon(screen,1) + posSet(1) * mon(screen,3);
      posSetPxl(3) = posSet(3) * mon(screen,3);
      posSetPxl([2,4]) = posSet([2,4]) * max(mon(1,4),mon(2,4));

      set(fig, 'position', posSetPxl);

      posSet = posSet - sizeStep;

    end;

  end;

return;
