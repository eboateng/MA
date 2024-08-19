close all
clear all
clc

% Load baseline and compensated videos
baselineVideoFile = './Videos/1_Roll_0_Pitch_0.MOV';
% Define an array of uncompensated video files
uncompensatedVideoFiles = {
    %'./Videos/1_Roll_2_Pitch_0.MOV','./Videos/1_Roll_4_Pitch_0.MOV', './Videos/1_Roll_8_Pitch_0.MOV','./Videos/1_Roll_10_Pitch_0.MOV'
    %'./Videos/1_Roll_8_Pitch_-8.MOV','./Videos/1_Roll_-8_Pitch_8.MOV'
    %'./Videos/2_Roll_0_Pitch_2.MOV','./Videos/2_Roll_0_Pitch_4.MOV', './Videos/2_Roll_0_Pitch_6.MOV','./Videos/2_Roll_0_Pitch_8.MOV'
    './Videos/1_Roll_-2_Pitch_0.MOV','./Videos/1_Roll_-4_Pitch_0.MOV', './Videos/1_Roll_-6_Pitch_0.MOV','./Videos/1_Roll_-8_Pitch_0.MOV'

    };

% Define an array of compensated video files
compensatedVideoFiles = {
    %'./Videos/1c_Roll_2_Pitch_0.MOV','./Videos/1c_Roll_4_Pitch_0.MOV', './Videos/1c_Roll_8_Pitch_0.MOV','./Videos/1c_Roll_10_Pitch_0.MOV'
    %'./Videos/1c_Roll_8_Pitch_-8.MOV','./Videos/1c_Roll_-8_Pitch_8.MOV'
    %'./Videos/2c_Roll_0_Pitch_2.MOV','./Videos/2c_Roll_0_Pitch_4.MOV', './Videos/2c_Roll_0_Pitch_6.MOV','./Videos/2c_Roll_0_Pitch_8.MOV'
    './Videos/1c_Roll_-2_Pitch_0.MOV','./Videos/1c_Roll_-4_Pitch_0.MOV', './Videos/1c_Roll_-6_Pitch_0.MOV','./Videos/1c_Roll_-6_Pitch_0.MOV'

};

% Define an array of names for the legend
legendNames = {
    %'Baseline', 'Roll 2 Pitch 0 (u)', 'Roll 4 Pitch 0 (u)', 'Roll 8 Pitch 0 (u)', 'Roll 10 Pitch 0 (u)', 'Roll 2 Pitch 0 (c)', 'Roll 4 Pitch 0 (c)', 'Roll 8 Pitch 0 (c)', 'Roll 10 Pitch 0 (c)'
    %'Baseline', 'Roll 8 Pitch -8 (u)', 'Roll -8 Pitch 8 (u)', 'Roll 8 Pitch -8 (c)', 'Roll -8 Pitch 8 (c)'
    %'Baseline', 'Roll 0 Pitch 2 (u)', 'Roll 0 Pitch 4 (u)', 'Roll 0 Pitch 6 (u)', 'Roll 0 Pitch 8 (u)', 'Roll 0 Pitch 2 (c)', 'Roll 0 Pitch 4 (c)', 'Roll 0 Pitch 6 (c)', 'Roll 0 Pitch 8 (c)'
    'Baseline', 'Roll -2 Pitch 0 (u)', 'Roll -4 Pitch 0 (u)', 'Roll -6 Pitch 0 (u)', 'Roll -8 Pitch 0 (u)', 'Roll -2 Pitch 0 (c)', 'Roll -4 Pitch 0 (c)', 'Roll -6 Pitch 0 (c)', 'Roll -8 Pitch 0 (c)'
    };

% Load the baseline video

baselineVideo = VideoReader(baselineVideoFile);

% Ensure all videos have the same frame rate and number of frames
frameRateTolerance = 0.1; % Adjust tolerance as needed

% Load the calibration parameters and inspect the contents
calibrationData = load('cameraParams_1_Roll_0_Pitch_0.mat');
disp(calibrationData);

% Assuming the correct variable name in the .mat file is 'cameraParams'
params = calibrationData.cameraParams;

% Initialize arrays to store the laser dot positions for the baseline video
numFramesBaseline = baselineVideo.NumFrames;
laserDotPositionsBaseline = nan(numFramesBaseline, 2);

