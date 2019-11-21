#include "SortPackageCtrl.h"

SortPackageCtrl::SortPackageCtrl()
{

}

void SortPackageCtrl::process(Event e)
{
    DBFUNCCALLln("SortPackageCtrl::process ");
    DBEVENTln(String("SortPackageCtrl ") + decodeEvent(e));
    switch (currentState)
    {
        case State::getTargetParking:
            if (Event::SorticReadyForUpload == e)
            {
                exitAction_getTargetParking();
                entryAction_moveToParking();
            }
            else if (Event::PrepareForSort == e)
            {
                exitAction_getTargetParking();
                entryAction_navigateToPackageUA();
            }
            else if (Event::Error == e)
            {
                exitAction_getTargetParking();
                entryAction_errorState();
            }
            break;
        case State::uploadPackage:
            if (Event::SorticReadyForTransport == e)
            {
                exitAction_uploadPackage();
                entryAction_moveToParking();
            }
            else if (Event::Error == e)
            {
                exitAction_uploadPackage();
                entryAction_errorState();
            }
            break;
        case State::moveToParking:
            if (Event::SorticReadyForUnload == e)
            {
                exitAction_moveToParking();
                entryAction_unloadPackage();
            }
            else if (Event::Error == e)
            {
                exitAction_unloadPackage();
                entryAction_errorState();
            }
            break;
        case State::navigateToPackageUA:
            if (Event::PackageUnloaded == e)
            {
                exitAction_navigateToPackageUA();
            }
            else if (Event::PrepareForSort == e)
            {
                exitAction_navigateToPackageUA();
                entryAction_uploadPackage();
            }
            else if (Event::Error == e)
            {
                exitAction_navigateToPackageUA();
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
                case State::getTargetParking:
                    entryAction_getTargetParking();
                    break;
                case State::uploadPackage:
                    entryAction_uploadPackage();
                    break;
                case State::moveToParking:
                    entryAction_moveToParking();
                    break;
                case State::unloadPackage:
                    entryAction_unloadPackage();
                    break;
                case State::navigateToPackageUA:
                    entryAction_navigateToPackageUA();
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
                exitAciton_resetState();
                entryAction_getTargetParking();
            }
            break;
        default:
            break;
    }
}

void SortPackageCtrl::entryAction_getTargetParking()
{
    DBSTATUSln("Enetering State: getTargetParking");
    currentState = State::getTargetParking;     // state transition
    doActionFPtr = &SortPackageCtrl::doAction_getTargetParking;
}

SortPackageCtrl::Event SortPackageCtrl::doAction_getTargetParking()
{
    DBINFO1ln("State: getTargetParking");
    // may we communicate or we get the infos otherwise?
        //TODO

    if (actualPosition == PACKAGEUPLOADAREA)
    {
        return SortPackageCtrl::Event::SorticReadyForUpload;
    }
    else
    {
        return SortPackageCtrl::Event::PrepareForSort;
    }
}

void SortPackageCtrl::exitAction_getTargetParking()
{
    DBSTATUSln("Leaving State: getTargetParking");
}

void SortPackageCtrl::entryAction_navigateToPackageUA()
{
    DBSTATUSln("Enetering State: navigateToPackageUA");
    currentState = State::navigateToPackageUA;     // state transition
    doActionFPtr = &SortPackageCtrl::doAction_navigateToPackageUA;

    // publish position
    
}

SortPackageCtrl::Event SortPackageCtrl::doAction_navigateToPackageUA()
{
    DBINFO1ln("State: navigateToPackageUA");
    pNavigation.loop(NavigationCtrl::Event::GoToUploadArea, actualPosition);
    if (SortPackageCtrl::State::getTargetParking == lastState)
    {
        return SortPackageCtrl::Event::SorticReadyForUpload;
    }
    else if (SortPackageCtrl::State::unloadPackage == lastState)
    {
        return SortPackageCtrl::Event::PackageSortedInBox;
    }
}

void SortPackageCtrl::exitAction_navigateToPackageUA()
{
    DBSTATUSln("Leaving State: navigateToPackageUA");
    // publish position
        // TODO
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
    
    // communicate to robotarm and wait for accept
        // TODO

    return SortPackageCtrl::Event::PackageReadyToSort;
}

void SortPackageCtrl::exitAction_uploadPackage()
{
    DBSTATUSln("Leaving State: uploadPackage");
}

 void SortPackageCtrl::entryAction_moveToParking()
 {
    DBSTATUSln("Enetering State: moveToParking");
    currentState = State::moveToParking;     // state transition
    doActionFPtr = &SortPackageCtrl::doAction_moveToParking;
 }

 SortPackageCtrl::Event SortPackageCtrl::doAction_moveToParking()
 {
     DBINFO1ln("Stae: moveToParking");
     pNavigation.loop(NavigationCtrl::Event::SorticReadyForTransport, targetParking);
     return SortPackageCtrl::Event::SorticReadyForUnload;
 }

 void SortPackageCtrl::exitAction_moveToParking()
 {
     DBSTATUSln("Leaving State: moveToParking");
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
    // communicate to robotarm and wait for accept
        // TODO

    return SortPackageCtrl::Event::PackageUnloaded;
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
    //Generate the Event

    return Event::NoEvent;
}


void SortPackageCtrl::exitAction_errorState() 
{
    DBSTATUSln("Leaving State: errorState");
}

String SortPackageCtrl::decodeState(State s)
{
    switch(s)
    {
        case State::getTargetParking:
            return "State::getTargetParking";
            break;
        case State::uploadPackage:
            return "State::uploadPackage";
            break;
        case State::moveToParking:
            return "State::moveToParking";
            break;
        case State::unloadPackage:
            return "State::unloadPackage";
            break;
        case State::navigateToPackageUA:
            return "State::navigateToPackageUA";
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

String SortPackageCtrl::decodeEvent(Event e)
{
    switch (e)
    {
    case Event::PackageReadyToSort:
        return "Event::PackageReadyToSort";
        break;
    case Event::PrepareForSort:
        return "Event::PrepareForSort";
        break;
    case Event::SorticReadyForTransport:
        return "Event::SorticReadyForTransport";
        break;
    case Event::SorticReadyForUpload:
        return "Event::SorticReadyForUpload";
        break;
    case Event::SorticReadyForUnload:
        return "Event::SorticReadyForUnload";
        break;
    case Event::PackageUnloaded:
        return "Event::PackageUnloaded";
        break;
    case Event::PackageSortedInBox:
        return "Event::PackageSortedInBox";
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
    default:
        return "ERROR: No matching event";
        break;
    }
}