% pcf.m
%
% Overview:
%  Sets orientation to portrait.  Prints the open figures specified 
%  in the input vector(s) as color jpeg files files.  The figure 
%  number will be appended to the file_base_name.
%
% Usage:
%  
%  pcf(figvec1,figvec2, ...,file_base_name);
%
% Author:
%  David Bruce
%
% Revisions:
%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function pcf(varargin);

  file=varargin{end};

  if length(varargin) == 1;
    orient(fig, 'portrait');
    print(gcf, '-djpeg90', [file, sprintf('%02d', gcf)]);
    orient(fig, 'landscape');
  end;  

  for kk=1:(length(varargin) -1);  % This section will be skipped if figure vector
                                   % was not passed in (ie varargin only contails
                                   % the file base name
  
    figs=varargin{kk};

    for ii=1:length(figs);
      fig = figs(ii);
      orient(fig, 'portrait');
      print(fig, '-djpeg90', [file, '_', sprintf('%02d', fig)]);
      orient(fig, 'landscape');
    end;
  end;

return;


