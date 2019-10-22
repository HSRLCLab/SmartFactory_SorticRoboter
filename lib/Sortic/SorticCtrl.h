#ifndef SORTICCTRL_H_
#define SORTICCTRL_H_

#include <Arduino.h>



class SorticCtrl
{
    public:

    enum class Event 
    {
        
    };

    SorticCtrl();
    
    void loop();

    void loop(Event currentEvent);

    private:
    
    void process(Event e);

    enum class State
    {

    };

    SorticCtrl::Event (SorticCtrl::*doActionFPtr)(void) = nullptr;

    void entryAction_readRfidVal();

    SorticCtrl::Event doAction_readRfidVal();

    void exitAction_readRfidVal();

    void entryAction_waitForBox();

    SorticCtrl::Event doAction_readRfidVal();

    void exitAction_waitForBox();

    void entryAction_calcOptBox();

    SorticCtrl::Event doAction_calcOptBox();

    void exitAction_calcOptBox();

    void entryAction_simulatePackageBuffer();

    SorticCtrl::Event doAction_simulatePacakgeBuffer();

    void exitAction_simulatePackageBuffer();

    void entryAction_requestOptBox();

    SorticCtrl::Event doAction_requestOptBox();

    void exitAction_requestOptBox();

    void entryAction_waitForSort();

    SorticCtrl::Event doAction_waitForBox();

    void exitAction_waitForBox();

    void entryAction_sortPackageInBox();

    SorticCtrl::Event doAction_sortPackageInBox();

    void exitAction_sortPackageInBox();

    void entryAction_waitForArriv();
    
    SorticCtrl::Event doAction_waitForArriv();

    void exitAction_waitForArriv();
    
};

#endif