#ifndef SORTICCTRL_H_
#define SORTICCTRL_H_

#include <Arduino.h>
#include "LogConfiguration.h"
#include "DetectPackageCtrl.h"



class SorticCtrl
{
    public:

    enum class Event 
    {
        PackageReadyToSort = 0,
        AnswerReceived = 1,
        NoAnswerReceived = 2,
        NoBoxAvailableForRegion = 3,
        BoxAvailableForRegion = 4,
        ReadyToSort = 5,
        SBFilled = 6,
        Error = 7,
        Resume = 8,
        Reset = 9,
        NoEvent = 10        
    };

    SorticCtrl();
    
    void loop();

    void loop(Event currentEvent);

    private:
    
    enum class State
    {
        readRfidVal = 1,
        waitForBox = 2,
        calcOptBox = 3,
        simulatePackageBuffer = 4,
        requestOptBox = 5,
        publishOptBox = 6,
        waitForSort = 7,
        sortPackageInBox = 8,
        waitForArriv = 9,
        errorState = 10,
        resetState = 11
    };

    unsigned long previousMillis;
    unsigned long previousMillisPublish;
    unsigned long currentMillis;

    State lastStateBeforeError;                
    State currentState;                        
    Event currentEvent;

    DetectPackageCtrl pDetectPackage;

    SorticCtrl::Event (SorticCtrl::*doActionFPtr)(void) = nullptr;

    void process(Event e);

    void entryAction_readRfidVal();

    Event doAction_readRfidVal();

    void exitAction_readRfidVal();

    void entryAction_waitForBox();

    Event doAction_waitForBox();

    void exitAction_waitForBox();

    void entryAction_calcOptBox();

    Event doAction_calcOptBox();

    void exitAction_calcOptBox();

    void entryAction_simulatePackageBuffer();

    Event doAction_simulatePackageBuffer();

    void exitAction_simulatePackageBuffer();

    void entryAction_requestOptBox();

    Event doAction_requestOptBox();

    void exitAction_requestOptBox();

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