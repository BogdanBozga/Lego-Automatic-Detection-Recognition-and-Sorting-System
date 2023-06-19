import os
import cv2
import numpy as np
import sys
import importlib.util
import tflite_runtime.interpreter as tflite
import datetime




def generate_name():
    now = datetime.datetime.now()
    date_string = now.strftime("%Y:%m:%d_%H:%M:%S:")
    date_string += str(now.microsecond//(10**4))
    print(date_string)




# detection[0] - object_name
# detection[1] - scores
# detection[2] - xmin
# detection[3] - ymin
# detection[4] - xmax
# detection[5] - ymax
def check_and_send(detections):
    for detection in detections:
        if detection[5] > 500:
            print(f'Sending {detection[0]} {detection[5]}')
            '''
            send detection[0] , detection[5]
            '''


def detect(interpreter, image_path,labels,min_conf_threshold,results_path):
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]

    image = cv2.imread(image_path)
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image_resized = cv2.resize(image_rgb, (width, height))
    input_data = np.expand_dims(image_resized, axis=0)

    input_data = np.float32(input_data)/255

    interpreter.set_tensor(input_details[0]['index'],input_data)
    interpreter.invoke()

    boxes = interpreter.get_tensor(output_details[1]['index'])[0] # Bounding box coordinates of detected objects
    classes = interpreter.get_tensor(output_details[3]['index'])[0] # Class index of detected objects
    scores = interpreter.get_tensor(output_details[0]['index'])[0] # Confidence of detected objects

    detections = []

    for i in range(len(scores)):
        if ((scores[i] > min_conf_threshold) and (scores[i] <= 1.0)):
            ymin = int(max(1,(boxes[i][0] * height)))
            xmin = int(max(1,(boxes[i][1] * width )))
            ymax = int(min(height,(boxes[i][2] * height)))
            xmax = int(min(width ,(boxes[i][3] * width )))
            cv2.rectangle(image, (xmin,ymin), (xmax,ymax), (255, 255, 0), 2)
            object_name = labels[int(classes[i])]
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



def main():
    image_path = "image/image.jpg"
    min_conf_threshold = 0.5
    my_model_path = "model/detect.tflite"
    label_path = "model/label.txt"
    results_path = "results/"
    labels = []

    with open(label_path, 'r') as file:
        # Read each line, strip any trailing/leading whitespace (such as newlines), and add it to a list
        labels = file.readlines()

    interpreter = tflite.Interpreter(model_path=my_model_path)
    interpreter.allocate_tensors()

    while True:
        try:
            print("Taking photo...")
            os.system(f"raspistill -o {image_path}")
            
        except Exception as e:
            print("Error with taking photo")
            print(e)
            sys.exit()
        try:
            detections = detect(interpreter, image_path,labels,min_conf_threshold,results_path)
            check_and_send(detections)
        except Exception as  e:
            print("Error with AI model")
            print(e)
            sys.exit()

main()
