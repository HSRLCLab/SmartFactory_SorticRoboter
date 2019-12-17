#SmartFactory_SorticRoboter
The SmartFactory_SorticRoboter is an implementation of the SmartFactory project for Sortic.
The SmartFactory_SorticRoboter scans the rfid chip from a package and sort the package to a suitable smart box to deliver the package.
To communicate with the SmartFactory_Box-Sortic is the SmartFactory_SorticRoboter in connection with the 
SmartFactory_SorticRoboter_CommunicationHub. The connection is made via an I2C data bus. 

The programmed algorithm is based on the finite state machine design pattern. 
The design pattern allows to give the roboter a state and to change it to another state by events.
The robot is built by different nested state machines and thus executes its tasks.

