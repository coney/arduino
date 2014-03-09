#!/usr/bin/env ruby

require "serialport"
require "awesome_print"

SerialPort.open("/dev/ttyUSB0", 9600, 8, 1, SerialPort::NONE) do |serial|
  ap serial.readline
  1000000.times { |i|
    str = "%d\0" % i
    ap str
    serial.write str
    sleep 0.001
  }
end