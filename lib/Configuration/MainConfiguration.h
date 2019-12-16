/**
 * @file MainConfiguration.h
 * @brief Configurations 
 * 
 * @author Philip Zellweger (philip.zellweger@hsr.ch)
 * 
 * @version 1.0 
 *  
 * @date 2019-04-03
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MAINCONFIGURATION_H
#define MAINCONFIGURATION_H


#define RFIDDETECTOR_SDA         53                     ///< SDA Pin of Arduino Mega for Rrfi detector
#define RFIDDETECTOR_RST_PIN     49                     ///< RST Pin of Arduino Mega for rfi detector

#define I2CSLAVEADDRUNO 7                               ///< I2C Adress of Arduino Mega
#define I2CMASTERADDRESP 33                             ///< I2C Adress of ESP32

const unsigned int TIME_BETWEEN_PUBLISH = 300;          ///< Time which will minimum reached between publish messages

#endif