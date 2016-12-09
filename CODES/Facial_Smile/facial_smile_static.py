####################################################################
## Name:Raghunathreddy  & Baozhi Yu
## Funtion:Detects face,smile,eyes
## Class Namr:CSCi 5722
## Instructor Name:Iowana Fleming
##
####################################################################



import numpy as np
import cv2
import glob
import os


def clean_up():

    """This is where cleaning of the result directory takes place."""
    
    try:

        os.chdir('smile')                                                #Change to result directory
        files=glob.glob('*')                                               #Look for all files
        for file in files:                                                    
            os.remove(file)                                                #Remove all the files         
        os.chdir('../') 

        os.chdir('face')                                                #Change to result directory
        files=glob.glob('*')                                               #Look for all files
        for file in files:                                                    
            os.remove(file)                                                #Remove all the files         
        os.chdir('../') 

        os.chdir('eyes')                                                #Change to result directory
        files=glob.glob('*')                                               #Look for all files
        for file in files:                                                    
            os.remove(file)                                                #Remove all the files         
        os.chdir('../') 



    except:

    	pass    
      

removed_images= 0
mouthPath = "mouth.xml"
smilePath = "haarcascade_smile.xml"
smile_cascade = cv2.CascadeClassifier(smilePath)
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('parojos.xml')
mouthCascade = cv2.CascadeClassifier(mouthPath)

print face_cascade
print eye_cascade

no_of_smiles  = 0
no_of_faces = 0;
no_of_eyes = 0;

clean_up();

os.chdir('All_images')

files=glob.glob('*')                                               

for file in files:

	try:

		img = cv2.imread(file)
		gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

		clahe = cv2.createCLAHE(clipLimit=1.6, tileGridSize=(3,3))
	 	gray   = clahe.apply(gray)

		faces = face_cascade.detectMultiScale(gray, 1.3, 5)


		# print file
		smile_found = 0;
		face_found  = 0;
		eyes_found  = 0;

		img = cv2.imread(file)


		for (x1,y1,w1,h1) in faces:

			face_found = 1
			cv2.rectangle(img,(x1,y1),(x1+w1,y1+h1),(255,0,0),2)
			roi_gray  = gray[y1+h1/2:y1+h1, x1:x1+w1]
			roi_color = img[y1+h1/2:y1+h1, x1:x1+w1]

			roi_gray_eye   = gray[y1:y1+h1/2, x1:x1+w1]
			roi_color_eye =  img[y1:y1+h1/2, x1:x1+w1]
			
			eyes = eye_cascade.detectMultiScale(roi_gray_eye)
			
			for (ex,ey,ew,eh) in eyes:

				eyes_found = 1
				cv2.rectangle(img,(ex+x1,ey+y1),(ex+ew+x1,ey+eh+y1),(0,255,0),2)

			mouth = mouthCascade.detectMultiScale(roi_gray,1.3,2)
			# Set region of interest for smiles
			for (x2, y2, w2, h2) in mouth:

				# print "Found", len(smile), "smiles!"
				cv2.rectangle(img, (x1+x2, y1+h1/2+y2), (x1+x2+w2, y1+h1/2+y2+h2), (0, 0, 255), 1)
				# cv2.imshow('img',gray)
				# cv2.waitKey(0) 
				# cv2.destroyAllWindows()

				roi_gray_2 = gray[y1+h1/2+y2:y1+h1/2+y2+h2, x1+x2:x1+x2+w2]
				# roi_color  = img[y:y+h, x:x+w]


				threshold_rc,threshold_image = cv2.threshold(roi_gray_2,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				# cv2.imshow('img',threshold_image)
				# cv2.waitKey(0) 
				# cv2.destroyAllWindows()

				# clahe 		 = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(2,2))
	 			# roi_gray_2     = clahe.apply(roi_gray_2)

				smile 		 = smile_cascade.detectMultiScale(
	            roi_gray_2,
	            scaleFactor= 1.1,
	            minNeighbors= 5,
	            minSize=(1,1),
	            maxSize=(40,40)
	            )
				# Set region of interest for smiles
		        for (x, y, w, h) in smile:

					cv2.rectangle(img, (x1+x2+x, y1+h1/2+y2+y), (x1+x2+x+w, y1+h1/2+y2+y+h), (255, 255, 0), 1)
					smile_found=1

	except:
		removed_images = removed_images +1
		pass

	os.chdir('../')

	if(smile_found == 1):

		no_of_smiles = no_of_smiles + 1
		os.chdir('smile')
		cv2.imwrite(file,img)
		os.chdir('../')


	if(face_found == 1):

		no_of_faces = no_of_faces + 1
		os.chdir('face')
		cv2.imwrite(file,img)
		os.chdir('../')

	if(eyes_found == 1):

		no_of_eyes = no_of_eyes + 1
		os.chdir('eyes')
		cv2.imwrite(file,img)
		os.chdir('../')

	os.chdir('All_images')


print no_of_smiles
print no_of_faces
print no_of_eyes
print removed_images



cv2.imshow('img',img)
cv2.waitKey(0) 
cv2.destroyAllWindows()