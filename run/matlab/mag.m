% Classification:  UNCLASSIFIED
%
% Name:  mag.m
%
% Overview:
%  Calculates the magnitude of vectors as the RSS.
%
% Usage:
%
%  magnitude = mag(arg);
%
%  arg can be either a column vector, row vector or a matrix 
%  of row vectors.
%
% Restrictions / Considerations:
%   None
%
% Author:
%  David Bruce        2/15/02
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function magnitude = mag(arg);

  [rows, cols] = size(arg);
  
  if (cols == 1)
    arg = arg';
  end;

  mag_sq = arg.^2;

  mag_sq_sum = sum(mag_sq,2);
  
  magnitude = sqrt(mag_sq_sum);
  
return;
