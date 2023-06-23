import cv2
import numpy as np
import sys
import importlib.util
import tflite_runtime.interpreter as tflite
# from tensorflow.lite.python.interpreter import Interpreter
import datetime
import serial
import time


# s
ser = serial.Serial('/dev/ttyACM0',115200,timeout=1.0)
time.sleep(3)

ser.reset_input_buffer()
print("Serial OK")

message = 'slope_lego,L_beam_lego,pin_lego,plates_long_lego,step_lego,wheel_lego,END\n'
ser.write(message.encode('utf-8') )
print("Labels send",message)

# while True:
#         line = ser.readline().decode('utf-8').rstrip()
#         print(line)
#         time.sleep(1)

# line = ser.readline()
# dec= line.decode('utf-8').rstrip()
# print(dec)


while True:
    opt = input("Chose:")
    if opt == '1':
        message = 'pin_lego:300:END\n'
    elif opt == '2':
        message = 'step_lego:300:END\n'
    else:
        message = 'wheel_lego:300:END\n'
    ser.write(message.encode('utf-8') )
    print("Option send",message)
    # line = ser.readline()
    # print(line)
    # dec= line.decode('utf-8').rstrip()
    # print(dec)



