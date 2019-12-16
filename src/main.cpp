#include <Arduino.h>
#include "SorticCtrl.h"

SorticCtrl *sorticRobot;

void setup() 
{
  
  Serial.begin(9600);
  Serial.println("Start Sortic");
  sorticRobot = new SorticCtrl();
}

void loop() 
{
  
  sorticRobot->loop();
  
}