#include <Arduino.h>
#include "SoftwareSerial.h"
#include "string.h"
#include "../include/receiveFromMBDandRaspToSTM.hpp"


double received;

void setup()
{
  API_Initialize();  
}

void loop()
{
  received  = API_receiveData();
  API_PID_SetDutyCycle( &received);
  delay(50);
}