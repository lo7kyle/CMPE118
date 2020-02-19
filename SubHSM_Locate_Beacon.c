/*
 * File: TemplateSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is for a substate machine. Make sure it has a unique name
 *
 * This is provided as an example and a good place to start.
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "roach.h"
#include "RoachHSM.h"
#include "SubHSM_Locate_Beacon.h"
#include "dinkbot_vars.h"
#include <stdio.h>
#include "RoachFrameworkEvents.h"
/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(TURN90CCW) \
        STATE(MOVE_TO_CORNER) \
        STATE(SCAN_B3) \
        STATE(BACKUP_FROM_CORNER) \
        STATE(TURN90CW) \
        STATE(SMALL_FORWARD) \
        STATE(SCAN_B1) \
        STATE(BACKUP_AND_CHECK) \
        STATE(TURN_10_RIGHT_BACK) \
        STATE(TURN_10_LEFT_BACK) \
        STATE(TURN_30_RIGHT_BACK) \
        STATE(TURN_30_LEFT_BACK) \
        STATE(WAIT) \
        STATE(BACKUP) \
		STATE(WAIT_FOR_KICKBACK) \


#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_TEMPLATE_STATES(ENUM_FORM)
} SubTemplateState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_TEMPLATE_STATES(STRING_FORM)
};


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static SubTemplateState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;

//extern const int16_t v_L_turn90cw, v_R_turn90cw, v_L_turn90ccw, v_R_turn90ccw;
//extern const uint16_t t_turn90;

typedef enum {
    TURN_TIME, PAUSE_TIME
} MOVEMENT_TYPE;
static uint16_t t_scan;
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateSubHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitSubHSMLocateBeacon(void)
{
    ES_Event returnEvent;

    //printf("LOCATE_BEACON: %d, %d, %d, %d, %d\n\r",   v_L_turn90cw, v_R_turn90cw, v_L_turn90ccw, v_R_turn90ccw, t_turn90);
    t_scan = 2000;
    CurrentState = InitPSubState;
    returnEvent = RunSubHSMLocateBeacon(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunTemplateSubHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */
