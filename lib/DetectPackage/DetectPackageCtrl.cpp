#include "DetectPackageCtrl.h"

DetectPackageCtrl::DetectPackageCtrl() : currentState (State::emptyState), doActionFPtr (&DetectPackageCtrl::doAction_emptyState)
{
    
}

void DetectPackageCtrl::loop()
{
    DBFUNCCALLln("DetectPackageCtrl::loop()");
    process((this->*doActionFPtr)());
}

void DetectPackageCtrl::loop(Event currentEvent)
{
    DBFUNCCALLln("DetectPackageCtrl::loop(Event)");
    process(currentEvent);
    process((this->*doActionFPtr)());
}


void DetectPackageCtrl::process(Event e)
{
    DBFUNCCALL("DetectPackageCtrl::process ")
    DBEVENTln(String("DetectPackageCtrl ") + String(decodeEvent(e)));
    switch (currentState)
    {
        case State::emptyState:
            if (Event::CheckForPackage == e)
            {
                exitAction_emptyState();
                entryAction_checking();
            }
            else if (Event::Error == e)
            {
                exitAction_emptyState();
                entryAction_errorState();
            }
            break;
        case State::checking:
            if (Event::PackageAvailableToSort == e)
            {
                exitAction_checking();
                entryAction_fullState();
            }
            else if (Event::NoPackageAvailable == e)
            {
                exitAction_checking();
                entryAction_emptyState();
            }
            else if (Event::Error == e)
            {
                exitAction_checking();
                entryAction_errorState();
            }
            break;
        case State::fullState:
            if (Event::PackageReadyToSort == e)
            {
                exitAction_fullState();
                entryAction_emptyState();
            }
            else if (Event::Error == e)
            {
                exitAction_fullState();
                entryAction_errorState();
            }
            break;
        case State::errorState:
            exitAction_errorState();
            switch (lastStateBevorError)
            {
                case State::emptyState:
                    entryAction_emptyState();
                    break;
                case State::checking:
                    entryAction_checking();
                    break;
                case State::fullState:
                    entryAction_fullState();
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

void DetectPackageCtrl::entryAction_emptyState()
{
    DBSTATUSln("Entering State: emptyState")
    currentState = State::emptyState;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_emptyState;
    // TODO need a Low indicator?
}

DetectPackageCtrl::Event DetectPackageCtrl::doAction_emptyState()
{
    DBINFO1ln("State: emptyState");
    delay(1000);
    
    return Event::CheckForPackage;
}

 void DetectPackageCtrl::exitAction_emptyState()
 {
     DBSTATUSln("Leaving State: emptyState")
 }

 void DetectPackageCtrl::entryAction_checking()
 {
    DBSTATUSln("Entering State: checking")
    currentState = State::checking;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_checking;

 }

 DetectPackageCtrl::Event DetectPackageCtrl::doAction_checking()
 {
     DBINFO1ln("State: checking");
     if (!pRfidReader.isPackageAvailable())
     {
        return Event::NoPackageAvailable;               
     }
     
     newPackage = pRfidReader.getPackageInformation();
     return Event::PackageAvailableToSort;
 }

 void DetectPackageCtrl::exitAction_checking()
 {
     DBSTATUSln("Leaving State: checking");

 }

 void DetectPackageCtrl::entryAction_fullState()
 {
    DBSTATUSln("Entering State: checking")
    currentState = State::fullState;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_fullState;

 }

 DetectPackageCtrl::Event DetectPackageCtrl::doAction_fullState()
 {
     DBINFO1ln("State: fullState");

     return Event::PackageReadyToSort;
 }

 void DetectPackageCtrl::exitAction_fullState()
 {
     DBSTATUSln("Leaving State: fullState");
 }

 void DetectPackageCtrl::entryAction_errorState() 
 {
    DBERROR("Entering State: errorState");
    lastStateBevorError = currentState;
    currentState = State::errorState;  // set errorState
    doActionFPtr = &DetectPackageCtrl::doAction_errorState;
}

DetectPackageCtrl::Event DetectPackageCtrl::doAction_errorState() 
{
    DBINFO1ln("State: errorState");
    //Generate the Event

    return Event::NoEvent;
}

void DetectPackageCtrl::exitAction_errorState() 
{
    DBSTATUSln("Leaving State: errorState");
}

String DetectPackageCtrl::decodeState(State state) 
{
    switch (state) 
    {
        case State::emptyState:
            return "State::emptyState";
            break;
        case State::fullState:
            return "State::fullState";
            break;
        case State::checking:
            return "State::checking";
            break;
        case State::errorState:
            return "State::errorState";
            break;
        default:
            return "ERROR: No matching state";
            break;
    }
}

String DetectPackageCtrl::decodeEvent(Event event) 
{
    switch (event) 
    {
        case Event::CheckForPackage:
            return "Event::CheckForPackage";
            break;
        case Event::NoPackageAvailable:
            return "Event::NoPackageAvailable";
            break;
        case Event::PackageAvailableToSort:
            return "Event::PackageAvailableToSort";
            break;
        case Event::PackageReadyToSort:
            return "Event::PackageReadyToSort";
            break;
        case Event::Error:
            return "Event::Error";
            break;
        case Event::Resume:
            return "Event::Resume";
            break;
        case Event::NoEvent:
            return "Event::NoEvent";
            break;
        default:
            return "ERROR: No matching event";
            break;
    }
}
