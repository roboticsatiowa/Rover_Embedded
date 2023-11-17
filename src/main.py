import serial

baud = 9600
ser = serial.Serial('/dev/ttyUSB0', baud)

while True:
    print("HEFSDGHJTREWQW")

def driveForward(jetson_message):
    jetson_message= input('Put in address: ')

    address = jetson_message[2:8]
    direction = jetson_message[8]
    enable = jeston_message[9]
    speed = jetson_message[10:]

    #if (address == 0001):
    #set to left port

    if (speed == 0):
        stop()

    elif (direction == 0):
        speed = -speed
    
    ST.motor(motorL,speed)






ser.close()