""" Runs some super-simple tests to check if the motor works """

import pytest
import time

import comms
import can_cmd
import basic_cmd

@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    basic_cmd.synchronize(ser)
    yield ser
    ser.close()


def regulator_set_p_gain(ser, p_gain):
    data = list(p_gain.to_bytes(2, byteorder='big'))
    can_cmd.can_transmit(ser, msg_id=901, msg_len=2, msg_data=data)


def regulator_read_output(ser):
    can_cmd.can_transmit(ser, msg_id=954, msg_len=0, msg_data=[])
    try:
        return comms.read_signed_32_from_node2(ser)
    except comms.ReadException:
        print("Trying to resync!")
        basic_cmd.synchronize(ser)


def motor_set_output(ser, direction, power):
    data = [direction]
    data.extend(list(power.to_bytes(2, byteorder='big')))
    can_cmd.can_transmit(ser, msg_id=1000, msg_len=3, msg_data=data)


def motor_read_encoder(ser):
    cmd = [4, 1]
    cmd.extend([0,0,0,0,0,0,0,0])

    comms.send_cmd(ser, cmd)

    returncode = ser.read(2)

    if returncode != b'':
        return int.from_bytes(returncode, byteorder='big', signed=True)


def test_motor_set_output(ser):
    motor_set_output(ser, 1, 1000)
    time.sleep(1)
    motor_set_output(ser, 0, 1000)
    time.sleep(1)
    motor_set_output(ser, 0, 0)


def test_motor_encoder(ser):
    motor_set_output(ser, 1, 1000)
    
    for i in range(0, 30):
        print(motor_read_encoder(ser))

    motor_set_output(ser, 1, 0)
    time.sleep(1)

    motor_set_output(ser, 0, 1000)
    for i in range(0, 30):
        print(motor_read_encoder(ser))
    motor_set_output(ser, 0, 0)
    time.sleep(1)


if __name__ == '__main__':
    ser = comms.open_serial_connection('COM3')
    while True:
        print(regulator_read_output(ser))
        time.sleep(0.1)
