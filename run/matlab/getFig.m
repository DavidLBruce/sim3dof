% get_fig.m
%
% Overview:
%  Return a figure handle for a named figure.  If the figure does not exist it
%  will be creaded.
%
% Usage:
%  fig = get_fig( tag, clearFig );
%
%  tag      = Text string identifying the figure.
%  clearFig = Flag to clear the figure.
%             Default = true
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function fig = get_fig( tag, clearFig )

  if ( ~exist('clearFig', 'var' ) )
    clearFig = true;
  end;

  fig = findobj('tag',tag);
  if (isempty(fig))
    fig = figure;
    set(fig,'tag',tag);
  end;

  set(fig, 'NumberTitle','off');
  set(fig, 'Name',[num2str(fig), ':  ',tag]);

  figure(fig);

  if ( clearFig )
    clf;
  end;

return;
