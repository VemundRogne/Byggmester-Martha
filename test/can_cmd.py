import serial
import pytest
import random
import time

import comms

def can_valid_tx_buffer(ser):
    cmd = [3, 2]
    cmd.extend([0,0,0,0,0,0,0,0])

    comms.send_cmd(ser, cmd)

    returncode = ser.read(2)

    if returncode != b'':
        return list(returncode)
    return None



def can_transmit(ser, msg_id, msg_len, msg_data):
    cmd = [3, 0]
    cmd.append(msg_id)
    cmd.append(msg_len)
    cmd.extend(msg_data)
    cmd_empty_len = 10 - (msg_len + 5)
    empty_vector = [0] * (cmd_empty_len + 1)
    cmd.extend(empty_vector)

    comms.send_cmd(ser, cmd)
    returncode = ser.read(1)

    if returncode != b'':
        return int.from_bytes(returncode, byteorder='big')
    return -1

def can_receive(ser):
    cmd = [3, 1]
    cmd.extend([0,0,0,0,0,0,0,0])

    comms.send_cmd(ser, cmd)

    returncode = ser.read(11)

    if returncode != b'':
        return list(returncode)
    return None
