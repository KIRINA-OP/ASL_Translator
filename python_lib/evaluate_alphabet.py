from tflite_runtime.interpreter import Interpreter
import numpy as np
import cv2
import time
import sys
import timeit
import visSocket

with open('label_map_ASL.txt', 'r') as f:
      lines = f.readlines()
      label_list = [line.replace('\n', '') for line in lines]

interpreter = Interpreter(
model_path='ASL_alphabet.tflite', num_threads=4)
interpreter.allocate_tensors()
output = interpreter.get_output_details()[0]
input = interpreter.get_input_details()[0]

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 128)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 128)

sock = visSocket.VisSocketAlgo(visSocket.APP_SOCKET_PATH, visSocket.ALGO_SOCKET_PATH)
sock.init()
while cap.isOpened():
    start = timeit.timeit()
    success, image = cap.read()
    if not success:
      sys.exit(
          'ERROR: Unable to read from webcam. Please verify your webcam settings.'
      )
    
    frame_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    frame = cv2.resize(frame_rgb, (28, 28))
    frame = np.expand_dims(frame, 0)
    frame = np.expand_dims(frame, 3)

    frame = np.float32(frame / 255)

    interpreter.set_tensor(input['index'], frame)
    interpreter.invoke()
    prediction = label_list[np.argmax(interpreter.get_tensor(output['index']))]
    end = timeit.timeit()
    print(f"time {end - start}")
    print(prediction)
    cv2.imwrite(f"test_image/{prediction}.png", image)
    sock.deliver(prediction)
    time.sleep(0.02)