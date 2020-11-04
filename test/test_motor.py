""" Runs some super-simple tests to check if the motor works """

import pytest
import time

import comms
import can_cmd

@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    yield ser
    ser.close()


def motor_set_output(ser, direction, power):
    data = [direction]
    data.extend(list(power.to_bytes(2, byteorder='big')))
    can_cmd.can_transmit(ser, msg_id=1000, msg_len=3, msg_data=data)


def test_motor_set_output(ser):
    motor_set_output(ser, 1, 1000)
    time.sleep(1)
    motor_set_output(ser, 0, 1000)
    time.sleep(1)
    motor_set_output(ser, 0, 0)