/*
 * File: RoachHSM.h
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
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
 * Created on 23/Oct/2011
 * Updated on 16/Sep/2013
 */

#ifndef HSM_ROACH_H  // <- This should be changed to your own guard on both
#define HSM_ROACH_H  //    of these lines


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/
// typedefs for the states, which are used for the internal definition and also for
// the return of the query function.
// A preprocessor trick is used to make a list of strings alongside an enum list. To see the result,
// Rightclick -> Navigate -> view macro expansion
// Use unique state names!
#define LIST_OF_HSM_STATES(STATE)\
        STATE(InitHState) /*We typically use this as the InitState*/ \
        STATE(LOCATE_BEACON) \
        STATE(LOCATE_BACK_WALL) \
        STATE(GO_TO_BEACON_1) \
        STATE(GO_TO_BEACON_2) \
        STATE(GO_TO_BEACON_3) \
        STATE(RETURN_TO_BEACON_1) \
        STATE(RETURN_TO_BEACON_2) \
        STATE(RETURN_TO_BEACON_3) \
        STATE(GET_CROWN) \
        STATE(GET_INSIDE) \
        STATE(RETURN_CROWN) \
        STATE(MOVE_RIGHT) \
        STATE(MOVE_LEFT) \
        STATE(TEST_FORWARD) \
        STATE(TEST_TURN90) \


#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd
typedef enum {
    LIST_OF_HSM_STATES(ENUM_FORM)
} HSMState_t;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
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
uint8_t InitRoachHSM(uint8_t Priority);


/**
 * @Function PostTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostRoachHSM(ES_Event ThisEvent);


/**
 * @Function QueryTemplateHSM(void)
 * @param none
 * @return Current state of the state machine
 * @brief This function is a wrapper to return the current state of the state
 *        machine. Return will match the ENUM above. Remember to rename to
 *        something appropriate, and also to rename the TemplateState_t to your
 *        correct variable name.
 * @author J. Edward Carryer, 2011.10.23 19:25 */
HSMState_t QueryRoachHSM(void);

/**
 * @Function RunRoachHSM(ES_Event ThisEvent)
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
ES_Event RunRoachHSM(ES_Event ThisEvent);

#endif /* HSM_Template_H */

