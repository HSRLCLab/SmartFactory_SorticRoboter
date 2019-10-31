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

Package RfidReaderCtrl::getPackageInformation()
{
    DBFUNCCALL("RfidReaderCtrl::getPackageInformation");
    DBINFO3("Return information about package in a struct");
    return *package;
}


void RfidReaderCtrl::parseInformationToStruct()
{
    DBFUNCCALL("RfidReaderCtrl::parseInformationToStruct");
    DBINFO3("Parse the package information to a struct");
    for (int i = 0; i < 18; i++)
    {
        package->id += readBlockMatrix[2][i];
    }
    
    for (int i = 0; i < 18; i++)
    {
        package->cargo += readBlockMatrix[3][i];
    }

    for (int i = 0; i < 18; i++)
    {
        package->targetDest += readBlockMatrix[4][i];
    }

    for (int i = 0; i < 18; i++)
    {
        package->targetReg += readBlockMatrix[5][i];
    }
}

int RfidReaderCtrl::readPackage() 
{  
    DBFUNCCALL("RfidReaderCtrl::readPackage()");
    DBINFO3("Read the information on the rfid-transponder");
    
    if (pReader->PICC_IsNewCardPresent() && pReader->PICC_ReadCardSerial() && pReader->uid.size != 0) 
    {
        DBINFO3("Scanner detected package");
        // Read 16 blocks and store it in a matrix
        for (int i = 0; i < 16; i++)
        {
            this->readBlock(i, &readBlockMatrix[i][0]);
        }
        // For Testing
        for (int i = 0; i < 16; i++)
        {
           for (int j = 0; j < 18; i++)
           {
               Serial.print(readBlockMatrix[i][j]);
           }
           Serial.println();
        }
        
        return 0;
    }
    else
    {
        return 1;
    }
    
    
}

int RfidReaderCtrl::readBlock(int blockNumber, byte* arrayAddress) 
{
  DBFUNCCALL("RfidReaderCtrl::readBlock()");
  DBINFO3("Read a block on the rfid-transponder");
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;

  byte status = pReader->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(pReader->uid));
  
  if (status != MFRC522::STATUS_OK) 
  {
    DBINFO3("PCD_Authenticate() failed (read): ");
    DBINFO3(pReader.GetStatusCodeName(status));
    return 3;//return "3" as error message
  }
          
  byte buffersize = 18; // variable to store buffersize
  status = pReader->MIFARE_Read(blockNumber, arrayAddress, &buffersize); //&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (status != MFRC522::STATUS_OK) 
  {
    DBINFO3("MIFARE_read() failed: ");
    DBINFO3(pReader.GetStatusCodeName(status));
    return 4;//return "4" as error message
  }

  DBINFO3("Bloack was read");

  return 0; // Evtl ändern
}
