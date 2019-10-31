#include <Arduino.h>
#include "DetectPackageCtrl.h"
#define DEBUGGER


DetectPackageCtrl *detectPackage;

void setup() 
{
  
  Serial.begin(9600);
  Serial.println("Im here");
  detectPackage  = new DetectPackageCtrl();
}

void loop() 
{
  Serial.println("State1");
  detectPackage->loop(DetectPackageCtrl::Event::CheckForPackage);
  delay(1000);
}