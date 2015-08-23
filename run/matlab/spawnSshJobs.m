% spawnSshJobs.m
%
% Overview:
%
%  Send a job to one of a list of Linux machines via ssh.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function spawnSshJobs( cmdVec, statusDir, hostList )

  defaultList(1).name = 'ec2364';
  defaultList(1).maxRuns = 10;
  defaultList(2).name = 'ec2393';
  defaultList(2).maxRuns = 12;
  defaultList(3).name = 'ec2591';
  defaultList(3).maxRuns = 12;
  defaultList(4).name = 'tus-session01';
  defaultList(4).maxRuns = 12;
  defaultList(5).name = 'tus-session02';
  defaultList(5).maxRuns = 12;
  defaultList(6).name = 'tus-session03';
  defaultList(6).maxRuns = 12;

%  defaultList(3).name = 'ec2590';
%  defaultList(3).maxRuns = 8;

  switch nargin
    case {1}
      statusDir = 'runningJobs';
      hostList = defaultList;
    case {2}
      if ( ischar(statusDir) )
        hostList = defaultList;
      else
        hostList = statusDir;
        statusDir = 'runningJobs';
      end;
    otherwise
  end;

  startDelay = 0.1;
  
  recheckDelay = 1;

  runDir = pwd;

  cmd = ['rm -rf ', statusDir];
  unix(cmd);

  cmd = [ 'mkdir -p ', statusDir ];
  unix(cmd);

  for hostIdx = 1 : length(hostList)
    hostList(hostIdx).waiting = false;
    hostList(hostIdx).checkQty = true;
    hostList(hostIdx).printEn = true;
  end;

  qtyRunning = 0;

  hostIdx = 0;
  cmdIdx = 1;
  done = false;
  while (~done )   
    hostIdx = hostIdx + 1;

    if ( hostIdx > length(hostList) )
      hostIdx = 1;
    end;

    if ( hostList(hostIdx).checkQty | qtyRunning < hostList(hostIdx).maxRuns )

      %
      % The goSsh.sh creates a file in the runningJobs directory just before
      % it calls the sim executable and then deletes it when the sim completes.
      % It puts the machine name in the name of the running file.  First, handle
      % the problem of the session server aliases.
      %
      idx = strfind( hostList(hostIdx).name, '-');
      if ( isempty(idx) )
        searchStr = hostList(hostIdx).name;
      else
        searchStr = hostList(hostIdx).name(idx+1 : end);
      end;

      cmd = sprintf( 'ls %s | grep -c %s', statusDir, searchStr );
      [sts, res] = unix(cmd);

      qtyRunning = str2num( res );

      if ( hostList(hostIdx).printEn | qtyRunning < hostList(hostIdx).maxRuns )

        fprintf( 'Number of jobs running on %s = %d \n', ...
                  hostList(hostIdx).name, qtyRunning );

        hostList(hostIdx).printEn = false;

      end;
    end;

    if ( qtyRunning >= hostList(hostIdx).maxRuns )

      curTime = fix(datevec(now));

      if ( ~hostList(hostIdx).waiting )

        hostList(hostIdx).checkQtyTime = curTime + [zeros(1,5), recheckDelay];

        hostList(hostIdx).waiting = true;

        hostList(hostIdx).checkQty = false;

      elseif ( datenum(curTime) >= datenum(hostList(hostIdx).checkQtyTime) )

        hostList(hostIdx).checkQty = true;

      end;

    else

      hostList(hostIdx).waiting = false;

      hostList(hostIdx).checkQty = true;

      hostList(hostIdx).printEn = true;

    end;

    if ( ~hostList(hostIdx).waiting )

      cmdStr = cmdVec{cmdIdx};
      disp(cmdStr);

      %
      % Execute the sim on a remote host
      %
      cmd = sprintf( 'ssh %s bash %s &', hostList(hostIdx).name, cmdStr );
      disp(cmd);
      [sts,res] = system(cmd);

      %
      % Give the job time to start up.
      %
      fprintf('Command index = %d \n', cmdIdx);
      pause( startDelay );

      if ( cmdIdx < length(cmdVec) )
        cmdIdx = cmdIdx + 1;
      else
        done = true;
      end;
    end;
  end;

return;
