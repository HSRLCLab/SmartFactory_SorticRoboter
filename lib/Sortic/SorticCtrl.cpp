#include "SorticCtrl.h"

SorticCtrl::SorticCtrl()
{

}

void SorticCtrl::loop()
{
    DBFUNCCALLln("SorticCtrl::loop()");
    process((this->*doActionFPtr)()); // do actions
}

void SorticCtrl::loop(Event currentEvent)
{
    DBFUNCCALLln("SorticCtrl::loop(Event)");
    process(currentEvent);
    process((this->*doActionFPtr)()); // do actions with current Event    
}

void SorticCtrl::process(Event e)
{
    DBFUNCCALL("SorticCtrl::process ")
    DBEVENTln(String("SorticCtrl ") + decodeEvent(e));
    switch (currentState)
    {
        case State::readRfidVal:
            if (Event::PackageReadyToSort == e) 
            {
                exitAction_readRfidVal();
                entryAction_waitForBox();
            }
            else if (Event::Error == e)
            {
                exitAction_readRfidVal();
                entryAction_errorState();
            }
            break;
        case State::waitForBox:
            if (Event::AnswerReceived == e)
            {
                exitAction_waitForBox();
                entryAction_calcOptBox();
            }
            else if (Event::Error == e)
            {
                exitAction_waitForBox();
                entryAction_errorState();
            }
        case State::calcOptBox:
            if (Event::BoxAvailableForRegion == e)
            {
                if (Event::AnswerReceived == e)
                {
                    exitAction_calcOptBox();
                    entryAction_requestOptBox();
                }
                else if (Event::NoAnswerReceived == e)
                {
                    exitAction_calcOptBox();
                    entryAction_waitForBox();
                }                               
            }
            else if (Event::NoBoxAvailableForRegion == e)
            {
                exitAction_calcOptBox();
                entryAction_simulatePackageBuffer();
            }
            else if (Event::Error == e) 
            {
                exitAction_calcOptBox();
                entryAction_errorState();
            }
            break;
        case State::simulatePackageBuffer:
            if (Event::AnswerReceived == e)
            {
                exitAction_simulatePackageBuffer();
                entryAction_readRfidVal();
            }
            else if (Event::Error == e)
            {
                exitAction_simulatePackageBuffer();
                entryAction_errorState();
            }
            break;
        case State::waitForSort:
            if (Event::ReadyToSort == e)
            {
                exitAction_waitForSort();
                entryAction_sortPackageInBox();
            }
            else if (Event::NoAnswerReceived == e)
            {
                exitAction_waitForSort();
                entryAction_waitForBox();
            }
            else if (Event::Error == e)
            {
                exitAction_waitForSort();
                entryAction_errorState();
            }
            break;
        case State::sortPackageInBox:
            if (Event::AnswerReceived == e)
            {
                exitAction_sortPackageInBox();
                entryAction_waitForArriv();
            }
            else if (Event
            ::Error == e)
            {
                exitAction_sortPackageInBox();
                entryAction_errorState();
            }
            break;
        case State::waitForArriv:
            if (Event::SBFilled == e)
            {
                exitAction_waitForArriv();
                entryAction_readRfidVal();
            }
            else if (Event::NoEvent == e)
            {
                exitAction_waitForArriv();
                entryAction_waitForArriv();
            }
            else if (Event::Error == e)
            {
                exitAction_waitForArriv();
                entryAction_errorState();
            }
            break;
        case State::errorState:
            if (Event::Resume == e)
            {
                exitAction_errorState();
                // Implement resume 
                switch (lastStateBeforeError)
                {
                case State::readRfidVal:
                    entryAction_readRfidVal();
                    break;
                case State::waitForBox:
                    entryAction_waitForBox();
                    break;
                case State::calcOptBox:
                    entryAction_calcOptBox();
                    break;
                case State::simulatePackageBuffer:
                    entryAction_calcOptBox();
                    break;
                case State::publishOptBox:
                    entryAction_simulatePackageBuffer();
                    break;
                case State::waitForSort:
                    entryAction_waitForSort();
                    break;
                case State::sortPackageInBox:
                    entryAction_sortPackageInBox();
                    break;
                case State::waitForArriv:
                    entryAction_waitForArriv();
                    break;
                default:
                    break;
                }
            }
            else if (Event::Reset == e)
            {
                exitAction_errorState();
                // Implement Reset
                entryAction_resetState();
            }
            break;
        case State::resetState:
            if (Event::Resume == e)
            {
                // Implement Resume
                exitAction_resetState();
                entryAction_readRfidVal();
            }
            break;
        default:
            break;     
    }
}

