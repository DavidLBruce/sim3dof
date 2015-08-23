% textArrow.m
%
% Overview:
%  Add a textarrow annotation to a plot.
%
% Usage:
%
%  textArrow( ah, xVal, yVal, len, tailOffset );
%
%  ah         = Axis handle
%               Default = current axis
%  xVal, yVal = Location of the arrow head
%  len        = Length of the arrow in normalized figure units.  If positive,
%               the arrow points left.  If negative the arrow points right.
%  tailOffset = Offset of the arrow tail with respect to the head.
%               Default = [0,0]
%               If tailOffset is scalar, the horizontal offset = 0
%  string     = Text to place at arrow tail
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [arrowHandle] = textArrow( ah, xVal, yVal, len, tailOffset, string )

  switch ( nargin )
    case {5}
      string = tailOffset;
      tailOffset = len;
      len = yVal;
      yVal = xVal;
      xVal = ah;
      ah = gca;

    case {6}
      % noop
    otherwise
      help textArrow;
      error('Input argument error');
  end;

  if ( length(tailOffset) == 1 )
    tailOffset = [ 0, tailOffset ];
  end;

  %
  % Make axis position relative to the axis numerical limits exclusive of
  % the axis labels and margins (position vs. outerposiiton(default)).
  %
  set( ah, 'activePositionProperty', 'position' );

  %
  %  Get the position of the axis in normalized figure units
  %
  %  [ left, bottom, width, height ]
  %
  axisPosVec = get(ah, 'position');

  %
  % Coordinates of the axis corners
  %
  llAxis = axisPosVec( 1:2 );
  lrAxis = llAxis + [ axisPosVec(3), 0 ];
  ulAxis = llAxis + [ 0, axisPosVec(4) ];
  urAxis = llAxis + axisPosVec(3:4);

  %
  % Limits of the axes in terms of the data values
  %
  xLim = get(ah, 'xLim');
  yLim = get(ah, 'yLim');

  %
  % Coordinates of data point of interest in terms on normalized figure units
  %
  yCoord = llAxis(2) + ( yVal - yLim(1) ) / ( yLim(2) - yLim(1) ) * ...
           ( urAxis(2) - llAxis(2) );

  xCoord = llAxis(1) + ( xVal - xLim(1) ) / ( xLim(2) - xLim(1) ) * ...
           ( urAxis(1) - llAxis(1) );

  %
  % Coordinate of the arrow head and tail in normalized figure units
  %
  head = [xCoord, yCoord];

  tail = [xCoord + len, yCoord] + tailOffset;

  %
  % Vector of arguments for annotation.  Row 1 is position argument 1 and
  % row 2 is argument 2.
  %
  arrowVec = [ tail(1), head(1); tail(2), head(2) ];

  arrowHandle = annotation( 'textArrow', arrowVec(1,:), arrowVec(2,:), ...
                            'string', string, ...
                            'fontName', 'courier', ...
                            'verticalAlignment', 'middle' );      

return;
