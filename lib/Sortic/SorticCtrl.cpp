#include "SorticCtrl.h"

SorticCtrl::SorticCtrl() : 
                            currentState(State::readRfidVal),
                            currentEvent(Event::NoEvent), 
                            doActionFPtr(&SorticCtrl::doAction_readRfidVal)
{
    DBFUNCCALLln("SorticCtrl::SorticCtrl()");
    pDetectPackage = new DetectPackageCtrl(&(sortic.package));

    // TEST
    /*
    strcpy((char *)&(gWriteMessage.event), "null#######");
    gWriteMessage.packageId = (uint8_t)10;
    gWriteMessage.targetDest = 8852;
    gWriteMessage.position = (int8_t)Line::UploadLine;
    gWriteMessage.state = (uint8_t)State::readRfidVal;
    gWriteMessage.error = false;
    gWriteMessage.token = false;
    */
    // TEST    
}

SorticCtrl::~SorticCtrl()
{
    delete pDetectPackage;
    delete pSortPackage;
    delete pBus;
}

void SorticCtrl::loop()
{
    DBFUNCCALLln("SorticCtrl::loop()");
    process((this->*doActionFPtr)()); // do actions
}

void SorticCtrl::loop(Event event)
{
    DBFUNCCALLln("SorticCtrl::loop(Event)");
    process(event);
    process((this->*doActionFPtr)()); // do actions with current Event
}

// Callbackfunctions for I2C Communication
// static function, can't handle buffer correct
void SorticCtrl::readCallback(int bytes)
{
    DBFUNCCALLln("I2cCommunication::ReadCallback(int)");
    
    while (0 < Wire.available())
    {
        Wire.readBytes( (char*) &gReceivedMessage, sizeof(ReceivedI2cMessage));
    }
}

void SorticCtrl::requestCallback()
{
    DBFUNCCALLln("I2cCommunication::RequestCallback()");
    DBINFO2("Request I2C event:  ");
    DBINFO2ln((String)(gWriteMessage.event));
    Wire.write((char *)&gWriteMessage,sizeof(WriteI2cMessage));
}


