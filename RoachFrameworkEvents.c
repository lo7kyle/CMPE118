#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "RoachFrameworkEvents.h"
#include <stdio.h>
#include "IO_Ports.h"
#define DARK_THRESHOLD 150
#define LIGHT_THRESHOLD 300

#define NUM_IR 5

static uint8_t lastState_beacon = 0xF0; // for beacon detector
static uint8_t currentState_beacon = 0x00; // for beacon detector
static uint8_t currentState_trackwire = 0;
static uint8_t lastState_trackwire = 0;
/**
 * uint8_t CheckLightLevel(void)
 * @note: EventParam is 16-bits value, containing info about IR sensor level
 *        in order:
 *        left IR         - DARK_TO_LIGHT1 = 0x001, LIGHT_TO_DARK1 = 0x002
 *        center left IR  - DARK_TO_LIGHT2 = 0x004, LIGHT_TO_DARK2 = 0x008,
 *        center IR       - DARK_TO_LIGHT3 = 0x010, LIGHT_TO_DARK3 = 0x020,
 *        center right IR - DARK_TO_LIGHT4 = 0x040, LIGHT_TO_DARK4 = 0x080,
 *        right IR        - DARK_TO_LIGHT5 = 0x100, LIGHT_TO_DARK5 = 0x200
 *
 *        three more IR sensor can be extended to use all 16 bits.
 * @return
 */
uint8_t CheckLightLevel(void) {
    static uint16_t threshold[NUM_IR] = {LIGHT_THRESHOLD, LIGHT_THRESHOLD,
        LIGHT_THRESHOLD, LIGHT_THRESHOLD, LIGHT_THRESHOLD}; // assume all the IR sensors
    // start on white floor.
    uint16_t currentLightLevel[NUM_IR] = {0};
    static const uint16_t dark_to_light[NUM_IR] = {DARK_TO_LIGHT1, DARK_TO_LIGHT2,
        DARK_TO_LIGHT3, DARK_TO_LIGHT4, DARK_TO_LIGHT5};
    static const uint16_t light_to_dark[NUM_IR] = {LIGHT_TO_DARK1, LIGHT_TO_DARK2,
        LIGHT_TO_DARK3, LIGHT_TO_DARK4, LIGHT_TO_DARK5};

    ES_Event thisEvent = {ES_NO_EVENT, 0};
    uint8_t i = 0;
    // check the light level and assign LIGHT or DARK
    for (i = 0; i < NUM_IR; i++) {
        currentLightLevel[i] = Roach_ReadIR(i);
        //printf("IR%d: %3d \n", i+1, currentLightLevel[i]);
        if (threshold[i] == LIGHT_THRESHOLD && currentLightLevel[i] <= DARK_THRESHOLD) { //event detected
            // printf("L2D->th[%i]: %i, IR[%i]: %i\n\r", i, threshold[i], i, currentLightLevel[i]);
            thisEvent.EventType = LIGHTLEVEL;
            thisEvent.EventParam |= light_to_dark[i];
            threshold[i] = DARK_THRESHOLD;
        } else if (threshold[i] == DARK_THRESHOLD && currentLightLevel[i] >= LIGHT_THRESHOLD) {
            //printf("D2L->th[%i]: %i, IR[%i]: %i\n\r", i, threshold[i], i, currentLightLevel[i]);
            thisEvent.EventType = LIGHTLEVEL;
            thisEvent.EventParam |= dark_to_light[i];
            threshold[i] = LIGHT_THRESHOLD;
        }
    }

    if (thisEvent.EventType == LIGHTLEVEL) {
        PostRoachHSM(thisEvent);
    }
}

