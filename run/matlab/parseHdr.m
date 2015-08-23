% parsehdr.m
%
% Overview:
%
%   Read a line of text and parse it into a cell array of words assuming blanks
%   as delimiters.
%
% Usage:
%
%  nameArray = parshdr(hdr);
%
%    hdr       = line of words separated by blanks.
%
%    nameArray = cell array containing the words of hdr.
%
%  This script is Octave friendly.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function nameArray = parseHdr(hdr);

  hdr = deblank(hdr);
  strLen = length(hdr);

  idx = 0;
  ptr = 0;
  done = false;
  strLen = length(hdr);
  while (~done)
    idx = idx + 1;
    [name, cnt] = sscanf(hdr, '%s', 1);
    nameArray{idx, 1} = name;
    ptr = strfind(hdr, name) + length(name);
    if (ptr < length(hdr))
      hdr = hdr(ptr:end);
      strLen = length(hdr);
    else
      done = true;
    end;
  end;

return;
