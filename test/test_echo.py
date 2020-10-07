
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

def echo(ser, number):
    cmd = [1, number]
    send_cmd(ser, cmd)
    returnvalue = ser.read(1)

    if returnvalue != b'':
        return int.from_bytes(returnvalue, byteorder='big')

def send_cmd(ser, cmd):
    for byte in cmd:
        ser.write(bytes([byte]))
        time.sleep(0.001)
# End of comms library



# Tests are below this line:
@pytest.fixture(scope='module')
def ser():
    ser = open_serial_connection('COM3')
    yield ser
    ser.close()


def test_echo(ser):
    random_number = random.randint(0, 255)
    assert echo(ser, random_number) == random_number
