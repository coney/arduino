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
    @shutting_down = false
  end

  def start
    read_form_serial do |line|
      if line == "hello"
        ap "replay heartbeat"
        heartbeat
      elsif line == "goodbye"
        unless @shutting_down
          ap "shutdown now"
          `shutdown -hH now`
          @shutting_down = true
        end
        wait_for_shutting_down
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

  def wait_for_shutting_down
    write_to_serial "wait"
  end

end

Daemon.daemonize "/var/run/power-switch.pid", "/var/log/power-switch.log"

serial = SerialPort.new SERIAL_PORT, SERIAL_RATE
serial.read_timeout = 100

PowerSwitch.new(serial).start