/**
 * @file SortPackageCtrl.h
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief The sort package controll class controlls the FSM to sort the current package in the available box
 *        To sort the package in the box it calls the navigation controll and chassis controll of the sortic 
 *        and communicate to the robotarm
 * @version 1.1
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SORTPACKAGECTRL_H_
#define SORTPACKAGECTRL_H_

#include <Arduino.h>
#include "LogConfiguration.h"


/**
 * @brief Class to controll the FSM of the sort package controll
 * 
 */
class SortPackageCtrl
{
    //======================Public===========================================================
    public:

    /**
     * @brief Enum class which holds all positions
     * 
     */
    enum class Line
    {
        UploadLine,
        Line1,
        Line2,
        Line3,
        ErrorLine
    };

    /**
     * @brief Enum class which holds all events
     * 
     */
    enum class Event
    {
        UploadPackage,
        UnloadPackage,
        PackageUnloaded,        
        Error,
        Resume,
        Reset,
        NoEvent
    };

    /**
     * @brief Construct a new Sort Package Ctrl object
     * 
     * @param actualLine 
     * @param targetLine 
     */
    SortPackageCtrl(int *actualLine, int *targetLine);

    /**
     * @brief Destroy the Sort Package Ctrl object
     * 
     */
    ~SortPackageCtrl();

    /**
     * @brief Calls the do-function of the active state and hence generates Event
     * 
     */
    void loop();

    /**
     * @brief Calls the do-function of the active state and hence generates Event
     * 
     * @param event 
     */
    void loop(Event event);

    //======================PRIVATE==========================================================
    private:

    /**
     * @brief Enum class holds all possible states
     * 
     */
    enum class State
    {
        waitForSort,
        uploadPackage,
        unloadPackage,
        errorState,
        resetState
    };

    State lastStateBeforeError;                                 ///< holds the lastState bevor error to resume after error          
    State currentState;                                         ///< holds the currentState of the FSM
    State lastState;                                            ///< holds the lastState of the FSM
    Event currentEvent;                                         ///< holds the currentEvent of the FSM

    int *pActualLine = nullptr;                                 ///< holds the acutal line of the sortic
    int *pTargetLine = nullptr;                                 ///< holds the target line of the sotic

    /**
     * @brief functionpointer to call the current states do-function
     * 
     */
    Event (SortPackageCtrl::*doActionFPtr)(void) = nullptr;

    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e 
     */
    void process(Event e);

    /**
     * @brief entry action of the state wait for sort
     * 
     */
    void entryAction_waitForSort();

    /**
     * @brief main action of the state wait for sort
     * 
     * - do nothing and wait for I2C Event
     * 
     * @return SortPackageCtrl::Event 
     */
    SortPackageCtrl::Event doAction_waitForSort();

    /**
     * @brief exit action of the state wait for sort
     * 
     */
    void exitAction_waitForSort();

    /**
     * @brief entry action of the state upload package
     * 
     */
    void entryAction_uploadPackage();

    /**
     * @brief main action of the state upload package
     * 
     * - if is sortic not at the upload line, call navigation controll and driv to them
     * - communicate with the robot arm to upload the package
     * 
     * @todo integrate navigation controll and communication to robot arm
     * 
     * @return SortPackageCtrl::Event 
     */
    SortPackageCtrl::Event doAction_uploadPackage();

    /**
     * @brief exit action of the state upload package
     * 
     */
    void exitAction_uploadPackage();

    /**
     * @brief entry action of the state unload package
     * 
     */
    void entryAction_unloadPackage();

    /**
     * @brief main action of the state unload package
     * - call navigation controll with target line to drive to target line
     * - communicate with the robot arm to unload the package
     * - call navigation controll with upload line to drive back to upload line
     * 
     * @todo integrate navigation controll and communication with robot arm
     * 
     * @return SortPackageCtrl::Event 
     */
    SortPackageCtrl::Event doAction_unloadPackage();

    /**
     * @brief exit action of the state unload package
     * 
     */
    void exitAction_unloadPackage();

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
     * @return SortPackageCtrl::Event 
     */
    SortPackageCtrl::Event doAction_errorState();

    /**
     * @brief exit action of the state error state
     * 
     */
    void exitAction_errorState();

    /**
     * @brief entry action of the state reset state
     * 
     */
    void entryAction_resetState();

    /**
     * @brief main action of the state reset state
     * 
     * @todo implement reset state
     * 
     * @return Event 
     */
    Event doAction_resetState();

    /**
     * @brief exit action of the state reset state
     * 
     */
    void exitAction_resetState();

    /**
     * @brief decodes the state of the sort package controll
     * 
     * @param s 
     * @return String 
     */
    String decodeState(State s);

    /**
     * @brief decodes the event of the sort package controll
     * 
     * @param e 
     * @return String 
     */
    String decodeEvent(Event e);

};


#endif // SORTPACKAGECTRL_H__