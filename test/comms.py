import serial
import pytest
import random
import time

class ReadException(Exception):
    """ Exception that is raised when we didn't get what we wanted """


def open_serial_connection(com_port):
    ser = serial.Serial()
    ser.port = com_port
    ser.baudrate = 9600
    ser.timeout = 1
    ser.open()
    return ser

def send_cmd(ser, cmd):
    #print("CMD: {}, len: {}".format(cmd, len(cmd)))
    for byte in cmd:
        ser.write(bytes([byte]))
        time.sleep(0.001)


def receive_n_bytes(ser, n):
    """ Function to receive n-bytes. Raises exception if that length was not read """
    read_bytes = ser.read(n)

    # If we actually got anything
    if read_bytes != b'':
        if len(read_bytes) == n:
            return read_bytes
        raise ReadException
    raise ReadException


def read_signed_32_from_node2(ser):
    returncode = receive_n_bytes(ser, 4)
    return int.from_bytes(returncode, byteorder='big', signed=True)