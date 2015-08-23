% plotText.m
%
% Overview:
%
%   Add text to the current plot.
%
% Usage:
% 
%  th = plotText(string, position, horzAlign, vertAlign);
%
%    textArraay = Cell array containing text to write to plot
%
%    position   = Location of text in normalized units 
%                  [0,0] = lower left corner
%                  [1,1] = upper rignt corner
%
%    horzAlign  = {'left', 'center', 'right'}
%
%    vertAlign  = {'bottom', 'middle', 'top'} 
%
%    th         = handle to the text object
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function th = plotText(textArray, position, horzAlign, vertAlign);

  th = text('position', position, ...
           'units', 'normalized', ...
           'horizontalAlignment', horzAlign, ...
           'verticalAlignment', vertAlign, ...
           'fontName', 'courier', ...
           'fontSize', 8, ...
           'string', textArray);

return;
