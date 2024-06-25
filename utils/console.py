import os
import time
import logging
# pip installed modules
from serial import *

class CanBox:

    def __init__(self: None) -> None:
        self.port_name = ""
        self.serial_port = Serial()
        self.baud = 115200
        self.bytesize = 8
        self.parity = 'N'
        self.stopbits = 1
        self.timeout = 1
        self.xonxoff = 0
        self.rtscts = 0

    def disconnect(self, selected_port: str) -> None:
        """ Method that defines the disconnection from the DFM board.
            keyword arguments:
                selected_port: a string (no default)
            Return: none """
        self.port_name = selected_port
        self.serial_port.flushOutput()
        try:    
            self.serial_port.close()  # close port
            print(f" {self.port_name} closed")
        except Exception as e:
            print(f" (disconnect): {e}")

    def connect(self, selected_port: str) -> None:
        """ Method that creates the connection to the DFM board. A valid connection can take place only if the selected port 
            exists and is not used, and the DFM board is identified.
            keyword arguments:
                selected_port: a string (no default)
            Return: None """
        self.port_name = selected_port
        try:
            self.serial_port = Serial(port=self.port_name,
                                baudrate=self.baud,
                                bytesize=self.bytesize,
                                parity=self.parity,
                                stopbits=self.stopbits,
                                timeout=self.timeout,
                                xonxoff=self.xonxoff,
                                rtscts=self.rtscts)  # open serial port
            print(f"Connected to {self.port_name}")
        except Exception as e:
            print(f" (connect): {e}")

    def send_command(self, command: list) -> None:
        """ Method used to send commands to the can-box.
            keyword arguments:
                command: a list (no default)
            Return: None """
        try:
            print(f"data_bytes: {command}")
            data = bytes(command)
            print(f"data: {data}")
            self.serial_port.write(data)
            #self.serial_port.write(b'\n')
            while self.serial_port.inWaiting():
                line = self.serial_port.readline()
                print(f"Received from can-box: {line}")
        except Exception as e:
            print(f" (send_command): {e}")

def main():
    print("Starting script")
    board = CanBox()
    print("Try to open serial port")
    board.connect("/dev/ttyACM2")
    #data_bytes = [0x34, 0x31, 0x65, 0x31]
    #board.send_command(data_bytes)
    #data_bytes = [0x35, 0x31, 0x65, 0x66, 0x67]
    #board.send_command(data_bytes)
    data_bytes = (0x04, 0x1e, 0x21, 0x01)
    board.send_command(data_bytes)
    board.disconnect("/dev/ttyACM2")

main()
