import serial
import pytest
import random
import time

import byggern
from byggern import comms
from byggern import can_cmd
from byggern import mcp2515_cmd as mcp

@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    mcp.write_can_rx_flag(ser, 0)
    yield ser
    mcp.write_can_rx_flag(ser, 1)
    ser.close()


def test_can_pending_rx_buffer_0(ser):
    result = can_cmd.can_pending_rx_buffer(ser)
    assert result[1] == 3


def test_can_transmit(ser):
    msg_id = 10
    msg_data = [1, 2, 3, 4, 5]
    msg_len = len(msg_data)
    assert can_cmd.can_transmit(ser, msg_id, msg_len, msg_data) == 0


def test_can_pending_rx_buffer(ser):
    result = can_cmd.can_pending_rx_buffer(ser)
    assert result[1] != 3


def test_can_receive(ser):
    assert can_cmd.can_receive(ser) != -1


def test_can_valid_tx_buffer(ser):
    result = can_cmd.can_valid_tx_buffer(ser)
    print(result)
    assert result[1] != 3


def test_can_loopback(ser):
    # Set the device in loopback mode
    assert mcp.mcp2515_init(ser, mcp.MCP_MODE.LOOPBACK) == mcp.MCP_MODE.LOOPBACK
    mcp.write_can_rx_flag(ser, 0)

    # After reset, we should not have a pending receive buffer
    result = can_cmd.can_pending_rx_buffer(ser)
    assert result[1] == 3   # The number 3 indicates no pending buffer

    # Construct a message to send
    msg_id = 10
    msg_data = [1, 2, 3, 4, 5]
    msg_len = len(msg_data)

    # Try to send the message. It returns 0 on success
    assert can_cmd.can_transmit(ser, msg_id, msg_len, msg_data) == 0

    time.sleep(0.5)

    # Check to see if we have a pending receive buffer
    result = can_cmd.can_pending_rx_buffer(ser)
    assert result[1] != 3   # 0 or 1 is a valid buffer, 3 is not
    mcp.write_can_rx_flag(ser, 1)

    # Read out the message
    received_msg = can_cmd.can_receive(ser)

    transmit_msg = [msg_id, msg_len]
    transmit_msg.extend(msg_data)

    print("Transmitted:", transmit_msg)
    print("Received   :", received_msg)
    assert received_msg == transmit_msg


def test_can_with_node_2(ser):
    """ Attempts to talk to node 2 """
    mcp.write_can_rx_flag(ser, 1)
    assert mcp.mcp2515_init(ser, mcp.MCP_MODE.NORMAL) == mcp.MCP_MODE.NORMAL

    value = random.randint(-40000, 40000)
    value_bytes = list(value.to_bytes(4, byteorder='big', signed=True))

    print("Sending", value, value_bytes)

    time.sleep(1)
    byggern.can_cmd.can_transmit(
        ser,
        msg_id=1100,
        msg_len=len(value_bytes),
        msg_data=value_bytes
    )

    read_value = byggern.comms.read_signed_32_from_node2(ser)

    print("Received:", read_value)
    assert read_value == value
