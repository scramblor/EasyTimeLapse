/*
 * camera.h
 *
 * Created: 7/11/2012 1:55:52 PM
 *  Author: brandonw
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

#include "Utils.h"

#define DEFAULT_EXPOSURE 100 //ms
//#define HDR_INTERVAL 1000 // TODO this should be configurable
#define SHUTTER_FEEDBACK_TIMEOUT 1000

#define EXPOSURE_WARNING_TIME_OFFSET (((uint32_t) 5)*60*1000) //ms
#define MINIMUM_PHOTO_LENGTH 1 //ms

#define DEFAULT_BULB_SHUTTER_OFFSET 35  //ms
#define DEFAULT_BUFFER_RECOVER_TIME 500 //ms

#define LED_CYCLE_TIMELAPSE_PAUSE LedCycle(400,2,1,GREEN,YELLOW,OFF,OFF,OFF,OFF)
#define LED_CYCLE_TIMELAPSE_COMPLETE LedCycle(333,5,0,GREEN,OFF,GREEN,OFF,GREEN,OFF)
#define LED_CYCLE_TIMELAPSE_ABANDON LedCycle(333,5,0,RED,OFF,RED,OFF,RED,OFF)
#define LED_CYCLE_TAKE_PICTURE LedCycle(100, 1, 0, GREEN,OFF,OFF,OFF,OFF,OFF)
#define LED_CYCLE_TIMELAPSE_INVALID LedCycle(333,5,0,RED,YELLOW,RED,YELLOW,RED,OFF)
#define LED_CYCLE_TIMELAPSE_CHANGE_READY LedCycle(250, 1, 0, YELLOW,OFF,OFF,OFF,OFF,OFF)
#define LED_CYCLE_TIMELAPSE_EXP_COLLISION LedCycle(250, 1, 0, RED,OFF,OFF,OFF,OFF,OFF)
#define LED_CYCLE_STOP_CHANGE_SUCCESS LedCycle(250,5,0,GREEN,OFF,GREEN,OFF,GREEN,OFF)
#define LED_CYCLE_STOP_CHANGE_FAILURE LedCycle(500,1,0,RED,OFF,RED,OFF,RED,OFF)
#define LED_CYCLE_PC_SYNC_TIMEOUT LedCycle(333,6,0,GREEN,RED,GREEN,RED,GREEN,RED)

void StartExposure();
void EndExposure();
void TimelapseResume();
void InitTimelapseState(uint32_t startTimeOffset);
void InitManualTimelapseState();
void InitTimelapsePauseState();
void ProcessTimelapseWaiting();
void ProcessTimeLapseWaitingFlash();
void ProcessTimeLapseExposing();

#endif /* CAMERA_H_ */