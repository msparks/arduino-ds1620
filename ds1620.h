/*
  DS160 Library

  by Ruben Laguna 2009 <ruben.laguna at gmail.com>
  based on examples from Tom Tigoe <http://www.arduino.cc/en/Reference/SoftwareSerialExample>
  and phanderson <http://www.phanderson.com/printer/ds1620/ds1620.html>

  written: 30 Aug 2008
*/
#ifndef ds1620_h
#define ds1620_h

#include <WConstants.h>

class Ds1620
{
 public:
  Ds1620(int rst, int clk, int dq);
  void config();
  void start_conv();
  void stop_conv();
  int read_data();

 private:
  const int rst_pin_;
  const int clk_pin_;
  const int dq_pin_;

  enum DataSize {
    eight_bits_,
    nine_bits_
  };

  enum Command {
    read_temp_    = 0xAA,
    write_th_     = 0x01,
    write_tl_     = 0x02,
    read_th_      = 0xA1,
    read_tl_      = 0xA2,
    read_cnt_     = 0xA0,
    read_slope_   = 0xA9,
    start_conv_   = 0xEE,
    stop_conv_    = 0x22,
    write_config_ = 0x0C,
    read_config_  = 0xAC
  };

  void start_transfer();
  void end_transfer();

  int read_raw_data(void);
  void write_data(word data, const DataSize size);
  void write_command(Command command);
  void write_command_8bit(Command command, uint8_t value);
};

#endif
