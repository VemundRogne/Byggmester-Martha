import serial
import pytest
import random
import time

import comms

def mcp2515_init(ser, mode):
    cmd = [2, 1, mode, 0, 0, 0, 0, 0, 0, 0]
    comms.send_cmd(ser, cmd)

    returncode = ser.read(1)

    if returncode != b'':
        return int.from_bytes(returncode, byteorder='big')
    return -1

def mcp2515_read(ser, address, data_length):
    cmd = [2, 3, address, data_length, 0, 0, 0, 0, 0, 0]
    comms.send_cmd(ser, cmd)

    returncode = ser.read(data_length)

    if returncode != b'':
        return int.from_bytes(returncode, byteorder='big')
    return -1

def mcp2515_write(ser, address, data_length):
    cmd = [2, 2, address, data_length, 0, 0, 0, 0, 0, 0]
    comms.send_cmd(ser, cmd)

    returncode = ser.read(1)

    if returncode != b'':
        return int.from_bytes(returncode, byteorder='big')
    return -1

