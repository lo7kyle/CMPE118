/* 
 * File:   RoachFrameworkEvents.h
 * Author: mdunne
 *
 * Created on January 3, 2013, 4:53 PM
 */

#ifndef ROACHFRAMEWORKEVENTS_H
#define	ROACHFRAMEWORKEVENTS_H

typedef enum {
    BUMPER_LEFT         = 0x01,
    BUMPER_CENTER_LEFT  = 0x02,
    BUMPER_CENTER       = 0x04,
    BUMPER_CENTER_RIGHT = 0x08,
    BUMPER_RIGHT        = 0x10
} Bumperstate_t;

typedef enum {
    DARK_TO_LIGHT1 = 0x001, LIGHT_TO_DARK1 = 0x002,
    DARK_TO_LIGHT2 = 0x004, LIGHT_TO_DARK2 = 0x008,
    DARK_TO_LIGHT3 = 0x010, LIGHT_TO_DARK3 = 0x020,
    DARK_TO_LIGHT4 = 0x040, LIGHT_TO_DARK4 = 0x080,
    DARK_TO_LIGHT5 = 0x100, LIGHT_TO_DARK5 = 0x200
} IRstate_t;

typedef enum {
    B_ON_TO_OFF = 0, B_OFF_TO_ON = 1
} BeaconState_t;

typedef enum {
    TW_ON_TO_OFF = 0, TW_OFF_TO_ON = 1
} TrackWireState_t;

uint8_t CheckLightLevel(void);
uint8_t CheckBumps(void);

void InitBeaconDetection();
uint8_t CheckBeaconDetector(void);
uint8_t CheckTrackWire(void);


#endif	/* ROACHFRAMEWORKEVENTS_H */

