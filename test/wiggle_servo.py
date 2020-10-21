import comms
import basic_cmd
import can_cmd
import mcp2515_cmd
import time


if __name__ == '__main__':
    ser = comms.open_serial_connection('COM3')
    basic_cmd.synchronize(ser)

    while True:
        for i in range(0, 255):
            can_cmd.can_transmit(ser, 50, 1, [i])
            time.sleep(0.01)
        
        for i in range(255, 0, -1):
            can_cmd.can_transmit(ser, 50, 1, [i])
            time.sleep(0.01)