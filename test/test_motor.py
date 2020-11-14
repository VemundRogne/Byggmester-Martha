""" Runs some super-simple tests to check if the motor works """

from enum import IntEnum

import pytest
import time

import byggern.comms as comms
import byggern.can_cmd as can_cmd
import byggern.basic_cmd as basic_cmd
import byggern.mcp2515_cmd as mcp2515_cmd

class regulator_variables(IntEnum):
    position = 952
    error = 953
    integral = 954
    output = 955


@pytest.fixture(scope='module')
def ser():
    ser = comms.open_serial_connection('COM3')
    basic_cmd.synchronize(ser)
    yield ser
    ser.close()


def regulator_set_p_gain(ser, p_gain):
    data = list(p_gain.to_bytes(2, byteorder='big'))
    can_cmd.can_transmit(ser, msg_id=901, msg_len=2, msg_data=data)


def regulator_set_setpoint(ser, setpoint):
    data = list(setpoint.to_bytes(4, byteorder='big', signed=True))
    print(data)
    can_cmd.can_transmit(ser, msg_id=903, msg_len=4, msg_data=data)


def read_all_regulator_variables(ser):
    read_variables = []
    for variable in regulator_variables:
        read_value = node2_read_variable(ser, variable.value)

        if variable.name == "output":
            if read_value is not None:
                read_value = abs(read_value)>>2

        read_variables.append(read_value)
    return read_variables


def node2_read_variable(ser, variable_id):
    can_cmd.can_transmit(ser, msg_id=variable_id, msg_len=0, msg_data=[])
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
    motor_set_output(ser, 1, 1500)
    time.sleep(1)
    motor_set_output(ser, 0, 1500)
    time.sleep(5)
    motor_set_output(ser, 0, 0)


def get_diff(input_list):
    return [input_list[i+1] - input_list[i] for i in range(0, len(input_list) -1)]


def ensure_monotonic(input_list, direction, tolerance):
    """ Ensures a list is monotonic. """
    true_if_positive = lambda x: (x > 0)
    true_if_negative = lambda x: (x < 0)

    diff = get_diff(input_list)

    if direction > 0:
        n_positive = len(list(filter(true_if_positive, diff)))
        assert n_positive/len(diff) >= tolerance
    else:
        n_negative = len(list(filter(true_if_negative, diff)))
        assert n_negative/len(diff) >= tolerance


def test_motor_encoder(ser):
    """ Verifies that the encoder works.

        Runs the motor both directions, and ensures that the encoder counts
        monotonically either up or down in those directions.    
    """
    motor_set_output(ser, 1, 1000)

    encoder = []
    for i in range(0, 30):
        encoder.append(motor_read_encoder(ser))
    print("Raw encoder readings:", encoder)
    ensure_monotonic(encoder, 1, 0.8)

    motor_set_output(ser, 1, 0)
    time.sleep(1)

    motor_set_output(ser, 0, 1000)

    encoder = []
    for i in range(0, 30):
        encoder.append(motor_read_encoder(ser))
    print("Raw encoder readings:", encoder)
    ensure_monotonic(encoder, -1, 0.8)

    motor_set_output(ser, 0, 0)
    time.sleep(1)


def test_regulator_init(ser):
    """ Tries to initialize the regulator

        If it works we should get a can mesage with ID 139, and the encoder
        value should be between 3000 and 4000
    """
    mcp2515_cmd.write_can_rx_flag(ser, 0)   # Disable node 1 can reception
    can_cmd.can_transmit(ser, 138, 1, [1])

    init_success = 0
    for i in range(0, 50):
        returncode = can_cmd.can_receive(ser)

        if returncode != 1:
            if returncode[0] == 139:
                init_success = 1
                break

        time.sleep(0.5)
    
    assert init_success == 1
    
    mcp2515_cmd.write_can_rx_flag(ser, 1)
    time.sleep(3)

    encoder_value = motor_read_encoder(ser)
    print("Encoder_value", motor_read_encoder(ser))

    assert encoder_value > 3000
    assert encoder_value < 4000


if __name__ == '__main__':
    ser = comms.open_serial_connection('COM3')
    basic_cmd.synchronize(ser)
    
    setpoint = 3000
    start_time = time.time()
    while True:
        print(read_all_regulator_variables(ser))
        time.sleep(1)

        #if ((time.time() - start_time) > 10):
        #    start_time = time.time()#

        #    if setpoint == 5000:
        #        setpoint = 3000
        #    else:
        #        setpoint = 5000
            
        #    print("Setting setpoint to", setpoint)
        #    regulator_set_setpoint(ser, setpoint)
