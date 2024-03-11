from video_classifier import VideoClassifier
from video_classifier import VideoClassifierOptions

import socket
import numpy as np
import cv2

_MAX_RESULTS = 1
_NUM_THREADS = 4
_MODEL_PATH = 'model.tflite'
_MODEL_PATH = 'label_map.txt'
_DIM = 172

def main():

    options = VideoClassifierOptions(
        num_threads=_NUM_THREADS, max_results=_MAX_RESULTS)
    classifier = VideoClassifier(_MODEL_PATH, _MODEL_PATH, options)

    server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    server.bind('/tmp/visAlgo.sock')

    while True:
        server.listen(1)
        conn, _ = server.accept()
        datagram = conn.recv(1024)

        data = datagram.decode()

        img = np.frombuffer(data)
        img = cv2.resize(img, (_DIM, _DIM))

        pred = classifier.classify(img)

        msg = pred.encode()
        conn.send(msg)

if __name__ == "__main__":
    main()










