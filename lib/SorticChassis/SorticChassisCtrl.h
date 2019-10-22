#ifndef SORTICCHASSISCTRL_H_
#define SORTICCHASSISCTRL_H_

#include <Arduino.h>

class SorticChassisCtrl
{
    public:

    enum class Event
    {

    };

    SorticChassisCtrl();

    SorticChassisCtrl::Event process();

    private:

    enum class State
    {

    };

    SorticChassisCtrl::Event (*doActionFPtr)(void) = nullptr;

    void entryAction_waitInPos1();

    SorticChassisCtrl::Event doAction_waitInPos1();

    void exitAction_waitInPos1();

    void entryAction_waitInPos2();

    SorticChassisCtrl::Event doAction_waitInPos2();

    void exitAction_waitInPos2();

    void entryAction_waitInPos3();

    SorticChassisCtrl::Event doAction_waitInPos3();

    void exitAction_waitInPos3();

    void entryAction_moveForward();

    SorticChassisCtrl::Event doAction_moveForward();

    void exitAction_moveForward();

    void entryAction_moveBackward();

    SorticChassisCtrl::Event doAction_moveBackward();

    void exitAction_moveBackward();

}

#endif