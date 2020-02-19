/*
 * File:   roach.h
 * Author: mdunne
 *
 * Created on January 6, 2012, 10:59 AM
 */

#ifndef ROACH_H
#define ROACH_H
#include <stdint.h>
/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
#define BUMPER_TRIPPED 1
#define BUMPER_NOT_TRIPPED 0


void tune(int32_t *var1, int32_t *var2, int32_t *var3);
void Roach_TurnOffIR(void);
/**
 * @Function Roach_ReadIR(uint8_t which);
 * @param which=[1:5], which IR to read the analog value
 * @return 0-1023
 * @author Team Dinkladge 11/15/14*/
uint16_t Roach_ReadIR(uint8_t which);

/**
 * @Function Roach_ReadSonar(uint8_t which);
 * @param which=[1:3], which Sonar to read the analog value
 * @return 0-1023
 * @author Team Dinkladge 11/15/14*/
uint16_t Roach_ReadSonar(uint8_t which);

/**
 * @Function Roach_ReadBumpers(uint8_t which);
 * @param which=[1:6], which bumper to read the digital value
 * @return 0 if not bumped, 1 if bumped.
 * @author Team Dinkladge 11/15/14*/
uint16_t Roach_ReadBumpers(void);

/**
 * @Function Roach_ReadBeaconDetector(void)
 * @return 1 if not sees beacon, else 0.
 * @author Team Dinkladge 11/15/14*/
uint8_t Roach_ReadBeaconDetector(void);

/**
 * @Function Roach_ReadTrackWire(void)
 * @return 1 if not track wire sensor gets within 2 inches, else 0.
 * @author Team Dinkladge 11/15/14*/
uint8_t Roach_ReadTrackWire(void);

/**
 * @Function Roach_Init(void)
 * @param None.
 * @return None.
 * @brief  Performs all the initialization necessary for the roach. this includes initializing
 * the PWM module, the A/D converter, the data directions on some pins, and
 * setting the initial motor directions.
 * @note  None.
 * @author Max Dunne, 2012.01.06 */
void Roach_Init(void);


/**
 * @Function Roach_LeftMtrSpeed(char newSpeed)
 * @param newSpeed - A value between -10 and 10 which is the new speed
 * @param of the motor. 0 stops the motor. A negative value is reverse.
 * @return SUCCESS or ERROR
 * @brief  This function is used to set the speed and direction of the left motor.
 * @author Max Dunne, 2012.01.06 */
char Roach_LeftMtrSpeed(int16_t newSpeed);

/**
 * @Function Roach_RightMtrSpeed(char newSpeed)
 * @param newSpeed - A value between -10 and 10 which is the new speed
 * @param of the motor. 0 stops the motor. A negative value is reverse.
 * @return SUCCESS or ERROR
 * @brief  This function is used to set the speed and direction of the left motor.
 * @author Max Dunne, 2012.01.06 */
char Roach_RightMtrSpeed(int16_t newSpeed);

/**
 * @Function Roach_BatteryVoltage(void)
 * @param None.
 * @return a 10-bit value corresponding to the current voltage of the roach
 * @brief  returns a 10:1 scaled value of the roach battery level
 * @author Max Dunne, 2013.07.12 */
unsigned int Roach_BatteryVoltage(void);

void PlaySound(uint8_t);


#endif
