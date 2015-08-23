% Classification:  UNCLASSIFIED
%
% Name:  curfigs.m
%
% Overview:
%  Returns the handles for all open figures in a row vector.
%
% Usage:
% 
%   figs = clr_figs();
%
% Restrictions / Concerns:
%  None.
%
% Author:
%  David Bruce  11/07/02
%
% Revisions:
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function figs = curfigs();

  figs = get(0,'children'); % Column vector
  figs = sort(figs)';       % Row vector

return;