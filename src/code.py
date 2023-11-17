import serial

while True:
    ser = serial.Serial('/dev/cu.usbmodem139592901', 9600)
    print("HHHII")
    driveForward()


def driveForward(jetson_message):
    jetson_message= input('Put in address: ')

    address = jetson_message[2:8]
    speed = jetson_message[10:]

    ser.write(int((address+0+speed)& 0b01111111))
    print('This fucking worked')