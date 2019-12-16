/**
 * @file DetectPackageCtrl.h
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief The detect package controll class controlls the FSM to detect packages
 *        It is a sub finite state machine of the sortic controll
 * @version 1.0
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef DETECTPACKAGECTRL_H_
#define DETECTPACKAGECTRL_H_

#include <Arduino.h>

// own files:
#include "RfidReaderCtrl.h"

/**
 * @brief The detect package controll class controlls the FSM to detect packages
 * 
 */
class DetectPackageCtrl
{
    //======================PUBLIC===========================================================
    public:

    /**
     * @brief Enum class holds all possible events for the FSM
     * 
     */
    enum class Event
    {
        CheckForPackage = 0,
        PackageAvailableToSort = 1,
        NoPackageAvailable = 2,
        PackageReadyToSort = 3,
        PackageSorted = 4,
        InterruptForSort = 5,
        Error = 5,
        Resume = 6,
        NoEvent = 7
    };

    /**
     * @brief Calls the do-function of the active state and hence generates Event
     * 
     */
    void loop();

    /**
     * @brief Calls the do-function of the active state and hence generates Event
     * 
     * @param currentEvent - Event
     */
    void loop(DetectPackageCtrl::Event event);

    

    /**
     * @brief Construct a new Detect Package Ctrl object
     * 
     */
    DetectPackageCtrl(RfidReaderCtrl::Package *packagePtr);

    //======================PRIVATE==========================================================
    private:

    /**
     * @brief Enum class holds all possible states
     * 
     */
    enum class State
    {
        emptyState,
        checking,
        fullState,
        errorState
    };

    State lastStateBevorError;                                          ///< holds the last State bevor error to resume after error
    State currentState;                                                 ///< holds the current state of the FSM
    DetectPackageCtrl::Event currentEvent;                              ///< holds the current event of the FSM

    RfidReaderCtrl::Package *pPackagePTr = nullptr;                     ///< pointer to the package struct of the sortic controll 
                                                                        ///< will be initialized in the constructor

    RfidReaderCtrl *pRfidReader = new RfidReaderCtrl(pPackagePTr);     ///< instance of rfid reader controll

    /**
     * @brief functionpointer to call the current states do-function
     * 
     */
    Event (DetectPackageCtrl::*doActionFPtr)(void) = nullptr;

    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);

    /**
     * @brief entry action of the state empty state
     * 
     */
    void entryAction_emptyState();

    /**
     * @brief main action of the state empty state
     * 
     * - do nothing and delay
     *
     * @return Event - generated Event 
     */
    Event doAction_emptyState();

    /**
     * @brief exit action of the state empty state
     * 
     */
    void exitAction_emptyState();
    
    /**
     * @brief entry action of the state checking
     * 
     */
    void entryAction_checking();

    /**
     * @brief main action of the state checking
     * 
     * - call rfid reader controll
     * --> check for available package
     * --> read package information
     * --> store package information to package struct in sortic controll
     * 
     * @return Event - generated Event
     */
    Event doAction_checking();

    /**
     * @brief exit action of the state checking
     * 
     */
    void exitAction_checking();

    /**
     * @brief entry action of the state full state
     * 
     */
    void entryAction_fullState();

    /**
     * @brief main action of the state full state
     * 
     * - do nothing and return package ready to sort
     * 
     * @return Event - generated Event
     */
    Event doAction_fullState();

    /**
     * @brief exit action of the state full state
     * 
     */
    void exitAction_fullState();

    /**
     * @brief entry action of the state error state
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the state error state
     * 
     * @todo implement error state
     * 
     * @return Event - generated Event
     */
    Event doAction_errorState();

    /**
     * @brief exit action of the state error state
     * 
     */
    void exitAction_errorState();

    /**
     * @brief decodes the event of detect package controll finite state machine
     * 
     * @param event - Event
     * @return String 
     */
    String decodeEvent(Event event);

    /**
     * @brief decodes the state of detect package controll finite state machine
     * 
     * @param state - State
     * @return String 
     */
    String decodeState(State state);
};

#endif // DETECTPACKAGECTRL_H_