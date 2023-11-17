import serial
import time

port = 'dev/tty/1'  # Change to desired port
baud = 9600

    
try:
    self.ser = serial.Serial(port=port, baudrate=baud, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=1)
except Exception as e:
    print(f"Error opening serial port: {e}")
    raise

def lerp(self, x, in_min, in_max, out_min, out_max):
    """Interpolate the given value from one range to another.

    Args:
        x (int or float): The input value to interpolate.
        in_min (int or float): The minimum value of the input range.
        in_max (int or float): The maximum value of the input range.
        out_min (int or float): The minimum value of the output range.
        out_max (int or float): The maximum value of the output range.

    Returns:
        int: The interpolated value.
    """
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

def motorA(self, speed):
    """Set the speed of motor A.

    Args:
        speed (int): The desired speed, between -100 and 100 inclusive.

    Raises:
        serial.SerialException: If an error occurs while writing to the serial port
        """
    try:
        output = self.lerp(speed, -100, 100, 1, 128)
        self.ser.write(output.to_bytes(1,'little'))
    except serial.SerialException as e:
        print(f"Error writing to serial port: {e}")
        raise

def motorB(self, speed):
    """Set the speed of motor B.

    Args:
        speed (int): The desired speed, between -100 and 100 inclusive.

    Raises:
        serial.SerialException: If an error occurs while writing to the serial port.
        """
    try:
        output = self.lerp(speed, -100, 100, 128, 256)
        self.ser.write(output.to_bytes(1,'little'))
    except serial.SerialException as e:
        print(f"Error writing to serial port: {e}")
        raise

def if __name__ == "__main__":
    start_time = time.time()
    while True:
        while (time.time() - start_time) < 2:
            motorA(25)
            motorB(25)
        while (time.time() - start_time) < 4:
            motorA(-25)
            motorB(-25)
        start_time = time.time()





ser.close()
