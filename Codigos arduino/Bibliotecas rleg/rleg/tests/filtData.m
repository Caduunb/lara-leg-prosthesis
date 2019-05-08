function filt_output = filtData(data,fs,cf,n)

%Filter low frequency data applying a low-pass butterworth filter

%Inputs:
% data: input data to filter
% fs: sampling frequency in Hz
% fs: cut-off frequncy in Hz
% n: filter order
[b, a] = butter(n,2*cf/fs ,'low');
filt_output =  filtfilt(b,a,data);


end