import sys
import serial
import time
# python 3!!!

ser = serial.Serial('/dev/ttyACM0', 9600)
# ser = serial.Serial('/dev/tty.usbmodem641', 9600)

def shift_byte():
    print ("los")
    while True:
            #line = ser.read(2)
            line = ser.readline()
            #ist = (line[0] | line[1] << 8) / 100
            #soll = (line[2] | line[3] << 8) / 100
            #print("ist: %f, soll: %f" % (ist, soll))
            #print("Temp: %.2f" % ist)
            print(line)

shift_byte()
