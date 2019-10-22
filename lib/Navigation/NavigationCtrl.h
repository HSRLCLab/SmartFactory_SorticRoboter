#ifndef NAVIGATIONCTRL_H_
#define NAVIGATIONCTRL_H_

#include <Arduino.h>

class NavigationCtrl
{
    public:

    enum class Event
    {

    };

    NavigationCtrl();

    void process(Event e);

    private:

    enum class State
    {

    };

    Event (NavigationCtrl::*doActionFPtr)(void) = nullptr;

    void entryAction_waitAtUA();

    NavigationCtrl::Event doAction_waitAtUA();

    void exitAction_waitAtUA();

    void entryAction_getTargetBoxArea();

    NavigationCtrl::Event doAction_getTargetBoxArea();

    void exitAction_getTargetBoxArea();

    void entryAction_MoveToBoxArea1();

    NavigationCtrl::Event doAction_moveToBoxArea1();

    void exitAction_moveToBoxArea1();

    void entryAction_MoveToBoxArea2();

    NavigationCtrl::Event doAction_moveToBoxArea2();

    void exitAction_moveToBoxArea2();

    void entryAction_MoveToBoxArea3():

    NavigationCtrl::Event doAction_moveToBoxArea3();

    void exitAction_moveToBoxArea3();

    void entryAction_waitAtBA1();

    NavigationCtrl::Event doAction_waitAtBA1();

    void exitAction_waitAtBA1();

    void entryAction_waitAtBA2();

    NavigationCtrl::Event doAction_waitAtBA2();

    void exitAction_waitAtBA2();

    void entryAction_waitAtBA3();

    NavigationCtrl::Event doAction_waitAtBA3();

    void exitAction_waitAtBA3();

    void entryAction_detectPosition();

    NavigationCtrl::Event doAction_detectPosition();

    void exitAction_detectPosition();

};

#endif