% Detect and track the laser dot in the baseline video
parfor frameIdx = 1:numFramesBaseline
    frame = read(baselineVideo, frameIdx);
    
    % Undistort the frame
    frame = undistortImage(frame, params);
    
    % Convert frame to grayscale
    grayFrame = rgb2gray(frame);
    
    % Detect the laser dot (assuming it's the brightest spot in the frame)
    laserDotPosition = detectLaserDot(grayFrame);
    
    if ~isempty(laserDotPosition)
        laserDotPositionsBaseline(frameIdx, :) = laserDotPosition;
    end
end

% Convert baseline positions to millimeters using the calibration parameters
laserDotPositionsBaselineMm = pointsToWorld(params.Intrinsics, params.RotationMatrices(:,:,1), params.TranslationVectors(1,:), laserDotPositionsBaseline) / 2.5;

% Extract x and y positions for the baseline
xPositionsBaseline = laserDotPositionsBaselineMm(:, 1);
yPositionsBaseline = laserDotPositionsBaselineMm(:, 2);

% Initialize figures for plotting
figureProjection = figure;
figureDisplacement = figure;

% Initialize legend array
legendEntries = cell(1, length(compensatedVideoFiles) + length(uncompensatedVideoFiles) + 1);

% Plot baseline positions on the projection surface
figure(figureProjection);
set(gcf, 'Position', [100, 100, 800, 600]);
plot(xPositionsBaseline, yPositionsBaseline, 'r+', 'DisplayName', 'Baseline');
legendEntries{1} = legendNames{1};
xlabel('X Position (cm)', 'FontSize', 28);
ylabel('Y Position (cm)', 'FontSize', 28);
%title('Position of the Laser Dot on the Projection Surface');
axis equal;
grid on;
hold on;

% Define colors for the uncompensated and compensated videos
colors = lines(length(compensatedVideoFiles) + length(uncompensatedVideoFiles));

% Process uncompensated videos
processVideoFiles(uncompensatedVideoFiles, 1, 'Uncompensated', params, baselineVideo, numFramesBaseline, laserDotPositionsBaselineMm, legendEntries, colors, figureProjection, figureDisplacement, legendNames, frameRateTolerance);

% Process compensated videos
processVideoFiles(compensatedVideoFiles, length(uncompensatedVideoFiles) + 1, 'Compensated', params, baselineVideo, numFramesBaseline, laserDotPositionsBaselineMm, legendEntries, colors, figureProjection, figureDisplacement, legendNames, frameRateTolerance);

% Finalize plots by adding legends
figure(figureProjection);
set(gcf, 'Position', [100, 100, 800, 600]);
legend('show', 'FontSize', 18); % Display all legends in the figure

figure(figureDisplacement);
legend;
xlabel('Frame Index');
ylabel('Displacement (cm)');
%title('Displacement between Undisturbed and Compensated Paths');
grid on;

% Function to detect the laser dot in an image
function laserDotPosition = detectLaserDot(grayImage)
    % Thresholding to isolate the bright laser spot
    threshold = 250; % Adjust based on laser brightness
    binaryImage = grayImage > threshold;
    
    % Find the centroid of the laser spot
    stats = regionprops(binaryImage, 'Centroid');
    if ~isempty(stats)
        laserDotPosition = stats.Centroid;
    else
        laserDotPosition = [];
    end
end

% Function to process videos and plot positions and displacements
function processVideoFiles(videoFiles, startIndex, type, params, baselineVideo, numFramesBaseline, laserDotPositionsBaselineMm, legendEntries, colors, figureProjection, figureDisplacement, legendNames, frameRateTolerance)
    for k = 1:length(videoFiles)
        % Check if the video file exists
        if ~isfile(videoFiles{k})
            error('The file %s was not found.', videoFiles{k});
        end
        
        % Load the video
        videoFile = videoFiles{k};
        video = VideoReader(videoFile);
        
        % Ensure the video has the same frame rate and number of frames
        assert(abs(baselineVideo.FrameRate - video.FrameRate) < frameRateTolerance, 'Frame rates of the videos do not match within tolerance');
        numFramesVideo = video.NumFrames;
        numFrames = min(numFramesBaseline, numFramesVideo);
        
        % Initialize array to store the laser dot positions for the video
        laserDotPositionsVideo = nan(numFrames, 2);
        
        % Detect and track the laser dot in the video
        parfor frameIdx = 1:numFrames
            frame = read(video, frameIdx);
            
            % Undistort the frame
            frame = undistortImage(frame, params);
            
            % Convert frame to grayscale
            grayFrame = rgb2gray(frame);
            
            % Detect the laser dot (assuming it's the brightest spot in the frame)
            laserDotPosition = detectLaserDot(grayFrame);
            
            if ~isempty(laserDotPosition)
                laserDotPositionsVideo(frameIdx, :) = laserDotPosition;
            end
        end
        
        % Ensure the same number of tracked points
        minLength = min(size(laserDotPositionsBaselineMm, 1), size(laserDotPositionsVideo, 1));
        laserDotPositionsBaselineTruncated = laserDotPositionsBaselineMm(1:minLength, :);
        laserDotPositionsVideo = laserDotPositionsVideo(1:minLength, :);
        
        % Convert video positions to millimeters using the calibration parameters
        laserDotPositionsVideoMm = pointsToWorld(params.Intrinsics, params.RotationMatrices(:,:,1), params.TranslationVectors(1,:), laserDotPositionsVideo) / 2.5;
        
        % Extract x and y positions for the video
        xPositionsVideo = laserDotPositionsVideoMm(:, 1);
        yPositionsVideo = laserDotPositionsVideoMm(:, 2);
        
        % Calculate x and y displacements
        xDisplacement = laserDotPositionsBaselineTruncated(:, 1) - xPositionsVideo;
        yDisplacement = laserDotPositionsBaselineTruncated(:, 2) - yPositionsVideo;
        
        % Determine the marker type based on the type of video
        if strcmp(type, 'Uncompensated')
            markerType = '+';
        else
            markerType = 'o';
        end
        
        % Plot the positions of the laser dot for the video
        figure(figureProjection);
        plot(xPositionsVideo, yPositionsVideo, markerType, 'Color', colors(startIndex + k - 1, :), 'DisplayName', legendNames{startIndex + k});
        legendEntries{startIndex + k} = legendNames{startIndex + k};
        
        % Plot the x and y displacements over frames
        figure(figureDisplacement);
        plot(xDisplacement, 'o', 'Color', colors(startIndex + k - 1, :), 'DisplayName', sprintf('X Displacement %s %d', type, k));
        hold on;
        plot(yDisplacement, 'x', 'Color', colors(startIndex + k - 1, :), 'DisplayName', sprintf('Y Displacement %s %d', type, k));
    end
end