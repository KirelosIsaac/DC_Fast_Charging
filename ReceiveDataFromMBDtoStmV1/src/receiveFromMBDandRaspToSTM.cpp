#include "../include/receiveFromMBDandRaspToSTM.hpp"
#include <Arduino.h>
#include "SoftwareSerial.h"
#include "string.h"
#include <stdio.h>

SoftwareSerial mySerial(PINRX, PINTX);

char receivedString[MAX_STRING_LENGTH];
double ConvertedDoubleData;

void StringToDouble(const char *str, double *value) // private
{
  // Convert the received string to a double
  // *value = atof(str);
  *value = strtod(str, NULL);
}

void API_Initialize(void)
{
  mySerial.begin(BAUDRATE);
}

void ReadFromSerial(char *receivedData, int StringSize)
{
  int dataIndex = 0;
  // Read the incoming character
  while (dataIndex < StringSize)
  {
    char incomingChar = mySerial.read();
    // Check if the character is a newline character (end of the string)
    if (incomingChar == '\0')
    {
      // Null-terminate the array to make it a valid C-string
      receivedData[dataIndex] = '\0';
      break;
    }
    else
    {
      // Store the character in the array
      receivedData[dataIndex] = incomingChar;
      dataIndex++;
    }
  }
}
double API_receiveData(void)
{
  int l;
  for (int i = 0; i < 20; i++)
  {
    receivedString[i] = '0';
  }

  if (mySerial.available() > 0)
  {
    // ReadFromSerial(receivedString,MAX_STRING_LENGTH);
    l = mySerial.readBytesUntil('\0', receivedString, MAX_STRING_LENGTH);
    mySerial.flush();

    receivedString[l] = '\0';

    // int i, j = 19;
    // for (i = l; i >= 0; i--)
    // {
    //   receivedString[j] = receivedString[i];
    //   receivedString[i] = '0';
    //   j--;
    // }

    // Convert the string to a double (assuming the data is a floating-point number)
    StringToDouble(receivedString, &ConvertedDoubleData);

    delay(30);
    mySerial.flush();
    return ConvertedDoubleData;
  }
  mySerial.flush();
  return 0;
}

void doubleToString(double value, char *output, int precision)
{
  memset((char *)output, '0', 20);
  long long wholePart = (long long)value;    // Extract the whole part of the double
  long long intPart = wholePart;             // Copy the whole part for further calculations
  double fractionalPart = value - wholePart; // Extract the fractional part

  int length = 0; // Variable to keep track of string length

  // Convert the whole part to string manually
  do
  {
    output[length++] = '0' + (intPart % 10); // Convert digit to character
    intPart /= 10;
  } while (intPart != 0);

  // Reverse the whole part string
  for (int i = 0, j = length - 1; i < j; i++, j--)
  {
    char temp = output[i];
    output[i] = output[j];
    output[j] = temp;
  }

  if (precision > 0)
  {
    output[length++] = '.'; // Add decimal point
    // Convert the fractional part to string manually with desired precision
    for (int i = 0; i < precision; i++)
    {
      fractionalPart *= 10;
      int digit = (int)fractionalPart;
      output[length++] = '0' + digit; // Convert digit to character
      fractionalPart -= digit;
    }
  }
  // 0.510000000000000

  output[19] = '\0';
  // output[length] = '\0'; // Null-terminate the string
}

void API_PID_SetDutyCycle(double *Local_doubleDutyCycle)
{
  char Local_u8Message[20];
  if (*Local_doubleDutyCycle)
  {
    doubleToString(*Local_doubleDutyCycle, Local_u8Message, Presicions);
    mySerial.write(Local_u8Message);
    // delay(30);
    mySerial.flush();
    *Local_doubleDutyCycle = 0;
  }
}
