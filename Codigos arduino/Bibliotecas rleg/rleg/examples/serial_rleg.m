%% serial_rleg.m
%% Reads data from USB serial. Port name must
%% be set on code, as well as other parameters.
%% s.InputBufferSize must be of minimum size
%% to fit input data. fscanf() must also be
%% adapted to input stream variables.
%% At last, plot variables must be defined
%% accordingly to what user means to graph.

function serial_rleg()
    % Limpando objetos antigos (port kidnapping)
    %newobjs = instrfind ; fclose(newobjs);

    clear all;
    clc;
    
    N = 100;
    i = 1;
    global time;
    time = 1;
    global times;
    times = 0;
    plot_period = 0;

    % Defines variables for plotting
    global samples;
    samples = 1:N;
    plot_data = NaN*zeros(7,N); % Number of variables to show

    % Create plot window, sets title and create stop button
    plot_window = figure;
    set(plot_window, 'Name', 'RLEG');
    set(plot_window, 'NumberTitle','off');
    uicontrol('Style', 'pushbutton', 'String', 'STOP', 'Units', 'normalized', 'Position', [.3 .0 .15 .1], 'Callback', {@stopEvent});
    uicontrol('Style', 'pushbutton', 'String', 'Heel Strike!', 'Units', 'normalized', 'Position', [.55 .0 .15 .1], 'Callback', {@saveTime});

    % Creates serial communication global 
    % variable and sets communication parameters
    global s;
    s = serial('/dev/cu.usbserial-A501DFDD');
    set(s, 'BaudRate', 57600);
    set(s, 'InputBufferSize', 1024);
    set(s, 'FlowControl', 'hardware');
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 10);

    % Sets buffer size (for thresh_control.ino)
    % and opens communication
    fopen(s);

    % Include line below in order to also
    % save data on file
    now = clock;
    global file_name;
    file_name = sprintf('teste_%02d%02d%d_%02d_%02d_%02.0f.mat', now(3), now(2), now(1), now(4), now(5), now(6));
    
    % Creates output stream file
    global saved_data;

    % Receiving data
    global keep_plotting;
    keep_plotting = 1;
    while(keep_plotting)          
        % Remember to set fscanf() accordingly
        % to input stream format
        %%%
        % periodic_task:
        RX = fscanf(s, '%f,%f,%f,%f,%f,%f,%f'); % Change this for variable format
        % Active:
        %RX = fscanf(s, '%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d');
        %fprintf(txt, '%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d', RX);
        % sensorPrint:
        %RX = fscanf(s, '%f,%f,%f,%f');
        %fprintf(txt, '%f,%f,%f,%f\n', RX);
        % first_control:
        %RX = fscanf(s, '%f,%f,%f,%f,%f');
        %fprintf(txt, '%f,%f,%f,%f,%f\n', RX);
        %%%
        
        check_size = size(RX) ~= size(plot_data(:,1));

        % Check if input data is of the right size
        if(check_size(1) == 1 || check_size(2) == 1)
           RX = NaN*ones(7,1); 
        end
        
        % Fills plot array until it reaches
        % maximum size
        if(i < N)
          plot_data(:,i) = RX;
          i = i + 1;
        % Shifts plotting array to the left
        else
          plot_data(:,1:N-1) = plot_data(:,2:N);
          plot_data(:,N) = RX;
        end
        
        saved_data = [saved_data RX];

        time = time+1;
        
        % Plots current array format and
        % force to draw on figure now
        plot_period = plot_period + 1;
        
        if(plot_period == 20) % Sampling period is here!
            [AX,H1,H2] = plotyy(samples, plot_data(1,:), [samples' samples' samples'], [plot_data(2,:)' plot_data(3,:)' plot_data(4,:)']); % Change this too
            plot_period = 0;
            axis([0 N 0 90]);
            set(AX(1),'ylim',[0 200]);
            set(AX(2),'ylim',[-2 2]); 
        end
        
        drawnow;
    end    
end

% Closes all communication (so serial
% port won't be stuck)
function stopEvent(source, eventdata)
    global keep_plotting;
    keep_plotting = 0;
    close all;
    global s;
    fclose(s);

    global samples;
    global saved_data;
    global times;
    global file_name;
    save(file_name);
end

function saveTime(source, eventdata)
    global time;
    global times;
    times = [times time];
end