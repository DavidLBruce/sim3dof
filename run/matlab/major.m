function [XTicksPerDiv,YTicksPerDiv]=major(AxesH);
%[XTicksPerDiv,YTicksPerDiv]=major(AxesH);

if nargin == 0;
   AxesH = gca;
end;

XEnds=get(AxesH,'xlim');
xmin=XEnds(1);
xmax=XEnds(2);

Xtens = floor(log10(xmax-xmin));  % Xtens = order of mag of delta beteen xmin and xmax 
%Xtens = nint(log10(xmax-xmin));  % Xtens = order of mag of delta beteen xmin and xmax 
Xmsd=(xmax-xmin)/(10^Xtens);     % Xmsd = range of most sig variable dig of X
iXmsd=round(Xmsd*10);

switch iXmsd
   case 100;
     xinc = 1*(10^Xtens);
     XTicksPerDiv = 5;
   case 90;
      xinc = 1*(10^Xtens);
      XTicksPerDiv = 5;   
   case 80;
      xinc = 1*(10^Xtens);
      XTicksPerDiv = 5;
   case 70;
      xinc = 1*(10^Xtens);
      XTicksPerDiv = 5;
   case 60;
      xinc = 1*(10^Xtens);
      XTicksPerDiv = 5;
   case 50;
      xinc = .5*(10^Xtens);
      XTicksPerDiv = 5;
   case 40;
      xinc = .5*(10^Xtens);
      XTicksPerDiv = 5;
   case 30;
      xinc = .5*(10^Xtens);
      XTicksPerDiv = 5;
   case 20;
      xinc = .2*(10^Xtens);
      XTicksPerDiv = 4;
   case 10;
      xinc = .1*(10^Xtens);
      XTicksPerDiv = 5;
   case 12;
      xinc = .1*(10^Xtens);
      XTicksPerDiv = 5;
   case 14;
      xinc = .1*(10^Xtens);
      XTicksPerDiv = 5;
   case 15;
      xinc = .25*(10^Xtens);
      XTicksPerDiv = 5;
   case 16;
      xinc = .2*(10^Xtens);
      XTicksPerDiv = 4;
   case 18;
      xinc = .2*(10^Xtens);
      XTicksPerDiv = 4;
   case 22;
      xinc = .2*(10^Xtens);
      XTicksPerDiv = 4;
   case 24;
      xinc = .4*(10^Xtens);
      XTicksPerDiv = 4;
   otherwise
      XmsdFrac = Xmsd-fix(Xmsd);
      XmsdInt = fix(Xmsd);
      if rem(Xmsd,XmsdFrac) == 0;
         xinc = XmsdFrac*(10^Xtens);
         XTicksPerDiv = 5;
      else
         error('major.m error:  Try to manually set Xlim axes property.');
      end;   
end;   

YEnds=get(AxesH,'ylim');
ymin=YEnds(1);
ymax=YEnds(2);

Ytens = floor(log10(ymax-ymin)); % Ytens = order of mag of delta beteen ymin and ymax 
Ymsd=(ymax-ymin)/(10^Ytens);    % Ymsd = range of most sig variable dig of Y
iYmsd=round(Ymsd*10);

switch iYmsd
   case 100;
      yinc = 1*(10^Ytens);
      YTicksPerDiv = 5;
   case 90;
      yinc = 1*(10^Ytens);
      YTicksPerDiv = 5;
   case 80;
      yinc = 1*(10^Ytens);
      YTicksPerDiv = 5;
   case 70;
      yinc = 1*(10^Ytens);
      YTicksPerDiv = 5;
   case 60;
      yinc = 1*(10^Ytens);
      YTicksPerDiv = 5;
   case 50;
      yinc = .5*(10^Ytens);
      YTicksPerDiv = 5;
   case 40;
      yinc = .5*(10^Ytens);
      YTicksPerDiv = 5;
   case 30;
      yinc = .5*(10^Ytens);
      YTicksPerDiv = 5;
   case 20;
      yinc = .2*(10^Ytens);
      YTicksPerDiv = 4;
   case 10;
      yinc = .1*(10^Ytens);
      YTicksPerDiv = 5;
   case 12;
      yinc = .1*(10^Ytens);
      YTicksPerDiv = 5;
   case 14;
      yinc = .1*(10^Ytens);
      YTicksPerDiv = 5;
   case 15;
      yinc = .25*(10^Ytens);
      YTicksPerDiv = 5;
   case 16;
      yinc = .2*(10^Ytens);
      YTicksPerDiv = 4;
   case 18;
      yinc = .2*(10^Ytens);
      YTicksPerDiv = 4;
   case 22;
      yinc = .2*(10^Ytens);
      YTicksPerDiv = 4;
   case 24;
      yinc = .4*(10^Ytens);
      YTicksPerDiv = 4;
   otherwise
      YmsdFrac = Ymsd-fix(Ymsd);
      YmsdInt = fix(Ymsd);
      if rem(Ymsd,YmsdFrac) == 0;
         yinc = YmsdFrac*(10^Ytens);
      YTicksPerDiv = 5;         
      else
         error('major.m error:  Try to manually set Ylim axes property.');
      end;   
end;   

% Make sure 0 is on a major tick mark if range spans 0
if (sign(xmin) == -sign(xmax)) & (mod(xmin,xinc) ~= 0);
  xmin = xmin - xinc/2;
  xmax = xmax + xinc/2;
end;
if (sign(ymin) == -sign(ymax)) & (mod(ymin,yinc) ~= 0);
  ymin = ymin - yinc/2;
  ymax = ymax + yinc/2;
end;

set(AxesH,'xlim',[xmin,xmax]);
set(AxesH,'Xtick',[xmin:xinc:xmax]);

set(AxesH,'ylim',[ymin,ymax]);
set(AxesH,'ytick',[ymin:yinc:ymax]);