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
#include "SubHSM_GetCrown.h"
#include <stdio.h>
#include "dinkbot_vars.h"
#include "roach.h"


/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(EJECT_TRAY) \
        STATE(LIFT_TRAY_1) \
        STATE(LIFT_TRAY_2) \
        STATE(RETRACT_TRAY) \
        STATE(LOWER_TRAY) \
        STATE(BACKUP_1) \
        STATE(BACKUP_FIX) \
        STATE(TURN_90_1) \
        STATE(BACKUP_2) \
        STATE(TURN_90_2) \
        STATE(BACKUP_3) \
        STATE(TURN_90_3) \
        STATE(FIX_POSITION) \
        STATE(TURN_10_RIGHT_BACK) \
        STATE(TURN_10_LEFT_BACK) \
        STATE(SHAKE_LEFT) \
        STATE(SHAKE_RIGHT) \


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


#define COUNT_MAX_WIGGLE 5
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
uint8_t InitSubHSMGetCrown(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubHSMGetCrown(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunSubHSMGetCrown(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState;
    static SubTemplateState_t prevState;
    static SubTemplateState_t afterState;
    static uint8_t count_shake = 0;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                CurrentState = EJECT_TRAY;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SHAKE_LEFT:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_shake);
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        break;
                    case ES_TIMEOUT:
                        nextState = SHAKE_RIGHT;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                } // end switch
                break;
            } // end if
            break;
        } // end case
        case SHAKE_RIGHT:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90cw);
                        Roach_RightMtrSpeed(0);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_shake);
                        break;
                    case ES_TIMEOUT:
                        if (count_shake < COUNT_MAX_WIGGLE) {
                            nextState = SHAKE_LEFT;
                            makeTransition = TRUE;
                            count_shake += 1;
                        } else {
                            if(prevState == LIFT_TRAY_1)
                            {
                                nextState = LIFT_TRAY_2;
                                makeTransition = TRUE;
                            }
                            else // if previous state is eject tray
                            { // after shaking for MAX_WIGGLE of times
                                Roach_MoveTrayMotor(v_eject); // keep ejecting tray to keep it from jamming
                                nextState = LIFT_TRAY_1; // then start lifting
                                makeTransition = TRUE;
                            }
                        }
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                } // end switch
                break;
            } // end if
            break;
        }

        case EJECT_TRAY:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_MoveTrayMotor(v_eject);
                        nextState = SHAKE_LEFT;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        prevState = CurrentState;
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                } // end switch
                break;
            } // end if
            break;
        } // end case

        case LIFT_TRAY_1: // this state is for preventing jamming to the hexagonal corners.
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_MoveLiftMotor(v_lift);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_lift1);
                        break;
                    case ES_TIMEOUT:
                        nextState = SHAKE_LEFT;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_MoveLiftMotor(0);
                        prevState = CurrentState;
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                } // end switch
                break;
            } // end ifv_L_forward
            break;
        } // end case
        case LIFT_TRAY_2: // then lift the tray to its fullest
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_MoveLiftMotor(v_lift);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_lift2);
                        break;
                    case ES_TIMEOUT:
                        nextState = RETRACT_TRAY;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_MoveLiftMotor(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                } // end switch
                break;
            } // end if
            break;
        }
        case RETRACT_TRAY:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        PlaySound(5);
                        Roach_MoveTrayMotor(v_retract);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_retract);
                        break;
                    case ES_TIMEOUT:
                        Roach_MoveTrayMotor(0);
                        nextState = BACKUP_1;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        // never exit this state, stay here.
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                } // end switch
                break;
            } // end if
            break;
        } // end case

        case BACKUP_1:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp+20);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, 650 - 470);
                        break;
                    case ES_TIMEOUT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        nextState = TURN_90_1;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case TURN_90_1:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90ccw);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp);
                        break;
                    case ES_TIMEOUT:
                        nextState = FIX_POSITION;
                        afterState = BACKUP_2;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case FIX_POSITION:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_forward);
                        Roach_RightMtrSpeed(v_L_forward);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, 5000);
                        break;
                    case BUMPED:
                        ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                        if (ThisEvent.EventParam == 0x1 || ThisEvent.EventParam == 0x2 || ThisEvent.EventParam == 0x3) {
                            nextState = TURN_10_LEFT_BACK;
                            makeTransition = TRUE;
                        }
                        if (ThisEvent.EventParam == 0x10 || ThisEvent.EventParam == 0x8 || ThisEvent.EventParam == 0x18) {
                            nextState = TURN_10_RIGHT_BACK;
                            makeTransition = TRUE;
                        }
                        if (ThisEvent.EventParam == 0x4) {
                            nextState = afterState;
                            makeTransition = TRUE;
                        }
                        break;
                    case ES_TIMEOUT:
                        nextState = BACKUP_FIX;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case BACKUP_FIX:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp-200);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUpFix);
                        break;
                    case ES_TIMEOUT:
                        nextState = FIX_POSITION;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case TURN_10_RIGHT_BACK:
        {
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
                        nextState = FIX_POSITION;
                        makeTransition = TRUE;
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case TURN_10_LEFT_BACK:
        {
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
                        nextState = FIX_POSITION;
                        makeTransition = TRUE;
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case BACKUP_2:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp+100);
                        break;
                    case ES_TIMEOUT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        nextState = TURN_90_2;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case TURN_90_2:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90ccw);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90);
                        break;
                    case ES_TIMEOUT:
                        nextState = FIX_POSITION;
                        afterState = BACKUP_3;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case BACKUP_3:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp -200);
                        break;
                    case ES_TIMEOUT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        nextState = TURN_90_3;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        case TURN_90_3:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90ccw);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90-100);
                        break;
                    case ES_TIMEOUT:
                        ThisEvent.EventType = DONE_EVADING;
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;
        }

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubHSMGetCrown(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMGetCrown(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

