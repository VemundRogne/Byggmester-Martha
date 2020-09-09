# Pyserial: https://pythonhosted.org/pyserial/
import serial

def establish_connection(port):
    ser = serial.Serial()
    
    ser.port = port
    
    ser.baudrate = 9600
    ser.timeout = 3
    ser.stopbits = serial.STOPBITS_ONE
    
    ser.open()
    return ser


if __name__ == '__main__':
    ser = establish_connection('COM3')
    
    while True:
        print(ser.readline())