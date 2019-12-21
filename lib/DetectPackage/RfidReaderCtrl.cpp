/**
 * @file RfidReaderCtrl.cpp
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief Library to controll rfid sensor MFRC522
 * @version 1.1
 * @date 2019-11-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "RfidReaderCtrl.h"

//======================Public===========================================================

RfidReaderCtrl::RfidReaderCtrl(RfidReaderCtrl::Package *packagePtr) : 
                                                                        pPackagePtr(packagePtr)
{
    DBFUNCCALLln("RfidReaderCtrl::RfidReaderCtrl(RfidReaderCtrl::Package)");
    // begin spi
    SPI.begin();

    // initialize the rfid sensor
    pReader->PCD_Init();
    for (byte i = 0; i < 6; i++) 
    {
        key.keyByte[i] = 0xFF; //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
    }
}

RfidReaderCtrl::~RfidReaderCtrl()
{
    DBFUNCCALLln("RfidReaderCtrl::~RfidReaderCtrl()");
}

RfidReaderCtrl::Event RfidReaderCtrl::getPackageInformation()
{
    DBFUNCCALLln("RfidReaderCtrl::getPackageInformation");
    
    // call rfid sensor and check if a transponder is present
    if (!pReader->PICC_IsNewCardPresent())
    {
        DBINFO3ln("No package present");
        return RfidReaderCtrl::Event::NoPackageAvailable;
    }

    // TEST 
    return RfidReaderCtrl::Event::NoEvent;
    // TEST

    // Cant read rfid transponder -> maybe hardware defect
    if (pReader->PICC_ReadCardSerial() && (pReader->uid.size != 0)) 
    {
        DBINFO3ln("Scanner detected package ");
        // Read 16 blocks and store it in a matrix
        for (byte j = 0; j < 16; j++)
        {
            if(this->readBlock(j, &readBlockMatrix[j][0]) != RfidReaderCtrl::Event::NoEvent);
            {
                return RfidReaderCtrl::Event::Error;
            }

            // TEST
            /*
            for (int i = 0; i < 4; i++)
            {
                Serial.print(readBlockMatrix[j][i] < 0x10 ? " 0" : " ");
                Serial.print(readBlockMatrix[j][i], HEX);
                //Serial.print(char(readBlockMatrix[j][i]));
            }
            Serial.println();
            */
            // TEST
            
        }
        return RfidReaderCtrl::Event::NoEvent;
    }
    else
    {
        DBINFO3ln("Error: Could not read package ");
        return RfidReaderCtrl::Event::Error;
    }
}

//======================PRIVATE==========================================================

//======================Aux-Functions====================================================
//=======================================================================================

void RfidReaderCtrl::parseInformationToStruct()
{   
    DBFUNCCALLln("RfidReaderCtrl::parseInformationToStruct");
    DBINFO3ln("Parse the package information to a struct");

    // Can't read the information on the rfid chip, it worked and now not anymore
    // Maybe Hardware defect
    // The card presents can be detected!
    // Check the hardware if it's damaged
    //
    // As second the logic to read data is not actually, the data types has been changed
    // Stream the bytes into the integer datatypes
        // TODO

    for (int j = 0; j < 2; j++)
    {
        pPackagePtr->id += char(readBlockMatrix[2][j]); // 2 bytes for packageid
    }
    /* removed cargo cause i2c message size
    for (int i = 3; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pPackagePtr->cargo += char(readBlockMatrix[i][j]); // 12 byte for packagecargo
        }
    }
    */
    for (int j = 2; j < 4; j++)
    {
        pPackagePtr->targetDest += uint8_t(readBlockMatrix[2][j]); // 2 bytes for packagetargetDest --> Postleitzahl
    }
    
}

RfidReaderCtrl::Event RfidReaderCtrl::readBlock(byte blockNumber, byte* arrayAddress) 
{
  DBFUNCCALL("RfidReaderCtrl::readBlock(byte, byte*):");
  
  DBINFO3ln("Read a block on the rfid-transponder ");
  RfidReaderCtrl::status = (MFRC522::StatusCode) pReader->MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (RfidReaderCtrl::status != MFRC522::STATUS_OK) 
  {
    DBINFO3ln("MIFARE_read() failed: ");
    DBINFO3ln(pReader->GetStatusCodeName(status));
    return RfidReaderCtrl::Event::Error;
  }

  DBINFO3ln("Block was read");
  return RfidReaderCtrl::Event::NoEvent;
}
