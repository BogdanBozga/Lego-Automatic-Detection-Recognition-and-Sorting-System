from picamera import PiCamera
from time import sleep
import subprocess



import cv2
import numpy as np


nr = 0
with open("data.txt","r") as nr_file:
    nr = int(nr_file.readline())

directory = "new_pics"+str(nr//25)
subprocess.Popen("mkdir "+directory,shell=True)
camera = PiCamera()  
camera.brightness = 50
camera.contrast = 0
print("start taking pictures")
for i in range(45,55,1):
    #input("Next "+str(i)+":")
    sleep(0)
    img_name = "pic"+str(i)
    img_path = "/home/bogdan/Desktop/"+directory+"/"+img_name+".jpg"
    camera.capture(img_path)
    
    print("picture "+str(i)+" tooked")
     
print("All pictures toked") 
with open("data.txt","w") as nr_file:
    nr_file.write(str(nr+25))


subprocess.Popen("tar -zcf "+directory+".tar "+directory,shell=True)

 
#subprocess.Popen("mv "+directory+".tar picttar/")  
print("all done ") 
#camera.stop_preview()   
