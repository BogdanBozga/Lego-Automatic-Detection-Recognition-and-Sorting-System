# This is my Theasis Project

An automatic sorting system that takes photos, recognizes, and moves the detected
objects to the predetermined storage place in order to sort them. A conveyer belt is
use to move the object to the area for photographing, and after that, to the specific
storage area. The conveyer belt is put in motion by Nema 17 stepper motors that
are controlled by an Arduino Uno via a CNC Shield V3, and A4988 Drivers. Photos
are taken by a NoIR V2 camera that is directly connected to a Raspberry Pi V4.
The image classification model is run on Raspberry Pi.

