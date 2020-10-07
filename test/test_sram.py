import serial
import pytest
import random
import time

import comms
import sram_cmd


@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    yield ser
    ser.close()


def test_sram(ser):
    random_val = random.randint(0, 255)
    sram_cmd.sram_write(ser, 1, random_val)
    read_val = sram_cmd.sram_read(ser, 1)
    assert read_val == random_val

