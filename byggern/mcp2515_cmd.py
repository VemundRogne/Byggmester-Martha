import serial
import pytest
import random
import time
from enum import IntEnum

import byggern.comms as comms

class MCP_MODE(IntEnum):
    NORMAL      = 0b000
    SLEEP       = 0b001
    LOOPBACK    = 0b010


def mcp2515_init(ser, mode):
    if not isinstance(mode, MCP_MODE):
        raise TypeError('mode is not a valid MPC_MODE')

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


def mcp2515_rx_status(ser):
    cmd = [2, 4, 0, 0, 0, 0, 0, 0, 0, 0]
    comms.send_cmd(ser, cmd)

    rx_status = ser.read(1)

    if rx_status != b'':
        return int.from_bytes(rx_status, byteorder='big')
    return None

def mcp2515_read_status(ser):
    cmd = [2, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    comms.send_cmd(ser, cmd)

    read_status = ser.read(1)

    if read_status != b'':
        return int.from_bytes(read_status, byteorder='big')
    return None


def write_can_rx_flag(ser, value):
    """ This is a helper function to set the receive_can_on_interrupt_flag """
    cmd = [2, 8, value, 0, 0, 0, 0, 0, 0, 0]
    comms.send_cmd(ser, cmd)

    status = ser.read(1)

    if status != b'':
        status = int.from_bytes(status, byteorder='big')
        if status == 0:
            return 0
    raise Exception


if __name__ == '__main__':
    ser = comms.open_serial_connection('COM3')
    print(write_can_rx_flag(ser, 0))