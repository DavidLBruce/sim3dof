% getColorList.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function colorList = getColorList

  idx = 1;
  
  Blue      = [0.0,   0.0,   1.0]; colorList(idx,:) = Blue;      idx = idx + 1;
  Red       = [1.0,   0.0,   0.0]; colorList(idx,:) = Red;       idx = idx + 1;
  Green     = [0.2,   0.7,   0.0]; colorList(idx,:) = Green;     idx = idx + 1;
  Magenta   = [1.0,   0.0,   1.0]; colorList(idx,:) = Magenta;   idx = idx + 1;
  Cyan      = [0.0,   1.0,   1.0]; colorList(idx,:) = Cyan;      idx = idx + 1;
  Brown     = [0.5,   0.0,   0.0]; colorList(idx,:) = Brown;     idx = idx + 1;
  Salmon    = [1.0,   0.4,   0.6]; colorList(idx,:) = Salmon;    idx = idx + 1;
  Orange    = [1.0,   0.6,   0.0]; colorList(idx,:) = Orange;    idx = idx + 1;
  Fusia     = [0.7,   0.0,   0.8]; colorList(idx,:) = Fusia;     idx = idx + 1;
  Grey      = [0.5,   0.5,   0.5]; colorList(idx,:) = Grey;      idx = idx + 1;
  Rust      = [0.8,   0.0,   0.0]; colorList(idx,:) = Rust;      idx = idx + 1;
  LightBlue = [0.0,   0.5,   1.0]; colorList(idx,:) = LightBlue; idx = idx + 1;
  Yellow    = [0.949, 0.949, 0.0]; colorList(idx,:) = Yellow;    idx = idx + 1;
  LimeGreen = [0.0,   1.0,   0.0]; colorList(idx,:) = LimeGreen; idx = idx + 1;
  Black     = [0.0    0.0    0.0]; colorList(idx,:) = Black;     idx = idx + 1;
  White     = [1.0    1.0    1.0]; colorList(idx,:) = White;     idx = idx + 1;

%  colorList   = [Blue; Red; Green; Magenta; Lime_Green; Cyan; Yellow; ...
%                 Brown; Salmon; Orange; Fusia; Light_Blue; Grey; Rust];


return;