void SorticCtrl::entryAction_readRfidVal()
{
    DBSTATUSln("Entering State: readRfidVal");
    currentState = State::readRfidVal;  // state transition
    doActionFPtr = &SorticCtrl::doAction_readRfidVal;

    publishState(currentState); // Update current state and publish for gui
    //publishPosition(); // update current position und publish for gui

    //clear gui

}

SorticCtrl::Event SorticCtrl::doAction_readRfidVal()
{
    DBINFO1ln("State: emptyState");
    pDetectPackage.loop(DetectPackageCtrl::Event::CheckForPackage);
    
    // do we need a return ? may not

    return SorticCtrl::Event::PackageReadyToSort;
}

void SorticCtrl::exitAction_readRfidVal()
{
    DBSTATUSln("Leaving State: readRfidVal");
    // comminucation
}

void SorticCtrl::entryAction_waitForBox()
{
    DBSTATUSln("Entering State: waitForBox");
    currentState = State::waitForBox;
    doActionFPtr = &SorticCtrl::doAction_waitForBox;
    publishState(currentState);

    
    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;
    
   // subscribe to topics


}

SorticCtrl::Event SorticCtrl::doAction_waitForBox()
{
    DBINFO1ln("State: waitForBox");
    //check for new messages
    // check for error
    currentMillis = millis();

    // publish available boxes
    if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH)
    {
        previousMillisPublish = millis();
        // publish available boxes here
    }

    // wait time 
    currentMillis = millis();
    if (((currentMillis - previousMillis) > SORTIC_WAITFOR_BOX_SECONDS * 1000) && (0 < 1 /*sizeOfComm*/))
    {
        return Event::AnswerReceived;
    }

    return Event::NoEvent;
}

void SorticCtrl::exitAction_waitForBox()
{
    DBSTATUSln("Leaving State: waitForBox");
    // unsubscribe ?
}

void SorticCtrl::entryAction_calcOptBox()
{
    DBSTATUSln("Entering State: calcOptBox");
    currentState = State::calcOptBox;
    doActionFPtr = &SorticCtrl::doAction_calcOptBox;
    publishState(currentState);
}

SorticCtrl::Event SorticCtrl::doAction_calcOptBox()
{
    // write algorithm for dynamic boxchoiche and store opt box
    return SorticCtrl::Event::NoEvent;
}

void SorticCtrl::exitAction_calcOptBox()
{
    DBSTATUSln("Leaving State: calcOptBox");
}

void SorticCtrl::entryAction_simulatePackageBuffer()
{
    DBSTATUSln("Entering State: simulatePackageBuffer");
    currentState = State::simulatePackageBuffer;
    doActionFPtr = &SorticCtrl::doAction_simulatePackageBuffer;
    publishState(currentState);
}

SorticCtrl::Event SorticCtrl::doAction_simulatePackageBuffer()
{
    // publish a package in the buffer to gui and wait for accept
    return SorticCtrl::Event::NoEvent;
}

void SorticCtrl::exitAction_simulatePackageBuffer()
{
    DBSTATUSln("Leaving State: simulatePackageBuffer");
}

void SorticCtrl::entryAction_requestOptBox()
{
    DBSTATUSln("Enetering State: doAction_requestOptBox");
    currentState = State::requestOptBox;
    doActionFPtr = &SorticCtrl::doAction_requestOptBox;
    publishState(currentState);

    // Subscribe to Topics 
    //TODO

    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;
}

SorticCtrl::Event SorticCtrl::doAction_requestOptBox()
{
    DBINFO1ln("State: requestOptBox");

    // check for new message
    // check for error

    // publish decision
    currentMillis = millis();
    // TODO

    // Wait for response
    //TODO


    return Event::NoEvent;
}

void SorticCtrl::exitAction_requestOptBox()
{
    DBSTATUSln("Leaving State: requestOptBox");
}

void SorticCtrl::entryAction_waitForSort()
{
    DBSTATUSln("Entering State: waitForSort");
    currentState = State::waitForSort;  // state transition
    doActionFPtr = &SorticCtrl::doAction_waitForSort;
    publishState(currentState);  //Update Current State and Publish
    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;
}

