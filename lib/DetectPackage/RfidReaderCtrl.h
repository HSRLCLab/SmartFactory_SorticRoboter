/**
 * @file RfidReaderCtrl.h
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * @brief Library to controll rfid sensor
 * @version 1.0
 * @date 2019-12-16
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

    /**
     * @brief Package struct holds all information of the package
     * 
     */
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

    /**
     * @brief Destroy the Rfid Reader Ctrl object
     * 
     */
    ~RfidReaderCtrl();

    /**
     * @brief get the package information on the rfid transponder
     * 
     * @return Event - generated Event
     */
    Event getPackageInformation();

    /**
     * @brief Get the Read Block Matrix object
     * 
     * @return byte* 
     */
    inline byte *getReadBlockMatrix()
    {
        byte *ptr = &readBlockMatrix[0][0];
        return ptr;
    }

    
    //======================PRIVATE==========================================================
    private:

    byte readBlockMatrix[16][18];                                               ///< matrix to store all bytes from the rfid transponder
    byte buffersize = 18;                                                       ///< size of all pages + 2

    MFRC522::StatusCode status;                                                 ///< instance of status code
    MFRC522 *pReader = new MFRC522(RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN);     ///< instance of mfrc sensor librarie
    MFRC522::MIFARE_Key key;                                                    ///< create a MIFARE_Key struct named 'key', 
                                                                                ///< which will hold the card information

    Package *pPackagePtr = nullptr;                                             ///< pointer to the storage place of the package struct

    /**
     * @brief read block on rfid transponder
     * 
     * @param blockNumber - byte
     * @param arrayAddress -byte*
     * @return Event - generated Event
     */
    Event readBlock(byte blockNumber, byte* arrayAddress);

    /**
     * @brief parse package information to package struct
     * 
     * @todo Can't read the information on the rfid chip, it worked and now not anymore
     *       Maybe Hardware defect
     *       The card presents can be detected!
     *       Check the hardware if it's damaged
     *       As second the logic to read data is not actually, the data types has been changed
     *       Stream the bytes into the integer datatypes
     * 
     */
    void parseInformationToStruct();
};

#endif