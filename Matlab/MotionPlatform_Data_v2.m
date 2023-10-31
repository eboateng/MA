% Specify the file paths for the two CSV files
file1 = '../Data/motion_data.csv';  % File 1
file2 = '../Data/Motion_Measurement2.csv';  % File 2

% Read data from the CSV file
data = readtable(file1);
data.Properties.VariableNames = {'Time', 'Roll (rad)', 'Pitch (rad)', 'Yaw (rad)', 'Heave', 'Sway', 'Surge', 'fkRoll', 'fkPitch', 'fkYaw', 'src'};

% Read data from the second CSV file
data2 = readtable(file2);
data2.Properties.VariableNames = {'Timestamp','Roll(deg)','Pitch(deg)','Yaw(deg)', 'Roll (rad)', 'Pitch (rad)', 'Yaw (rad)'};


% Extract relevant data
time = datenum(data.('Time'));  % Convert the time string
roll = rad2deg(data.('Roll (rad)'));
pitch = rad2deg(data.('Pitch (rad)'));
yaw = rad2deg(data.('Yaw (rad)'));
fkroll = rad2deg(data.('fkRoll'));
fkpitch = rad2deg(data.('fkPitch'));
fkyaw = rad2deg(data.('fkYaw'));

time2 = datenum(data2.('Timestamp'));  % Convert the time string
roll2 = data2.('Roll(deg)');
pitch2 = data2.('Pitch(deg)');
yaw2 = data2.('Yaw(deg)');
roll3 = rad2deg(data2.('Roll (rad)'));
pitch3 = rad2deg(data2.('Pitch (rad)'));
yaw3 = rad2deg(data2.('Yaw (rad)'));

% try to synchromize timestamps
newTable = synchronize(time, time2,'commonrange', 'linear')
% Plot both files over time
figure;

% Subplot 1: Roll and FKRoll from File 1
plot(time, roll, 'r', 'LineWidth', 0.5);
hold on;
%plot(time, roll, 'b--', 'LineWidth', 0.5);
plot(time2, pitch2, 'g', 'LineWidth', 0.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('fkRoll and Roll (IMU)');
legend('fkRoll', 'Roll(IMU)');
hold off 

%%
% Subplot 2: Pitch and FKPitch from File 1
plot(time, fkpitch, 'b', 'LineWidth', 1.5);
hold on;
plot(time, pitch, 'g--', 'LineWidth', 1.5);
plot(time2, roll2, 'm', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('fkPitch and Pitch (IMU)');
legend('fkPitch1', 'Pitch(IMU)');


%plot appearance
set(gcf, 'Position', [100, 100, 1200, 800]);
grid on;
datetick('x', 'mm:ss', 'keepticks', 'keeplimits');
