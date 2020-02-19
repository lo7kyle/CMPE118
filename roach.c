/*
 * File:   roach.c
 * Author: mdunne
 *
 * Created on December 15, 2011, 12:59 PM
 */

#include "roach.h"
#include <xc.h>

#include "pwm.h"
#include "serial.h"
#include "AD.h"
#include "BOARD.h"
#include "IO_Ports.h"
#include <stdbool.h>
#include "Stack.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define ROACH_BAT_VOLTAGE BAT_VOLTAGE

// sound
#define PORT_SOUND PORTY
#define PIN_SOUND PIN6

// left motor
#define PIN_LMOTOR_PWM PWM_PORTY10
#define PORT_LMOTOR_DIR PORTY
#define PIN_LMOTOR_DIR PIN9

// right motor
#define PIN_RMOTOR_PWM PWM_PORTZ06
#define PORT_RMOTOR_DIR PORTZ
#define PIN_RMOTOR_DIR PIN5

// motors for lifter
#define PIN_LIFT_PWM PWM_PORTX11
#define PORT_LIFT_DIR PORTX
#define PIN_LIFT_DIR PIN12

// tray motor
#define PIN_TRAY_PWM PWM_PORTY04
#define PORT_TRAY_DIR PORTY
#define PIN_TRAY_DIR PIN7

// IR sensors
#define NUM_IR 5
#define PIN_IR1 AD_PORTW4
#define PIN_IR2 AD_PORTW5
#define PIN_IR3 AD_PORTW6
#define PIN_IR4 AD_PORTW7
#define PIN_IR5 AD_PORTW8

// bumpers
#define PORT_BUMP PORTZ
#define PIN_BUMP1 PIN3
#define PIN_BUMP2 PIN4
#define PIN_BUMP3 PIN7
#define PIN_BUMP4 PIN8
#define PIN_BUMP5 PIN9
#define PIN_BUMP6 PIN11

#define PORT_BEACON PORTY
#define PIN_BEACON PIN5

#define PORT_TRACK_WIRE PORTX
#define PIN_TRACK_WIRE PIN7

#define MOTOR_SPEED_MAX 1000
#define MOTOR_SPEED_MIN -1000

#define diff 20

static uint16_t vBat; // locally global variable for mapping battery voltage to PWM.
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function Roach_Init(void)
 * @param None.
 * @return None.
 * @brief  Performs all the initialization necessary for the roach. this includes initializing 
 * the PWM module, the A/D converter, the data directions on some pins, and 
 * setting the initial motor directions.
 * @note  None.
 * @author Max Dunne, 2012.01.06 */
void Roach_Init(void) {

    //set the control pins for the motors
    PWM_Init();
    PWM_SetFrequency(PWM_30KHZ);
    PWM_AddPins(PIN_LMOTOR_PWM | PIN_RMOTOR_PWM | PIN_LIFT_PWM | PIN_TRAY_PWM);

    IO_PortsSetPortOutputs(PORT_LMOTOR_DIR, PIN_LMOTOR_DIR);
    IO_PortsSetPortOutputs(PORT_RMOTOR_DIR, PIN_RMOTOR_DIR);
    IO_PortsSetPortOutputs(PORT_LIFT_DIR, PIN_LIFT_DIR);
    IO_PortsSetPortOutputs(PORT_TRAY_DIR, PIN_TRAY_DIR);
    IO_PortsSetPortOutputs(PORT_SOUND, PIN_SOUND);

    IO_PortsSetPortInputs(PORT_BUMP, PIN_BUMP1 | PIN_BUMP2 | PIN_BUMP3 |
            PIN_BUMP4 | PIN_BUMP5 | PIN_BUMP6);

    IO_PortsSetPortInputs(PORT_BEACON, PIN_BEACON);
    IO_PortsSetPortInputs(PORT_TRACK_WIRE, PIN_TRACK_WIRE);

    // X5 for LED control, X6 for Switch
    IO_PortsSetPortOutputs(PORTX, PIN5 | PIN3);
    IO_PortsWritePort(PORTX, PIN5);


    //Initialize the light sensor
    AD_Init();
    AD_AddPins(PIN_IR1 | PIN_IR2 | PIN_IR3 | PIN_IR4 | PIN_IR5);

    //enable interrupts
}

