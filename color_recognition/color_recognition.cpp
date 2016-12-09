/*
 *	Baozhi Yu
 *  color recognition
 *  CSCI5722
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#define timepro 1
using namespace cv;
using namespace std;

#define HRES 640
#define VRES 480

// Transform display window
//char timg_window_name[] = "Color Recognition";



///, *cdst, *timg_gray, *timg_binary;

Mat frame;
Mat colorimg;



int main( int argc, char** argv )
{   double prev_frame_time;
    double curr_frame_time;
    struct timespec frame_time;
    VideoCapture capture(0);
	capture >> frame;
	double Vwidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double Vheight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    int dev=0;

    if(argc > 1)
    {
        sscanf(argv[1], "%d", &dev);
        printf("using %s\n", argv[1]);
    }
    else if(argc == 1)
        printf("using default\n");

    else
    {
        printf("usage: capture [dev]\n");
        exit(-1);
    }
		
		//preset color settings
		//red 
		int iLowH4R=156;
		int iHighH4R=180;
		
		int iLowS4R=43;
		int iHighS4R=255;
		
		int iLowV4R=46;
		int iHighV4R=255;
		//orange
		int iLowH4O=11;
		int iHighH4O=25;
		
		int iLowS4O=43;
		int iHighS4O=255;
		
		int iLowV4O=46;
		int iHighV4O=255;
		//yellow
		int iLowH4Y=26;
		int iHighH4Y=34;
		
		int iLowS4Y=43;
		int iHighS4Y=255;
		
		int iLowV4Y=46;
		int iHighV4Y=255;
		//green
		int iLowH4G=35;
		int iHighH4G=77;
		
		int iLowS4G=43;
		int iHighS4G=255;
		
		int iLowV4G=46;
		int iHighV4G=255;
		////c
		int iLowH4C=78;
		int iHighH4C=99;
		
		int iLowS4C=43;
		int iHighS4C=255;
		
		int iLowV4C=46;
		int iHighV4C=255;
		//blue
		int iLowH4B=100;
		int iHighH4B=124;
		
		int iLowS4B=43;
		int iHighS4B=255;
		
		int iLowV4B=46;
		int iHighV4B=255;
		//purple
		int iLowH4P=125;
		int iHighH4P=155;
		
		int iLowS4P=43;
		int iHighS4P=255;
		
		int iLowV4P=46;
		int iHighV4P=255;
		//black
		int iLowH4BLACK=0;
		int iHighH4BLACK=180;
		
		int iLowS4BLACK=0;
		int iHighS4BLACK=255;
		
		int iLowV4BLACK=0;
		int iHighV4BLACK=46;
	//
		
	//	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	//	cvCreateTrackbar("LowH","Control",&iLowH,179);
	//	cvCreateTrackbar("HighH","Control",&iHighH,179);
		
	//	cvCreateTrackbar("LowS","Control",&iLowS,255);
	//	cvCreateTrackbar("HighS","Control",&iHighS,255);
		
	//	cvCreateTrackbar("LowV","Control",&iLowV,255);
	//	cvCreateTrackbar("HighV","Control",&iHighV,255);
   // namedWindow( timg_window_name, CV_WINDOW_AUTOSIZE );
    // Create a Trackbar for user to enter threshold
    //createTrackbar( "Min Threshold:", timg_window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

    //capture = (CvCapture *)cvCreateCameraCapture(dev);
    //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES);
    //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES);
	double ave_framedt=0.0, ave_frame_rate=0.0, fc=0.0, framedt=0.0;
        unsigned int frame_count=0;
    while(1)
    { 	
        capture>>frame;
       
                clock_gettime(CLOCK_REALTIME, &frame_time);
                curr_frame_time=((double)frame_time.tv_sec * 1000.0) + 
                                ((double)((double)frame_time.tv_nsec / 1000000.0));
                frame_count++;

                if(frame_count > 2)
                {
                    fc=(double)frame_count;
                    ave_framedt=((fc-1.0)*ave_framedt + framedt)/fc;
                    ave_frame_rate=1.0/(ave_framedt/1000.0);
                }
           
        imwrite("color.jpg",frame);
        colorimg=imread("color.jpg");
		imshow("original",colorimg);
		Mat imgHSV;
		vector<Mat> hsvSplit;
		cvtColor(colorimg,imgHSV,COLOR_BGR2HSV);
		
		split(imgHSV,hsvSplit);
		//equalizeHist(hsvSplit[2],hsvSplit[2]);
		//merge(hsvSplit,imgHSV);
		
		
		//int countr=0;int counto=0;int county=0;int countg=0;int countc=0;int countb=0;int countp=0;int countblack=0;
		
		
		//Mat imgThresholded;
		//inRange(imgHSV,Scalar(iLowH4O,iLowS4O,iLowV4O),Scalar(iHighH4O,iHighS4O,iHighV4O),imgThresholded);
		//imwrite("color2.jpg",imgThresholded);
		//Mat element = getStructuringElement(MORPH_RECT,Size(5,5));
		//morphologyEx(imgThresholded,imgThresholded,MORPH_OPEN,element);
		//morphologyEx(imgThresholded,imgThresholded,MORPH_CLOSE,element);
		//imwrite("color1.jpg",imgThresholded);
		//imshow("Thresholded Image",imgThresholded);
		
		imshow("original",colorimg);
		waitKey(1);
		
		int nr=hsvSplit[0].rows;
		int nc=hsvSplit[0].cols;
		int totalr=0;int totalo=0;int totaly=0;int totalg=0;int totalc=0;int totalb=0;int totalp=0;int totalblack=0;
		for (int i=0;i<nr;i++)
		{   uchar* ptr1=(uchar*)hsvSplit[0].ptr<uchar>(i);
			uchar* ptr2=(uchar*)hsvSplit[1].ptr<uchar>(i);
			uchar* ptr3=(uchar*)hsvSplit[2].ptr<uchar>(i);
			
			//color checked pixel by pixel
			for (int j=0;j<nc;j++)
			{
				//printf("H: %d",ptr1[j]);
				//printf("    S: %d",ptr2[j]);
				//printf("    V: %d\n",ptr3[j]);
				//totalblack=totalblack+ptr[j];
				if (11<=ptr1[j] && ptr1[j]<=25 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totalo++;}
				if (26<=ptr1[j] && ptr1[j]<=34 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totaly++;}
				if (35<=ptr1[j] && ptr1[j]<=77 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totalg++;}
				if (78<=ptr1[j] && ptr1[j]<=99 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totalc++;}
				if (100<=ptr1[j] && ptr1[j]<=124 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totalb++;}
				if (125<=ptr1[j] && ptr1[j]<=155 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totalp++;}
				if (156<=ptr1[j] && ptr1[j]<=180 && 43<=ptr2[j] && ptr2[j]<=255 && 90<=ptr3[j] && ptr3[j]<=255)
				{totalr++;}
				if (0<=ptr1[j] && ptr1[j]<=180 && 0<=ptr2[j] && ptr2[j]<=255 && 0<=ptr3[j] && ptr3[j]<=90)
				{totalblack++;}
			}
		}
		//printf("black pixel: %d\n",totalblack);
		//printf("orange pixel: %d\n",totalo);
		//printf("yellow pixel: %d\n",totaly);
		//printf("green pixel: %d\n",totalg);
		//printf("cyan pixel: %d\n",totalc);
		//printf("blue pixel: %d\n",totalb);
		//printf("purple pixel: %d\n",totalp);
		//printf("red pixel: %d\n",totalr);
		
		int color[8]={totalblack,totalo,totaly,totalg,totalc,totalb,totalp,totalr};
		int max=color[0];
		int pos=0;
		for (int i=1;i<=7;i++)
		{
			if (color[i]>max)
			{max=color[i];
				pos=i;}
		}
		//printf("%d\n",pos);
		if (pos==0)
		{printf("Hi, you are wearing black!\n");}
		else if (pos==1)
		{printf("Hi, you are wearing orange!\n");}
		else if (pos==2)
		{printf("Hi, you are wearing yellow!\n");}
		else if (pos==3)
		{printf("Hi, you are wearing green!\n");}
		else if (pos==4)
		{printf("Hi, you are wearing cyan!\n");}
		else if (pos==5)
		{printf("Hi, you are wearing blue!\n");}
		else if (pos==6)
		{printf("Hi, you are wearing purple!\n");}
		else if (pos==7)
		{printf("Hi, you are wearing red!\n");}
		//printf("row: %d\n",nr);
		//printf("col: %d\n",nc);
		
		//timing analysis
	#ifdef
  	printf("Frame @ %u sec, %lu nsec, dt=%5.2lf msec, avedt=%5.2lf msec, rate=%5.2lf fps\n", 
                  (unsigned)frame_time.tv_sec, 
                 (unsigned long)frame_time.tv_nsec,
                  framedt, ave_framedt, ave_frame_rate);
	    framedt=curr_frame_time - prev_frame_time;
            prev_frame_time=curr_frame_time;

	#endif	
    
     }  
    capture.release();
    cvDestroyWindow("Color Recognition");
};

