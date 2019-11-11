#include "RfidReaderCtrl.h"

RfidReaderCtrl::RfidReaderCtrl()
{
    SPI.begin();
    pReader->PCD_Init();
    for (byte i = 0; i < 6; i++) 
    {
        key.keyByte[i] = 0xFF;//keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
    }
}


bool RfidReaderCtrl::isPackageAvailable()
{
    if (pReader->PICC_IsNewCardPresent())
    {
        return true;
    }
    else
    {
        return false;
    }
}


RfidReaderCtrl::Event RfidReaderCtrl::getPackageInformation()
{
    DBFUNCCALLln("RfidReaderCtrl::getPackageInformation");
    DBINFO3ln("Return if Succesfull or not");

    return readPackage(); 
}


void RfidReaderCtrl::parseInformationToStruct()
{   
    DBFUNCCALLln("RfidReaderCtrl::parseInformationToStruct");
    DBINFO3ln("Parse the package information to a struct");
    Serial.println("Parsing!");
    for (int j = 0; j < 4; j++)
    {
        package->id += char(readBlockMatrix[2][j]); // 4 bytes for packageid
    }
    for (int i = 3; i < 7; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            package->cargo += char(readBlockMatrix[i][j]); // 16 byte for packagecargo
        }
    }
    for (int i = 7; i < 11; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            package->targetDest += char(readBlockMatrix[i][j]); // 16 bytes for packagetargetDest
        }
    }
}

RfidReaderCtrl::Event RfidReaderCtrl::readPackage() 
{  
    DBFUNCCALLln("RfidReaderCtrl::readPackage()");
    DBINFO3ln("Read the information on the rfid-transponder ");
    if (pReader->PICC_ReadCardSerial() && pReader->uid.size != 0) 
    {
        DBINFO3ln("Scanner detected package ");
        // Read 16 blocks and store it in a matrix
        for (byte j = 0; j < 16; j++)
        {
            this->readBlock(j, &readBlockMatrix[j][0]);

            // For testing
            /*
            for (int i = 0; i < 4; i++)
            {
                Serial.print(readBlockMatrix[j][i] < 0x10 ? " 0" : " ");
                Serial.print(readBlockMatrix[j][i], HEX);
                //Serial.print(char(readBlockMatrix[j][i]));
            }
            Serial.println();
            */
        }
        return RfidReaderCtrl::Event::NoEvent;
    }
    else
    {
        DBINFO3ln("Error: Could not read package ");
        return RfidReaderCtrl::Event::Error;
    }
}

RfidReaderCtrl::Event RfidReaderCtrl::readBlock(byte blockNumber, byte* arrayAddress) 
{
  DBFUNCCALL("RfidReaderCtrl::readBlock(): ");
  DBINFO3ln("Read a block on the rfid-transponder ");

  RfidReaderCtrl::status = (MFRC522::StatusCode) pReader->MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (RfidReaderCtrl::status != MFRC522::STATUS_OK) 
  {
    DBINFO3ln("MIFARE_read() failed: ");
    DBINFO3ln(pReader->GetStatusCodeName(status));
    return RfidReaderCtrl::Event::Error;//return "4" as error message
  }
  DBINFO3ln("Block was read");

  return RfidReaderCtrl::Event::NoEvent; // Evtl ändern
}
