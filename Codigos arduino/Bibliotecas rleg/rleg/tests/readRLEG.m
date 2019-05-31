%readRLEG.m

%Description: this script reads the .txt files (RLEG measurements on-board) and the
%.mat files (wireless adquisition) from the experiments.

%velocity values
%speed1: 1.7 mph = 0.76 m/s (normal velocity)
%speed2: 2.1 mph = 0.94 m/s (1.25 normal velocity)
%speed3: 2.4 mph = 1.07 m/s (1.4 normal velocity)


close all;
clear all;
clc;

basedir ='/Users/claudiapodiaz/RLEG2014/lib/relg/tests';                     % github folder path
addpath(genpath(basedir))


% reading the .txt files
%data format:
% encoder|ax|ay|az|dtheta_x|dtheta_y|dtheta_z|

fs=1/10e-3;
static1 = importdata('estatico1.TXT',',');
static2 = importdata('estatico2.TXT',',');

speed1_trial1 = importdata('trial1.TXT',',');
speed1_trial2 = importdata('trial2.TXT',',');
speed1_trial3 = importdata('trial3.TXT',',');

%rejecting the first five gait cycles

[max, min] = peakdet(speed1_trial1(:,1), 50);                                % the maximum angle is considered to be above 50 degrees

min = min(6:end,:);
speed1_trial1 = speed1_trial1(min(1):end,:);
min(:,1)= min(:,1)-min(1);
min(1,1)=1; 

plot(speed1_trial1(min(1):min(6),1))                                             % five consecutive steps
ylabel('knee angle')
xlabel('samples')

%% filtering the accel and the gyro information

cf = 10;                                                                         % cut-off frequency
a_speed1_t1 =  filtData(speed1_trial1(:,2:4),fs,cf,2);

figure;plot(a_speed1_t1(min(1):min(6),1))                                        % five consecutive steps
ylabel('a_x')
xlabel('samples')

figure;plot(a_speed1_t1(min(1):min(6),2))                                        % five consecutive steps
ylabel('a_y')
xlabel('samples')

figure;plot(a_speed1_t1(min(1):min(6),3))                                        % five consecutive steps
ylabel('a_z')
xlabel('samples')


% now gyro information
cf = 10;                                                                         % cut-off frequency
gyro_speed1_t1 =  filtData(speed1_trial1(:,5:end),fs,cf,2);


figure;plot(gyro_speed1_t1(min(1):min(6),1))                                     % five consecutive steps
ylabel('gyro_x')
xlabel('samples')

figure;plot(gyro_speed1_t1(min(1):min(6),2))                                     % five consecutive steps
ylabel('gyro_y')
xlabel('samples')

figure;plot(gyro_speed1_t1(min(1):min(6),3))                                      % five consecutive steps
ylabel('gyro_z')
xlabel('samples')


% plotting the angle information with the accel info to see if the peaks
% detected by the IMU corresponds to heel strikes
five_gc = 1:length(a_speed1_t1(min(1):min(6)));
angle = speed1_trial1(min(1):min(6),1);
a_z = a_speed1_t1(min(1):min(6),3);
[axes,h1,h2] = plotyy(five_gc,angle,five_gc,a_z);
set(h1,'color','b')
set(h2,'color','r')
set(get(axes(1),'Ylabel'),'String','angle') 
set(get(axes(2),'Ylabel'),'String','a_z') 
grid on

