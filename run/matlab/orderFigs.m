% orderFigs.m
%
% Overview:
%
%  Puts focus on figures in figure number order.
%
% Usage:
%
%  orderFigs(figVec);
%    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function orderFigs(figVec)

  if (exist('figVec', 'var'))
    figList = figVec;
  else
    figList = curFigs;
  end;

  for (idx=length(figList):-1:1); 
    figure(figList(idx)); 
  end;

return;
