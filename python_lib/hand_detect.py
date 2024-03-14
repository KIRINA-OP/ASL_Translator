import cv2 as cv
import mediapipe as mp
import time
 
 
class HandDetector():
    def __init__(self, mode=False,
                 maxNumHands=2,
                 modelComplexity=1,
                 minDetectionConfidence=0.5,
                 minTrackingConfidence=0.5):
        self.mode = mode
        self.maxNumHands = maxNumHands
        self.modelComplexity = modelComplexity
        self.minDetectionConfidence = minDetectionConfidence
        self.minTrackingConfidence = minTrackingConfidence
        #创建mediapipe的solutions.hands对象
        self.mpHands = mp.solutions.hands
        self.handsDetector = self.mpHands.Hands(self.mode, self.maxNumHands, self.modelComplexity, self.minDetectionConfidence, self.minTrackingConfidence)
        #创建mediapipe的绘画工具
        self.mpDrawUtils = mp.solutions.drawing_utils
 
    def findHands(self, img, drawOnImage=True):
        #mediapipe手部检测器需要输入图像格式为RGB
        #cv默认的格式是BGR，需要转换
        imgRGB = cv.cvtColor(img, cv.COLOR_BGR2RGB)
        #调用手部检测器的process方法进行检测
        self.results = self.handsDetector.process(imgRGB)
        #print(results.multi_hand_landmarks)
    
        #如果multi_hand_landmarks有值表示检测到了手
        if self.results.multi_hand_landmarks:
            #遍历每一只手的landmarks
            for handLandmarks in self.results.multi_hand_landmarks:
                if drawOnImage:
                    self.mpDrawUtils.draw_landmarks(img, handLandmarks, self.mpHands.HAND_CONNECTIONS)
        return img;
 
    #从结果中查询某只手的landmark list
    def findHandPositions(self, img, handID=0, drawOnImage=True):
        landmarkList = []
        if self.results.multi_hand_landmarks:
            handLandmarks = self.results.multi_hand_landmarks[handID]
            for id,landmark in enumerate(handLandmarks.landmark):
                #处理每一个landmark,将landmark里的X,Y（比例）转换为帧数据的XY坐标
                h,w,c = img.shape
                centerX,centerY = int(landmark.x * w), int(landmark.y * h)
                landmarkList.append([id, centerX, centerY])
                if (drawOnImage):
                    #将landmark绘制成圆
                    cv.circle(img, (centerX,centerY), 8, (0,255,0), cv.FILLED)
        return landmarkList
 
def DisplayFPS(img, preTime):
    curTime = time.time()
    if (curTime - preTime == 0):
        return curTime;
    fps = 1 / (curTime - preTime)
    cv.putText(img, "FPS:" + str(int(fps)), (10,70), cv.FONT_HERSHEY_PLAIN,
              3, (0,255,0), 3)
    return curTime
 
 
def main():
    video = cv.VideoCapture('../../SampleVideos/hand.mp4')
    #FPS显示
    preTime = 0
    handDetector = HandDetector()
    
    while True:
        ret,frame = video.read()
        if ret == False:
            break;
        frame = handDetector.findHands(frame)
        hand0Landmarks = handDetector.findHandPositions(frame)
        #if len(hand0Landmarks) != 0:
            #print(hand0Landmarks)
        preTime = DisplayFPS(frame, preTime)
        cv.imshow('Real Time Hand Detection', frame)
        if cv.waitKey(1) & 0xFF == ord('q'):
            break;
    video.release()
    cv.destroyAllWindows()
 
if __name__ == "__main__":
    main()