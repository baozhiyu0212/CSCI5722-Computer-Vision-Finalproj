####################################################################
## Name:Raghunathreddy  & Baozhi Yu
## Funtion:Detects face,smile,eyes
## Class Namr:CSCi 5722
## Instructor Name:Iowana Fleming
##
####################################################################

import cv2
import numpy as np
import sys

#Camera parameters

cap = cv2.VideoCapture(0)
cap.set(3,640)
cap.set(4,480)
rock_count = 0
sci_count = 0
paper_count = 0 
distance_array= []

while True:

    max_area = 0 
    ret, frame = cap.read() # Capture frame-by-frame
    img = frame
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray,(5,5),0)
    ret,thresh1 = cv2.threshold(blur,70,255,cv2.THRESH_BINARY_INV+cv2.THRESH_OTSU)
    contours, hierarchy = cv2.findContours(thresh1,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    for i in range(len(contours)):
        cnt=contours[i]
        area = cv2.contourArea(cnt)
        if(area>max_area):
            max_area=area
            ci=i

    cnt=contours[ci]
    hull = cv2.convexHull(cnt)
    x,y,w,h = cv2.boundingRect(cnt)

	# print x
	# print y
	# print w
	# print h 

    center_x = x+w/2
    center_y = y+h/2

	# print center_x
	# print center_y


    drawing = np.zeros(img.shape,np.uint8)
    cv2.rectangle(drawing,(x,y),(x+w,y+h),(255,0,0),2)
    cv2.drawContours(drawing,[cnt],0,(0,255,0),2)
    cv2.drawContours(drawing,[hull],0,(0,0,255),2)


    hull = cv2.convexHull(cnt,returnPoints = False)
    defects = cv2.convexityDefects(cnt,hull)

    mind=0
    maxd=0
    i=0

    # print defects.shape[0]


    deform_count = 0 
    min_distance = 0
    max_distance = 0
    j =0 

    print "one set"
    for i in range(defects.shape[0]):

        s,e,f,d = defects[i,0]
        start = tuple(cnt[s][0])
        end = tuple(cnt[e][0])
        far = tuple(cnt[f][0])
        [far_x,far_y] = far
        # print far_y
        # print far_x 
        
        
        if((far_y-center_y)<0):


            distance = ((((far_y-center_y)**(2)) + ((far_y-center_y)**(2))) ** (0.5))
            distance_array.insert(j,distance)
            j = j+1
            #print distance
            # dist = cv2.pointPolygonTest(cnt,(center_x,center_y),True)
            # print dist
            cv2.line(drawing,start,end,[0,255,0],2)                
            cv2.circle(drawing,far,5,[0,0,255],-1)
            cv2.circle(drawing,(center_x,center_y),5,[255,0,0],-1)

            deform_count = deform_count+1;
        
            if(distance < min_distance):

                min_distance = distance

            elif(distance > max_distance):

                max_distance = distance

    high_dist = 0
    low_dist  = 0 
    normalize = 0 
    normalize_last = 0 
    paper_sci_count = 0 

    for i in range(j-1):

        normalize =(distance_array[i] - min_distance)/(max_distance - min_distance)
        #print normalize
        normalize_change = abs(normalize - normalize_last)
        
        normalize_last = normalize

        #print normalize_change

        if(normalize_change > 0.5):

        	paper_sci_count = paper_sci_count + 1
        	high_dist = high_dist + 1

        else:

        	low_dist = low_dist + 1 

    # print high_dist
    # print low_dist
    print paper_sci_count

    
            
        # print(i)

    #print deform_count

    if(deform_count > 13 ):

        rock_count = rock_count + 1
        sci_count = 0
        paper_count = 0

    elif(deform_count > 5):


        rock_count = 0

        if(paper_sci_count > 3 ):

            paper_count = paper_count + 1
            sci_count = 0

        elif(paper_sci_count > 2 ):

            sci_count = sci_count + 1
            paper_count = 0


    # print rock_count

    if(rock_count > 2 ):

	    font = cv2.FONT_HERSHEY_SIMPLEX
	    cv2.putText(drawing,'ROCK',(10,400), font,1,(255,255,255),2)

    elif(sci_count > 2):

        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(drawing,'SCISSOR',(10,400), font,1,(255,255,255),2)

    elif(paper_count > 2):

    	font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(drawing,'PAPER',(10,400), font,1,(255,255,255),2)

    else :

    	font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(drawing,'UNKNOWN',(10,400), font,1,(255,255,255),2)




    # print "One sec"
    cv2.imshow('Hand', drawing)

    c = cv2.cv.WaitKey(7) % 0x100

    if c == 27:
        break

 



