import os
import cv2
import numpy as np
import sys
import importlib.util
import tflite_runtime.interpreter as tflite
# from tensorflow.lite.python.interpreter import Interpreter
import datetime
import serial
import time



def generate_name():
    now = datetime.datetime.now()
    date_string = now.strftime("%Y:%m:%d_%H:%M:%S:")
    date_string += str(now.microsecond//(10**4))
    return date_string





# detection[0] - object_name
# detection[1] - scores
# detection[2] - xmin
# detection[3] - ymin
# detection[4] - xmax
# detection[5] - ymax
def check_and_send(detections,ser):
    for detection in detections:
        if detection[5] > 500:
            print(f'Sending {str(detection[0]).strip()} {str(detection[5]).strip()}')
            '''
            send detection[0] , detection[5]
            '''
            message = f"{detection[0]}:{detection[5]}"

            ser.write(message.encode('utf-8') )





def detect(interpreter, image_path,labels,min_conf_threshold,results_path,ser):
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]

    float_input = (input_details[0]['dtype'] == np.float32)
    print(float_input)
    input_mean = 127.5
    input_std = 127.5

    if True:

        # Load image and resize to expected shape [1xHxWx3]
        image = cv2.imread(image_path)
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        imH, imW, _ = image.shape
        # print(f"img {imH} {imW}, model {height,width}")
        image_resized = cv2.resize(image_rgb, (width, height))
        input_data = np.expand_dims(image_resized, axis=0)

        # Normalize pixel values if using a floating model (i.e. if model is non-quantized)
        if float_input:
            input_data = (np.float32(input_data) - input_mean) / input_std

        # Perform the actual detection by running the model with the image as input
        interpreter.set_tensor(input_details[0]['index'],input_data)
        interpreter.invoke()

        # Retrieve detection results
        boxes = interpreter.get_tensor(output_details[1]['index'])[0] # Bounding box coordinates of detected objects
        classes = interpreter.get_tensor(output_details[3]['index'])[0] # Class index of detected objects
        scores = interpreter.get_tensor(output_details[0]['index'])[0] # Confidence of detected objects

        detections = []

        # Loop over all detections and draw detection box if confidence is above minimum threshold
        for i in range(len(scores)):
            if ((scores[i] > min_conf_threshold) and (scores[i] <= 1.0)):

                # Get bounding box coordinates and draw box
                # Interpreter can return coordinates that are outside of image dimensions, need to force them to be within image using max() and min()
                ymin = int(max(1,(boxes[i][0] * imH)))
                xmin = int(max(1,(boxes[i][1] * imW)))
                ymax = int(min(imH,(boxes[i][2] * imH)))
                xmax = int(min(imW,(boxes[i][3] * imW)))

                cv2.rectangle(image, (xmin,ymin), (xmax,ymax), (10, 255, 0), 2)

                # Draw label
                object_name = labels[int(classes[i])] # Look up object name from "labels" array using class index
                label = '%s: %d%%' % (object_name, int(scores[i]*100)) # Example: 'person: 72%'
                labelSize, baseLine = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.7, 2) # Get font size
                label_ymin = max(ymin, labelSize[1] + 10) # Make sure not to draw label too close to top of window
                cv2.rectangle(image, (xmin, label_ymin-labelSize[1]-10), (xmin+labelSize[0], label_ymin+baseLine-10), (255, 255, 255), cv2.FILLED) # Draw white box to put label text in
                cv2.putText(image, label, (xmin, label_ymin-7), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 0), 2) # Draw label text
                
                detections.append([object_name, scores[i], xmin, ymin, xmax, ymax])
        entry_name = generate_name()
        text_name = entry_name +'.txt'
        image_name = entry_name +'.jpg'
        txt_savepath = os.path.join(results_path, text_name)
        image_savepath = os.path.join(results_path, image_name)
        with open(txt_savepath,'w') as f:
            for detection in detections:
                f.write(f'{detection[0]} {round(detection[1], 3)} {detection[2]} {detection[3]} {detection[4]} {detection[5]}\n')
        cv2.imwrite(image_savepath, image)

    return detections











    # input_details = interpreter.get_input_details()
    # output_details = interpreter.get_output_details()
    # height = input_details[0]['shape'][1]
    # width = input_details[0]['shape'][2]

    # image = cv2.imread(image_path)
    # image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    # image_resized = cv2.resize(image_rgb, (width, height))
    # input_data = np.expand_dims(image_resized, axis=0)
    # float_input = (input_details[0]['dtype'] == np.float32)
    # input_mean = 127.5
    # input_std = 127.5
    # if float_input:
    #   input_data = (np.float32(input_data) - input_mean) / input_std

    # interpreter.set_tensor(input_details[0]['index'],input_data)
    # interpreter.invoke()

    # boxes = interpreter.get_tensor(output_details[1]['index'])[0] # Bounding box coordinates of detected objects
    # classes = interpreter.get_tensor(output_details[3]['index'])[0] # Class index of detected objects
    # scores = interpreter.get_tensor(output_details[0]['index'])[0] # Confidence of detected objects

    # detections = []

    # for i in range(len(scores)):
    #     if ((scores[i] > min_conf_threshold) and (scores[i] <= 1.0)):
    #         ymin = int(max(1,(boxes[i][0] * height)))
    #         xmin = int(max(1,(boxes[i][1] * width )))
    #         ymax = int(min(height,(boxes[i][2] * height)))
    #         xmax = int(min(width ,(boxes[i][3] * width )))
    #         cv2.rectangle(image, (xmin,ymin), (xmax,ymax), (255, 255, 0), 2)
    #         object_name = labels[int(classes[i])]
    #         detections.append([object_name, scores[i], xmin, ymin, xmax, ymax])

    # print("Number of objects detected:",len(detections))
    # if len(detections) > 0:
    #     entry_name = generate_name()
    #     print(entry_name)
    #     text_name = entry_name +'.txt'
    #     image_name = entry_name +'.jpg'
    #     txt_savepath = os.path.join(results_path, text_name)
    #     image_savepath = os.path.join(results_path, image_name)



    #     with open(txt_savepath,'w') as f:
    #         for detection in detections:
    #             f.write(f'{detection[0]} {round(detection[1], 3)} {detection[2]} {detection[3]} {detection[4]} {detection[5]}\n')
    #     cv2.imwrite(image_savepath, image)
    #     check_and_send(detections,ser)



