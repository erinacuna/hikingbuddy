clear; close all; clc;

%% data
data = importdata("fastreading.txt", ';'); %import data
time = data(:,1)-data(1,1);
time =time/1000;
intv = mean(diff(time)); % 0.0050 seconds

gFx = data(:,2);
meanFx = data(:,3);
gFy = data(:,4);
meanFy = data(:,5);
gFz = data(:,6);
meanFz = data(:,7);
rms = data(:,8);

figure(1)
subplot(3,1,1)
plot(time, gFx, 'r'); hold on;
ylim([min(gFx)-1, max(gFx)+1]);
xlim([0, 10])
title('Fast Walking Raw X Acceleration Data')
xlabel('Time (s)');
ylabel('Acceleration (m/s^2)');

subplot(3,1,2)
plot(time, gFy, 'g'); hold on;
ylim([min(gFx)-1, max(gFx)+1]);
xlim([0, 10])
title('Fast Walking Raw Y Acceleration Data')
xlabel('Time (s)');
ylabel('Acceleration (m/s^2)');

subplot(3,1,3)
plot(time, gFz, 'b'); hold on;
ylim([min(gFx)-1, max(gFx)+1]);
xlim([0, 10])
title('Fast Walking Raw Z Acceleration Data')
xlabel('Time (s)');
ylabel('Acceleration (m/s^2)');

axis tight;
grid on;
legend('x_a_c_c','y_a_c_c','z_a_c_c');
title('Fast Walking Raw Acceleration Data')
xlabel('Time (s)');
ylabel('Acceleration (m/s^2)');
hold off;