
import serial
import pytest
import random
import time

import byggern.comms as comms
import byggern.basic_cmd as  basic_cmd

def make_random_list(length, minimum_value, maximum_value):
    random_list = []
    for i in range(0, length):
        random_list.append(random.randint(minimum_value, maximum_value))
    return random_list

@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    yield ser
    ser.close()


def test_synchronize(ser):
    assert basic_cmd.synchronize(ser) == 0

def test_echo(ser):
    random_list = make_random_list(8, 0, 255)
    assert basic_cmd.echo(ser, random_list) == random_list

def test_synchronize_2(ser):
    assert basic_cmd.synchronize(ser) == 0