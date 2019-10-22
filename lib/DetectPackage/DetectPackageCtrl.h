#ifndef DETECTPACKAGECTRL_H_
#define DETECTPACKAGECTRL_H_

#include <Arduino.h>

class DetectPackageCtrl
{
    public:

    enum class Event
    {

    };

    DetectPackageCtrl();

    DetectPackageCtrl::Event process(Event e);

    private:

    enum class State
    {

    };

    Event (SortPAckageCtrl::*doActionFPtr)(void) = nullptr;

    void entryAction_emptyState();

    DetectPackageCtrl::Event doAction_emptyState();

    void exitAction_emptyState();
    
    void entryAction_checkForPackage();

    DetectPackageCtrl::Event doAction_checkForPackage();

    void exitAction_checkForPackage();

    void entryAction_fullState();

    DetectPackageCtrl::Event doAction_fullState();

    void exitAction_fullState();
}

#endif