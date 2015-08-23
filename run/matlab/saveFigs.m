% savefigs.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function savefigs(figvec, figname)

  DIR = 7;

  idxDelim = sort([findstr(figname,'/'), findstr(figname,'\')]);
  if (~isempty(idxDelim))
    % Create directories, if necessary
    if (exist(figname(1:idxDelim(end))) ~= DIR)
      if (idxDelim(1) == 1) 
        % Absolute path
        idxDelim = idxDelim(2:end);
      end;
      for (idx = idxDelim)
        dirstr = figname(1:idx);
        if (exist(dirstr) ~= DIR)
          disp(['Making directory:  ', dirstr]);
          mkdir(dirstr);
        end;
      end;
    end;
  end;

  for (fig = figvec)
    tag = get(fig, 'tag');
    set(fig, 'tag','');
    savestr = [figname, '_',sprintf('%02d',fig), '.fig'];
    disp(savestr);
    saveas(fig,savestr);
    set(fig, 'tag',tag);
    pause(1);
  end;


return;