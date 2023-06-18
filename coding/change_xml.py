import xml.etree.ElementTree as ET
import glob
import os

files = glob.glob(os.getcwd()+'/*.xml')

for file in files:
    tree = ET.parse(file)  # parse XML file
    root = tree.getroot()  # get root element

    # Iterate over all elements in the XML file
    for elem in root.iter():
        if elem.text == 'Slope':
            elem.text = 'slope_lego'  # replace text

        if elem.text == 'Wheel':
            elem.text = 'wheel_lego'  # replace text

        if elem.text == 'Plate Long':
            elem.text = 'plates_long_lego'  # replace text

        if elem.text == 'L Beam':
            elem.text = 'L_beam_lego'  # replace text

        if elem.text == 'Step':
            elem.text = 'step_lego'  # replace text

        if elem.text == 'Pin':
            elem.text = 'pin_lego'  # replace text

    # Save changes back to the XML file
    tree.write(file)

