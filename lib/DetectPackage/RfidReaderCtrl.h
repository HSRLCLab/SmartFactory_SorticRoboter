/**
 * @file RfidReaderCtrl.h
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief Librarie to controll rfid sensor
 * @version 0.1
 * @date 2019-11-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef RFIDREADERCTRL_H
#define RFIDREADERCTRL_H

#include <Arduino.h>
#include <MFRC522.h>

// own files:
#include "LogConfiguration.h"
#include "MainConfiguration.h"


/**
 * @brief The rfid reader controll class controlls the rfid sensor
 * 
 */
class RfidReaderCtrl
{
    //======================PUBLIC===========================================================
    public:

    /**
     * @brief Enum class holds all possible Events
     * 
     */
    enum class Event
    {
        NoEvent,
        NoPackageAvailable,
        Error
    };

    struct Package
    {
        int id = 0;
        String cargo = "";
        uint8_t targetDest = 0;
    };

    
    /**
     * @brief Construct a new Rfid Reader Ctrl object
     * 
     */
    RfidReaderCtrl(RfidReaderCtrl::Package *packagePtr);

    ~RfidReaderCtrl();

    /**
     * @brief Get the Package Information object
     * 
     * @return Event - generated Event
     */
    Event getPackageInformation();

    /**
     * @brief Check if a package is available
     * 
     * @return true 
     * @return false 
     */
    bool isPackageAvailable();

    inline byte *getReadBlockMatrix()
    {
        byte *ptr = &readBlockMatrix[0][0];
        return ptr;
    }

    
    //======================PRIVATE==========================================================
    private:

    byte readBlockMatrix[16][18]; 
    byte buffersize = 18;

    MFRC522::StatusCode status;     ///< instance of status code
    MFRC522 *pReader = new MFRC522(RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN);     ///< instance of mfrc sensor librarie
    MFRC522::MIFARE_Key key;        ///< create a MIFARE_Key struct named 'key', which will hold the card information

    Package *pPackagePtr;
    

    /**
     * @brief read package information from rfid transponder
     * 
     * @return Event - generated Event
     */
    Event readPackage();

    /**
     * @brief read block on rfid transponder
     * 
     * @param blockNumber - byte
     * @param arrayAddress -byte*
     * @return Event - generated Event
     */
    Event readBlock(byte blockNumber, byte* arrayAddress);

    /**
     * @brief parse package information to package message
     * 
     */
    void parseInformationToStruct();
};

#endif