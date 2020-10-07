import serial
import pytest
import random
import time

def open_serial_connection(com_port):
    ser = serial.Serial()
    ser.port = com_port
    ser.baudrate = 9600
    ser.timeout = 5
    ser.open()
    return ser

def send_cmd(ser, cmd):
    for byte in cmd:
        ser.write(bytes([byte]))
        time.sleep(0.001)