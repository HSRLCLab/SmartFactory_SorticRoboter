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

    RfidReaderCtrl();
    Package getPackageInformation();
    bool isPackageAvailable();

   private:

    MFRC522 pReader(RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN);
    MFRC522::MIFARE_Key key;//create a MIFARE_Key struct named 'key', which will hold the card information
    byte readBlockMatrix[16][18];
    Package package;

    int readPackage();
    int readBlock(int blockNumber, byte* arrayAddress);
    void parseInformationToStruct();

};

#endif