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
    #print("Returncode:", returncode)
    if returncode != b'':
        # Not a zero indicates no message received. Return 1 to indicate failiure
        if int.from_bytes(returncode, byteorder='big') != 0:
    #        print("No message received")
            return 1

    raw_data = ser.read(11)
    #print("Raw_data:", raw_data, list(raw_data))

    if raw_data != b'':
        # Convert the data into a nicer format
        # The main thing is to combine two bytes into the ID,
        # and throw away data bytes that are not actually valid. (We will always
        # get 11 bytes, but not all are valid data because the data.len matters)
        ID = int.from_bytes(raw_data[0:2], byteorder='big')
        Length = int.from_bytes(raw_data[2:3], byteorder='big')
        data = list(raw_data[3:4+Length-1])

        print("Received a message width ID={}, len={}, data={}".format(
            ID, Length, data
        ))

        returnlist = [ID, Length]
        returnlist.extend(data)
        return returnlist

    return 1