uint8_t CheckBumps(void) {
    static uint16_t lastBumpState = 0;
    uint16_t currentBumpState = 0;
    ES_Event thisEvent;

    uint8_t returnVal = FALSE;
    // check the light level and assign LIGHT or DARK
    currentBumpState = Roach_ReadBumpers();
    //printf("bumps: %04x\n\r", Roach_ReadBumpers());
    if (currentBumpState != lastBumpState) { //event detected
        thisEvent.EventType = BUMPED;
        thisEvent.EventParam = (uint16_t) currentBumpState;
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastBumpState = currentBumpState;
    }

    return returnVal;
}

void InitBeaconDetection() {
    currentState_beacon = lastState_beacon = 0;
    return;
}

uint8_t CheckBeaconDetector(void) {
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    /*//without debouncing: one shot 
    static uint8_t lastState = 1;
   uint8_t currentState;

   currentState = Roach_ReadBeaconDetector();
   //printf("b: %04x\n", currentState);
   if (currentState != lastState) { //event detected
     thisEvent.EventType = BEACON_DETECTED;
     if (currentState == 1) {
         thisEvent.EventParam = (uint16_t) B_OFF_TO_ON;
     } else {
         thisEvent.EventParam = (uint16_t) B_ON_TO_OFF;
     }
     PostRoachHSM(thisEvent);
     returnVal = TRUE;
     lastState = currentState;
   }
 //*/

    //*//debouncing version: 4 buffers
    currentState_beacon = (currentState_beacon << 1) | (Roach_ReadBeaconDetector() ? 0x01 : 0x0);
    if (((currentState_beacon & 0xF0) == 0xF0) && ((lastState_beacon & 0xF0) == 0x00)) {
        thisEvent.EventType = BEACON_DETECTED;
        thisEvent.EventParam = B_OFF_TO_ON;
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastState_beacon = currentState_beacon;
    } else if (((currentState_beacon & 0xF0) == 0x00) && ((lastState_beacon & 0xF0) == 0xF0)) {
        thisEvent.EventType = BEACON_DETECTED;
        thisEvent.EventParam = B_ON_TO_OFF;
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastState_beacon = currentState_beacon;
    }
    //*/

    return returnVal;
}

void InitTrackWireDetection() {
    currentState_trackwire = lastState_trackwire = 0;
    return;
}

uint8_t CheckTrackWire(void) {

    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    /*
    static uint8_t lastState = 1;
    uint8_t currentState;
    currentState = Roach_ReadTrackWire();
    if (currentState != lastState) { //event detected
        thisEvent.EventType = TRACKWIRE;
        if (currentState == 1) {
            thisEvent.EventParam = (uint16_t) TW_OFF_TO_ON;
        } else {
            thisEvent.EventParam = (uint16_t) TW_ON_TO_OFF;
        }
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastState = currentState;
    }
    //*/

    //*
    currentState_trackwire = Roach_ReadTrackWire();
    if (currentState_trackwire != lastState_trackwire) { //event detected
        thisEvent.EventType = TRACKWIRE;
        if (currentState_trackwire == 1) {
            thisEvent.EventParam = (uint16_t) TW_OFF_TO_ON;
        } else {
            thisEvent.EventParam = (uint16_t) TW_ON_TO_OFF;
        }
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastState_trackwire = currentState_trackwire;
    }
    //*/

    /*debouncing version: 4 buffers
    static uint8_t lastState = 0, currentState = 0;
    currentState = (currentState << 1) | (Roach_ReadBeaconDetector() ? 0x01:0x0);
    if (((currentState & 0xF0) == 0xF0) && ((lastState & 0xF0) == 0x00)) {
        thisEvent.EventType = TRACKWIRE;
        thisEvent.EventParam = TW_OFF_TO_ON;
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastState = currentState;
    } else if (((currentState & 0xF0) == 0x00) && ((lastState & 0xF0) == 0xF0)) {
        thisEvent.EventType = TRACKWIRE;
        thisEvent.EventParam = TW_ON_TO_OFF;
        PostRoachHSM(thisEvent);
        returnVal = TRUE;
        lastState = currentState;
    }
    //*/
    return returnVal;
}