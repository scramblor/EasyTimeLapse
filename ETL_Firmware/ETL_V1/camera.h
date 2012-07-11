/*
 * camera.h
 *
 * Created: 7/11/2012 1:55:52 PM
 *  Author: brandonw
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

void TimelapseResume();
void InitTimelapseState();
void InitTimelapsePauseState();
void ProcessTimelapseWaiting();
void ProcessTimeLapseWaitingFlash();
void ProcessTimeLapseExposing();

#endif /* CAMERA_H_ */