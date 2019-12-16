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


#define RFIDDETECTOR_SDA         53
#define RFIDDETECTOR_RST_PIN     49

#define I2CSLAVEADDRESP 7

const unsigned int TIME_BETWEEN_PUBLISH = 300;
const unsigned int SORTIC_WAITFOR_BOX_SECONDS = 5;   

#endif