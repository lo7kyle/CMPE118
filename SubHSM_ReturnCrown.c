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
#include "SubHSM_ReturnCrown.h"
//#include "dinkbot_vars.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_TEMPLATE_STATES(STATE) \
        STATE(InitPSubState) \
        STATE(EJECT_TRAY) \
        STATE(LIFT_TRAY) \
        STATE(RETRACT_TRAY) \
        STATE(LOWER_TRAY) \


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

static SubTemplateState_t CurrentState = InitPSubState;   // <- change name to match ENUM
static uint8_t MyPriority;

static int16_t v_eject, v_lift, v_retract, v_lower, t_eject, t_lift, t_retract,
        t_lower;
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
uint8_t InitSubHSMReturnCrown(void)
{
    ES_Event returnEvent;
    
    t_eject = 1500;
    t_lower = 1000;
    t_retract = 1050;
    v_eject = 500;
    v_lower = -500;
    v_retract = -500;

    CurrentState = InitPSubState;
    returnEvent = RunSubHSMReturnCrown(INIT_EVENT);
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
ES_Event RunSubHSMReturnCrown(ES_Event ThisEvent)
{
    uint8_t makeTransition = FALSE; // use to flag transition
    SubTemplateState_t nextState;      // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
    case InitPSubState: // If current state is initial Psedudo State
        if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
        {
            CurrentState = EJECT_TRAY;
            ThisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case EJECT_TRAY: {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_MoveTrayMotor(v_eject);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_eject);
                break;
            case ES_TIMEOUT:
                nextState = LOWER_TRAY;
                makeTransition = TRUE;
                break;
            case ES_EXIT:
                Roach_MoveTrayMotor(0);
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            } // end switch
            break;
        } // end if
        break;
    } // end case

    case LOWER_TRAY: {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                Roach_MoveLiftMotor(v_lower);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_lower);
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
    } // end case
    
    case RETRACT_TRAY: {
        if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
            switch (ThisEvent.EventType) {
            case ES_ENTRY:
                PlaySound(6);
                Roach_MoveTrayMotor(v_retract);
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, t_retract);
                break;
            case ES_TIMEOUT:
                // exit sub state machine here to higher level state machine.
                Roach_MoveTrayMotor(0);
                ThisEvent.EventType = CROWN_PLACED;
                break;
            case ES_EXIT:
                break;
            default: // all unhandled events pass the event back up to the next level
                break;
            } // end switch
            break;
        } // end if
        break;
    } // end case

    default: // all unhandled states fall into here
        break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubHSMReturnCrown(EXIT_EVENT);   // <- rename to your own Run function
        CurrentState = nextState;
        RunSubHSMReturnCrown(ENTRY_EVENT);  // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

