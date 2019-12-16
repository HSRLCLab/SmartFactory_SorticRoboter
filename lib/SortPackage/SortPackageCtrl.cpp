/**
 * @file SortPackageCtrl.cpp
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief The sort package controll class controlls the FSM to sort the current package in the available box
 *        To sort the package in the box it calls the navigation controll and chassis controll of the sortic 
 *        and communicate to the robotarm
 * @version 1.1
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "SortPackageCtrl.h"

//======================Public===========================================================

SortPackageCtrl::SortPackageCtrl(int *actualLine, int *targetLine) :
                                                                        currentState(State::waitForSort),
                                                                        currentEvent(Event::NoEvent),
                                                                        pActualLine(actualLine), 
                                                                        pTargetLine(targetLine)
{
    DBFUNCCALLln("SortPackageCtrl::SortPackageCtrl(int*, int*)");
}

SortPackageCtrl::~SortPackageCtrl()
{
    DBFUNCCALLln("SortPackageCtrl::~SortPackageCtrl()");    
}

void SortPackageCtrl::loop()
{
    DBFUNCCALLln("SortPackageCtrl::loop()");
    do
    {
        // process last generated event
        process((this->*doActionFPtr)()); // do actions
    } while (!(currentEvent == Event::PackageUnloaded));  
}

void SortPackageCtrl::loop(Event event)
{
    DBFUNCCALLln("SortPackageCtrl::loop(Event)");
    currentEvent = event;

    // process current event
    process(currentEvent);

    // process generated event
    loop(); 
}

//======================PRIVATE==========================================================

void SortPackageCtrl::process(Event e)
{
    DBFUNCCALLln("SortPackageCtrl::process(Event)");

    DBEVENTln(String("SortPackageCtrl ") + decodeEvent(e));
    // controll the finite state machine
    // switch with current state and generated event to next state
    switch (currentState)
    {
        case State::waitForSort:
            if (Event::UploadPackage == e)
            {
                exitAction_waitForSort();
                entryAction_uploadPackage();
            }
            else if (Event::Error == e)
            {
                exitAction_waitForSort();
                entryAction_errorState();
            }
            break;
        case State::uploadPackage:
            if (Event::UnloadPackage == e)
            {
                exitAction_uploadPackage();
                entryAction_unloadPackage();
            }
            else if (Event::Error == e)
            {
                exitAction_uploadPackage();
                entryAction_errorState();
            }
            break;
        case State::unloadPackage:
            if (Event::PackageUnloaded == e)
            {
                exitAction_unloadPackage();
                entryAction_waitForSort();
            }
            else if (Event::Error == e)
            {
                exitAction_unloadPackage();
                entryAction_errorState();
            }
            break;
        
        case State::errorState:
            if (Event::Resume == e)
            {
                exitAction_errorState();
                // Implement Resume
                    //TODO
                switch (lastStateBeforeError)
                {
                case State::waitForSort:
                    entryAction_waitForSort();
                    break;
                case State::uploadPackage:
                    entryAction_uploadPackage();
                    break;
                case State::unloadPackage:
                    entryAction_unloadPackage();
                    break;
                default:
                    break;
                }
            }
            else if (Event::Reset == e)
            {
                exitAction_errorState();
                // Implement Resume
                    //TODO
                entryAction_resetState();
            }
        case State::resetState:
            if (Event::Resume == e)
            {
                // Implement Resume
                    //TODO
                exitAction_resetState();
                entryAction_waitForSort();
            }
            break;
        default:
            break;
    }
}

//======================State-Functions==================================================

//======================waitForSort======================================================
//=======================================================================================

void SortPackageCtrl::entryAction_waitForSort()
{
    DBSTATUSln("Enetering State: waitForSort");
    currentState = State::waitForSort;                          // set current state
    doActionFPtr = &SortPackageCtrl::doAction_waitForSort;      // set do-action function
    
}

SortPackageCtrl::Event SortPackageCtrl::doAction_waitForSort()
{
    DBINFO1ln("State: waitForSort");
    // do nothing
    delay(50);
    
    return Event::UploadPackage;
}

void SortPackageCtrl::exitAction_waitForSort()
{
    DBSTATUSln("Leaving State: waitForSort");
}


//======================uploadPackage====================================================
//=======================================================================================

void SortPackageCtrl::entryAction_uploadPackage()
{
    DBSTATUSln("Enetering State: uploadPackage");
    currentState = State::uploadPackage;                        // set current state
    doActionFPtr = &SortPackageCtrl::doAction_uploadPackage;    // set do-action function
}

SortPackageCtrl::Event SortPackageCtrl::doAction_uploadPackage()
{
    DBINFO1ln("State: uploadPackage");

    // check if sortic is at upload, if not call navigation controll and drive to upload line
    if (!(*pActualLine == (int)Line::UploadLine))
    {
        // Call NavigationCtrl with actualLine and drive to UploadLine
            //TODO
    }

    // Call RobotArm to upload the package
        //TODO
    
    return SortPackageCtrl::Event::UnloadPackage;
}

void SortPackageCtrl::exitAction_uploadPackage()
{
    DBSTATUSln("Leaving State: uploadPackage");
}

//======================unloadPackage====================================================
//=======================================================================================

void SortPackageCtrl::entryAction_unloadPackage()
{
    DBSTATUSln("Enetering State: unloadPackage");
    currentState = State::unloadPackage;                        // set current state
    doActionFPtr = &SortPackageCtrl::doAction_unloadPackage;    // set do-action function
}

SortPackageCtrl::Event SortPackageCtrl::doAction_unloadPackage()
{
    DBINFO1ln("State: unloadPackage");

    // Call NavigationCtrl with target line
        // TODO
    // Call RobotArmCtrl to unload the package
        // TODO
    // Call NavigationCtrl with upload line
        // TODO

    // store current Event to leave while loop
    currentEvent = SortPackageCtrl::Event::PackageUnloaded;
    return currentEvent;
}

void SortPackageCtrl::exitAction_unloadPackage()
{
    DBSTATUSln("Leaving State: unloadPackage");
}


//======================errorState=======================================================
//=======================================================================================

void SortPackageCtrl::entryAction_errorState()
{
    DBERROR("Entering State: errorState");
    lastStateBeforeError = currentState;                        // set last state before error
    currentState = State::errorState;                           // set current state
    doActionFPtr = &SortPackageCtrl::doAction_errorState;       // set do-action function
}

SortPackageCtrl::Event SortPackageCtrl::doAction_errorState() 
{
    DBINFO1ln("State: errorState");
    
    // Implementate error state
        // TODO

    return Event::NoEvent;
}


void SortPackageCtrl::exitAction_errorState() 
{
    DBSTATUSln("Leaving State: errorState");
}


//======================resetState=======================================================
//=======================================================================================

void SortPackageCtrl::entryAction_resetState()
{
    DBERROR("Entering State: resetState");
    currentState = State::resetState;                           // set current state
    doActionFPtr = &SortPackageCtrl::doAction_resetState;       // set do-action function
}

SortPackageCtrl::Event SortPackageCtrl::doAction_resetState()
{
    DBINFO1ln("State: resetState");
    
    // Implementate reset state
        // TODO

    return Event::NoEvent;
}

void SortPackageCtrl::exitAction_resetState()
{
    DBSTATUSln("Leaving State: resetState");
}

//======================Aux-Functions====================================================
//=======================================================================================

String SortPackageCtrl::decodeState(State s)
{
    DBFUNCCALLln("SortPackageCtrl::decodeState(State)");
    switch(s)
    {
        case State::waitForSort:
            return "State::waitForSort";
        case State::uploadPackage:
            return "State::uploadPackage";
        case State::unloadPackage:
            return "State::unloadPackage";
        case State::errorState:
            return "State::errorState";
        case State::resetState:
            return "State::resetState";
        default:
            return "ERROR: No matching state";
    }
}

String SortPackageCtrl::decodeEvent(Event e)
{
    DBFUNCCALLln("SortPackageCtrl::decodeEvent(Event)");
    switch (e)
    {
    case Event::UploadPackage:
        return "Event::UploadPackage";
    case Event::UnloadPackage:
        return "Event::UnloadPackage";
    case Event::PackageUnloaded:
        return "Event::PackageUnloaded";
    case Event::Error:
        return "Event::Error";
    case Event::Resume:
        return "Event::Resume";
    case Event::Reset:
        return "Event::Reset";
    default:
        return "ERROR: No matching event";
    }
}