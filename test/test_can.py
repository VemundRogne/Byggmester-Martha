import serial
import pytest
import random
import time

import comms
import can_cmd
import mcp2515_cmd as mcp

@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    yield ser
    ser.close()


def test_can_transmit(ser):
    msg_id = 10
    msg_data = [1, 2, 3, 4, 5]
    msg_len = len(msg_data)
    assert can_cmd.can_transmit(ser, msg_id, msg_len, msg_data) == 0


def test_can_receive(ser):
    assert can_cmd.can_receive(ser) != -1

def test_can_valid_tx_buffer(ser):
    result = can_cmd.can_valid_tx_buffer(ser)
    print(result)
    assert result[1] != 3

def test_can(ser):
    assert mcp.mcp2515_init(ser, mcp.MCP_MODE.LOOPBACK) == mcp.MCP_MODE.LOOPBACK 
    msg_id = 10
    msg_data = [1, 2, 3, 4, 5]
    msg_len = len(msg_data)


    transmit_msg = [msg_id, msg_len]
    transmit_msg.extend(msg_data)

    assert can_cmd.can_transmit(ser, msg_id, msg_len, msg_data) == 0
    receive_msg = can_cmd.can_receive(ser)
    assert receive_msg == transmit_msg

