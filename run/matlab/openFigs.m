% openfigs.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function openfigs(figvec, figname);

  for (fig = figvec)
    openstr = [figname, '_',sprintf('%02d',fig), '.fig'];
    hfig = open(openstr);
    pause(1);     % Added to prevent Linux crash

    % Delete "Fig: " from window title
    windowstr = get(hfig,'name');
    windowstr = [sprintf('%d:  ', hfig), windowstr(5:end)];
    set(hfig, 'Name',windowstr);
  end;

return;