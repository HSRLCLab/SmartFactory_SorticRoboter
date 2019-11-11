#ifndef RFIDREADERCTRL_H
#define RFIDREADERCTRL_H

#include <Arduino.h>
#include <MFRC522.h>
#include "LogConfiguration.h"
#include "MainConfiguration.h"
#include "Package.h"



class RfidReaderCtrl
{
   public:

   enum class Event
    {
        NoEvent = 0,
        Error = 1
    };
    
    RfidReaderCtrl();
    Event getPackageInformation();
    bool isPackageAvailable();
    
    
   private:

    byte readBlockMatrix[16][18];
    byte buffersize = 18;
    MFRC522::StatusCode status;
    MFRC522 *pReader = new MFRC522(RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN);
    MFRC522::MIFARE_Key key;  //create a MIFARE_Key struct named 'key', which will hold the card information
    Package *package = new Package();

    Event readPackage();
    Event readBlock(byte blockNumber, byte* arrayAddress);
    void parseInformationToStruct();
};

#endif