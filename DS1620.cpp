#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "DS1620.h"

// Configuration flag bitmasks.
#define FLAG_DONE  0x80
#define FLAG_THF   0x40
#define FLAG_TLF   0x20
#define FLAG_NVB   0x10
#define FLAG_CPU   0x20
#define FLAG_1SHOT 0x01


DS1620::DS1620(const int rst, const int clk, const int dq)
    : rst_pin_(rst),
      clk_pin_(clk),
      dq_pin_(dq)
{
  pinMode(rst, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dq, OUTPUT);
}


void DS1620::config()
{
  // Write configuration register in DS1620.
  byte flags = FLAG_CPU | FLAG_1SHOT;
  write_command_8bit(write_config_, flags);

  // E^2 memory has a small write delay. According to the datasheet, this is
  // around 10ms at room temperature.
  delay(50);
}


float DS1620::temp_c()
{
  // Start temperature conversion.
  start_conv();

  start_transfer();
  write_data(read_temp_, eight_bits_);
  const int temp_read = read_data(nine_bits_) >> 1;  // strip the LSB
  end_transfer();

  start_transfer();
  write_data(read_cnt_, eight_bits_);
  const int count_remain = read_data(nine_bits_);
  end_transfer();

  start_transfer();
  write_data(read_slope_, eight_bits_);
  const int count_per_c = read_data(nine_bits_);
  end_transfer();

  // High-resolution temperature calculation. See page 4 of the datasheet for
  // details.
  const float temp_c =
      temp_read - 0.25 +
      (count_per_c - count_remain) / static_cast<float>(count_per_c);
  return temp_c;
}


void DS1620::start_conv()
{
  write_command(start_conv_);

  // Max conversion delay is 750ms according to the datasheet.
  delay(750);
}


void DS1620::stop_conv()
{
  write_command(stop_conv_);
}


word DS1620::read_data(const DataSize size)
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


void DS1620::write_data(const word data, const DataSize size)
{
  // Always write out the lower byte.
  shiftOut(dq_pin_, clk_pin_, LSBFIRST, (data & 0xFF));

  if (size == nine_bits_) {
    // Write out upper byte.
    shiftOut(dq_pin_, clk_pin_, LSBFIRST, ((data & 0xFF00) >> 8));
  }
}


void DS1620::write_command(const Command command)
{
  start_transfer();
  write_data(command, eight_bits_);
  end_transfer();
}


void DS1620::write_command_8bit(const Command command, const byte value)
{
  start_transfer();
  write_data(command, eight_bits_);
  write_data(value, eight_bits_);
  end_transfer();
}


void DS1620::start_transfer()
{
  digitalWrite(clk_pin_, LOW);
  digitalWrite(rst_pin_, HIGH);
}


void DS1620::end_transfer()
{
  digitalWrite(rst_pin_, LOW);
}
