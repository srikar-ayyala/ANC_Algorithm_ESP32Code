#include "Arduino.h"
#include "driver/i2s.h" // Library of I2S routines, comes with ESP32 standard install

#include "CustomEspI2SInput.h"
// #include "CustomEspI2SOutput.h"

#include "CustomEspFilter.h"

static const int inputPin = 34;

void printStamp(bool isUp)
{
  Serial.print(">Data:");
  if (isUp)
  {
    Serial.println(100000);
  }
  else
  {
    Serial.println(-100000);
  }
}

// ESP FUNC
void setup()
{
  setupInputI2S();
  // setupOutputI2S();
  pinMode(inputPin, INPUT);
  Serial.begin(115200);

  // pushTestDataInRecording();
}

void useFilterSingleInput()
{
  int16_t prevOutput = 0;
  const int lag = 1;
  for (int i = lag; i < recIndex2; i++)
  {
    Serial.print(">DATA:");
    Serial.println(recording[i]);
    prevOutput = useFilter(recording[i], recording[i - lag] - prevOutput);
  }
}

int count = 0;
void loop()
{
  if(digitalRead(inputPin)) {
    runInputForTimeInterval(1000);
    useFilterSingleInput();
  }
  Serial.print(">STOP:");
  Serial.println(1);
  delay(100);
}