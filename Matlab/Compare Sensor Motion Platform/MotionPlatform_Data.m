% Read the CSV file into a table

data = readtable('../motion_data.csv');

data.Properties.VariableNames = {'Time', 'Roll (rad)', 'Pitch (rad)', 'Yaw (rad)', 'Heave', 'Sway', 'Surge', 'fkRoll', 'fkPitch', 'fkYaw', 'src'};

% Extract relevant data
time = datenum(data.('Time'));  % Convert the time string
roll = rad2deg(data.('Roll (rad)'));
pitch = rad2deg(data.('Pitch (rad)'));
yaw = rad2deg(data.('Yaw (rad)'));
fkroll = rad2deg(data.('fkRoll'));
fkpitch = rad2deg(data.('fkPitch'));
fkyaw = rad2deg(data.('fkYaw'));

% Plot roll, fkroll
figure;
plot(time, roll, 'r', 'LineWidth', 0.5);
hold on;
plot(time, fkroll, 'g--', 'LineWidth', 0.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('Roll and fkRoll');
legend('Roll', 'fkRoll');
hold off;

%%
% Plot pitch and fkpitch
plot(time, pitch, 'r', 'LineWidth', 1.5);
hold on;
plot(time, fkpitch, 'g--', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Angle (radians/degrees)');
title('Pitch and fkPitch');
legend('Pitch and fkPitch');
hold off;

% Customize the plot appearance
set(gcf, 'Position', [100, 100, 1000, 600]);
grid on;
datetick('x', 'mm:ss', 'keepticks', 'keeplimits');
ylim([-15 15]);  % Adjust the y-axis limits as needed
