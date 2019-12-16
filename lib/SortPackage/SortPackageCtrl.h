#ifndef SORTPACKAGECTRL_H_
#define SORTPACKAGECTRL_H_

#include <Arduino.h>
#include "LogConfiguration.h"
//#include "NavigationCtrl.h"

class SortPackageCtrl
{
    public:

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
        UploadPackage,
        UnloadPackage,
        PackageUnloaded,        
        Error,
        Resume,
        Reset,
        NoEvent
    };

    SortPackageCtrl(int *actualLine, int *targetLine);

    ~SortPackageCtrl();

    void loop();

    void loop(Event event);

    private:

    void process(Event e);

    enum class State
    {
        waitForSort,
        uploadPackage,
        unloadPackage,
        errorState,
        resetState
    };

    State lastStateBeforeError;                
    State currentState;
    State lastState;                    
    Event currentEvent;

    int *pActualLine = nullptr;
    int *pTargetLine = nullptr;

    Event (SortPackageCtrl::*doActionFPtr)(void) = nullptr;

    void entryAction_waitForSort();

    SortPackageCtrl::Event doAction_waitForSort();

    void exitAction_waitForSort();

    void entryAction_uploadPackage();

    SortPackageCtrl::Event doAction_uploadPackage();

    void exitAction_uploadPackage();

    void entryAction_unloadPackage();

    SortPackageCtrl::Event doAction_unloadPackage();

    void exitAction_unloadPackage();

    void entryAction_errorState();

    SortPackageCtrl::Event doAction_errorState();

    void exitAction_errorState();

    void entryAction_resetState();

    Event doAction_resetState();

    void exitAction_resetState();

    String decodeState(State s);

    String decodeEvent(Event e);

};


#endif