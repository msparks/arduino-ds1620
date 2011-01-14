/*
  DS160 Library

  by Ruben Laguna 2009 <ruben.laguna at gmail.com>

  based on examples from Tom Tigoe <http://www.arduino.cc/en/Reference/SoftwareSerialExample>
  and phanderson <http://www.phanderson.com/printer/ds1620/ds1620.html>

  written: 30 Aug 2008
*/

#include "WProgram.h"
#include "ds1620.h"

// Configuration flag bitmasks.
#define FLAG_DONE  0x80
#define FLAG_THF   0x40
#define FLAG_TLF   0x20
#define FLAG_NVB   0x10
#define FLAG_CPU   0x20
#define FLAG_1SHOT 0x01


Ds1620::Ds1620(int rst, int clk, int dq)
    : rst_pin_(rst),
      clk_pin_(clk),
      dq_pin_(dq)
{
  pinMode(rst, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dq, OUTPUT);
}

void Ds1620::config()
{
  // write configuration register in DS1620
  byte flags = FLAG_CPU | FLAG_1SHOT;
  write_command_8bit(write_config_, flags);
  delay(200); //wait until the configuration register is written
}


void Ds1620::start_conv()
{
  // START_CONV
  write_command(start_conv_); //start conversion
  delay(700);
}

void Ds1620::stop_conv()
{
  // STOP_CONV
  write_command(stop_conv_); //stop conversion
}

void Ds1620::write_data(word data, const DataSize size)
{
  // Always write out the lower byte.
  shiftOut(dq_pin_, clk_pin_, LSBFIRST, (data & 0xFF));

  if (size == nine_bits_) {
    // Write out upper byte.
    shiftOut(dq_pin_, clk_pin_, LSBFIRST, ((data & 0xFF00) >> 8));
  }
}

void Ds1620::write_command(Command command)
{
  start_transfer();
  write_data(command, eight_bits_);
  end_transfer();
}

void Ds1620::write_command_8bit(Command command, uint8_t value)
{
  start_transfer();
  write_data(command, eight_bits_);
  write_data(value, eight_bits_);
  end_transfer();
}

int Ds1620::read_data()
{
  //READ DATA
  start_transfer();
  write_data(read_temp_, eight_bits_);
  int raw_data = read_raw_data();
  end_transfer();
  return raw_data;
}

int Ds1620::read_raw_data(void)
{
  int bit,n;
  int raw_data=0;

  pinMode(dq_pin_, INPUT);

  /* jam the dq lead high to use as input */
  for(n=0;n<9;n++)
  {
     digitalWrite(clk_pin_, LOW);
     bit=(digitalRead(dq_pin_));
     digitalWrite(clk_pin_, HIGH);
     raw_data = raw_data | (bit <<  n);
  }
  pinMode(dq_pin_, OUTPUT);
  return(raw_data);
}

void Ds1620::start_transfer()
{
  digitalWrite(clk_pin_, LOW);
  digitalWrite(rst_pin_, HIGH);
}

void Ds1620::end_transfer()
{
  digitalWrite(rst_pin_, LOW);
}
