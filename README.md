# SmartFactory_SorticRoboter

The [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter) is an implementation of the SmartFactory project for Sortic.
The [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter) scans the rfid chip from a package and sort the package to a suitable [SmartFactory_Box-Sortic](https://github.com/LMazzole/SmartFactory_Box-Sortic) to deliver the package.
To communicate with the [SmartFactory_Box-Sortic](https://github.com/LMazzole/SmartFactory_Box-Sortic) is the [SmartFactory_SorticRoboter](https://github.com/philipzellweger/SmartFactory_SorticRoboter) in connection with the [SmartFactory_SorticRoboter_CommunicationHub](https://github.com/philipzellweger/SmartFactory_SorticRoboter_CommunicationHub). The connection is made via an I2C data bus. 

The programmed algorithm is based on the finite state machine design pattern. 
The design pattern allows to give the roboter a state and to change it to another state by events.
The robot is built by different nested state machines and thus executes its tasks.

## Table of contents
- Working area
- Tools and technologies
- Hardware
- Software
- ToDo's

## Working are
The existing sorting robot is designed for 3 lines. The number of lines can be increased modularly. In addition to the three lines, the system has a loading line to load the packages.

![lines](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/Übersicht_Sortic_Lines.jpg)

[Image: SorticRoboter line overview]

## Tools and technologies

The source code is implemented in the programming language C++. In the following, the tools for editing the project are listed.

#### Visual Studio Code
The development environment used is [Visual Studio Code](https://code.visualstudio.com/) with the [PlatformIO extension](https://docs.platformio.org/en/latest/ide/vscode.html). The development environment can be downloaded for free. For an installation guide look here.  

#### Doxygen
Doxygen was used for documentation of the source code. For using Doxygen in Visual Studio code, the [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen) extension can be used.

#### I2C
The connection to the hub is via i2c. For an explanation of the technology and the library look [here](https://github.com/philipzellweger/SmartFactory_I2cCommunication).

#### SPI
The data on the rfid transponder is read by the spi protocol. Spi is a serial data bus which is used to read the data of the rfid scanner. For an explanation of the technology look [here](http://www.circuitbasics.com/basics-of-the-spi-communication-protocol/).

#### RFID
The radio frequency identification technology is used to store and read the information of the package. This is a non-contact automatic transmitter receiver system based on the physical principle of a capacitor. Electromagnetic waves read the data from the transponder with the scanner. A detailed explanation can be found [here](https://www.atlasrfidstore.com/rfid-beginners-guide/).

#### Ultrasound
For the navigation of the robot, the position is measured by an ultrasonic sensor. The sensor emits sound waves and receives them again, so that the distance to an object can be determined. The sensor is read by the SPI protocol. A detailed explanation of the technology can be found [here](https://www.maxbotix.com/articles/how-ultrasonic-sensors-work.htm).

## Hardware

The elements used for the construction are made of Lego. Lego serves as a basis, as there are no real construction requirements in the project and only means to illustrate the Sortic concept. The principle can be applied to any conceivable installation. Lego also reflects the modularity of the project. 

### SorticRobot
The Sortic robot is constructed as shown in the figure below. The robot consists of the following elements:
- Arduino Uno
- Arduino Motorshield V2.0
- Sonar HC-SR04
- Lego motor
- MFRC522 RFID Reader
- Sortic substructure based on LEGO

![SorticRobot](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/SorticRoboter_Konstruktion.jpg)

[Image: SmartFactory_SorticRoboter]

#### Arduino Uno
The microcontroller used is an [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3). For the existing system the memory and clock speed are sufficient, for a more complex system it is recommended to change to an [Arduino Mega](https://store.arduino.cc/arduino-mega-2560-rev3).

![arduino uno](https://www.distrelec.ch/Web/WebShopImages/landscape_large/6-/01/ArduinoUNO_SMD_A000073_30101956-01.jpg)

[Image: [Distrelec: Arduino Uno](https://www.distrelec.ch/de/arduino-uno-rev3-smd-arduino-a000073/p/30101956)]

#### Arduino Motor Shield
A motor shield is required to control the motor. The used [motorshield](https://store.arduino.cc/arduino-motor-shield-rev3) is from Arduino and is compatible to the Arduino Uno and Mega.

![motorshield](https://ardushop.hu/image/cache/catalog/termekek/arduino-motor-shield-v3-1_1-900x900.jpg)

[Image: [ARDUSHOP: Arduino Motor Shield Rev3](https://ardushop.hu/arduino-motor-shield-rev3-ard35)]

#### LEGO Motor
A [Lego motor](https://www.lego.com/de-ch/product/lego-power-functions-xl-motor-8882) is used to move the Sortic robot.

![legomotor](https://www.meinbaukasten.de/media/catalog/product/cache/1/image/1000x1000/040ec09b1e35df139433887a97daa66f/l/e/lego-motor-xl-8882.jpg)

[Image: [Mein Baukaste: LEGO exucation Power Functions XL-Motor](https://www.meinbaukasten.de/lego-power-functions-xl-motor-8882.html)]

#### MFRC522
The sensor used to read the data on the rfid transponder is the [MFRC522](https://www.yampe.com/product/details.jsp?curren=1&la=3&type=detail&product_id=2883&product_type_id=38&brand_id=34) Rfid sensor.

![rfidscanner](https://3.bp.blogspot.com/-LIRYnr98scs/WxPHATmSfdI/AAAAAAABIlA/UzwXyHHkqCQpFBX0bW4NZXvnm_ARMAEDQCPcBGAYYCw/s1600/arduino-rfid-rc522-esquema.png)

[Image: [Arduino Tutorial Online: RFID-RC522](http://www.arduinotutorialonline.com/2018/06/arduino-nano-rfid-rc522-arduino-tutorial.html)]

## Software

#### Finite State Machine
The design pattern used to implement the software is the Finite State Machine. The robot always has a state. The states are transformed into other states by events. The figure below shows the finite state machine of the sortic robot in blue.  The main state machine is the SorticCtrl, which contains the finite state machine DetectPackageCtrl, SortPackageCtrl and NavigationCtrl.

![FSM](https://github.com/philipzellweger/SmartFactory_SorticRoboter/blob/master/docs/FSM_MASTER.jpg)

[Image: Finite State Machine SorticRoboter with SorticRoboter CommunicationHub]

#### UML
The figure below shows the data model in UML notation. A separate class has been implemented for each finite state machine.

================== IMAGE ===============================

#### Dependency Graph
The figure below shows the dependency tree of the main FSM SorticCtrl.
The used extern libraries for the project:
* [SmartFactory_I2cCommunication](https://github.com/philipzellweger/SmartFactory_I2cCommunication)


 ==================== IMAGE ==================================
 
 
 
#### Collaboration Diagram
The figure below shows the collaboration tree of the main FSM SorticCtrl. The arrow simbolizes an instanced object.

=================== IMAGE ==========================================

## ToDo
- [ ]
- [ ]
