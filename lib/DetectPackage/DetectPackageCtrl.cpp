#include <DetectPackageCtrl.h>

DetectPackageCtrl::DetectPackageCtrl() : currentState (State::emptyState), doActionFPtr (&DetectPackageCtrl::doAction_emptyState)
{
    
}

DetectPackageCtrl::Event process(Event e)
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
        case State::checking
            if (Event::PackageAvailableToSort == e)
            {
                exitAction_checking();
                entryAction_fullState();
            }
            else if (Event::NoPackageReady == e)
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
        case State::fullState
            if (Event::PackageReadyToSort)
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
        case State::errorState
            exitAction_errorState();
            switch (lastStateBevorError)
            {
                case State::emptyState:
                    entryAction_emptyState();
                    break;
                case State::checking
                    entryAction_checking();
                    break;
                case State::fullState
                    entryAction_fullState();
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

void entryAction_emptyState()
{
    DBSTATUSln("Entering State: emptyState")
    currentState = State::emptyState;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_emptyState();
    // TODO need a Low indicator?
}

DetectPackageCtrl::Event doAction_emptyState()
{
    DBINFO01ln("State: emptyState")
    delay(1000);
    
    return Event::CheckForPackage
}

 void exitAction_emptyState()
 {
     DBSTATUSln("Leaving State: emptyState")
 }

 void entryAction_checking()
 {
    DBSTATUSln("Entering State: checking")
    currentState = State::checking;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_checking();

 }

 DetectPackageCtrl::Event doAction_checking()
 {
     DBINFO01ln("State: checking")
     if (!pRfidCtrl.isPackageAvailable)
     {
        return Event::NoPackageAvailable;               
     }
     else
     {
         newPackage = pRfidCtrl->getPackageInformation();
         return Event::PackageAvailableToSort;
     }

 }

 void exitAction_checking()
 {
     DBSTATUSln("Leaving State: checking");

 }

 void entryAction_fullState()
 {
    DBSTATUSln("Entering State: checking")
    currentState = State::fullState;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_fullState();

 }

 DetectPackageCtrl::Event doAction_fullState()
 {
     DBINFO1ln("State: fullState");

     return Event::PackageReadyToSort;
 }

 void exitAction_fullState()
 {
     DBSTATUSln("Leaving State: fullState");
 }

 void BoxLevelCtrl::entryAction_errorState() 
 {
    DBERROR("Entering State: errorState");
    lastStateBevorError = currentState;
    currentState = State::errorState;  // set errorState
    doActionFPtr = &BoxLevelCtrl::doAction_errorState;
}

BoxLevelCtrl::Event BoxLevelCtrl::doAction_errorState() 
{
    DBINFO1ln("State: errorState");
    //Generate the Event

    return Event::NoEvent;
}

void BoxLevelCtrl::exitAction_errorState() 
{
    DBSTATUSln("Leaving State: errorState");
}

String BoxLevelCtrl::decodeState(State state) 
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

String BoxLevelCtrl::decodeEvent(Event event) 
{
    switch (event) {
        case Event::CheckForPackage:
            return "Event::CheckForPackage";
            break;
        case Event::NoPackageReady:
            return "Event::NoPackageReady";
            break;
        case Event::PackageAvailableToSort:
            return "Event::PackageAvailableToSort";
            break;
        case Event::PackageReadyToSort:
            return "Event::PackageReadyToSort"
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
