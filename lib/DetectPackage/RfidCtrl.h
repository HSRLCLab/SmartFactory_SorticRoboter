#ifndef RFIDCTRL_H
#define RFIDCTRL_H

#include <Arduino.h>
#include <MFRC522.h>
#include "LogConfiguration.h"
#include "MainConfiguration.h"
#include "Package.h"



class RfidCtrl
{
   public:

    RfidCtrl();
    Package getPackageInformation();
    bool isPackageAvailable();

   private:

    MFRC522 pRfidReader(RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN);
    MFRC522::MIFARE_Key key;//create a MIFARE_Key struct named 'key', which will hold the card information

    byte readBlockMatrix[16][18];
    Package package;

    Stream &readPackage(Stream &obj);
    int readBlock(int blockNumber, byte arrayAddress);
    void parseInformationToStruct();

};

#endif