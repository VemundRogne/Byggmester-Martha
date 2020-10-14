import serial
import pytest
import random
import time

import comms
import mcp2515_cmd as cmd


@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    yield ser
    ser.close()


def test_mcp2515_init_normal(ser):
    mode_id = 0b000 #normal mode
    assert cmd.mcp2515_init(ser, mode_id) != -1

    canctrl_adr = 0x0F
    assert cmd.mcp2515_read(ser, canctrl_adr, 1) >> 5 == mode_id

    # I've copied these expected values from the MCP_configure_bit_timing
    # function. This verifies that the init actually managed to write
    # the values as it tries to.
    Expected_CNF1 = (0x3F) | (0x00)
    Expected_CNF2 = (0x01) | (0x06 << 3)
    Expected_CNF3 = (0x05)
    CNF1_addr = 0x2A
    CNF2_addr = 0x29
    CNF3_addr = 0x28
    read_CNF1 = cmd.mcp2515_read(ser, CNF1_addr, 1)
    read_CNF2 = cmd.mcp2515_read(ser, CNF2_addr, 1)
    read_CNF3 = cmd.mcp2515_read(ser, CNF3_addr, 1)

    assert read_CNF1 == Expected_CNF1
    assert read_CNF2 == Expected_CNF2
    assert read_CNF3 == Expected_CNF3
