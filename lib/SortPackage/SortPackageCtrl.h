#ifndef SORTPACKAGECTRL_H_
#define SORTPACKAGECTRL_H_

#include <Arduino.h>
#include "LogConfiguration.h"
#include "NavigationCtrl.h"

class SortPackageCtrl
{
    public:

    enum class Event
    {
        PackageReadyToSort = 0,
        PrepareForSort = 1,
        SorticReadyForUpload = 2,
        SorticReadyForTransport = 3,
        SorticReadyForUnload = 4,
        PackageUnloaded = 5,
        PackageSortedInBox = 6,
        Error = 7,
        Resume = 8,
        Reset = 9,
        NoEvent = 10
    };

    SortPackageCtrl();

    void process(Event e);

    private:

    enum class State
    {
        getTargetParking = 0,
        uploadPackage = 1,
        moveToParking = 2,
        unloadPackage = 3,
        navigateToPackageUA = 4,
        errorState = 5,
        resetState = 6
    };

    State lastStateBeforeError;                
    State currentState;
    State lastState;                    
    Event currentEvent;

    NavigationCtrl pNavigation;

    Event (SortPackageCtrl::*doActionFPtr)(void) = nullptr;

    void entryAction_getTargetParking();

    SortPackageCtrl::Event doAction_getTargetParking();

    void exitAction_getTargetParking();

    void entryAction_navigateToPackageUA();

    SortPackageCtrl::Event doAction_navigateToPackageUA();

    void exitAction_navigateToPackageUA();

    void entryAction_uploadPackage();

    SortPackageCtrl::Event doAction_uploadPackage();

    void exitAction_uploadPackage();

    void entryAction_moveToParking();

    SortPackageCtrl::Event doAction_moveToParking();

    void exitAction_moveToParking();

    void entryAction_unloadPackage();

    SortPackageCtrl::Event doAction_unloadPackage();

    void exitAction_unloadPackage();

    void entryAction_errorState();

    SortPackageCtrl::Event doAction_errorState();

    void exitAction_errorState();

    void entryAction_resetState();

    void exitAciton_resetState();

    String decodeState(State s);

    String decodeEvent(Event e);

};


#endif