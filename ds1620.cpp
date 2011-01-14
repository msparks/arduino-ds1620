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
  // Write configuration register in DS1620.
  byte flags = FLAG_CPU | FLAG_1SHOT;
  write_command_8bit(write_config_, flags);
  delay(200);
}


float Ds1620::temp_c()
{
  // Start temperature conversion.
  start_conv();

  // Read temperature.
  start_transfer();
  write_data(read_temp_, eight_bits_);
  word raw_data = read_data(nine_bits_);
  end_transfer();

  // Stopping conversion is not necessary here while in 1-shot mode.

  float temp_c = raw_data / 2.0;
  return temp_c;
}


void Ds1620::start_conv()
{
  write_command(start_conv_);

  // Max conversion delay is 750ms according to the datasheet.
  delay(750);
}


void Ds1620::stop_conv()
{
  write_command(stop_conv_);
}


word Ds1620::read_data(const DataSize size)
{
  byte bit = 0;
  word data = 0;

  pinMode(dq_pin_, INPUT);

  const uint8_t bits = (size == eight_bits_) ? 8 : 9;
  for (byte i = 0; i < bits; ++i) {
    digitalWrite(clk_pin_, LOW);
    bit = digitalRead(dq_pin_);
    digitalWrite(clk_pin_, HIGH);

    data |= (bit << i);
  }

  pinMode(dq_pin_, OUTPUT);
  return data;
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


void Ds1620::write_command_8bit(Command command, byte value)
{
  start_transfer();
  write_data(command, eight_bits_);
  write_data(value, eight_bits_);
  end_transfer();
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
