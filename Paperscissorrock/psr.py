import cv2
import numpy
import time
import random
import os
def judge( ):

    #cv2.namedWindow("test",1)
    img = cv2.imread("cur.jpg",0)
    #morphology filter
    element = cv2.getStructuringElement(cv2.MORPH_RECT,(11,11))
    dilate = cv2.dilate(img, element)
    erode = cv2.erode(img, element)
    
    result = cv2.absdiff(dilate,erode);
    #cv2.imshow("test",result)
    retval, result = cv2.threshold(result, 40, 255, cv2.THRESH_BINARY);
	
    #proprocess
    result = cv2.bitwise_not(result);
    result =cv2.medianBlur(result,23)
    a=[]
    posi =[]
    width =[]
    count = 0
    area = 0 
    for i in range(result.shape[1]):
        for j in range(result.shape[0]):
            if(result[j][i]==0):
                area+=1
    for i in range(result.shape[1]):
        if(result[5*result.shape[0]/16][i]==0 and result[5*result.shape[0]/16][i-1]!=0 ):
            count+=1
            width.append(0)
            posi.append(i)
        if(result[5*result.shape[0]/16][i]==0):
            width[count-1]+=1
    
    
    
    width_length=0
    width_jiandao = True
    for i in range(count):
        if width[i]>45:
           # paper
            return 2;
        if width[i]<=20 or width[i]>=40:
            width_jiandao= False
        width_length += width[i]
    if width_jiandao==True and count==2:
        return 1;
    if(area <8500):
        #rock
        return 0;
    print "width_leng",width_length
    if(width_length<35):
       
        a=[]
        posi =[]
        width =[]
        count = 0
        for i in range(result.shape[1]):
            if(result[11*result.shape[0]/16][i]==0 and result[11*result.shape[0]/16][i-1]!=0 ):
                count+=1
                width.append(0)
                posi.append(i)
            if(result[11*result.shape[0]/16][i]==0):
                width[count-1]+=1
       
    width_length=0
    width_jiandao = True
    for i in range(count):
        if width[i]>45:
            #print 'paper';
            return 2;
        if width[i]<=20 or width[i]>=40:
            width_jiandao= False
        width_length += width[i]
    if width_jiandao==True and count==2:
        return 1;
    if(area>14000 or count>=3):
        #print 'rock';
        return 2;
    if(width_length<110):
        #print 'scissors';
        return 1;
    else:
        #print 'paper';
        return 2;
 
 


def game():
    out =[]
    out.append("Rock")
    out.append("Scissors")
    out.append("Paper")
    capture = cv2.VideoCapture(0)
    cv2.namedWindow("camera",1)
    start_time = time.time()
    print("put your hand in box and fill it as much as possible\n")
    while(1):
        ha,img =capture.read()
        end_time = time.time()
        cv2.rectangle(img,(426,0),(640,250),(0,0,255))
        cv2.putText(img,'time remaining', (0,100), 0,0.5,(0,0,255),2)
        cv2.putText(img,str(int((5-(end_time- start_time)))), (150,100),0, 0.5,(0,0,255),2)
        cv2.imshow("camera",img)
        if(end_time-start_time>5):
            break
        if(cv2.waitKey(30)>=0):
            break
    ha,img = capture.read()
    capture.release()
    cv2.imshow("camera",img)
    img = img[0:210,426:640]
    cv2.imwrite("cur.jpg",img)
    p1 = judge()
    pc = random.randint(0,2)
    #print p1,' ',pc,'\n'
    print "Your choice",out[p1]," AI choice",out[pc],"\n"
    cv2.destroyAllWindows()
    if(p1==pc):
        print "draw\n"
        return 0
    if((p1==0 and pc ==1)or(p1==1 and pc ==2)or(p1==2 and pc ==0)):
        print 'you win\n'
        return 1
    else:
        print 'you lose\n'
        return -1
def main():
    you_win=0
    pc_win=0
    print("Rock-Paper-Scissors Game: press enter to start\n")
    s = raw_input()
    while(1):
        
        s = raw_input()
        
        ans =game()
        if(ans == 1):
            you_win+=1
        elif(ans == -1):
            pc_win+=1
        print ""
main()
