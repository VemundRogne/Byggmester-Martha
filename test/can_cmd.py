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
    print(returncode)

    if returncode != b'':
        return list(returncode)
    return None

def can_pending_rx_buffer(ser):
    cmd = [3, 3]
    cmd.extend([0,0,0,0,0,0,0,0])
    comms.send_cmd(ser, cmd)
    returncode = ser.read(2)
    print(returncode, list(returncode))

    if returncode != b'':
        return list(returncode)
    return None
    

def can_transmit(ser, msg_id, msg_len, msg_data):
    cmd = [3, 0]
    cmd.extend(list(msg_id.to_bytes(2, byteorder='big')))
    cmd.append(msg_len)
    cmd.extend(msg_data)
    cmd_empty_len = 10 - (msg_len + 5)
    empty_vector = [0] * (cmd_empty_len)
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

    # Check the can_status.
    # The first byte returned will be the can_status. A 0 indicated that
    # we received a message, a 1 indicates that we did not
    returncode = ser.read(1)
    print("Returncode:", returncode)
    if returncode != b'':
        # Not a zero indicates no message received. Return 1 to indicate failiure
        if int.from_bytes(returncode, byteorder='big') != 0:
            print("No message received")
            return 1

    data = ser.read(11)
    print(data, list(data))

    if data != b'':
        return list(data)
    return 1
