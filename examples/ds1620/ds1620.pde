#include <stdlib.h>
#include <math.h>
#include "DS1620.h"


// set up ds1620
// this sets the pins as outputs
DS1620 ds1620 = DS1620(7/*rst*/,8/*clk*/,9/*dq*/);

void setup()
{
  Serial.begin(9600);
  delay(100);

  ds1620.config();
}


void loop()
{
  // read the last temperature converson
  float temp = ds1620.temp_c();

  Serial.println(temp, 1);
}