// Privates

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
                entryAction_waitForSort();
            }
            else if (Event::Error == e)
            {
                exitAction_readRfidVal();
                entryAction_errorState();
            }
            break;
        case State::waitForSort:
            if (Event::ReadyToSort == e)
            {
                exitAction_waitForSort();
                entryAction_sortPackageInBox();
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
            else if (Event::Error == e)
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
                switch (lastStateBeforeError)
                {
                case State::readRfidVal:
                    entryAction_readRfidVal();
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
                entryAction_resetState();
            }
            break;
        case State::resetState:
            if (Event::Resume == e)
            {
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
    pDetectPackage = new DetectPackageCtrl(&(sortic.package));

    DBINFO1("Publish State:   ");
    DBINFO1ln(gWriteMessage.state);
    strcpy(gWriteMessage.event, "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::readRfidVal;
    delay(1000); // Wait for publish

    strcpy(gWriteMessage.event, "PublishPOS#");
    gWriteMessage.position = (int8_t)sortic.actualLine;
    delay(1000); // Wait for publish

}

SorticCtrl::Event SorticCtrl::doAction_readRfidVal()
{
    DBINFO1ln("State: emptyState");
    DBINFO2ln("Wait for package...");
    pDetectPackage->loop(DetectPackageCtrl::Event::CheckForPackage);
    return SorticCtrl::Event::PackageReadyToSort;
}

void SorticCtrl::exitAction_readRfidVal()
{
    DBSTATUSln("Leaving State: readRfidVal");

    strcpy(gWriteMessage.event, "PublishPAC#");
    gWriteMessage.packageId = sortic.package.id;
    gWriteMessage.targetDest = sortic.package.targetDest;
    delay(1000); // Wait for publish

    delete pDetectPackage;
}

void SorticCtrl::entryAction_waitForSort()
{
    DBSTATUSln("Entering State: waitForSort");
    currentState = State::waitForSort;  // state transition
    doActionFPtr = &SorticCtrl::doAction_waitForSort;

    strcpy((char*)&(gWriteMessage.event), "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::waitForSort;
    DBINFO1("Publish State:   ");
    DBINFO1ln(decodeState((State)gWriteMessage.state));
    delay(1000); // Wait for publish
    
    strcpy(gWriteMessage.event, "BoxComm####");
    delay(1000);
}

SorticCtrl::Event SorticCtrl::doAction_waitForSort()
{
    DBINFO1ln("State: waitForSort");

    // TEST
    //strcpy(gReceivedMessage.event, "SortPackage");
    // TEST

    DBINFO2ln((String)"Received I2c Event: " + (String)gReceivedMessage.event);
    if (!strcmp(gReceivedMessage.event, "SortPackage"))
    {
        sortic.targetLine = (SorticCtrl::Line)gReceivedMessage.targetLine;
        return Event::ReadyToSort;
    }
    return Event::NoEvent;
}

void SorticCtrl::exitAction_waitForSort()
{
    DBSTATUSln("Leaving State: waitForSort");
}

void SorticCtrl::entryAction_sortPackageInBox()
{
    DBSTATUSln("Entering State: sortPackageInBox");
    currentState = State::sortPackageInBox;  // state transition
    doActionFPtr = &SorticCtrl::doAction_sortPackageInBox;
    pSortPackage = new SortPackageCtrl((int*)&(sortic.actualLine), (int*)&(sortic.targetLine));

    strcpy(gWriteMessage.event, "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::sortPackageInBox;
    delay(1000);
}

SorticCtrl::Event SorticCtrl::doAction_sortPackageInBox()
{
    DBINFO1ln("State: sortPackageInBox");

    pSortPackage->loop(SortPackageCtrl::Event::UploadPackage);

    return SorticCtrl::Event::AnswerReceived;
}

void SorticCtrl::exitAction_sortPackageInBox()
{
    DBSTATUSln("Leaving State: sortPackageInBox");
    delete pSortPackage;
    
}

void SorticCtrl::entryAction_waitForArriv()
{
    DBSTATUSln("Entering State: waitForArriv");
    currentState = State::waitForArriv;  // state transition
    doActionFPtr = &SorticCtrl::doAction_waitForArriv;
    
    strcpy(gWriteMessage.event, "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::waitForArriv;
    delay(1000); // Wait for publish

    strcpy(gWriteMessage.event, "ArrivConf##");
    delay(1000); // Wait for publish
}

SorticCtrl::Event SorticCtrl::doAction_waitForArriv()
{
    DBINFO1ln("State: waitForArriv");

    // TEST
    //strcpy(gReceivedMessage.event, "BoxFilled##");
    // TEST

    DBINFO2ln((String)"Received I2c Event: " + (String)gReceivedMessage.event);
    if (!strcmp(gReceivedMessage.event, "BoxFilled##"))
    {
        return SorticCtrl::Event::SBFilled;
    }
    return SorticCtrl::Event::NoEvent;
}

void SorticCtrl::exitAction_waitForArriv()
{
    DBSTATUSln("Leaving State: waitForArriv");
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
    
    // Implementate error state
        // TODO

    return Event::NoEvent;
}

void SorticCtrl::exitAction_errorState() 
{
    DBSTATUSln("Leaving State: errorState");
}

void SorticCtrl::entryAction_resetState()
{
    DBERROR("Entering State: resetState");
    currentState = State::resetState; 
    doActionFPtr = &SorticCtrl::doAction_resetState;

}

SorticCtrl::Event SorticCtrl::doAction_resetState()
{
    DBINFO1ln("State: resetState");
    
    // Implementate reset state
        // TODO

    return Event::NoEvent;
}

void SorticCtrl::exitAction_resetState()
{
    DBSTATUSln("Leaving State: resetState");
}

String SorticCtrl::decodeState(State s)
{
    switch(s)
    {
        case State::readRfidVal:
            return "State::readRfidVal";
        case State::waitForSort:
            return "State::waitForSort";
        case State::sortPackageInBox:
            return "State::SortPackageCtrl";
        case State::waitForArriv:
            return "State::waitForArriv";
        case State::errorState:
            return "State::errorState";
        case State::resetState:
            return "State::resetState";
        default:
            return "ERROR: No matching state";
    }
}

String SorticCtrl::decodeEvent(Event e)
{
    switch (e)
    {
    case Event::PackageReadyToSort:
        return "Event::PackageReadyToSort";
    case Event::AnswerReceived:
        return "Event::AnswerReceived";
    case Event::NoAnswerReceived:
        return "Event::NoAnswerReceived";
    case Event::NoBoxAvailableForRegion:
        return "Event::NoBoxAvailableForRegion";
    case Event::BoxAvailableForRegion:
        return "Event::BoxAvailableForRegion";
    case Event::ReadyToSort:
        return "Event::ReadyToSort";
    case Event::SBFilled:
        return "Event::SBFilled";
    case Event::Error:
        return "Event::Error";
    case Event::Resume:
        return "Event::Resume";
    case Event::Reset:
        return "Event::Reset";
    case Event::NoEvent:
        return "Event::NoEvent";
    default:
        return "ERROR: No matching event";
    }
}

