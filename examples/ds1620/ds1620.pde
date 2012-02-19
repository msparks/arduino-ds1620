/*
Example sketch for interfacing with the DS1620 temperature chip.

Copyright (c) 2011, Matt Sparks
All rights reserved.
*/
#include <stdlib.h>
#include <DS1620.h>


// Set the appropriate digital I/O pin connections.
// See the datasheet for more details.
static const uint8_t RST_PIN = 7;  // pin 3 on DS1620
static const uint8_t CLK_PIN = 8;  // pin 2 on DS1620
static const uint8_t DQ_PIN  = 9;  // pin 1 on DS1620


DS1620 ds1620(RST_PIN, CLK_PIN, DQ_PIN);


void setup()
{
  Serial.begin(9600);
  delay(100);

  ds1620.config();
}


void loop()
{
  float temp = ds1620.temp_c();

  Serial.println(temp, 1);  // 1 decimal place
}
