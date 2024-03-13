import tensorflow as tf
import numpy as np
import cv2
import time
import sys

with open('label_map_ASL.txt', 'r') as f:
      lines = f.readlines()
      label_list = [line.replace('\n', '') for line in lines]

model = tf.keras.models.load_model('model')

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 32)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 32)

while cap.isOpened():
    success, image = cap.read()
    if not success:
      sys.exit(
          'ERROR: Unable to read from webcam. Please verify your webcam settings.'
      )

    frame_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    frame = cv2.resize(frame_rgb, (28, 28))
    frame = np.expand_dims(frame, 0)
    frame = np.expand_dims(frame, 3)
    result = model.predict(frame)
    try:
        prediction = label_list[list(result[0]).index(1)]
        print(prediction)
    except:
        prediction = ''
    time.sleep(1)
