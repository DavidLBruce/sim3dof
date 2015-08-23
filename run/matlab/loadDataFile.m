% loaddatafile.m
%
%  Overview:
%
%    Load a table of data into a structure of vectors.  The field names are
%    taken from the table column headers.  Each field is a vector of the 
%    corresponding column.
%    
%  Usage:
%
%    dat = loadDataFile(fileName);
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function datStr = loadDataFile(fileName)

  fp = fopen(fileName);

  hdr = fgetl(fp);

  fieldNames = parseHdr(hdr);

  qtyCols = length(fieldNames);

  [datTbl, qtyDat] = fscanf(fp, '%f', inf);

  qtyRows = qtyDat / qtyCols;

  datTbl = reshape(datTbl, qtyCols, qtyRows)';

  for (col = 1:qtyCols)
    cmd = ['datStr.',fieldNames{col}, ' = datTbl(:,col);'];
    eval(cmd);
  end;

  fclose(fp);

return;
