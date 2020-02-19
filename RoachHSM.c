/*
 * File: TemplateSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel Elkaim and Soja-Marie Morgens
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is another template file for the SubHSM's that is slightly differet, and
 * should be used for all of the subordinate state machines (flat or heirarchical)
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
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "RoachHSM.h"
#include "dinkbot_vars.h"
#include "RoachFrameworkEvents.h"
#include "SubHSM_Locate_Beacon.h"
#include "SubHSM_GoBeacon1.h"
#include "SubHSM_GoBeacon2.h"
#include "SubHSM_GoBeacon3.h"
#include "SubHSM_GetInside.h"
#include "SubHSM_LocateBackWall.h"
#include "SubHSM_GetOutMoat.h"
#include "SubHSM_GetCrown.h"
#include "SubHSM_ReturnCrown.h"
#include "IO_Ports.h"
#include "roach.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//Include any defines you need to do
/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_HSM_STATES(STRING_FORM)
};

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine
   Example: char RunAway(uint_8 seconds);*/
/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static HSMState_t CurrentState = InitHState; // <- change enum name to match ENUM
static uint8_t MyPriority;
static uint8_t returning = 0;
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitRoachHSM(uint8_t Priority) {
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = InitHState;
    // post the initial transition event
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostRoachHSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function QueryTemplateHSM(void)
 * @param none
 * @return Current state of the state machine
 * @brief This function is a wrapper to return the current state of the state
 *        machine. Return will match the ENUM above. Remember to rename to
 *        something appropriate, and also to rename the TemplateState_t to your
 *        correct variable name.
 * @author J. Edward Carryer, 2011.10.23 19:25 */
HSMState_t QueryRoachHSM(void) {
    return (CurrentState);
}

/**
 * @Function RunTemplateHSM(ES_Event ThisEvent)
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
ES_Event RunRoachHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    HSMState_t nextState; // <- change type to correct enum
    static uint8_t returningBeaconNum = 0;

    ES_Tattle(); // trace call stack
    Roach_BatteryVoltage();
    switch (CurrentState) {
        case InitHState: // If current state is initial Pseudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                InitSubHSMLocateBackWall();
                InitSubHSMGetCrown();
                InitSubHSMGoBeacon1();
                InitSubHSMGoBeacon2();
                InitSubHSMGoBeacon3();                InitSubHSMGetInside();
                InitSubHSMGetCrown();
                InitSubHSMReturnCrown();
                InitSubHSMLocateBeacon();

                Roach_MoveLiftMotor(0);
                Roach_MoveTrayMotor(0);
                int i = 0;
                for (i = 0; i < 10; i++)
                    Roach_BatteryVoltage();
                ES_Timer_InitTimer(FANCY_ROACH_TIMER, 1000);
            }
            if (ThisEvent.EventType == ES_TIMEOUT) {
                nextState = LOCATE_BACK_WALL;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;


        case LOCATE_BACK_WALL:
        {
            ThisEvent = RunSubHSMLocateBackWall(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        nextState = LOCATE_BEACON;
                        makeTransition = TRUE;
                    case ES_EXIT:
                        break;
                } // end switch
            } // end if
            break;
        } // end of case LOCATE_BACK_WALL

        case LOCATE_BEACON:
        {
            ThisEvent = RunSubHSMLocateBeacon(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case GO_BEACON:
                        switch (ThisEvent.EventParam) {
                            case 0x01:
                                makeTransition = TRUE;
                                nextState = GO_TO_BEACON_1;
                                returningBeaconNum = 3;
                                break;
                            case 0x02:
                                printf("B2\n\r");
                                makeTransition = TRUE;
                                nextState = GO_TO_BEACON_2;
                                returningBeaconNum = 2;
                                break;
                            case 0x03:
                                printf("B3\n\r");
                                makeTransition = TRUE;
                                nextState = GO_TO_BEACON_3;
                                returningBeaconNum = 1;
                                break;
                            default:
                                break;
                        }
                        break;
                    case ES_EXIT:
                        break;
                } // end switch
            } // end if
            break;
        } // end of case LOCATE_BEACON

        case GO_TO_BEACON_1:
        {
            ThisEvent = RunSubHSMGoBeacon1(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        printf("--DONE_EVADING--\n");
                        nextState = GET_INSIDE;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }
        case GO_TO_BEACON_2:
        {
            ThisEvent = RunSubHSMGoBeacon2(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        nextState = GET_INSIDE;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }
        case GO_TO_BEACON_3:
        {
            ThisEvent = RunSubHSMGoBeacon3(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        nextState = GET_INSIDE;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }

        case GET_INSIDE:
        {
            ThisEvent = RunSubHSMGetInside(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        if (returning == 0) {
                            nextState = GET_CROWN;
                            makeTransition = TRUE;
                            returning = 1;
                        } else {
                            nextState = RETURN_CROWN;
                            makeTransition = TRUE;
                        }
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }

        case GET_CROWN:
        {
            ThisEvent = RunSubHSMGetCrown(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        switch (returningBeaconNum) {
                            case 0x01:
                                printf("--go b1--\n");
                                nextState = RETURN_TO_BEACON_1;
                                makeTransition = TRUE;
                                break;
                            case 0x02:
                                printf("--go b2--\n");
                                nextState = RETURN_TO_BEACON_2;
                                makeTransition = TRUE;
                                break;
                            case 0x03:
                                printf("--go b3--\n");
                                nextState = RETURN_TO_BEACON_3;
                                makeTransition = TRUE;
                                break;
                        } // end of switch
                        break;
                    case ES_EXIT:
                        break;
                } // end switch
            } // end if
            break;
        } // end of case GET_CROWN

        case RETURN_TO_BEACON_1:
        {
            ThisEvent = RunSubHSMGoBeacon1(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        nextState = GET_INSIDE;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }
        case RETURN_TO_BEACON_2:
        {
            ThisEvent = RunSubHSMGoBeacon2(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        nextState = GET_INSIDE;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }
        case RETURN_TO_BEACON_3:
        {
            ThisEvent = RunSubHSMGoBeacon3(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        nextState = GET_INSIDE;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }
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

        case RETURN_CROWN:
        {
            ThisEvent = RunSubHSMReturnCrown(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;
                    case DONE_EVADING:
                        // crown returned
                        break;
                    case ES_EXIT:
                        break;
                }
            }
            break;
        }

        case TEST_FORWARD:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(-250);
                        Roach_RightMtrSpeed(-250);
                        //ES_Timer_InitTimer(FANCY_ROACH_TIMER, 800);
                        break;
                    case ES_TIMEOUT:
                        //nextState = TEST_TURN90;
                        //makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                }
            }
            break;
        }

        case TEST_TURN90:
        {
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        Roach_LeftMtrSpeed(-250);
                        Roach_RightMtrSpeed(250);
                        ES_Timer_InitTimer(FANCY_ROACH_TIMER, 430);
                        break;
                    case ES_TIMEOUT:
                        nextState = TEST_FORWARD;
                        makeTransition = TRUE;
                        break;
                    case ES_EXIT:
                        Roach_LeftMtrSpeed(0);
                        Roach_RightMtrSpeed(0);
                        break;
                }
            }
            break;
        }
        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunRoachHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunRoachHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
/*Here's where you put the actual content of your functions.
Example:
 * char RunAway(uint_8 seconds) {
 * Lots of code here
 * } */

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
/* Define TEMPLATEFSM_TEST to run this file as your main file (without the rest
 * of the framework)-useful for debugging */
#ifdef TEMPLATEHSM_TEST // <-- change this name and define it in your MPLAB-X
//     project to run the test harness
#include <stdio.h>

void main(void) {
    ES_Return_t ErrorType;
    BOARD_Init();
    // When doing testing, it is useful to annouce just which program
    // is running.

    printf("Starting the Hierarchical State Machine Test Harness \r\n");
    printf("using the 2nd Generation Events & Services Framework\n\r");

    // Your hardware initialization function calls go here

    // now initialize the Events and Services Framework and start it running
    ErrorType = ES_Initialize();

    if (ErrorType == Success) {
        ErrorType = ES_Run();
    }

    //
    //if we got to here, there was an error
    //

    switch (ErrorType) {
        case FailedPointer:
            printf("Failed on NULL pointer");
            break;
        case FailedInit:
            printf("Failed Initialization");
            break;
        default:
            printf("Other Failure");
            break;
    }

    while (1) {
        ;
    }
}

#endif // TEMPLATEHSM_TEST