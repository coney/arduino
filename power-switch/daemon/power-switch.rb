#!/usr/bin/env ruby

require "awesome_print"
require "daemon"
require "serialport"

SERIAL_PORT = "/dev/ttyUSB0"
SERIAL_RATE = 9600


class PowerSwitch
  EOM = "\0"

  def initialize sp
    @serial_io = sp
  end

  def start
    read_form_serial do |line|
      if line == "hello"
        ap "replay heartbeat"
        heartbeat
      elsif line == "goodbye"
        ap "shutdown now"
        `shutdown -H now`
      else
        ap "read #{line}"
      end
    end
  end

  private
  def read_form_serial
    loop do
      @serial_io.readlines.each do |line|
        yield line if block_given?
      end
    end
  end

  def write_to_serial msg
    @serial_io.write "#{msg}#{EOM}"
  end

  def heartbeat
    write_to_serial "alive"
  end

end

serial = SerialPort.new SERIAL_PORT, SERIAL_RATE
serial.read_timeout = 100

PowerSwitch.new(serial).start