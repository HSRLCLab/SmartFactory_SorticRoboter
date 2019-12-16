#include "SortPackageCtrl.h"

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
    
}

void SortPackageCtrl::loop()
{
    DBFUNCCALLln("SorticCtrl::loop()");
    do
    {
        process((this->*doActionFPtr)()); // do actions
    } while (!(currentEvent == Event::PackageUnloaded));  
}

void SortPackageCtrl::loop(Event event)
{
    DBFUNCCALLln("SorticCtrl::loop()");
    currentEvent = event;
    process(currentEvent);
    loop(); 
}


void SortPackageCtrl::process(Event e)
{
    DBFUNCCALLln("SortPackageCtrl::process ");
    DBEVENTln(String("SortPackageCtrl ") + decodeEvent(e));
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

void SortPackageCtrl::entryAction_waitForSort()
{
    DBSTATUSln("Enetering State: waitForSort");
    currentState = State::waitForSort;     // state transition
    doActionFPtr = &SortPackageCtrl::doAction_waitForSort;
    
}

SortPackageCtrl::Event SortPackageCtrl::doAction_waitForSort()
{
    DBINFO1ln("State: waitForSort");
    // do nothing
    
    return Event::UploadPackage;
}

void SortPackageCtrl::exitAction_waitForSort()
{
    DBSTATUSln("Leaving State: waitForSort");
}



void SortPackageCtrl::entryAction_uploadPackage()
{
    DBSTATUSln("Enetering State: uploadPackage");
    currentState = State::uploadPackage;     // state transition
    doActionFPtr = &SortPackageCtrl::doAction_uploadPackage;
}

SortPackageCtrl::Event SortPackageCtrl::doAction_uploadPackage()
{
    DBINFO1ln("State: uploadPackage");

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


 void SortPackageCtrl::entryAction_unloadPackage()
 {
    DBSTATUSln("Enetering State: unloadPackage");
    currentState = State::unloadPackage;     // state transition
    doActionFPtr = &SortPackageCtrl::doAction_unloadPackage;
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

    currentEvent = SortPackageCtrl::Event::PackageUnloaded;
    return currentEvent;
 }

 void SortPackageCtrl::exitAction_unloadPackage()
 {
     DBSTATUSln("Leaving State: unloadPackage");
 }

void SortPackageCtrl::entryAction_errorState()
 {
    DBERROR("Entering State: errorState");
    lastStateBeforeError = currentState;
    currentState = State::errorState;  // set errorState
    doActionFPtr = &SortPackageCtrl::doAction_errorState;
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

void SortPackageCtrl::entryAction_resetState()
{
    DBERROR("Entering State: resetState");
    currentState = State::resetState; 
    doActionFPtr = &SortPackageCtrl::doAction_resetState;
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

String SortPackageCtrl::decodeState(State s)
{
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