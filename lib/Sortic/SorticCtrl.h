#ifndef SORTICCTRL_H_
#define SORTICCTRL_H_

#include <Arduino.h>
#include "LogConfiguration.h"
#include "DetectPackageCtrl.h"
#include "SortPackageCtrl.h"
#include "I2cCommunication.h"



// I2C Buffers
static ReceivedI2cMessage gReceivedMessage;
static WriteI2cMessage gWriteMessage;


class SorticCtrl
{
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

    struct Sortic
    {
        Line actualLine = Line::UploadLine;
        Line targetLine = Line::UploadLine;
        RfidReaderCtrl::Package package;   ///< instance of package message
    } sortic;


    SorticCtrl();

    ~SorticCtrl();
    
    void loop();

    void loop(Event currentEvent);

    /**
     * @brief I2c request callback function to request on call from master
     * 
     */
    static void requestCallback();
    
    /**
     * @brief I2c read callback function to read message from master
     * 
     * @param bytes - int
     */
    static void readCallback(int bytes);

    private:
    
    enum class State
    {
        readRfidVal,
        waitForSort,
        sortPackageInBox,
        waitForArriv,
        errorState,
        resetState
    };

    unsigned long previousMillis;
    unsigned long previousMillisPublish;
    unsigned long currentMillis;

    State lastStateBeforeError;                
    State currentState;            
    Event currentEvent;     

    DetectPackageCtrl *pDetectPackage;// = new DetectPackageCtrl(&(sortic.package));
    SortPackageCtrl *pSortPackage;// = new SortPackageCtrl((int*)&(sortic.actualLine), (int*)&(sortic.targetLine));
    I2cCommunication *pBus = new I2cCommunication(I2CSLAVEADDRESP, &readCallback, &requestCallback);

    Event (SorticCtrl::*doActionFPtr)(void) = nullptr;

    void process(Event e);

    void entryAction_readRfidVal();

    Event doAction_readRfidVal();

    void exitAction_readRfidVal();


    void entryAction_waitForSort();

    Event doAction_waitForSort();

    void exitAction_waitForSort();


    void entryAction_sortPackageInBox();

    Event doAction_sortPackageInBox();

    void exitAction_sortPackageInBox();


    void entryAction_waitForArriv();
    
    Event doAction_waitForArriv();

    void exitAction_waitForArriv();


    void entryAction_errorState();

    Event doAction_errorState();

    void exitAction_errorState();


    void entryAction_resetState();

    Event doAction_resetState();

    void exitAction_resetState();
    

    void publishState(State s);

    String decodeState(State s);

    String decodeEvent(Event e);

    
    
};

#endif