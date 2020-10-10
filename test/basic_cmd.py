import serial
import pytest
import random
import time

import comms

def echo(ser, number_list):
    cmd = [0, 1]
    cmd.extend(number_list)
    comms.send_cmd(ser, cmd)
    returnvalue = ser.read(8)

    if returnvalue != b'':
        return list(returnvalue)


def synchronize(ser):
    """ Tries to sync sender and receiver.
        
        Returns 0 on success
        Returns 1 on failiure
    """
    _timeout = ser.timeout
    ser.timeout = 0.1
    cmd = [0]
    for i in range(0, 100):
        comms.send_cmd(ser, cmd)
        returnvalue = ser.read(1)

        if returnvalue != b'':
            if int.from_bytes(returnvalue, byteorder='big') == 1:
                ser.timeout = _timeout
                return 0
    ser.timeout = _timeout
    return 1