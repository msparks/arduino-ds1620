/*
  DS160 Library

  by Ruben Laguna 2009 <ruben.laguna at gmail.com>

  based on examples from Tom Tigoe <http://www.arduino.cc/en/Reference/SoftwareSerialExample>
  and phanderson <http://www.phanderson.com/printer/ds1620/ds1620.html>

  written: 30 Aug 2008
*/

#include "WProgram.h"
#include "ds1620.h"


Ds1620::Ds1620(int rst, int clk, int dq):rstPin(rst),clkPin(clk),dqPin(dq)
{
  pinMode(rst, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dq, OUTPUT);
}

void Ds1620::config()
{

  // write configuration register in DS1620
  rst_low();
  clk_high();
  rst_high(); //all data transfer are initiated by driving RST high
  write_command(0x0c); // write config command
  write_command(0x03); // cpu mode + 1 SHOT
  rst_low();
  delay(200); //wait until the configuration register is written
}


void Ds1620::start_conv()
{
  // START_CONV
  clk_high();
  rst_high();
  write_command(0xEE); //start conversion
  rst_low();
  delay(700);
}

void Ds1620::stop_conv()
{
  // STOP_CONV
  clk_high();
  rst_high();
  write_command(0x22); //stop conversion
  rst_low();
  //delay(200);
}

void Ds1620::write_command(int command)
/* sends 8 bit command on DQ output, least sig bit first */
{
  int n, bit;

  for(n=0;n<8;n++)
  {
    bit = ((command >> n) & (0x01));
    out_bit(bit);
  }
}

int Ds1620::read_data()
{
  //READ DATA
  clk_high();
  rst_high();
  write_command(0xAA);
  int raw_data = read_raw_data();
  rst_low();
  return raw_data;
}

int Ds1620::read_raw_data(void)
{
  int bit,n;
  int raw_data=0;

  pinMode(dqPin,INPUT);

  /* jam the dq lead high to use as input */
  for(n=0;n<9;n++)
  {
     clk_low();
     bit=(digitalRead(dqPin));
     clk_high();
     raw_data = raw_data | (bit <<  n);
  }
  pinMode(dqPin, OUTPUT);
  return(raw_data);
}

void Ds1620::out_bit(int bit)
{
  digitalWrite(dqPin, bit);  /* set up the data */
  clk_low();             /* and then provide a clock pulse */
  clk_high();
}

void Ds1620::clk_high(void)
{
  digitalWrite(clkPin,HIGH);
}

void Ds1620::clk_low(void)
{
  digitalWrite(clkPin,LOW);
}

void Ds1620::rst_high(void)
{
  digitalWrite(rstPin,HIGH);
}

void Ds1620::rst_low(void)
{
  digitalWrite(rstPin,LOW);
}
