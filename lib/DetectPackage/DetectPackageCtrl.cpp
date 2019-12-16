/**
 * @file DetectPackageCtrl.cpp
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief 
 * @version 0.1
 * @date 2019-11-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "DetectPackageCtrl.h"

DetectPackageCtrl::DetectPackageCtrl(RfidReaderCtrl::Package *packagePtr) : 
                                currentState(State::emptyState),
                                pPackagePTr(packagePtr),
                                doActionFPtr(&DetectPackageCtrl::doAction_emptyState)
                                
{
    
}

void DetectPackageCtrl::loop()
{
    DBFUNCCALLln("DetectPackageCtrl::loop()");
    do
    {
        process((currentEvent = (this->*doActionFPtr)()));
        delay(5000);
    } while (currentEvent != Event::PackageReadyToSort);
}

void DetectPackageCtrl::loop(DetectPackageCtrl::Event event)
{
    DBFUNCCALLln("DetectPackageCtrl::loop(Event)");
    currentEvent = event;
    process(currentEvent);
    loop();   
}


void DetectPackageCtrl::process(Event e)
{
    DBFUNCCALLln("DetectPackageCtrl::process ")

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
            break;
        }
        else if (Event::PackageSorted == e)
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
    DBSTATUSln("Entering State: emptyState");
    currentState = State::emptyState;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_emptyState;
}

DetectPackageCtrl::Event DetectPackageCtrl::doAction_emptyState()
{
    DBINFO1ln("State: emptyState");
    delay(1000);
    
    return Event::CheckForPackage;
}

 void DetectPackageCtrl::exitAction_emptyState()
 {
     DBSTATUSln("Leaving State: emptyState");
 }

 void DetectPackageCtrl::entryAction_checking()
 {
    DBSTATUSln("Entering State: checking");
    currentState = State::checking;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_checking;

 }

 DetectPackageCtrl::Event DetectPackageCtrl::doAction_checking()
 {
     DBINFO1ln("State: checking");
    // TEST
    //return Event::PackageAvailableToSort;
    // TEST

    // Can't read the information on the rfid chip, it worked at date XXXX and now not anymore
    // Maybe Hardware defect
    // The card presents can be detected!
    RfidReaderCtrl::Event tempEvent = pRfidReader->getPackageInformation();
    switch (tempEvent)
    {
    case RfidReaderCtrl::Event::NoEvent:
        return DetectPackageCtrl::Event::PackageAvailableToSort;
    case RfidReaderCtrl::Event::Error:
        return DetectPackageCtrl::Event::Error;
    default:
        return DetectPackageCtrl::Event::NoPackageAvailable;
    }
    
 }

 void DetectPackageCtrl::exitAction_checking()
 {
     DBSTATUSln("Leaving State: checking");

 }

 void DetectPackageCtrl::entryAction_fullState()
 {
    DBSTATUSln("Entering State: fullState");
    currentState = State::fullState;  // set currentState
    doActionFPtr = &DetectPackageCtrl::doAction_fullState;
 }

 DetectPackageCtrl::Event DetectPackageCtrl::doAction_fullState()
 {
    DBINFO1ln("State: fullState");

    // TEST
    pPackagePTr->id = 2;
    pPackagePTr->cargo = "Computer###";
    pPackagePTr->targetDest = (uint8_t)8854;
    // TEST
    
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
    
    // Implement error state
        // TODO

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
        case DetectPackageCtrl::Event::CheckForPackage:
            return "Event::CheckForPackage";
        case DetectPackageCtrl::Event::NoPackageAvailable:
            return "Event::NoPackageAvailable";
        case DetectPackageCtrl::Event::PackageAvailableToSort:
            return "Event::PackageAvailableToSort";
        case DetectPackageCtrl::Event::PackageReadyToSort:
            return "Event::PackageReadyToSort";
        case DetectPackageCtrl::Event::Error:
            return "Event::Error";
        case DetectPackageCtrl::Event::Resume:
            return "Event::Resume";
        case DetectPackageCtrl::Event::NoEvent:
            return "Event::NoEvent";
        default:
            return (String)"ERROR: No matching event";
    }
}
