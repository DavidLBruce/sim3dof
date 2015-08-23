% atmosSI.m
%
% Overview:
%
%  Standard 1976 atmosphere using MKS SI unitis
%
% Usage:
%
%  [rho, Vsound, press] = atmosSI(alt);
%
% References:
%
%   Anderson, J. D., Introduction to Flight, 4th ed.,McGraw Hill, 2000
%
% Note:  All units are SI.  Temperatures in K.  Altitudes are geopential
% (geodetic).
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [rho, Vsound, press] = atmosSI(alt)

  gee = 9.8067;           % m/s^2 Acceleration of gravity at sea level
  RR = 287.0;             % J/kg-K Specific gas constant for air
  lambda = 1.4;           % cp / cv Specific heat ratio for air
                          %  (constant pressure, constant volume)


  TseaLvl = 288.16;       % Sea level temperature
  PseaLvl = 1.01325e5;    % Sea level pressure
  rhoSeaLvl = 1.2250;     % Sea level density
  lapseRateSL = -6.5e-3;  % Sea level lapseRate

  %
  % Sea level
  %
  alt0 = 0.0;
  T0 = TseaLvl;
  P0 = PseaLvl;
  rho0 = rhoSeaLvl;
  lapseRate0 = lapseRateSL;

  %
  % 1st breakpoint.  End of temperature gradient region
  %
  alt1 = 11.0e3;
  T1 = 216.66;

  P1 = pressGrad(T1, P0, T0, lapseRate0);  % pressure at 11.0e3

  rho1 = densGrad(T1, rho0, T0, lapseRate0);

  %
  % 2nd breakpoint.  End of isothermal region
  %
  alt2 = 25.0e3;
  T2 = T1;
  lapseRate2 = 3.0e-3;

  P2 = pressIso(T2, alt2, P1, alt1);  % pressure at 25.0e3

  rho2 = densIso(T2, alt2, rho1, alt1);

  %
  % 3rd breakpoint.  End of gradient.
  %
  alt3 = 47.0e3;
  T3 = 282.66;

  P3 = pressGrad(T3, P2, T2, lapseRate2);  % pressure at 47.0e3

  rho3 = densGrad(T3, rho2, T2, lapseRate2);

  %
  % 4th breakpoint.  End of isothermal
  %
  alt4 = 53.0e3;
  T4 = T3;
  lapseRate4 = -4.5e-3;

  P4 = pressIso(T4, alt4, P3, alt3);  % pressure at 53.0e3

  rho4 = densIso(T4, alt4, rho3, alt3);

  %
  % 5th breakpoint.  End of gradient
  %
  alt5 = 79.0e3;
  T5 = 165.66;
  
  P5 = pressGrad(T5, P4, T4, lapseRate4);  % pressure at 79.0e3

  rho5 = densGrad(T5, rho4, T4, lapseRate4);

  %
  % 6th breakpoint.  End of isothermal
  %
  alt6 = 90.0e3;
  T6 = T5;
  lapseRate6 = 4.0e-3;

  P6 = pressIso(T6, alt6, P5, alt5);

  rho6 = densIso(T6, alt6, rho5, alt5);

  %
  % 7th breakpoint.  End of gradient
  %
  alt7 = 105.0e3;
  T7 = 225.66;

  P7 = pressGrad(T7, P6, T6, lapseRate6);  % pressure at 105.0e3

  rho7 = densGrad(T7, rho6, T6, lapseRate6);

  if (alt < alt1)

    %
    % 1st gradient region
    %

    temp = T0 + lapseRate0 * (alt - alt0);

    press = pressGrad(temp, P0, T0, lapseRate0);

    rho = densGrad(temp, rho0, T0, lapseRate0);

  elseif (alt < alt2)

    %
    % 1st isothermal region
    %
    temp = T1;

    press = pressIso(temp, alt, P1, alt1);

    rho = densIso(temp, alt, rho1, alt1);

  elseif (alt < alt3)

   %
   % 2nd gradient
   %
   temp = T2 + lapseRate2 * (alt - alt2);

   press = pressGrad(temp, P2, T2, lapseRate2);

   rho = densGrad(temp, rho2, T2, lapseRate2);

  elseif (alt < alt4)

    %
    % 2nd isothermal
    %
    temp = T3;

    press = pressIso(temp, alt, P3, alt3);

    rho = densIso(temp, alt, rho3, alt3);
 
  elseif (alt < alt5)

    %
    % 3rd gradient
    %
    temp = T4 + lapseRate4 * (alt - alt4);

    press = pressGrad(temp, P4, T4, lapseRate4);

    rho = densGrad(temp, rho4, T4, lapseRate4);

  elseif (alt < alt6)

    %
    % 3rd isothermal
    %
    temp = T5;
   
    press = pressIso(temp, alt, P5, alt5);

    rho = densIso(temp, alt, rho5, alt5);

  elseif (alt < alt7)

    %
    % 4th gradient
    %
    temp = T6 + lapseRate6 * (alt - alt6);

    press = pressGrad(temp, P6, T6, lapseRate6);

    rho = densGrad(temp, rho6, T6, lapseRate6);

  else
    temp = 0.0;  % force invalid Vsound
    press = 0.0;
    rho = 0.0;
  end;

  Vsound = sqrt(lambda * RR * temp);

return;

%
% Pressure in thermal gradiant region
%
function Pout = pressGrad(Tin, Pref, Tref, lapseRate)
  gee = 9.8067;         % m/sec^2 Acceleratio of gravity
  RR = 287.0;           % J/kg-K Specific gas constant for air

  Pout = Pref * (Tin / Tref)^(-gee / (lapseRate * RR));

return;

%
% Pressure in isothermal region
%
function Pout = pressIso(Tin, altIn, Pref, altRef)
  gee = 9.8067;         % m/sec^2 acceleratio of gravity
  RR = 287.0;           % J/kg-K specific gas constant for air

  Pout = Pref * exp(-gee * (altIn - altRef) / (RR * Tin));

return;

%
% Density in thermal gradiant region
%
function rhoOut = densGrad(Tin, rhoRef, Tref, lapseRate)
  gee = 9.8067;         % m/sec^2 acceleratio of gravity
  RR = 287.0;           % J/kg-K specific gas constant for air

  rhoOut = rhoRef * (Tin / Tref)^-(gee / (lapseRate * RR) + 1);

return;

%
% Density in isothermal region
%
function rhoOut = densIso(Tin, altIn, rhoRef, altRef)
  gee = 9.8067;         % m/sec^2 acceleratio of gravity
  RR = 287.0;           % J/kg-K specific gas constant for air

  rhoOut = rhoRef * exp(-gee * (altIn - altRef) / (RR * Tin));

return;



