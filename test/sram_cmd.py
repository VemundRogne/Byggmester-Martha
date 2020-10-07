import serial
import pytest
import random
import time

import comms

def sram_write(ser, address, value):
    cmd = [1, 0]
    cmd.extend(list(address.to_bytes(2, byteorder='big')))
    cmd.append(value)
    cmd.extend([0,0,0,0,0,0])

    returncode = ser.read(1)

    if returncode is not b'':
        return int.from_bytes(returncode, byteorder='big')
    return -1

def sram_read(ser, address):
    cmd = [1, 1]
    cmd.extend(list(address.to_bytes(2, byteorder='big')))
    cmd.extend([0,0,0,0,0,0,0])

    returncode = ser.read(1)

    if returncode is not b'':
        return int.from_bytes(returncode, byteorder='big')
    return -1




