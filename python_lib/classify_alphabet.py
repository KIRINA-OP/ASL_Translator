import cv2
import sys
import time
from video_classifier import VideoClassifier
from video_classifier import VideoClassifierOptions

_MODEL_PATH = 'ASL_alphabet.tflite'
_LABEL_PATH = 'label_map_ASL.txt'
_DIM = 172

options = VideoClassifierOptions(
    num_threads=4, max_results=1)
classifier = VideoClassifier(_MODEL_PATH, _LABEL_PATH, options)

# Start capturing video input from the camera
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, _DIM)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, _DIM)

while cap.isOpened():
    success, image = cap.read()
    if not success:
      sys.exit(
          'ERROR: Unable to read from webcam. Please verify your webcam settings.'
      )

    frame_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

      # Feed the frame to the video classification model.
    result = classifier.classify(frame_rgb)
    print(result)
    time.sleep(0.1)
