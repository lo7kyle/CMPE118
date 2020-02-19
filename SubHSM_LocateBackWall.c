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
#include "RoachHSM.h"
#include "SubHSM_LocateBackWall.h"
#include "IO_Ports.h"
#include "dinkbot_vars.h"
#include "RoachFrameworkEvents.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(MOVE_FORWARD) \
        STATE(STOP_AND_CHECK) \
        STATE(REVERSE_SAME_TIME) \
        STATE(TURN_30_RIGHT) \
        STATE(FIX_POSITION) \
        STATE(TURN_10_RIGHT_BACK) \
        STATE(TURN_10_LEFT_BACK) \
        STATE(TURN_30_RIGHT_BACK) \
        STATE(TURN_30_LEFT_BACK) \
        STATE(TURN_90_LEFT) \
        STATE(MOVE_TILL_HIT) \
        STATE(TURN_90_LEFT_BACK) \
        STATE(BACKUP) \
        STATE(BACKUP_FIX) \
        STATE(BACKUP_SIDE_WALL) \
        STATE(TURN_90_LEFT_BACK_SIDE_WALL) \
        STATE(TURN_10_RIGHT_BACK_SIDE_WALL) \
        STATE(TURN_10_LEFT_BACK_SIDE_WALL) \
        STATE(FIX_POSITION_SIDE_WALL) \

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
uint8_t InitSubHSMLocateBackWall(void)
{
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubHSMLocateBackWall(INIT_EVENT);

    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunSubHSMLocateBackWall(ES_Event ThisEvent)
{
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState; // <- change type to correct enum
    static SubTemplateState_t afterState; // <- change type to correct enum
    static uint16_t time;
    uint32_t i;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
    case InitPSubState: // If current state is initial Psedudo State
        if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
        {
            CurrentState = MOVE_FORWARD;
            ThisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case MOVE_FORWARD:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_forward);
                time = ES_Timer_GetTime();
                break;
            case BUMPED:
                PlaySound(4);
                ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                time = ES_Timer_GetTime() - time;
                nextState = STOP_AND_CHECK;
                makeTransition = TRUE;
                break;
            case ES_TIMEOUT:
                time = t_forward;
                nextState = STOP_AND_CHECK;
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

    case STOP_AND_CHECK:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 100);
                InitTrackWireDetection();
                break;
            case TRACKWIRE:
                if (ThisEvent.EventParam == TW_OFF_TO_ON) {
                    ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                    nextState = FIX_POSITION;
                    makeTransition = TRUE;
                }
                break;
            case ES_TIMEOUT:
                nextState = REVERSE_SAME_TIME;
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

    case REVERSE_SAME_TIME:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                if (time == 0)
                    time = 2000;
                Roach_LeftMtrSpeed(v_L_backUp + 50);
                Roach_RightMtrSpeed(v_R_backUp + 50);
                //printf("%d\n", time);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, time);
                break;
            case ES_TIMEOUT:
                nextState = TURN_30_RIGHT;
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

    case TURN_30_RIGHT:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90cw);
                Roach_RightMtrSpeed(v_R_turn90cw);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30);
                time = ES_Timer_GetTime();
                break;
            case ES_TIMEOUT:
                nextState = MOVE_FORWARD;
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


        /********************************************************************************
         FIX POSITION
         ********************************************************************************/

    case FIX_POSITION:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 5000);
                break;
            case BUMPED:
                PlaySound(4);
                if (ThisEvent.EventParam == 0x1) {
                    nextState = TURN_30_LEFT_BACK;
                    makeTransition = TRUE;
                }
                if (ThisEvent.EventParam == 0x2 || ThisEvent.EventParam == 0x3) {
                    nextState = TURN_10_LEFT_BACK;
                    makeTransition = TRUE;
                }
                if (ThisEvent.EventParam == 0x10) {
                    nextState = TURN_30_RIGHT_BACK;
                    makeTransition = TRUE;
                }
                if (ThisEvent.EventParam == 0x8 || ThisEvent.EventParam == 0x18) {
                    nextState = TURN_10_RIGHT_BACK;
                    makeTransition = TRUE;
                }
                if (ThisEvent.EventParam == 0x4 || ThisEvent.EventParam == 0xE || ThisEvent.EventParam == 0x12) {
                    nextState = BACKUP;
                    makeTransition = TRUE;
                }
                break;
            case ES_TIMEOUT:
                nextState = BACKUP_FIX;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                afterState = FIX_POSITION;
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
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUpFix);
                break;
            case ES_TIMEOUT:
                nextState = afterState;
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

    case TURN_30_RIGHT_BACK:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn10RBack);
                Roach_RightMtrSpeed(v_R_turn10RBack);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Back);
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

    case TURN_30_LEFT_BACK:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn10LBack);
                Roach_RightMtrSpeed(v_R_turn10LBack);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Back);
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

    case BACKUP:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 500);
                break;
            case ES_TIMEOUT:
                nextState = TURN_90_LEFT;
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

    case BACKUP_SIDE_WALL:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 200);
                break;
            case ES_TIMEOUT:
                nextState = TURN_90_LEFT_BACK;
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

    case TURN_90_LEFT:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90ccw);
                Roach_RightMtrSpeed(v_R_turn90ccw);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90);
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            case ES_TIMEOUT:
                nextState = FIX_POSITION_SIDE_WALL;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    }

    case MOVE_TILL_HIT:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                break;
            case BUMPED:
                nextState = TURN_90_LEFT_BACK;
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

    case FIX_POSITION_SIDE_WALL:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_forward);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 5000);
                break;
            case BUMPED:
                ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                if (ThisEvent.EventParam == 0x1 || ThisEvent.EventParam == 0x2 || ThisEvent.EventParam == 0x3) {
                    nextState = TURN_10_LEFT_BACK_SIDE_WALL;
                    makeTransition = TRUE;
                }
                if (ThisEvent.EventParam == 0x10 || ThisEvent.EventParam == 0x8 || ThisEvent.EventParam == 0x18) {
                    nextState = TURN_10_RIGHT_BACK_SIDE_WALL;
                    makeTransition = TRUE;
                }
                if (ThisEvent.EventParam == 0x4 || ThisEvent.EventParam == 0xE || ThisEvent.EventParam == 0x12) {
                    nextState = BACKUP_SIDE_WALL;
                    afterState = TURN_90_LEFT;
                    makeTransition = TRUE;
                }
                break;
            case ES_TIMEOUT:
                nextState = BACKUP_FIX;
                afterState = FIX_POSITION_SIDE_WALL;
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

    case TURN_10_RIGHT_BACK_SIDE_WALL:
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
                nextState = FIX_POSITION_SIDE_WALL;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    }

    case TURN_10_LEFT_BACK_SIDE_WALL:
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
                nextState = FIX_POSITION_SIDE_WALL;
                makeTransition = TRUE;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    }

    case TURN_90_LEFT_BACK:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) {
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90ccw);
                Roach_RightMtrSpeed(v_R_turn90ccw);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90);
                break;
            case ES_TIMEOUT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                PlaySound(2);
                ThisEvent.EventType = DONE_EVADING;
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

    default: // all unhandled states fall into here
        break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubHSMLocateBackWall(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMLocateBackWall(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