void PlaySound(uint8_t value) {
    static uint8_t total = 6;
    static uint8_t currentAudio = 1;

    printf("PLAYING AUDIO!!!!!!!!!!!!!!!! %d,%d                                 \n", value, currentAudio);

    int i, nextTimes;
    if (value < currentAudio)
        nextTimes = total - (currentAudio - value);
    else if (value > currentAudio)
        nextTimes = value - currentAudio;
    else
        nextTimes = total;

    for (i = 1; i <= nextTimes; i++) {
        IO_PortsTogglePortBits(PORT_SOUND, PIN_SOUND);
        int aux;
        for (aux = 0; aux < 10000; aux++);
        IO_PortsTogglePortBits(PORT_SOUND, PIN_SOUND);
        for (aux = 0; aux < 12000; aux++);
    }

    currentAudio = value;
}

void Roach_TurnOffIR(void) {
    IO_PortsClearPortBits(PORTX, PIN5);
    return;
}

/**
 * @Function Roach_LeftMtrSpeed(uint16_t newSpeed)
 * @param newSpeed - A value between -1000 and 1000 which is the new speed
 * @param of the motor. 0 stops the motor. A negative value is reverse.
 * @return SUCCESS or ERROR
 * @brief  This function is used to set the speed and direction of the left motor.
 * @author Team Dinkladge Andre 11/17/14 */
