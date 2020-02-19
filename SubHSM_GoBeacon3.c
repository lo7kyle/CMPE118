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
#include "SubHSM_GoBeacon3.h"
#include "roach.h"
#include "SubHSM_GetCrown.h"
#include <stdio.h>
#include "RoachFrameworkEvents.h"
#include "dinkbot_vars.h"
/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(GoForward1) \
        STATE(GoForward2) \
        STATE(GoForward3) \
        STATE(TURN_90_RIGHT_EVADE) \
        STATE(TURN_90_LEFT_EVADE) \
        STATE(EVADE_FORWARD) \
        STATE(TURN90CCW) \
        STATE(Turn90_2) \
        STATE(GetInside) \
        STATE(FIX_POSITION) \
        STATE(TURN_10_RIGHT_BACK) \
        STATE(TURN_10_LEFT_BACK) \
        STATE(TURN_30_RIGHT_BACK) \
        STATE(TURN_30_LEFT_BACK) \
        STATE(TURN_30_RIGHT_CENTER) \
        STATE(TURN_30_LEFT_CENTER) \
        STATE(BACKUP) \
        STATE(BACKUP_FIX) \
        STATE(SMALL_BACKUP) \
        STATE(BACKUP_EVADE) \



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

uint16_t t_forwardCenter;

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
uint8_t InitSubHSMGoBeacon3(void) {
    ES_Event returnEvent;

    t_forwardCenter = t_forward2;
    CurrentState = InitPSubState;
    returnEvent = RunSubHSMGoBeacon3(INIT_EVENT);
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
ES_Event RunSubHSMGoBeacon3(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState; // <- change type to correct enum
    static SubTemplateState_t prevState = 0;
    ES_Tattle(); // trace call stack
    static uint32_t now, time, passed = 0;
    static uint32_t backUpTime = t_backUp;
    static uint8_t tapeOrWall = 0;

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state
                // now put the machine into the actual initial state
                CurrentState = GoForward1;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case GoForward1:
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
                            t_forwardCenter += 200;
                            tapeOrWall = 1;
                            nextState = BACKUP;
                            makeTransition = TRUE;
                        }
                        break;
                    case LIGHTLEVEL:
                        if (ThisEvent.EventParam & (LIGHT_TO_DARK2 |
                                LIGHT_TO_DARK3 | LIGHT_TO_DARK4)) {
                            backUpTime +=400;
                            nextState = BACKUP;
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
                break;
            }
            break;


        case GoForward2:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_forward + 50);
                        Roach_RightMtrSpeed(v_R_forward + 50);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, 1000);
                        break;
                    case BUMPED:
                        PlaySound(4);
                        ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                        if (ThisEvent.EventParam == 0x1) {
                            nextState = TURN_30_RIGHT_CENTER;
                            makeTransition = TRUE;
                        } else if (ThisEvent.EventParam == 0x10) {
                            nextState = TURN_30_LEFT_CENTER;
                            makeTransition = TRUE;
                        }
                        break;
                    case ES_TIMEOUT:
                        nextState = Turn90_2;
                        makeTransition = TRUE;
                        break;
                    case LIGHTLEVEL:
                        ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                        if (ThisEvent.EventParam & (LIGHT_TO_DARK1)) {
                            nextState = TURN_30_RIGHT_CENTER;
                            makeTransition = TRUE;
                        } else if (ThisEvent.EventParam & (LIGHT_TO_DARK5)) {
                            nextState = TURN_30_LEFT_CENTER;
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
                break;
            }
            break;

        case GoForward3:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_forward);
                        Roach_RightMtrSpeed(v_R_forward);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, 5000);
                        now = ES_Timer_GetTime();
                        break;
                    case BUMPED:
                        PlaySound(4);
                        time = ES_Timer_GetTime() - now;
                        if((time) >= 500)
                        {
                            passed = 1;
                        }
                        if(passed == 0)
                        {
                            nextState = BACKUP_EVADE;
                            makeTransition = TRUE;
                        }
                        else if (ThisEvent.EventParam == 0x1) {
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
                        } else if (ThisEvent.EventParam == 0x4 || ThisEvent.EventParam == 0xE || ThisEvent.EventParam == 0xA) {
                            nextState = SMALL_BACKUP;
                            makeTransition = TRUE;
                        }
                        break;
                    case LIGHTLEVEL:
                        if (ThisEvent.EventParam & (LIGHT_TO_DARK1)) {
                            nextState = TURN_30_RIGHT_CENTER;
                            makeTransition = TRUE;
                        } else if (ThisEvent.EventParam & (LIGHT_TO_DARK5)) {
                            nextState = TURN_30_RIGHT_CENTER;
                            makeTransition = TRUE;
                        } else if (ThisEvent.EventParam & (LIGHT_TO_DARK2 | LIGHT_TO_DARK3 | LIGHT_TO_DARK4)) {
                            nextState = BACKUP_EVADE;
                            makeTransition = TRUE;
                        }
                        break;
                    case ES_TIMEOUT:
                        nextState = BACKUP_FIX;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        prevState = CurrentState;
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
                break;
            }
            break;

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
                        nextState = GoForward3;
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

        case BACKUP_EVADE:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp + 100);
                        break;
                    case ES_TIMEOUT:
                        nextState = TURN_90_RIGHT_EVADE;
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

        case TURN_90_RIGHT_EVADE:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90cw);
                        Roach_RightMtrSpeed(v_R_turn90cw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90);
                        break;
                    case ES_TIMEOUT:
                        if (prevState == EVADE_FORWARD) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else {
                            nextState = EVADE_FORWARD;
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
                break;
            }
            break;
        }

        case TURN_90_LEFT_EVADE:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90ccw);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90+50);
                        break;
                    case ES_TIMEOUT:
                        if (prevState == EVADE_FORWARD) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else {
                            nextState = EVADE_FORWARD;
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
                break;
            }
            break;
        }

        case EVADE_FORWARD:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_forward);
                        Roach_RightMtrSpeed(v_R_forward);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_evade);
                        break;
                    case ES_TIMEOUT:
                        if (prevState == TURN_90_RIGHT_EVADE) {
                            nextState = TURN_90_LEFT_EVADE;
                            makeTransition = TRUE;
                        } else if (prevState == TURN_90_LEFT_EVADE) {
                            nextState = TURN_90_RIGHT_EVADE;
                            makeTransition = TRUE;
                        }
                        break;
                    case BUMPED:
                        break;
                    case LIGHTLEVEL:
                        break;
                    case ES_EXIT:
                        prevState = CurrentState;
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

        case TURN90CCW:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90ccw);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90 - 100);
                        break;
                    case ES_TIMEOUT:
                        nextState = GoForward2;
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

        case Turn90_2:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) { // An event is still active
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn90ccw);
                        Roach_RightMtrSpeed(v_R_turn90ccw);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90-100);
                        break;
                    case BUMPED:
                        PlaySound(4);
                        nextState = TURN_30_RIGHT_CENTER;
                        makeTransition = TRUE;
                        break;
                    case ES_TIMEOUT:
                        nextState = GoForward3;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case FIX_POSITION:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_forward);
                        Roach_RightMtrSpeed(v_R_forward);
                        break;
                    case BUMPED:
                        PlaySound(4);
                        ES_Timer_StopTimer(FANCY_ROACH_TIMER);

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
                            nextState = SMALL_BACKUP;
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
                        if (prevState == FIX_POSITION) {
                            nextState = FIX_POSITION;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward3) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward1) {
                            nextState = GoForward1;
                            makeTransition = TRUE;
                        }
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
                        if (prevState == FIX_POSITION) {
                            nextState = FIX_POSITION;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward3) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward1) {
                            nextState = GoForward1;
                            makeTransition = TRUE;
                        }
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case TURN_30_RIGHT_CENTER:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn30RCenter);
                        Roach_RightMtrSpeed(v_R_turn30RCenter);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Center);
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    case ES_TIMEOUT:
                        if (prevState == FIX_POSITION) {
                            nextState = FIX_POSITION;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward3) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward1) {
                            nextState = GoForward1;
                            makeTransition = TRUE;
                        } else if (prevState == Turn90_2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        }
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case TURN_30_LEFT_CENTER:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_turn30LCenter);
                        Roach_RightMtrSpeed(v_R_turn30LCenter);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Center);
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    case ES_TIMEOUT:
                        if (prevState == FIX_POSITION) {
                            nextState = FIX_POSITION;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward3) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward1) {
                            nextState = GoForward1;
                            makeTransition = TRUE;
                        }
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
                        if (prevState == FIX_POSITION) {
                            nextState = FIX_POSITION;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward3) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward1) {
                            nextState = GoForward1;
                            makeTransition = TRUE;
                        }
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
                        if (prevState == FIX_POSITION) {
                            nextState = FIX_POSITION;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward3) {
                            nextState = GoForward3;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward2) {
                            nextState = GoForward2;
                            makeTransition = TRUE;
                        } else if (prevState == GoForward1) {
                            nextState = GoForward1;
                            makeTransition = TRUE;
                        }
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case BACKUP:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, backUpTime);
                        break;
                    case ES_TIMEOUT:
                        nextState = TURN90CCW;
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

        case SMALL_BACKUP:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(v_L_backUp);
                        Roach_RightMtrSpeed(v_R_backUp);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp);
                        break;
                    case ES_TIMEOUT:
                        ThisEvent.EventType = DONE_EVADING;
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                    case ES_EXIT:
                        CurrentState = GoForward1;
                        break;
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

            //*/




        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubHSMGoBeacon3(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMGoBeacon3(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
