/**
 * @file SorticCtrl.h
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief The sortic controll class controlls the FSM of the sortic roboter
 *        The workflow is:
 *          - Check and read the package
 *          - Sort the received packgage in the correct smart box
 *          - Wait for the confirmation of the smart box
 * @version 1.1
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SORTICCTRL_H_
#define SORTICCTRL_H_

#include <Arduino.h>
#include "LogConfiguration.h"
#include "DetectPackageCtrl.h"
#include "SortPackageCtrl.h"
#include "I2cCommunication.h"



// I2C Buffers
// the buffer and the callback function are implemented in the sortic controll
// reasons are problems with implementation in class of i2c, maybe its possible to implement both global in the .h file
static ReceivedI2cMessage gReceivedMessage;                 ///< instance of received i2c message struct
static WriteI2cMessage gWriteMessage;                       ///< instance of write i2c message struct


/**
 * @brief Class to controll the FSM of the sortic controll
 * 
 */
class SorticCtrl
{
    //======================Public===========================================================
    public:

    /**
     * @brief Line class holds the different lines on the gametable
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
     * @brief Enum class which holds all possible events
     * 
     */
    enum class Event 
    {
        PackageReadyToSort,
        AnswerReceived,
        NoAnswerReceived,
        NoBoxAvailableForRegion,
        BoxAvailableForRegion,
        ReadyToSort,
        SBFilled,
        Error,
        Resume,
        Reset,
        NoEvent        
    };

    /**
     * @brief Parameter struct of the sortic which holds information about the current package and the sortic
     * 
     */
    struct Sortic
    {
        Line actualLine = Line::UploadLine;     ///< actualLine holds the acutal line (position) of the sortic
        Line targetLine = Line::UploadLine;     ///< targetLine holds the target line to sort the package
        RfidReaderCtrl::Package package;        ///< instance of package message struct
    } sortic;                                   ///< instance of sortic parameter struct             


    /**
     * @brief Construct a new Sortic Ctrl object
     * 
     */
    SorticCtrl();

    /**
     * @brief Destroy the Sortic Ctrl object
     * 
     */

    ~SorticCtrl();
    
    /**
     * @brief Calls the do-function of the active state and hence generates Event
     * 
     */
    void loop();

    /**
     * @brief Calls the do-function of the active state and hence generates Event
     * 
     * @param currentEvent 
     */
    void loop(Event currentEvent);

    /**
     * @brief I2c request callback function to request on call from master
     * 
     * @todo try to implement global in I2cCommunication.h file, check if informations are visible here!
     * 
     */
    static void requestCallback();
    
    /**
     * @brief I2c read callback function to read message from master
     * 
     * @todo try to implement global in I2cCommunication.h file, check if informations are visible here!
     * 
     * @param bytes - int
     */
    static void readCallback(int bytes);

    //======================PRIVATE==========================================================
    private:
    
    /**
     * @brief Enum class which holds all possible states of the FSM
     * 
     */
    enum class State
    {
        readRfidVal,
        waitForSort,
        sortPackageInBox,
        waitForArriv,
        errorState,
        resetState
    };

    unsigned long previousMillis;                   ///< store previous millis
    unsigned long previousMillisPublish;            ///< store previous mills of publish
    unsigned long currentMillis;                    ///< store current millis

    State lastStateBeforeError;                     ///< store the last state before error, will be needed to resume              
    State currentState;                             ///< store the current state of the FSM 
    Event currentEvent;                             ///< store the current event of the FSM

    DetectPackageCtrl *pDetectPackage;              ///< pointer to detect package controll
    SortPackageCtrl *pSortPackage;                  ///< pointer to sort package controll
    I2cCommunication *pBus = new I2cCommunication(I2CSLAVEADDRUNO, &readCallback, &requestCallback); ///< instance of i2c communication class

    /**
     * @brief functionpointer to call the current states do-function
     * 
     */
    Event (SorticCtrl::*doActionFPtr)(void) = nullptr;

    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e 
     */
    void process(Event e);

    /**
     * @brief entry action of the state read rfid value
     * 
     * - set i2c write message event to publish state
     * - wait for publish
     * - set i2c write message event to publish position
     * - wait for publish
     * 
     */
    void entryAction_readRfidVal();

    /**
     * @brief main action of the state read rfid value
     * 
     * - call the detect package controll to detect and read the package
     * 
     * @return Event 
     */
    Event doAction_readRfidVal();

    /**
     * @brief exit action of the state read rfid value
     * 
     * - set i2c write message event to publish package
     * - wait for publish
     * 
     */
    void exitAction_readRfidVal();

    /**
     * @brief entry action of the state wait for sort
     * 
     * - set i2c write message event to publish state
     * - wait for publish
     * - set i2c write message event to start box communications
     * - wait for begin
     * 
     */
    void entryAction_waitForSort();

    /**
     * @brief main action of the state wait for sort
     * 
     * - wait for i2c receives message event to sort package
     * 
     * @return Event 
     */
    Event doAction_waitForSort();

    /**
     * @brief exit action of the state wait for sort
     * 
     */
    void exitAction_waitForSort();

    /**
     * @brief entry action of the state sort package in box
     * 
     * - set i2c write message event to publish state
     * - wait for publish
     * 
     */
    void entryAction_sortPackageInBox();

    /**
     * @brief main action of the state sort package in box
     * 
     * - call the sort package controll to sort the package in the correct box
     * 
     * @return Event 
     */
    Event doAction_sortPackageInBox();

    /**
     * @brief exit action of the state sort package in box
     * 
     */
    void exitAction_sortPackageInBox();

    /**
     * @brief entry action of the state wait for arriv confirmation
     * 
     * - set i2c write message event to publish state
     * - wait for publish
     * 
     * - set i2c write message event to start arriv confirmation communications
     * - wait for begin
     * 
     */
    void entryAction_waitForArriv();
    
    /**
     * @brief wait for i2c received message event of smart box filled
     * 
     * @return Event 
     */
    Event doAction_waitForArriv();

    /**
     * @brief exit action of the stait wait for arriv confirmation
     * 
     */
    void exitAction_waitForArriv();

    /**
     * @brief entry action of the state error state
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the state error state
     * 
     * @todo Implementate the error state
     * 
     * @return Event 
     */
    Event doAction_errorState();

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
     * @todo Implementate reset state
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
     * @brief decode the state of the sortic controll FSM to a string
     * 
     * @param s 
     * @return String 
     */
    String decodeState(State s);

    /**
     * @brief decode the event of the sortic controll FSM to a string
     * 
     * @param e 
     * @return String 
     */
    String decodeEvent(Event e);
};

#endif // SORTICCTRL_H__