char Roach_LeftMtrSpeed(int16_t newSpeed) {
    if ((newSpeed < MOTOR_SPEED_MIN) || (newSpeed > MOTOR_SPEED_MAX)) {
        return (ERROR);
    }
    //newSpeed = (2.996*newSpeed/300)*(1/Roach_BatteryVoltage()*31.0303)*1000;
    //newSpeed = 310.0 * newSpeed / vBat;
    //newSpeed = newSpeed * (200*1.0/vBat);
    newSpeed = 322.7 * newSpeed / vBat;
    //printf("SPEED_L: %d\n",newSpeed);
    if (newSpeed < 0) {
        IO_PortsClearPortBits(PORT_LMOTOR_DIR, PIN_LMOTOR_DIR);
        newSpeed = newSpeed * -1; // set speed to a positive value
    } else {
        IO_PortsWritePort(PORT_LMOTOR_DIR, PIN_LMOTOR_DIR);
    }
    if (PWM_SetDutyCycle(PIN_LMOTOR_PWM, newSpeed) == ERROR) {
        //printf("ERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);
}

/**
 * @Function Roach_RightMtrSpeed(uint16_t newSpeed)
 * @param newSpeed - A value between -1000 and 1000 which is the new speed
 * @param of the motor. 0 stops t-he motor. A negative value is reverse.
 * @return SUCCESS or ERROR
 * @brief  This function is used to set the speed and direction of the right motor.
 * @author Team Dinkladge Andre 11/17/14 */
char Roach_RightMtrSpeed(int16_t newSpeed) {
    if ((newSpeed < MOTOR_SPEED_MIN) || (newSpeed > MOTOR_SPEED_MAX)) {
        return (ERROR);
    }
    //newSpeed = (2.996*newSpeed/300)*((1/Roach_BatteryVoltage())*31.0303)*1000;
    //newSpeed = 310.0 * newSpeed / vBat;//newSpeed * (27000*motor/batery)
    //newSpeed = newSpeed * (200*0.9/vBat);

    //printf("SPEED_R: %d\n",newSpeed);
    if (newSpeed < 0) {
        IO_PortsWritePort(PORT_RMOTOR_DIR, PIN_RMOTOR_DIR);
        newSpeed = newSpeed * -1; // setq speed to a positive value
        newSpeed = (322.7 - diff) * newSpeed / vBat;
    } else {
        IO_PortsClearPortBits(PORT_RMOTOR_DIR, PIN_RMOTOR_DIR);
        newSpeed = (322.7 - diff) * newSpeed / vBat;
    }
    if (PWM_SetDutyCycle(PIN_RMOTOR_PWM, newSpeed) == ERROR) {
        //printf("ERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);
}

int MoveAverage(int newValue) {
    static int32_t runSum = 0;
    static int16_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    static int8_t newest = 0;
    static int8_t oldest = 1;

    runSum = runSum - buffer[oldest] + newValue;
    buffer[newest] = newValue;

    newest = (newest + 1) & 0x07;
    oldest = (oldest + 1) & 0x07;

    return (runSum >> 3);
}

/**
 * @Function Roach_BatteryVoltage(void)
 * @param None.
 * @return a 10-bit value corresponding to the current voltage of the roach
 * @brief  returns a 10:1 scaled value of the roach battery level
 * @author Max Dunne, 2013.07.12 */
unsigned int Roach_BatteryVoltage(void) {
    vBat = MoveAverage(AD_ReadADPin(ROACH_BAT_VOLTAGE));
    return vBat;
}

/**
 * @Function Roach_ReadBumpers(void)
 * @param None.
 * @return 6-bit value representing allsix bumpers in following order:
 *          front, front left, front right, back, back left, back right.
 * @brief  Returns the state of all 6 bumpers
 * @author Team Dinkladge - Andre 11/20/14 */
uint16_t Roach_ReadBumpers(void) {
    uint16_t read = IO_PortsReadPort(PORT_BUMP);
    uint16_t bump_state = (read & PIN3) >> 3 | (read & PIN4) >> 3 | (read & PIN7) >> 5 |
            (read & PIN8) >> 5 | (read & PIN9) >> 5;
    return bump_state;
}

/**
 * @Function Roach_MoveTrayMotor(uint16_t newSpeed)
 * @param newSpeed - A value between -1000 and 1000 which is the new speed
 * @param of the motor. 0 stops the motor. A negative value is reverse.
 * @return SUCCESS or ERROR
 * @brief  This function is used to set the speed and direction of the tray motor.
 * @author Team Dinkladge Andre 11/17/14 */
unsigned char Roach_MoveTrayMotor(int16_t newSpeed) {
    if ((newSpeed < MOTOR_SPEED_MIN) || (newSpeed > MOTOR_SPEED_MAX)) {
        return (ERROR);
    }
    newSpeed = 300.0 * newSpeed / vBat;
    if (newSpeed < 0) {
        IO_PortsWritePort(PORT_TRAY_DIR, PIN_TRAY_DIR);
        newSpeed = 1000 + newSpeed; // set speed to a positive value
    } else {
        IO_PortsClearPortBits(PORT_TRAY_DIR, PIN_TRAY_DIR);
    }
    if (PWM_SetDutyCycle(PIN_TRAY_PWM, newSpeed) == ERROR) {
        //printf("ERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);
}

/**
 * @Function Roach_MoveLiftMotor(uint16_t newSpeed)
 * @param newSpeed - A value between -1000 and 1000 which is the new speed
 * @param of the motor. 0 stops the motor. A negative value is reverse.
 * @return SUCCESS or ERROR
 * @brief  This function is used to set the speed and direction of the lift motor.
 * @author Team Dinkladge Andre 11/17/14 */
unsigned char Roach_MoveLiftMotor(int16_t newSpeed) {
    if ((newSpeed < MOTOR_SPEED_MIN) || (newSpeed > MOTOR_SPEED_MAX)) {
        return (ERROR);
    }
    newSpeed = 300.0 * newSpeed / vBat;
    if (newSpeed < 0) {
        IO_PortsWritePort(PORT_LIFT_DIR, PIN_LIFT_DIR);
        newSpeed = 1000 + newSpeed; // set speed to a positive value
    } else {
        IO_PortsClearPortBits(PORT_LIFT_DIR, PIN_LIFT_DIR);
    }
    if (PWM_SetDutyCycle(PIN_LIFT_PWM, newSpeed) == ERROR) {
        //printf("ERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);
}

uint16_t Roach_ReadIR(uint8_t which) {
    uint16_t pin[NUM_IR] = {PIN_IR1, PIN_IR2, PIN_IR3, PIN_IR4, PIN_IR5};
    return AD_ReadADPin(pin[which]);
}

uint16_t Roach_ReadSonar(uint8_t which);
/**
 * @return: 1 is beacon found else 0.
 */
uint8_t Roach_ReadBeaconDetector(void) {
    return (IO_PortsReadPort(PORT_BEACON) & PIN_BEACON) != PIN_BEACON;
}
/**
 * @return: 1 is track wire in range else 0.
 */
uint8_t Roach_ReadTrackWire(void) {
    return (IO_PortsReadPort(PORT_TRACK_WIRE) & PIN_TRACK_WIRE) != PIN_TRACK_WIRE;
}

void tune(int32_t *var1, int32_t *var2, int32_t *var3) {
    int i, j = 0;
    struct Stack stack;
    StackInit(&stack);
    char c = 0;
    char *token = 0;
    char buffer[256];

    uint16_t reads[3];

    while (StackIsEmpty(&stack)) {
        // push characters until no more character.
        while (c = GetChar()) {
            if (StackPush(&stack, c) == ERROR) {
                break;
            }
        }

        token = strtok(stack.stackItems, " ");
        while (token) {
            //printf("%s\n\r", token);
            strncpy(buffer, token, 256);
            reads[j++] = atoi(token);
            token = strtok(NULL, " ");
        }
        for (i = 0; i < 10000; i++);
    }

    if (var1) {
        *var1 = reads[0];
    }
    if (var2) {
        *var2 = reads[1];
    }
    if (var3) {
        *var3 = reads[2];
    }
}

//#define ROACH_TEST
#ifdef ROACH_TEST
#pragma config FPLLIDIV 	= DIV_2		//PLL Input Divider
#pragma config FPLLMUL 		= MUL_20	//PLL Multiplier
#pragma config FPLLODIV 	= DIV_1 	//System PLL Output Clock Divid
#pragma config FNOSC 		= PRIPLL	//Oscillator Selection Bits
#pragma config FSOSCEN 		= OFF		//Secondary Oscillator Enable
#pragma config IESO 		= OFF		//Internal/External Switch O
#pragma config POSCMOD 		= XT		//Primary Oscillator Configuration
#pragma config OSCIOFNC 	= OFF		//CLKO Output Signal Active on the OSCO Pin
#pragma config FPBDIV 		= DIV_2		//Peripheral Clock Divisor
#pragma config FCKSM 		= CSECMD	//Clock Switching and Monitor Selection
#pragma config WDTPS 		= PS1		//Watchdog Timer Postscaler
#pragma config FWDTEN		= OFF		//Watchdog Timer Enable
#pragma config ICESEL		= ICS_PGx2	//ICE/ICD Comm Channel Select
#pragma config PWP 			= OFF		//Program Flash Write Protect
#pragma config BWP 			= OFF		//Boot Flash Write Protect bit
#pragma config CP 			= OFF		//Code Protect

//#define WANDER_MODE
#define WANDER_SAMPLE_RATE 100

#define FLEFT_BUMP_MASK (1)
#define FRIGHT_BUMP_MASK (1<<1)
#define RLEFT_BUMP_MASK (1<<2)
#define RRIGHT_BUMP_MASK (1<<3)

#include <xc.h>
#include <stdio.h>
#include "serial.h"
#include "roach.h"
#include "BOARD.h"
#include "IO_Ports.h"
#include "Stack.h"
#include <string.h>
//#include <plib.h>
#include <stdlib.h>

uint32_t wait;
#define DELAY(x)         for (wait = 0; wait <= x; wait++) {asm("nop");}

int main(void) {

    int i;

    BOARD_Init(); // needs to be called before Roach_Init();
    Roach_Init();
    SERIAL_Init();
    IO_PortsSetPortOutputs(PORTX, PIN5);
    IO_PortsWritePort(PORTX, PIN5);

    //Roach_LeftMtrSpeed(300);
    //Roach_RightMtrSpeed(300);
    while (0) {
        printf("IR1: %3d, IR2: %3d, IR3: %3d, IR4: %3d, IR5: %3d\n\r",
                Roach_ReadIR(0), Roach_ReadIR(1), Roach_ReadIR(2),
                Roach_ReadIR(3), Roach_ReadIR(4));
        for (i = 0; i < 100000; i++);
    }

    IO_PortsSetPortOutputs(PORTX, PIN3);
    while (0) {
        IO_PortsWritePort(PORTX, PIN3);
        for (i = 0; i < 1000000; i++);
        IO_PortsClearPortBits(PORTX, PIN3);
        for (i = 0; i < 1000000; i++);
    }
    //mPORTAClearBits(BIT_3 | BIT_2 | BIT_1 | BIT_0);

    // Set lower four bits of PORTA as output
    //mPORTASetPinsDigitalOut(BIT_3 | BIT_2 | BIT_1 | BIT_0);
    while (0) {
        printf("Testing L motors.\n\r");

        printf("500: %d\n\r", Roach_LeftMtrSpeed(300));
        for (i = 0; i < 1000000; i++);
        printf("-500: %d\n\r", Roach_LeftMtrSpeed(-300));
        for (i = 0; i < 1000000; i++);
    }
    while (0) {
        printf("Testing R motors.\n\r");

        //printf("500: %d\n\r", Roach_RightMtrSpeed(200));
        for (i = 0; i < 1000000; i++);
        printf("-500: %d\n\r", Roach_RightMtrSpeed(-1000));
        for (i = 0; i < 1000000; i++);
    }

    while (0) {
        printf("Testing lift motors.\n\r");

        printf("500: %d\n\r", Roach_MoveLiftMotor(500));
        for (i = 0; i < 1000000; i++);
        printf("-500: %d\n\r", Roach_MoveLiftMotor(-500));
        for (i = 0; i < 1000000; i++);
    }

    while (0) {
        printf("Testing tray motors.\n\r");

        printf("500: %d\n\r", Roach_MoveTrayMotor(500));
        for (i = 0; i < 1000000; i++);
        printf("-500: %d\n\r", Roach_MoveTrayMotor(-500));
        for (i = 0; i < 1000000; i++);
    }

    while (0) {
        printf("500: %d\n\r", Roach_MoveLiftMotor(-999));
        printf("500: %d\n\r", Roach_MoveTrayMotor(0));
        for (i = 0; i < 1000000; i++);
        //printf("-500: %d\n\r", Roach_MoveLiftMotor(500)); //625
        printf("-500: %d\n\r", Roach_MoveTrayMotor(0));
        for (i = 0; i < 1000000; i++);
    }

    // test all
    while (0) {
        printf("left motor: %d\n\r", Roach_LeftMtrSpeed(400));
        printf("right motor: %d\n\r", Roach_RightMtrSpeed(400));
        DELAY(1550000);
        printf("lift: %d\n\r", Roach_MoveLiftMotor(500));
        printf("tray: %d\n\r", Roach_MoveTrayMotor(500));
        DELAY(675000);
        printf("left motor: %d\n\r", Roach_LeftMtrSpeed(-400));
        printf("right motor: %d\n\r", Roach_RightMtrSpeed(-400));
        DELAY(1550000);
        printf("lift: %d\n\r", Roach_MoveLiftMotor(-500));
        printf("tray: %d\n\r", Roach_MoveTrayMotor(-500));
        DELAY(675000);
    }
    // test effect of reading port Z while Z5 Z6 set to output.
    //IO_PortsSetPortInputs(PORTZ, PIN6 | PIN5);
    while (0) {
        //printf("right motor: %d\n\r", Roach_RightMtrSpeed(200));
        //IO_PortsWritePort(PORTZ, PIN5);
        //IO_PortsClearPortBits(PORTZ, PIN6);
        printf("HIGH, port Z: 0x%04x\n\r", IO_PortsReadPort(PORTZ));
        for (i = 0; i < 1000000; i++);
        //IO_PortsClearPortBits(PORTZ, PIN5);
        //IO_PortsWritePort(PORTZ, PIN6);

        //printf("LOW, port Z: 0x%04x\n\r", IO_PortsReadPort(PORTZ));
        //for(i = 0; i < 1000000; i++);
    }

    while (0) {
        printf("bumpers: %04x\n\r", Roach_ReadBumpers());
        for (i = 0; i < 1000000; i++);
    }


    // fine-tune timing
    struct Stack stack;
    StackInit(&stack);
    char c = 0;
    char *token = 0;
    char buffer[256];

    printf("Started tokenization test\n\r");
    printf("d: %i\n\r", atoi("123456798"));
    while (0) {
        // push characters until no more character.
        while (c = GetChar()) {
            if (StackPush(&stack, c) == ERROR) {
                break;
            }
        }
        if (!StackIsEmpty(&stack)) {
            printf("test: %s\n\r", stack.stackItems);

        }
        token = strtok(stack.stackItems, " ");

        while (token) {
            printf("s: %s\n\r", token);
            strncpy(buffer, token, 256);
            printf("d: %i\n\r", atoi(token));
            token = strtok(NULL, " ");
        }

        // if reading is over, print and clear the stack.
        if (!StackIsEmpty(&stack)) {
            //printf("test: %s\n\r", stack.stackItems);
            StackInit(&stack); //
        }
        for (i = 0; i < 1000000; i++);
    }


    uint16_t v1, v2, v3;
    v1 = v2 = v3 = 0;
    printf("Started tuning test\n\r");
    while (0) {
        tune(&v1, &v2, &v3);
        printf("v1: %d, v2: %d, v3: %d\n\r", v1, v2, v3);
        v1 = v2 = v3 = 0;
        tune(&v1, &v2, 0);
        printf("v1: %d, v2: %d, v3: %d\n\r", v1, v2, v3);
        for (i = 0; i < 1000000; i++);
    }

    printf("Testing bumpers\n\r");
    IO_PortsSetPortOutputs(PORTY, PIN6);
    IO_PortsWritePort(PORTY, PIN6);
    while (0) {
        printf("bump_port: %0x4, bumps: %04x\n\r", IO_PortsReadPort(PORTZ), Roach_ReadBumpers());
        for (i = 0; i < 100000; i++);
    }

    printf("Testing TrackWire detector\n\r");
    while (0) {
        if (Roach_ReadTrackWire()) {
            //printf("Trackwire On\n\r");
            Roach_LeftMtrSpeed(400);
            Roach_RightMtrSpeed(400);
        } else {
            //printf("Trackwire Off\n\r");
            Roach_LeftMtrSpeed(-400);
            Roach_RightMtrSpeed(-400);
        }
        //for (i = 0; i < 10000; i++);
    }
    printf("Testing Beacon detector\n\r");
    while (0) {
        if (Roach_ReadBeaconDetector()) {
            printf("Beacon Not Detected\n\r");
            Roach_LeftMtrSpeed(200);
            Roach_RightMtrSpeed(-200);
        } else {
            printf("Beacon Detected\n\r");
            Roach_LeftMtrSpeed(0);
            Roach_RightMtrSpeed(0);
        }
        for (i = 0; i < 10000; i++);
    }
    return 0;
}

#endif