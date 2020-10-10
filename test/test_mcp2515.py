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
