#ifndef FORCE_SEAT_MI_SIMULINK_H
#define FORCE_SEAT_MI_SIMULINK_H

void SetPosPhy(double yaw_rad, double pitch_rad, double roll_rad, double heave_mm, double sway_mm, double surge_mm, int enable);
void SetPosLog(double yaw, double pitch, double roll, double heave, double sway, double surge, int enable);
void Refresh(int enable);

double GetYaw_rad();
double GetPitch_rad();
double GetRoll_rad();
double GetHeave_mm();
double GetSway_mm();
double GetSurge_mm();

void initialize();
void terminate();

#endif
