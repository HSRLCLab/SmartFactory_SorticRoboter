/**
 * @file DetectPackageCtrl.h
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief The detect package controll class controlls the FSM to detect packages
 * @version 0.1
 * @date 2019-11-25
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

    State lastStateBevorError;  ///< holds the last State bevor error to resume after error
    State currentState;         ///< holds the current state of the FSM
    DetectPackageCtrl::Event currentEvent;         ///< holds the current event of the FSM

    RfidReaderCtrl::Package *pPackagePTr = nullptr;

    RfidReaderCtrl *pRfidReader = new RfidReaderCtrl(pPackagePTr);     ///< instance of rfid reader controll

    /**
     * @brief Functionpointer to call the current states do-function
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
     * - check for available package
     * - read package information
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
     * @return Event - generated Event
     */
    Event doAction_errorState();

    /**
     * @brief exit action of the state error state
     * 
     */
    void exitAction_errorState();

    /**
     * @brief decode the event
     * 
     * @param event - Event
     * @return String 
     */
    String decodeEvent(Event event);

    /**
     * @brief decode the state
     * 
     * @param state - State
     * @return String 
     */
    String decodeState(State state);
};

#endif