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


def test_mcp2515_init(ser):
    mode_id = 0b011 #Listen mode
    cmd.mcp2515_init(ser, mode_id)

    canctrl_adr = 0x0F
    assert cmd.mcp2515_read(ser, canctrl_adr, 1) >> 5 == mode_id


def test_mcp2515_loopback(ser):
    mode_id = 0b010    #Init to loopback mode
    cmd.mcp2515_init(ser, mode_id)

    test_data = [1, 1, 2, 3, 5, 8]
    #cmd.mcp2525_write(ser, )
