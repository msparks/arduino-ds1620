/*
Copyright (c) 2011, Matt Sparks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

/* Inspired by a similar library by Ruben Laguna. */

#ifndef DS1620_h
#define DS1620_h

#include <math.h>
#include <WConstants.h>


class DS1620
{
 public:
  DS1620(int rst, int clk, int dq);
  void config();

  float temp_c();

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

  void start_conv();
  void stop_conv();

  word read_data(DataSize size);
  void write_data(word data, DataSize size);
  void write_command(Command command);
  void write_command_8bit(Command command, byte value);
};

#endif
