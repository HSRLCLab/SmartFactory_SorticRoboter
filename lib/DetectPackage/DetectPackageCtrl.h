
#ifndef DETECTPACKAGECTRL_H_
#define DETECTPACKAGECTRL_H_

#include <Arduino.h>
#include "RfidCtrl.h"



class DetectPackageCtrl
{
    public:

    enum class Event
    {
        CheckForPackage = 0,
        PackageAvailableToSort = 1,
        NoPackageAvailable = 2,
        PackageReadyToSort = 3,
        Error = 4,
        Resume = 5,
        NoEvent = 6
    };

    
    Package newPackage;

    DetectPackageCtrl();

    DetectPackageCtrl::Event process(Event e);

    private:

    enum class State
    {
        emptyState = 0,
        checking = 1,
        fullState = 2,
        errorState = 3
    };

    State lastStateBevorError;  // holds the last State bevor error to resume after error
    State currentState;         // holds the current state of the FSM
    Event currentEvent;         // holds the current event of the FSM

    RfidCtrl pRfidCtrl;


    DetectPackageCtrl::Event (DetectPackageCtrl::*doActionFPtr)(void) = 0;

    void entryAction_emptyState();

    Event doAction_emptyState();

    void exitAction_emptyState();
    
    void entryAction_checking();

    DetectPackageCtrl::Event doAction_checking();

    void exitAction_checking();

    void entryAction_fullState();

    DetectPackageCtrl::Event doAction_fullState();

    void exitAction_fullState();

    DetectPackageCtrl::Event doAction_errorState();

    void exitAction_errorState();

    String decodeEvent(Event event);

    String decodeState(State state);
}

#endif