SorticCtrl::Event SorticCtrl::doAction_waitForSort()
{
    DBINFO1ln("State: waitForSort");
    currentMillis = millis();

    // read response
    // check for ack from box
    // check for new messages
    // check for error
        // TODO

    // wait for response
        //TODO
    
    

    return Event::NoEvent;
}

void SorticCtrl::exitAction_waitForSort()
{
    DBSTATUSln("Leaving State: waitForSort");
    // unsubscribe
        // TODO
}

void SorticCtrl::entryAction_sortPackageInBox()
{
    DBSTATUSln("Entering State: sortPackageInBox");
    currentState = State::sortPackageInBox;  // state transition
    doActionFPtr = &SorticCtrl::doAction_sortPackageInBox;
    publishState(currentState);  //Update Current State and Publish
}

SorticCtrl::Event SorticCtrl::doAction_sortPackageInBox()
{
    DBINFO1ln("State: sortPackageInBox");
    // Call SortPackageCtrl with Event PackageReadyToSort
        // TODO
    return SorticCtrl::Event::NoEvent;
}

void SorticCtrl::exitAction_sortPackageInBox()
{
    DBSTATUSln("Leaving State: sortPackageInBox");
    // unsubscribe
        // TODO
}

void SorticCtrl::entryAction_waitForArriv()
{
    DBSTATUSln("Entering State: waitForArriv");
    currentState = State::waitForArriv;  // state transition
    doActionFPtr = &SorticCtrl::doAction_waitForArriv;
    publishState(currentState);  //Update Current State and Publish
}

SorticCtrl::Event SorticCtrl::doAction_waitForArriv()
{
    DBINFO1ln("State: waitForArriv");
    // check for new messages
    // check for error
    // check if box updated fill level
        // TODO

    return SorticCtrl::Event::NoEvent;
}

void SorticCtrl::exitAction_waitForArriv()
{
    DBSTATUSln("Leaving State: waitForArriv");
    // unsubscribe
        // TODO
}

void SorticCtrl::entryAction_errorState() 
 {
    DBERROR("Entering State: errorState");
    lastStateBeforeError = currentState;
    currentState = State::errorState;  // set errorState
    doActionFPtr = &SorticCtrl::doAction_errorState;
}

SorticCtrl::Event SorticCtrl::doAction_errorState() 
{
    DBINFO1ln("State: errorState");
    //Generate the Event

    return Event::NoEvent;
}

void SorticCtrl::exitAction_errorState() 
{
    DBSTATUSln("Leaving State: errorState");
}

String SorticCtrl::decodeState(State s)
{
    switch(s)
    {
        case State::readRfidVal:
            return "State::readRfidVal";
            break;
        case State::waitForBox:
            return "State::waitForBox";
            break;
        case State::calcOptBox:
            return "State::calcOptBox";
            break;
        case State::simulatePackageBuffer:
            return "State::simulatePackageBuffer";
            break;
        case State::requestOptBox:
            return "State::requestOptBox";
            break;
        case State::publishOptBox:
            return "State::publishOptBox";
            break;
        case State::waitForSort:
            return "State::waitForSort";
            break;
        case State::sortPackageInBox:
            return "State::SortPackageCtrl";
            break;
        case State::waitForArriv:
            return "State::waitForArriv";
            break;
        case State::errorState:
            return "State::errorState";
            break;
        case State::resetState:
            return "State::resetState";
            break;
        default:
            return "ERROR: No matching state";
            break;
    }
}

String SorticCtrl::decodeEvent(Event e)
{
    switch (e)
    {
    case Event::PackageReadyToSort:
        return "Event::PackageReadyToSort";
        break;
    case Event::AnswerReceived:
        return "Event::AnswerReceived";
        break;
    case Event::NoAnswerReceived:
        return "Event::NoAnswerReceived";
        break;
    case Event::NoBoxAvailableForRegion:
        return "Event::NoBoxAvailableForRegion";
        break;
    case Event::BoxAvailableForRegion:
        return "Event::BoxAvailableForRegion";
        break;
    case Event::ReadyToSort:
        return "Event::ReadyToSort";
        break;
    case Event::SBFilled:
        return "Event::SBFilled";
        break;
    case Event::Error:
        return "Event::Error";
        break;
    case Event::Resume:
        return "Event::Resume";
        break;
    case Event::Reset:
        return "Event::Reset";
        break;
    case Event::NoEvent:
        return "Event::NoEvent";
        break;    
    default:
        return "ERROR: No matching event";
        break;
    }
}

