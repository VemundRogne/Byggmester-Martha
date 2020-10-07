
import serial
import pytest
import random
import time

# This is the communication library
def open_serial_connection(com_port):
    ser = serial.Serial()
    ser.port = com_port
    ser.baudrate = 9600
    ser.timoeut = 5
    ser.open()
    return ser

def echo(ser, number_list):
    cmd = [1]
    cmd.extend(number_list)
    send_cmd(ser, cmd)
    returnvalue = ser.read(9)

    if returnvalue != b'':
        return list(returnvalue)


def send_cmd(ser, cmd):
    for byte in cmd:
        ser.write(bytes([byte]))
        time.sleep(0.001)
# End of comms library

def make_random_list(length, minimum_value, maximum_value):
    random_list = []
    for i in range(0, length):
        random_list.append(random.randint(minimum_value, maximum_value))
    return random_list

# Tests are below this line:
@pytest.fixture(scope='module')
def ser():
    ser = open_serial_connection('COM3')
    yield ser
    ser.close()


def test_echo(ser):
    random_list = make_random_list(9, 0, 255)
    assert echo(ser, random_list) == random_list
