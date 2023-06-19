import os
import glob
import random
import shutil
import argparse

'''
This is a small script that the images into test, validate and test folders.
'''

parser = argparse.ArgumentParser(description="This script takes path from a directory that contains images and xml and splits that images is two directories.")
parser.add_argument('--images', type=str, required=True, help='Path to images')
args = parser.parse_args()

images_path = args.images
print(images_path)
path  = os.path.dirname(images_path)
if path:
    train_path =path+ '/train'
    validate_path =path+ '/validate'
    test_path =path+ '/test'
else:
    train_path = 'train'
    validate_path = 'validate'
    test_path = 'test'

os.makedirs(train_path, exist_ok=True)
os.makedirs(validate_path, exist_ok=True)
os.makedirs(test_path, exist_ok=True)


images = glob.glob(images_path + '/*.jpg')
number_images_train =len(images)*80//100 # 80% of the image go to the train

images_to_train = random.sample(images,number_images_train)

for image in images_to_train:
    file_name, _ = os.path.splitext(image)
    shutil.move(file_name+'.jpg',train_path)
    shutil.move(file_name+'.xml',train_path)


images = glob.glob(images_path + '/*.jpg')
number_images_validate =len(images)*50//100 # 50% of the remaining image go to the validating (10% of original size)
images_to_validate = random.sample(images,number_images_validate)
images_validate = glob.glob(images_path + '/*.jpg') # the images that remain
for image in images_to_validate:
    file_name, _ = os.path.splitext(image)
    shutil.move(file_name+'.jpg',validate_path)
    shutil.move(file_name+'.xml',validate_path)

images = glob.glob(images_path + '/*.jpg') # the remaining 10% of original to test
for image in images:
    file_name, _ = os.path.splitext(image)
    shutil.move(file_name+'.jpg',test_path)
    shutil.move(file_name+'.xml',test_path)
