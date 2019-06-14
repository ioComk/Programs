#include <Wire.h>
#include "im920_receiver.h"

const uint8_t RX = 2;
const uint8_t TX = 3;

const uint8_t DATA_SIZE = 9;

const uint8_t I2C_ADDR = 15;

IM920Receiver im920(RX, TX, DATA_SIZE);

byte padData[DATA_SIZE] = {0, 0, 127, 127, 127, 127, 0, 0, 0};

long startTime;

void setup()
{
  Wire.begin(I2C_ADDR);
  Wire.setClock(400000UL); // use 400 kHz I2C
  Wire.onRequest(requestEvent);

  Serial.begin(115200);

  im920.init();
  startTime = millis();

  //  delay(1000);
}

void loop()
{
  if (millis() - startTime >= 3000)
    im920.update();

  im920.getData(padData);
  im920.show();
}

void requestEvent(int any)
{
  Wire.write(padData, 9);
}
