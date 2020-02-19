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
#include "SubHSM_Party.h"
#include <stdio.h>
#include "dinkbot_vars.h"
#include "roach.h"


/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(MOVE_LEFT) \
        STATE(MOVE_RIGHT) \


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
uint8_t InitSubHSMParty(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubHSMParty(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunSubHSMParty(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                PlaySound(6);
                CurrentState = MOVE_LEFT;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MOVE_LEFT:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90+500);
                        Roach_LeftMtrSpeed(-280);
                        Roach_RightMtrSpeed(280);
                        Roach_MoveLiftMotor(v_lift);
                        Roach_MoveTrayMotor(v_eject);

                        break;
                    case ES_TIMEOUT:
                        nextState = MOVE_RIGHT;
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
        case MOVE_RIGHT:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(280);
                        Roach_RightMtrSpeed(-280);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_turn90+500);
                        Roach_MoveLiftMotor(-v_lift);
                        Roach_MoveTrayMotor(v_retract);
                        break;
                    case ES_TIMEOUT:
                        nextState = MOVE_LEFT;
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
        }

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubHSMParty(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMParty(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

