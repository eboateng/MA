%% Calibrate the Camera
% Camera calibration using a checkerboard pattern.
% This script reads a calibration video, detects the checkerboard pattern in frames, 
% and estimates the camera parameters, saving them to a .mat file.

close all;
clear;
clc;

%% Settings
scriptPath = fileparts(mfilename('fullpath'));

% Define the relative path to the folder
relativeFolder = './Videos/';

% Construct the full path to the folder
dir2load = fullfile(scriptPath, relativeFolder);

% Directory to save calibration results
dir2saveCali = fullfile(scriptPath, './Videos/Calib/processing/');

% Ensure the save directory exists
if ~exist(dir2saveCali, 'dir')
    mkdir(dir2saveCali);
end

% Ensure the subdirectory for calibration images exists
caliImagesDir = fullfile(dir2saveCali, 'cali_images');
if ~exist(caliImagesDir, 'dir')
    mkdir(caliImagesDir);
end

% File name of the calibration video
SetBoardSize = [7, 10]; % 7 x 10 checkerboard
SetBoardPointSize = [54, 2]; % Size of the coordinate matrix of the board corner
video_cali_name = '1_Roll_0_Pitch_0';

if ~exist(fullfile(dir2saveCali, ['Cali_', video_cali_name]), 'dir')
    mkdir(fullfile(dir2saveCali, ['Cali_', video_cali_name]));
end

% Number of frames for checking the detection of the checkerboard
NumFrameCheck = 30;  % fps 24 => check one frame per sec

%% Read calibration video
video_cali = VideoReader(fullfile(dir2load, [video_cali_name, '.MOV']));

frame2check_id = 1:floor(video_cali.NumFrames/NumFrameCheck):video_cali.NumFrames; 
disp(video_cali.NumFrames)

% Prepare Calibration Images
imagesPoints_cell = cell(video_cali.NumFrames, 1);
thisfig = figure;
tic
for iframe = 1:video_cali.NumFrames
    clc % clear area
    clear images_cali thisImagePoints 
    disp(iframe) % Keep it alive
    
    images_cali = read(video_cali, iframe);

    % Detect the checkerboard corners in the images
    [thisImagePoints, boardSize] = detectCheckerboardPoints(images_cali);
    
    if (sum(isnan(thisImagePoints), 'all') == 0) && all(boardSize == SetBoardSize)    
        imagesPoints_cell{iframe, 1} = thisImagePoints;
    end

    if ismember(iframe, frame2check_id)
        clear figname
        figname = ['Checkerboard_', video_cali_name, '_idx', num2str(iframe), '.png'];
        if ~exist(fullfile(caliImagesDir, figname), 'file')
            J = insertText(images_cali, thisImagePoints, 1:size(thisImagePoints, 1));
            J = insertMarker(J, thisImagePoints, 'o', 'Color', 'red', 'Size', 5);
            imshow(J);
            title([sprintf('Detected a %d x %d Checkerboard', boardSize), ': ', num2str(iframe)]);
            saveas(gcf, fullfile(caliImagesDir, figname))
            clf(thisfig)
        end
    end
end
toc

% Remove wrong detection frames 
GetBoardPointSize = nan(length(imagesPoints_cell), 2);
[GetBoardPointSize(:, 1), GetBoardPointSize(:, 2)] = cellfun(@size, imagesPoints_cell);
CorrectBoardPointSize = GetBoardPointSize == SetBoardPointSize;
IdxCorrect = all(CorrectBoardPointSize, 2);

imagePoints = cat(3, imagesPoints_cell{IdxCorrect});

clear imagesPoints_cell

%% Generate the world coordinates of the checkerboard corners
squareSize = 25; % in millimeters
worldPoints = generateCheckerboardPoints(SetBoardSize, squareSize);

% Calibrate the camera
imageSize = [video_cali.Height, video_cali.Width];
cameraParams = estimateCameraParameters(imagePoints, worldPoints, 'ImageSize', imageSize);

% Evaluate calibration accuracy
figure; showReprojectionErrors(cameraParams);
title('Reprojection Errors');

% Get the mean errors of each frame
a = get(gca, 'Children');
ydata = get(a, 'YData');
meanErrors = ydata{3};
overallMeanErrors = mean(meanErrors);
idx2remove = meanErrors > overallMeanErrors;

% Remove frames with mean errors over the overall mean errors and recalibrate 
imagePoints(:, :, idx2remove) = [];
clear cameraParams 
[cameraParams, ~, estimationErrors] = estimateCameraParameters(imagePoints, worldPoints, 'ImageSize', imageSize);

displayErrors(estimationErrors, cameraParams);

% Show recalibration reprojection errors
figure; showReprojectionErrors(cameraParams);
title('Reprojection Errors for the recalibration');
figname = ['ReprojectionErrors_', video_cali_name, '.fig'];
savefig(gcf, fullfile(caliImagesDir, figname))

% Show extrinsics
figure; 
showExtrinsics(cameraParams, 'CameraCentric');
figname = ['Extrinsics_', video_cali_name, '_CameraCentric.fig'];
savefig(gcf, fullfile(caliImagesDir, figname))

figure; 
showExtrinsics(cameraParams, 'PatternCentric');
figname = ['Extrinsics_', video_cali_name, '_PatternCentric.fig'];
savefig(gcf, fullfile(caliImagesDir, figname))

% Save calibration parameters
save(fullfile(dir2saveCali, ['cameraParams_', video_cali_name, '.mat']), 'cameraParams')

%% Undistort the last image in the calibration video
% The position of the camera has not been moved after the calibration
% [image_cali_last_undist, newOrigin] = undistortImage(image_cali_last, cameraParams, 'OutputView', 'full');
% imageSize_undist = size(image_cali_last_undist);
% figure; imshow(image_cali_last_undist, 'InitialMagnification', magnification);
% title('Undistorted Image');