def main():
    image_path = "image/image.jpg"
    min_conf_threshold = 0.5
    my_model_path = "model/detect.tflite"
    label_path = "model/label.txt"
    results_path = "results/"
    labels = []



    ser = serial.Serial('/dev/ttyACM0',115200,timeout=1.0)
    time.sleep(3)

    ser.reset_input_buffer()
    print("Serial OK")


    with open(label_path, 'r') as file:
        # Read each line, strip any trailing/leading whitespace (such as newlines), and add it to a list
        labels = file.readlines()


        message = "labels,"
        for label in labels:
            message = message + label + ','
        ser.write(message.encode('utf-8') )


    interpreter = tflite.Interpreter(model_path=my_model_path)
    interpreter.allocate_tensors()
    try:
        while True:
            try:
                print("Taking photo...")
                os.system(f"raspistill -o {image_path} -w 640 -h 640 -t 600 -rot 90")
                
            except Exception as e:
                print("Error with taking photo")
                print(e)
                sys.exit()
            try:
                print("Start detection...")
                detections = detect(interpreter, image_path,labels,min_conf_threshold,results_path,ser)
                print("Detestion done...")
                check_and_send(detections,ser)
            except Exception as  e:
                print("Error with AI model")
                print(e)
                sys.exit()
    except KeyboardInterrupt:
        ser.close()
main()
