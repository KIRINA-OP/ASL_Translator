from tflite_runtime.interpreter import Interpreter
import numpy as np
import cv2
import time
import sys

import mediapipe as mp

CLASSIFICATION_MODEL = 'ASL_classifier.tflite'
LABEL_MAP = 'label_map_ASL_big.txt'

mphands = mp.solutions.hands
hands = mphands.Hands()

with open(LABEL_MAP, 'r') as f:
      lines = f.readlines()
      label_list = [line.replace('\n', '') for line in lines]

interpreter = Interpreter(
        model_path=CLASSIFICATION_MODEL, num_threads=4)
interpreter.allocate_tensors()
output = interpreter.get_output_details()[0]
input = interpreter.get_input_details()[0]

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 500)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 500)

while cap.isOpened():
    success, image = cap.read()
    if not success:
        sys.exit(
            'ERROR: Unable to read from webcam. Please verify your webcam settings.'
        )
    image = cv2.flip(image, 1)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    result = hands.process(image)
    hand_landmarks = result.multi_hand_landmarks

    h, w, c = image.shape

    if hand_landmarks:
        for handLMs in hand_landmarks:
            x_max = 0
            y_max = 0
            x_min = w
            y_min = h
            for lm in handLMs.landmark:
                x, y = int(lm.x * w), int(lm.y * h)
                if x > x_max:
                    x_max = x
                if x < x_min:
                    x_min = x
                if y > y_max:
                    y_max = y
                if y < y_min:
                    y_min = y

    cropped = image[y_min-10:y_max+10, x_min-10:x_max+10, :]

    frame = cv2.resize(cropped, (224, 224))
    frame = np.expand_dims(frame, 0)

    frame = np.float32(frame / 255)

    interpreter.set_tensor(input['index'], frame)
    interpreter.invoke()
    prediction = label_list[np.argmax(interpreter.get_tensor(output['index']))]
    print(prediction)
    time.sleep(2)
