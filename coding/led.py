# from gpiozero import LED #imports the LED functions from gpiozero library
from time import sleep
# from time import sleep #imports the sleep function from time library

# led = LED(17) #declared gpio pin 17 for LED and store it in led

# # while True:

# #initiated the while loop until the conditions are true

# led.on() #turn on led
# print("on")
# sleep(10) #generate a delay of 2 seconds

# led.off() #turn off led
# print("off")
# sleep(2) #generate a delay of 2 seconds

pin = 4

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(pin,GPIO.OUT)
GPIO.output(pin, True)
sleep(10)
GPIO.output(pin, False)
GPIO.cleanup()