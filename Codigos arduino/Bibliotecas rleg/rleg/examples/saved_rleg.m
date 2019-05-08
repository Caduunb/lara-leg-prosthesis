x = 1:size(saved_data,2);
y = saved_data;
[AX,H1,H2] = plotyy(x, saved_data(1,:), [x' x' x'], [y(2,:)' y(3,:)' y(4,:)']);           
%axis([0 N 0 90]);
% set(AX(1),'ylim',[0 90]);
% set(AX(2),'ylim',[-2 2]);
hold on
plot(times(2:end),saved_data(1,times(2:end)),'xr')