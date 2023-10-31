% Specify the file paths for the two CSV files
file1 = 'motion_data1.csv';  % File 1
file2 = 'motion_data2.csv';  % File 2

% Read data from the first CSV file
data = readtable(file1);
data.Properties.VariableNames = {'Time', 'Roll (rad)', 'Pitch (rad)', 'Yaw (rad)', 'Heave', 'Sway', 'Surge', 'fkRoll', 'fkPitch', 'fkYaw', 'src'};

% Read data from the second CSV file
data2 = readtable(file2, 'VariableNames', {'Time2', 'Roll2_radians', 'Pitch2_radians', 'FKRoll2_deg', 'FKPitch2_deg'});
data2.Properties.VariableNames = {'Time', 'Roll', 'Pitch', 'Yaw'};


% Extract relevant data
time = datenum(data.('Time'));  % Convert the time string
roll = rad2deg(data.('Roll (rad)'));
pitch = rad2deg(data.('Pitch (rad)'));
yaw = rad2deg(data.('Yaw (rad)'));
fkroll = rad2deg(data.('fkRoll'));
fkpitch = rad2deg(data.('fkPitch'));
fkyaw = rad2deg(data.('fkYaw'));


time2 = datenum(data2.('Time'));  % Convert the time string
roll2 = rad2deg(data2.('Roll'));
pitch2 = rad2deg(data2.('Pitch'));
yaw2 = rad2deg(data2.('Yaw'));


% Plot roll and fkroll from both files over time
figure;

% Subplot 1: Roll and FKRoll from File 1
plot(time, fkroll, 'r', 'LineWidth', 1.5);
hold on;
plot(time2, roll2, 'g', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('fkRoll and Roll (IMU)');
legend('fkRoll', 'Roll(IMU)');
hold off 

%%
% Subplot 2: Pitch and FKPitch from File 1
plot(time, fkpitch, 'b', 'LineWidth', 1.5);
hold on;
plot(time2, pitch2, 'm', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('fkPitch and Pitch (IMU)');
legend('fkPitch1', 'Pitch(IMU)');


%plot appearance
set(gcf, 'Position', [100, 100, 1200, 800]);
grid on;
datetick('x', 'mm:ss', 'keepticks', 'keeplimits');
