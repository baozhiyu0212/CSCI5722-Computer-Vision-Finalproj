####################################################################
## Name:Raghunathreddy  & Baozhi Yu
## Funtion:Detects face,smile,eyes
## Class Namr:CSCi 5722
## Instructor Name:Iowana Fleming
##
####################################################################

import numpy as np
import sys
import cv2


#Paths to be added
facePath    = "haarcascade_frontalface_default.xml"
smilePath   = "haarcascade_smile.xml"
faceCascade = cv2.CascadeClassifier(facePath)
smileCascade = cv2.CascadeClassifier(smilePath)
eye_cascade = cv2.CascadeClassifier('parojos.xml')
mouthPath = "mouth.xml"
mouth_cascade = cv2.CascadeClassifier(mouthPath)
##########

#Start the vidoe capture


capture = cv2.VideoCapture(0)

#Set the resolution of the camera

capture.set(3,640)                         
capture.set(4,480)
#

#Adjust the scale factor 
scalefactor = 1.05


#Main part of the function

while True:

    ret, frame = capture.read() #Capture frame-by-frame
    img = frame
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    #Adjust the parameters 
    faces = faceCascade.detectMultiScale(gray,scaleFactor= scalefactor,minNeighbors=8,minSize=(55, 55),flags=cv2.cv.CV_HAAR_SCALE_IMAGE)
    

    #Look for features inside the face

    for (x, y, w, h) in faces:


        
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 2)
        
        # Find the regio of interests
        # For eye it is the upper half of the face
        gray_img = gray[y:y+h/2, x:x+w]
        color_img = frame[y:y+h/2, x:x+w]
        eye = eye_cascade.detectMultiScale(gray_img)
        
        # Find the regio of interests
        # For smile it is the lower half of the face
        for (x, y, w, h) in eye:

            cv2.rectangle(color_img, (x, y), (x+w, y+h), (255, 0, 0), 1)

        gray_img  = gray[y+h/2:y+h, x+w/2:x+w]
        color_img = frame[y+h/2:y+h, x+w/2:x+w]

        smile = mouth_cascade.detectMultiScale(gray_img)
        

        for (x, y, w, h) in smile:

            cv2.rectangle(color_img, (x, y), (x+w, y+h), (255, 255, 0), 1)


    cv2.imshow('Face,eye,smile Detector', frame)
    c = cv2.cv.WaitKey(7) % 0x100
    if c == 27:
        break

#clean things
cap.release()
cv2.destroyAllWindows()