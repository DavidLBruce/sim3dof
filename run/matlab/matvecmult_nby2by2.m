% Classification:  UNCLASSIFIED
%
% Name:  matvecmult_nby3by3.m
%
% Overview:
%  Multiply a (:,2,2) matrix and a (:,2) vector such that the result
%  is a (:,2) vector.
%
% Usage:
%
%  c = a*b;
%  c = matvecmult_nby3by3(a,b);
% or
%  [c1,c2] = matvecmult_nby3by3(a,b);
%
%  b, and c are (:,2) vectors; a is a (:,2,2) matrix.
%  c1 and c2 are the columns of c.
%
% Restrictions / Considerations:
%  None.
%
% Author:
%  David Bruce        2/26/14
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [arg1, arg2, arg3] = matvecmult_nby3by3(aa, bb);

  [rows, cols] = size(aa);

  if (rows == 2)
    oneVec = ones( size(bb,1), 1 );
    for (row = 1:2)
      for (col = 1:2)
        dcmVec(:,row,col) = aa(row,col) .* oneVec;
      end;
    end;
  end;

  cc(:,1) = dcmVec(:,1,1).*bb(:,1) + dcmVec(:,1,2).*bb(:,2);
  cc(:,2) = dcmVec(:,2,1).*bb(:,1) + dcmVec(:,2,2).*bb(:,2);

  if (nargout == 1)
    arg1 = cc;
  else
    arg1 = cc(:,1);
    arg2 = cc(:,2);
  end;    
    
return;
