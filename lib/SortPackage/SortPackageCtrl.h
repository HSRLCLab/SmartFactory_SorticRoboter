#ifndef SORTPACKAGECTRL_H_
#define SORTPACKAGECTRL_H_

#include <Arduino.h>

class SortPackageCtrl
{
    public:

    enum class Event
    {

    };

    SortPackageCtrl();

    SortPackageCtrl::Event process(Event e);

    private:

    enum class State
    {

    };

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

    void entryAction_unloadParking();

    SortPackageCtrl::Event doAction_unloadParking();

    void exitAction_unloadParking();

};


#endif