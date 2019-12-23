# SmartFactory_SorticRoboter

The [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter) is an implementation of the SmartFactory project for Sortic.
The [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter) scans the rfid chip from a package and sort the package to a suitable [SmartFactory_Box-Sortic](https://github.com/LMazzole/SmartFactory_Box-Sortic) to deliver the package.
To communicate with the [SmartFactory_Box-Sortic](https://github.com/LMazzole/SmartFactory_Box-Sortic) the [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter) is in connection with the [SmartFactory_SorticRoboter_CommunicationHub](https://github.com/philipzellweger/SmartFactory_SorticRoboter_CommunicationHub). The connection is made via an I2C data bus. 

The programmed algorithm is based on the finite state machine design pattern. 
The design pattern allows to give the robot a state and to change it to another state by events.
The robot is built by different nested state machines and thus executes its tasks.

## Table of contents
- [Systemarchitecture SmartFactory](#systemarchitecture-smartfactory)
- [Working area](#working-area)
- [Tools and technologies](tools-and-techniques)
   - [Visual Studio Code](#visual-studi-code)
   - [Doxygen](#doxygen)
   - [I2C](#i2c)
   - [SPI](#spi)
   - [RFID](#rfid)
   - [Ultrasound](#ultrasound)
- [Hardware](#hardware)
   - [Sortic Robot](#sortic-robot)
     - [Electrical shematic](#electrical-shematic)
     - [Arduino Uno](#arduino-uno)
     - [Arduino Motor Shield](#arduino-motor-shield)
     - [LEGO Motor](#lego-motor)
     - [MFRC522](#mfrc522)
- [Software](#software)
  - [Finite State Machine](#finite-state-machine)
  - [UML](#uml)
  - [Dependency Graph](#dependency-graph)
  - [Collaboration Diagram](#collaboration-diagram)
- [ToDo's](#todo's)
- [Contributors](#contributors)
- [Changelog](#changelog)
- [License](#license)
- [Links to SmartFactory](#links-to-smartfactory)

## Systemarchitecture SmartFactory

The following diagram shows the system architecture of the SmartFactory.

![systemarchitecture](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/Systemarchitektur_SmartFactory.jpg)

[Image: SmartFactory system architecture]

## Working area
The existing sorting robot is designed with three transition lines. The number of lines can be extended modularly. In addition (to the three lines) the system has a upload line to load the packages.

![workingarea](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/SorticRoboter_Workingarea.jpg)

[Image: SorticRoboter workingarea]

## Tools and technologies

The source code is implemented in the programming language C++. In the following, the tools for editing the project are listed.

#### Visual Studio Code
The development environment used is [Visual Studio Code](https://code.visualstudio.com/) with the [PlatformIO extension](https://docs.platformio.org/en/latest/ide/vscode.html). The development environment can be downloaded open sourced. For an installation guide look [here](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/Installation_Guide_SmartFactory.pdf).  

#### Doxygen
Doxygen was used for documenting the source code. For using Doxygen in Visual Studio code, the [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen) extension is available.

#### Fritzing

For the creation of the electrical shema the software Fritzing was used. A link to the software can be found [here](https://fritzing.org/home/).

#### I2C
The connection to the [SmartFactory_SorticRoboter_CommunicationHub](https://github.com/philipzellweger/SmartFactory_SorticRoboter_CommunicationHub) is via i2c. For an explanation of the technology and the library look [here](https://github.com/philipzellweger/SmartFactory_I2cCommunication).

#### SPI
The data on the rfid transponder is transmitted by the spi protocol. Spi is a serial data bus which is used to read the data of the rfid scanner. For an explanation of the technology look [here](http://www.circuitbasics.com/basics-of-the-spi-communication-protocol/).

#### RFID
The radio frequency identification technology is used to write and read the information of the package. This is a non-contact automatic transmitter receiver system based on the physical principle of a capacitor. Electromagnetic waves transmit the data from the transponder with the scanner. A detailed explanation can be found [here](https://www.atlasrfidstore.com/rfid-beginners-guide/).

#### Ultrasound
For the navigation of the robot, the position is measured by an ultrasonic sensor. The sensor emits sound waves and receives them again, so that the distance to an object can be determined. The sensor data is transmitted by the SPI protocol. A detailed explanation of the technology can be found [here](https://www.maxbotix.com/articles/how-ultrasonic-sensors-work.htm).

## Hardware

The elements used for the construction are built of Lego. Lego is used, as there are no real construction requirements in the project. It is needed to illustrate the SmartFactory concept. This principle can be applied to any conceivable installation. In addition Lego also reflects the modularity of the project. 

### SorticRobot
The Sortic robot is constructed as shown in the figure below. The robot consists of the following elements:
- Arduino Uno
- Arduino Motorshield V2.0
- Sonar HC-SR04
- Lego Motor
- MFRC522 RFID Reader
- Sortic substructure based on LEGO

![SorticRobot](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/SorticRoboter_Konstruktion.jpg)

[Image: SmartFactory_SorticRoboter with robot arm]

#### Electrical shematic

The following illustrations show the electro shema of the [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter). The connection to the [SmartFactory_SorticRoboter_CommunicationHub](https://github.com/philipzellweger/SmartFactory_SorticRoboter_CommunicationHub) is shown on it.

![Breadbord](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/SorticRoboter_with_CommunicationHub_Fritzing/SorticRoboter_with_CommunicationHub_BreadBoard.jpg)

[Image: Breadboard SmartFactory_SorticRoboter with SmartFactory_SorticRoboter_CommunicationHub]

![Electrical shematic](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/SorticRoboter_with_CommunicationHub_Fritzing/SorticRoboter_with_CommunicationHub_electrical_shematic.jpg)

[Image: Electrical shematic SmartFactory_SorticRoboter with SmartFactory_SorticRoboter_CommunicationHub]

#### Arduino Uno
The microcontroller used is an [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3). For the existing system the memory and clock speed are sufficient, for a more complex system it is recommended to change to an [Arduino Mega](https://store.arduino.cc/arduino-mega-2560-rev3).

![arduino uno](https://www.distrelec.ch/Web/WebShopImages/landscape_large/6-/01/ArduinoUNO_SMD_A000073_30101956-01.jpg)

[Image: [Distrelec: Arduino Uno](https://www.distrelec.ch/de/arduino-uno-rev3-smd-arduino-a000073/p/30101956)]

#### Arduino Motor Shield
A motor shield is required to control the motor. The used [motorshield](https://store.arduino.cc/arduino-motor-shield-rev3) is from Arduino and is compatible to the Arduino Uno and Arduino Mega.

![motorshield](https://ardushop.hu/image/cache/catalog/termekek/arduino-motor-shield-v3-1_1-900x900.jpg)

[Image: [ARDUSHOP: Arduino Motor Shield Rev3](https://ardushop.hu/arduino-motor-shield-rev3-ard35)]

#### LEGO Motor
The [Lego motor](https://www.lego.com/de-ch/product/lego-power-functions-xl-motor-8882) is used for the linear motion of the Sortic robot.

![legomotor](https://www.meinbaukasten.de/media/catalog/product/cache/1/image/1000x1000/040ec09b1e35df139433887a97daa66f/l/e/lego-motor-xl-8882.jpg)

[Image: [Mein Baukasten: LEGO exucation Power Functions XL-Motor](https://www.meinbaukasten.de/lego-power-functions-xl-motor-8882.html)]

#### MFRC522
The sensor used to read the data on the rfid transponder is the [MFRC522](https://www.yampe.com/product/details.jsp?curren=1&la=3&type=detail&product_id=2883&product_type_id=38&brand_id=34) Rfid sensor.

![rfidscanner](https://3.bp.blogspot.com/-LIRYnr98scs/WxPHATmSfdI/AAAAAAABIlA/UzwXyHHkqCQpFBX0bW4NZXvnm_ARMAEDQCPcBGAYYCw/s1600/arduino-rfid-rc522-esquema.png)

[Image: [Arduino Tutorial Online: RFID-RC522](http://www.arduinotutorialonline.com/2018/06/arduino-nano-rfid-rc522-arduino-tutorial.html)]

## Software

#### Finite State Machine
The design pattern used to implement the software is the Finite State Machine. The robot always has a state. The states are transformed into other states by events. The figure below shows the finite state machine of the sortic robot seen in the blue area. The main state machine is the SorticCtrl, which contains the finite state machine DetectPackageCtrl, SortPackageCtrl and NavigationCtrl.

![FSM](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/FSM_MASTER.jpg)

[Image: Finite State Machine SorticRoboter with SorticRoboter CommunicationHub]

#### UML
The figure below shows the data model in UML notation. A separate class has been implemented for each finite state machine.

<p align="center">
    <a href=https://philipzellweger.github.io/SmartFactory_SorticRoboter/class_sortic_ctrl.html>
        <img src="https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/html/class_sortic_ctrl__coll__graph.png" style="border:none;"/>
    </a>
    <p align="center"><small>Click on the image to open doxygen-documentation.</p>
</p>

#### Dependency Graph
The figure below shows the dependency tree of the main file.
The used extern libraries for the project:
* [SmartFactory_I2cCommunication](https://github.com/philipzellweger/SmartFactory_I2cCommunication)
* MFRC522.h

<p align="center">
    <a href=https://philipzellweger.github.io/SmartFactory_SorticRoboter/main_8cpp.html>
        <img src="https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/html/main_8cpp__incl.png" style="border:none;"/>
    </a>
    <p align="center"><small>Click on the image to open doxygen-documentation.</p>
</p> 

## ToDo's

All the ToDo's are documented in the source code with Doxygen.

# Contributors
- [Philip Zellweger](https://github.com/philipzellweger)

# Changelog

V 1.0   -	Release SA HS20 -	[Philip Zellweger](https://github.com/philipzellweger)

# License

MIT License

# Links to SmartFactory
- [SmartFactory-Sortic](https://github.com/LMazzole/SmartFactory-Sortic)
- [SmartFactory_Box-Sortic](https://github.com/LMazzole/SmartFactory_Box-Sortic)
- [SmartFactory_Vehicle-Sortic](https://github.com/LMazzole/SmartFactory_Vehicle-Sortic)
- [SmartFactory_Vehicle-Basis](https://github.com/LMazzole/SmartFactory_Vehicle-Basis)
- [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter)
- [SmartFactory_SorticRoboter_CommunicationHub](https://github.com/philipzellweger/SmartFactory_SorticRoboter_CommunicationHub)
- [SmartFactory_MQTTCommunication](https://github.com/LMazzole/SmartFactory_MQTTCommunication) for Adafruit Feather M0 Wifi
- [SmartFactory_MQTTCommunication](https://github.com/philipzellweger/SmartFactory_MQTTCommunication) for Esp32 DevKitC
- [SmartFactory_I2cCommunication](https://github.com/philipzellweger/SmartFactory_I2cCommunication)
- [SmartFactory_Messages](https://github.com/philipzellweger/SmartFactory_Messages)
