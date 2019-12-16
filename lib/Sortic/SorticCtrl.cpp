/**
 * @file SorticCtrl.cpp
 *@author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief The sortic controll class controlls the FSM of the sortic roboter
 *        The workflow is:
 *          - Check and read the package
 *          - Sort the received packgage in the correct smart box
 *          - Wait for the confirmation of the smart box
 * @version 1.1
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "SorticCtrl.h"

//======================Public===========================================================

SorticCtrl::SorticCtrl() : 
                            currentState(State::readRfidVal),
                            currentEvent(Event::NoEvent), 
                            doActionFPtr(&SorticCtrl::doAction_readRfidVal)
{
    DBFUNCCALLln("SorticCtrl::SorticCtrl()");
    pDetectPackage = new DetectPackageCtrl(&(sortic.package));              ///< dynamic instance of detect package control

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
    DBFUNCCALLln("SorticCtrl::~SorticCtrl()");
    delete pDetectPackage;                          ///< delete instance of detect package control class
    delete pSortPackage;                            ///< delete instance of sort package control class
    delete pBus;                                    ///< delete isntance of i2c communication class
}

void SorticCtrl::loop()
{
    DBFUNCCALLln("SorticCtrl::loop()");

    // process last generated event
    process((this->*doActionFPtr)()); // do actions
}

void SorticCtrl::loop(Event event)
{
    DBFUNCCALLln("SorticCtrl::loop(Event)");

    // process current event
    process(event);

    // proces generated event
    process((this->*doActionFPtr)()); // do actions with current Event
}


void SorticCtrl::readCallback(int bytes)
{
    DBFUNCCALLln("SorticCtrl::ReadCallback(int)");
    
    // check if data is available
    while (0 < Wire.available())
    {
        // read byte for byte and store to struct
        Wire.readBytes( (char*) &gReceivedMessage, sizeof(ReceivedI2cMessage));
    }
}

void SorticCtrl::requestCallback()
{
    DBFUNCCALLln("SorticCtrl::RequestCallback()");
    DBINFO2("Request I2C event:  ");
    DBINFO2ln((String)(gWriteMessage.event));

    // write data to wire
    Wire.write((char *)&gWriteMessage,sizeof(WriteI2cMessage));
}

//======================PRIVATE==========================================================

void SorticCtrl::process(Event e)
{
    DBFUNCCALL("SorticCtrl::process(Event)")
    DBEVENTln(String("SorticCtrl ") + decodeEvent(e));
    // controll the finite state machine
    // switch with current state and generated event to next state
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

//======================State-Functions==================================================

//======================readRfidVal======================================================
//=======================================================================================


void SorticCtrl::entryAction_readRfidVal()
{
    DBSTATUSln("Entering State: readRfidVal");
    currentState = State::readRfidVal;                                  // set current state
    doActionFPtr = &SorticCtrl::doAction_readRfidVal;                   // set do-action function
    pDetectPackage = new DetectPackageCtrl(&(sortic.package));          ///< dynamic instance of detect package control class

    // set i2c write message to publish the current event
    strcpy(gWriteMessage.event, "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::readRfidVal;
    delay(1000); // Wait for publish

    // set i2c write message to publish the actual line
    strcpy(gWriteMessage.event, "PublishPOS#");
    gWriteMessage.position = (int8_t)sortic.actualLine;
    delay(1000); // Wait for publish

}

SorticCtrl::Event SorticCtrl::doAction_readRfidVal()
{
    DBINFO1ln("State: emptyState");
    DBINFO2ln("Wait for package...");

    // call detect package controll to read and store the package information
    pDetectPackage->loop(DetectPackageCtrl::Event::CheckForPackage);

    return SorticCtrl::Event::PackageReadyToSort;
}

void SorticCtrl::exitAction_readRfidVal()
{
    DBSTATUSln("Leaving State: readRfidVal");

    // set i2c write message to publish package information
    strcpy(gWriteMessage.event, "PublishPAC#");
    gWriteMessage.packageId = sortic.package.id;
    gWriteMessage.targetDest = sortic.package.targetDest;
    delay(1000); // Wait for publish

    delete pDetectPackage;
}

//======================waitForSort======================================================
//=======================================================================================

void SorticCtrl::entryAction_waitForSort()
{
    DBSTATUSln("Entering State: waitForSort");
    currentState = State::waitForSort;                      // set current state
    doActionFPtr = &SorticCtrl::doAction_waitForSort;       // set do-action function

    // set i2c write message to publish curren state
    strcpy((char*)&(gWriteMessage.event), "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::waitForSort;
    delay(1000); // Wait for publish
    
    // set i2c write message to start the box communications
    strcpy(gWriteMessage.event, "BoxComm####");
    delay(1000); // Wait for beging
}

SorticCtrl::Event SorticCtrl::doAction_waitForSort()
{
    DBINFO1ln("State: waitForSort");

    // TEST
    //strcpy(gReceivedMessage.event, "SortPackage");
    // TEST

    DBINFO2ln((String)"Received I2c Event: " + (String)gReceivedMessage.event);
    // check if received i2c message event is sort package
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

//======================sortPackageInBox=================================================
//=======================================================================================

void SorticCtrl::entryAction_sortPackageInBox()
{
    DBSTATUSln("Entering State: sortPackageInBox");
    currentState = State::sortPackageInBox;                                                     // set current state
    doActionFPtr = &SorticCtrl::doAction_sortPackageInBox;                                      // set do-action function
    pSortPackage = new SortPackageCtrl((int*)&(sortic.actualLine), (int*)&(sortic.targetLine)); ///< dynamic instance of sort package control class

    // set i2c write message to publish current state
    strcpy(gWriteMessage.event, "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::sortPackageInBox;
    delay(1000); // Wait for publish
}

SorticCtrl::Event SorticCtrl::doAction_sortPackageInBox()
{
    DBINFO1ln("State: sortPackageInBox");

    // call sort package control with event upload package to sort the package in the smart box
    pSortPackage->loop(SortPackageCtrl::Event::UploadPackage);

    return SorticCtrl::Event::AnswerReceived;
}

void SorticCtrl::exitAction_sortPackageInBox()
{
    DBSTATUSln("Leaving State: sortPackageInBox");
    delete pSortPackage;                            ///< delete instance of sort package control
    
}

//======================waitForArriv=====================================================
//=======================================================================================

void SorticCtrl::entryAction_waitForArriv()
{
    DBSTATUSln("Entering State: waitForArriv");
    currentState = State::waitForArriv;                     // set current state
    doActionFPtr = &SorticCtrl::doAction_waitForArriv;      // set do-action function
    
    // set i2c write message to publish current state
    strcpy(gWriteMessage.event, "PublishSTA#");
    gWriteMessage.state = (uint8_t)State::waitForArriv;
    delay(1000); // Wait for publish

    // set i2c write message to start arriv confirmation communications
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
    // check if i2c received message event is box filled
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

//======================errorState=======================================================
//=======================================================================================

void SorticCtrl::entryAction_errorState() 
 {
    DBERROR("Entering State: errorState");
    lastStateBeforeError = currentState;
    currentState = State::errorState;                   // set current state
    doActionFPtr = &SorticCtrl::doAction_errorState;    // set do-action function
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

//======================resetState=======================================================
//=======================================================================================

void SorticCtrl::entryAction_resetState()
{
    DBERROR("Entering State: resetState");
    currentState = State::resetState;                   // set current state
    doActionFPtr = &SorticCtrl::doAction_resetState;    // set do-action function

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

//======================Aux-Functions====================================================
//=======================================================================================

String SorticCtrl::decodeState(State s)
{
    DBFUNCCALLln("SorticCtrl::decodeState(State)");
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
    DBFUNCCALLln("SorticCtrl::decodeEvent(Event)");
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