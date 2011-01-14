/*
  DS160 Library
 
  by Ruben Laguna 2009 <ruben.laguna at gmail.com>
  based on examples from Tom Tigoe <http://www.arduino.cc/en/Reference/SoftwareSerialExample>
  and phanderson <http://www.phanderson.com/printer/ds1620/ds1620.html>
   
*/

#define ledPin 13

#include <stdlib.h>
#include <math.h>
//#include <Stdio.h>



#include "ds1620.h"


// set up ds1620
// this sets the pins as outputs
Ds1620 ds1620 = Ds1620(7/*rst*/,8/*clk*/,9/*dq*/);
 
void setup()
{
  
  // configure ds1620
  // this does sets cpu mode 1shot
  // see datasheet http://pdfserv.maxim-ic.com/en/ds/DS1620.pd 
  ds1620.config();

  Serial.begin(9600);
  delay(100);
  
  
  Serial.print("RST\r\n");  
}

void loop() 
{ 
   // start the temperature conversion
   // 1 shot mode
   ds1620.start_conv();
   
   // read the last temperature converson
   float temp = ds1620.temp_c();
   // stop conversion not really needed since we are in 
   // 1 shot mode
   ds1620.stop_conv();

   Serial.print((int)temp,DEC);
   Serial.print("\r\n");
}