ES_Event RunSubHSMLocateBeacon(ES_Event ThisEvent)
{
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState; // <- change type to correct enum
    static SubTemplateState_t prevState;
    static uint16_t detected = 0;
    ES_Tattle(); // trace call stack

    switch (CurrentState) {
    case InitPSubState: // If current state is initial Psedudo State
        if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
        {
            CurrentState = TURN90CCW;
            ThisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case TURN90CCW: // move back facing outward from beginning castle
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90ccw);
                Roach_RightMtrSpeed(v_R_turn90ccw);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90);
                break;
            case ES_TIMEOUT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                nextState = MOVE_TO_CORNER;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case MOVE_TO_CORNER:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                break;
            case BUMPED:
                PlaySound(4);
                if (ThisEvent.EventParam == 0x1) {
                    nextState = TURN_30_RIGHT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x2 || ThisEvent.EventParam == 0x3) {
                    nextState = TURN_10_LEFT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x10) {
                    nextState = TURN_30_LEFT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x8 || ThisEvent.EventParam == 0x18) {
                    nextState = TURN_10_RIGHT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x4 || ThisEvent.EventParam == 0xE || ThisEvent.EventParam == 0x12) {
                    nextState = SCAN_B3;
                    makeTransition = TRUE;
                }
                break;
            case ES_EXIT:
                prevState = CurrentState;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }


    case SCAN_B3:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_scan);
                InitBeaconDetection();
                break;
            case BEACON_DETECTED:
                ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                if (ThisEvent.EventParam == B_OFF_TO_ON) {
                    detected = 3;
                    nextState = BACKUP_FROM_CORNER;
                    makeTransition = TRUE;
                }
                break;
            case ES_TIMEOUT:
                nextState = BACKUP_FROM_CORNER;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case BACKUP_FROM_CORNER:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 1500);
                break;
            case ES_TIMEOUT:
                nextState = SMALL_FORWARD;
                makeTransition = TRUE;
                break;

            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case BACKUP_AND_CHECK:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 200);
                break;
            case ES_TIMEOUT:
                nextState = WAIT_FOR_KICKBACK;
                makeTransition = TRUE;
                break;

            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case WAIT_FOR_KICKBACK:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 1000);
                break;
            case ES_TIMEOUT:
                nextState = WAIT;
                makeTransition = TRUE;
                break;

            case ES_EXIT:
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }
	
    case SMALL_FORWARD:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 100);
                break;
            case ES_TIMEOUT:
                nextState = TURN90CW;
                makeTransition = TRUE;
                break;

            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case TURN90CW: // move back facing outward from beginning castle
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90cw);
                Roach_RightMtrSpeed(v_R_turn90cw);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90+100);
                break;
            case ES_TIMEOUT:
                nextState = SCAN_B1;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case SCAN_B1:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                break;
            case BUMPED:
                PlaySound(4);
                if (ThisEvent.EventParam == 0x1) {
                    nextState = TURN_30_RIGHT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x2 || ThisEvent.EventParam == 0x3) {
                    nextState = TURN_10_LEFT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x10) {
                    nextState = TURN_30_LEFT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x8 || ThisEvent.EventParam == 0x18) {
                    nextState = TURN_10_RIGHT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam == 0x4 || ThisEvent.EventParam == 0xE || ThisEvent.EventParam == 0x12) {
                    nextState = WAIT;
                    makeTransition = TRUE;
                }
                break;
            case LIGHTLEVEL:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                if (ThisEvent.EventParam & (LIGHT_TO_DARK2 | LIGHT_TO_DARK3 | LIGHT_TO_DARK4)) {
                    nextState = BACKUP_AND_CHECK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam & (LIGHT_TO_DARK1)) {
                    nextState = TURN_30_RIGHT_BACK;
                    makeTransition = TRUE;
                } else if (ThisEvent.EventParam & (LIGHT_TO_DARK5)) {
                    nextState = TURN_30_LEFT_BACK;
                    makeTransition = TRUE;
                }
                break;
            case ES_EXIT:
                prevState = CurrentState;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case TURN_10_RIGHT_BACK:
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn10RBack);
                Roach_RightMtrSpeed(v_R_turn10RBack);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn10Back);
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            case ES_TIMEOUT:
                nextState = prevState;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    case TURN_10_LEFT_BACK:
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn10LBack);
                Roach_RightMtrSpeed(v_R_turn10LBack);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn10Back);
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            case ES_TIMEOUT:
                nextState = prevState;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;

    case TURN_30_RIGHT_BACK:
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn30RBack);
                Roach_RightMtrSpeed(v_R_turn30RBack);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Back);
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            case ES_TIMEOUT:
                nextState = prevState;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;

    case TURN_30_LEFT_BACK:
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn30LBack);
                Roach_RightMtrSpeed(v_R_turn30LBack);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Back);
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            case ES_TIMEOUT:
                nextState = prevState;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;


    case WAIT:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                if (detected == 0)
                {
                    ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_scan);
                    InitBeaconDetection();
                }
                else
                {
                    nextState = BACKUP;
                    makeTransition = TRUE;
                }
                break;
            case BEACON_DETECTED:
                ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                if (detected == 0 && ThisEvent.EventParam == B_OFF_TO_ON) {
                    ES_Timer_StopTimer(FANCY_ROACH_TIMER); // stop when it finds, no need to scan other beacon
                    detected = 1;
                    nextState = BACKUP;
                    makeTransition = TRUE;
                }
                break;
            case ES_TIMEOUT:
                nextState = BACKUP;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                CurrentState = TURN90CCW;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case BACKUP:
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp);
                break;
            case ES_TIMEOUT:
                PlaySound(3);
                if (detected == 0) { // if b1/b3 not found
                    ThisEvent.EventType = GO_BEACON;
                    ThisEvent.EventParam = 0x2;
                }
                else if (detected == 1) {
                    ThisEvent.EventType = GO_BEACON;
                    ThisEvent.EventParam = 0x1;
                }
                else if (detected == 3) {
                    ThisEvent.EventType = GO_BEACON;
                    ThisEvent.EventParam = 0x3;
                }
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            case ES_EXIT:
                CurrentState = TURN90CCW;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    default: // all unhandled states fall into here
        break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubHSMLocateBeacon(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMLocateBeacon(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

