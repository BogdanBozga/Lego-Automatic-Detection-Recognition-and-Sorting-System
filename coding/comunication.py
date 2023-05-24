#!/usr/bin/env python3

import serial
import time

ser = serial.Serial('/dev/ttyACM0',115200,timeout=1.0)
time.sleep(3)

ser.reset_input_buffer()
print("Serial OK")

try:
    while True:
        
        user_input = input("Inroduce angle  ").lower()

        ser.write(user_input.encode('utf-8') )

        # if ser.in_waiting > 0:
            # line = ser.readline().decode('utf-8').rstrip()
            # print(line)
   
except KeyboardInterrupt:
    ser.close()
    print("Connection closed")