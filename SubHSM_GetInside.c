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
#include "SubHSM_GetInside.h"
#include "roach.h"
#include <stdio.h>
#include "RoachFrameworkEvents.h"
#include "dinkbot_vars.h"
/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(TOWARD_CASTLE) \
        STATE(TOWARD_BACK_WALL) \
        STATE(TURN90) \
        STATE(TURN90_2) \
        STATE(BACKUP) \
        STATE(TOWARD_THRONE) \
        STATE(FIX_POSITION) \
        STATE(TURN_10_RIGHT_BACK) \
        STATE(TURN_10_LEFT_BACK) \
        STATE(TURN_30_RIGHT_BACK) \
        STATE(TURN_30_LEFT_BACK) \
        STATE(SMALL_BACKUP) \
        STATE(BACKUP_FIX) \



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
uint8_t InitSubHSMGetInside(void)
{
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubHSMGetInside(INIT_EVENT);
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
ES_Event RunSubHSMGetInside(ES_Event ThisEvent)
{
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState; // <- change type to correct enum
    static SubTemplateState_t prevState = 0;
    ES_Tattle(); // trace call stack

    switch (CurrentState) {
    case InitPSubState: // If current state is initial Psedudo State
        if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
        {
            CurrentState = TOWARD_CASTLE;

            ThisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case TOWARD_CASTLE:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_forward);
                Roach_RightMtrSpeed(v_R_forward);
                nextState = FIX_POSITION;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                prevState = CurrentState;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    }
    case TURN90:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90cw);
                Roach_RightMtrSpeed(v_R_turn90cw);
                if(prevState == TOWARD_BACK_WALL)
                    ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90+50);
                else if(prevState == TOWARD_CASTLE)
                    ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90-100);
                else
                    ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90);
                break;
            case ES_TIMEOUT:
                if (prevState == TOWARD_CASTLE) {
                    nextState = TOWARD_BACK_WALL;
                    makeTransition = TRUE;
                } else if (prevState == SMALL_BACKUP) {
                    nextState = TOWARD_THRONE;
                    makeTransition = TRUE;
                }
                break;
            case LIGHTLEVEL:
                // if the front right
                /*if (ThisEvent.EventParam & (DARK_TO_LIGHT4)) {
                    ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                    nextState = TOWARD_THRONE;
                    makeTransition = TRUE;
                }*/
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                prevState = CurrentState;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
        }
        break;
    }

    case TURN90_2:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_turn90cw);
                Roach_RightMtrSpeed(v_R_turn90cw);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90 - 100);
                break;
            case ES_TIMEOUT:
                if (prevState == TOWARD_CASTLE) {
                    nextState = TOWARD_BACK_WALL;
                    makeTransition = TRUE;
                } else if (prevState == SMALL_BACKUP) {
                    nextState = TOWARD_THRONE;
                    makeTransition = TRUE;
                }
                break;
            case LIGHTLEVEL:
            {
                // if the front right
                if (ThisEvent.EventParam & (DARK_TO_LIGHT4)) {
                    ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                    nextState = TOWARD_THRONE;
                    makeTransition = TRUE;
                }
            }
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                prevState = CurrentState;
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
                Roach_LeftMtrSpeed(v_L_forward - 20);
                Roach_RightMtrSpeed(v_R_forward - 20);
                ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 5000);
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
                } else if (ThisEvent.EventParam == 0x4) {
                    if (prevState == TOWARD_THRONE) {
                        ThisEvent.EventType = DONE_EVADING;
                    } else if (prevState == TOWARD_CASTLE) {
                        nextState = BACKUP;
                        makeTransition = TRUE;
                    } else if (prevState == TOWARD_BACK_WALL) {
                        nextState = SMALL_BACKUP;
                        makeTransition = TRUE;
                    }
                }
                break;
            case ES_TIMEOUT:
                nextState = BACKUP_FIX;
                makeTransition = TRUE;
                break;

            case ES_EXIT:
                if (prevState == TOWARD_THRONE) {
                    CurrentState = TOWARD_CASTLE;
                }
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    } // end of case FIX_POS

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
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Back-50);
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
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn30Back-50);
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
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_backUp - 290);
                break;
            case ES_TIMEOUT:
                if (prevState == TOWARD_CASTLE) {
                    nextState = TURN90;
                    makeTransition = TRUE;
                }
                break;

            case ES_EXIT:
                ES_Timer_StopTimer(FANCY_ROACH_TIMER);
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
                nextState = FIX_POSITION;
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
            break;
        }
        break;
    }

    case SMALL_BACKUP:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_LeftMtrSpeed(v_L_backUp);
                Roach_RightMtrSpeed(v_R_backUp);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_smallBackup);
                break;
            case ES_TIMEOUT:
                makeTransition = TRUE;
                nextState = TURN90_2;
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                prevState = CurrentState;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    }

    case TOWARD_BACK_WALL:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                nextState = FIX_POSITION;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                prevState = CurrentState;
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            }
            break;
        }
        break;
    }

    case TOWARD_THRONE:
    {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                nextState = FIX_POSITION;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                prevState = CurrentState;
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

    if (makeTransition == TRUE) {
        RunSubHSMGetInside(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMGetInside(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
