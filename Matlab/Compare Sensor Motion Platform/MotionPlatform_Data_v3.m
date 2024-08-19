% Specify the file paths for the two CSV files
file1 = '../Data/motion_data.csv';  % File 1
file2 = '../Data/Motion_Measurement.csv';  % File 2

% Read data from the CSV file
data = readtable(file1);
data.Properties.VariableNames = {'Time', 'Roll (rad)', 'Pitch (rad)', 'Yaw (rad)', 'Heave', 'Sway', 'Surge', 'fkRoll', 'fkPitch', 'fkYaw', 'src'};

% Read data from the second CSV file
data2 = readtable(file2);
data2.Properties.VariableNames = {'Timestamp','Roll(deg)','Pitch(deg)','Yaw(deg)'}; %, 'Roll (rad)', 'Pitch (rad)', 'Yaw (rad)'};

tt_data = table2timetable(data, 'RowTimes', 'Time');
tt_data2 = table2timetable(data2, 'RowTimes', 'Timestamp');

tt = synchronize(tt_data,tt_data2);
tt = removevars(tt, 'src');

tt2 = rmmissing(tt);

% Extract relevant data
roll = rad2deg(tt.('Roll (rad)'));
pitch = rad2deg(tt.('Pitch (rad)'));
yaw = rad2deg(tt.('Yaw (rad)'));
fkroll = rad2deg(tt.('fkRoll'));
fkpitch = rad2deg(tt.('fkPitch'));
fkyaw = rad2deg(tt.('fkYaw'));

%time2 = datenum(tt.('Timestamp'));  % Convert the time string
roll2 = tt.('Roll(deg)');
pitch2 = tt.('Pitch(deg)');
yaw2 = tt.('Yaw(deg)');
roll3 = rad2deg(tt.('Roll (rad)'));
pitch3 = rad2deg(tt.('Pitch (rad)'));
yaw3 = rad2deg(tt.('Yaw (rad)'));

% Plot both files over time
figure;

% Subplot 1: Roll and FKRoll from File 1
plot(tt.Time, roll, 'r', 'LineWidth', 0.5);
hold on;
%plot(time, roll, 'b--', 'LineWidth', 0.5);
plot(tt.Time, roll2, 'g', 'LineWidth', 0.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('fkRoll and Roll (IMU)');
legend('fkRoll', 'Roll(IMU)');
hold off 

%%
% Subplot 2: Pitch and FKPitch from File 1
plot(tt.Time, fkpitch, 'b', 'LineWidth', 1.5);
hold on;
plot(tt.Time, pitch, 'g--', 'LineWidth', 1.5);
plot(tt.Time, roll2, 'm', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('fkPitch and Pitch (IMU)');
legend('fkPitch1', 'Pitch(IMU)');


%plot appearance
set(gcf, 'Position', [100, 100, 1200, 800]);
grid on;
datetick('x', 'mm:ss', 'keepticks', 'keeplimits');
