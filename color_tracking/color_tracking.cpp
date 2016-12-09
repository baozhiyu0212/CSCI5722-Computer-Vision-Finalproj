/*Baozhi Yu
 * Color_tracking
 * Object color:red
 *CSCI 5722
 * 
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
using namespace cv;
using namespace std;

#define HRES 1280
#define VRES 960

// Transform display window
//char timg_window_name[] = "Color Recognition";


///, *cdst, *timg_gray, *timg_binary;

Mat frame;
Mat colorimg;



int main( int argc, char** argv )
{   double prev_frame_time;
    double curr_frame_time;
    struct timespec frame_time;
    //Capture
   VideoCapture capture(0);
	capture >> frame;
	double Vwidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double Vheight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    int dev=0;
	unsigned prev_locx=0;
	unsigned prev_locy=0;
	unsigned curr_locx;
	unsigned curr_locy;
	unsigned move_h;	
	unsigned move_v;
	int dir=0;	
	int flag=0;

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
    
		 //Object color setting
		int iLowH=156;
		int iHighH=180;
		
		int iLowS=90;
		int iHighS=255;
		
		int iLowV=90;
		int iHighV=255;
		
		
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
    //CvVideoWriter* video=NULL;
    //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES);
    //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES);
	double ave_framedt=0.0, ave_frame_rate=0.0, fc=0.0, framedt=0.0;
        unsigned int frame_count=0;
    while(1)
    { 	
        capture>>frame;
        imwrite("color.jpg",frame);
        colorimg=imread("color.jpg");
    
		Mat imgHSV;
		vector<Mat> hsvSplit;
		cvtColor(colorimg,imgHSV,COLOR_BGR2HSV);
		
		split(imgHSV,hsvSplit);
		equalizeHist(hsvSplit[2],hsvSplit[2]);
		merge(hsvSplit,imgHSV);
		Mat imgThresholded;
		inRange(imgHSV,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),imgThresholded);
		imwrite("color2.jpg",imgThresholded);
		Mat element = getStructuringElement(MORPH_RECT,Size(5,5));
		morphologyEx(imgThresholded,imgThresholded,MORPH_OPEN,element);
		
		morphologyEx(imgThresholded,imgThresholded,MORPH_CLOSE,element);
		imwrite("color1.jpg",imgThresholded);
		imshow("Thresholded Image",imgThresholded);
		imshow("original",colorimg);

		waitKey(1);
		int nr=imgThresholded.rows;
		int nc=imgThresholded.cols;
		//printf("ROw is %d, Coloumn is %d\n",nr,nc);
		unsigned isum=0;unsigned jsum=0;unsigned counter=1;
		int total=0;
		//size_t intensity=0;
		//intensity calculation
		for (int i=0;i<nr;i++)
		{  uchar* ptr=(uchar*)imgThresholded.ptr<uchar>(i);
			
			for (int j=0;j<nc;j++)
			{	//printf("value :%d\n",ptr[j]);
				if (ptr[j]==255)
				{	isum+=i;
					jsum+=j;
					counter++;
					}
			}
		}

		curr_locx=isum/counter;
		curr_locy=jsum/counter;
		
		//printf("%d\n",curr_locx);
		//printf("%d\n",curr_locy);
		/*
		if (curr_locy>prev_locy)
			{move_h=curr_locy-prev_locy;
				dir=1;
				printf("%d\n",dir);}
		if (curr_locy<prev_locy)
			{move_h=prev_locy-curr_locy;
				dir=2;
				printf("%d\n",dir);}
		
		if (curr_locx>prev_locx)
			{move_v=curr_locx-prev_locx;
				dir=3;
				printf("%d\n",dir);}
		if (curr_locx<prev_locx)
			{move_v=prev_locx-curr_locx;
				dir=4;
				printf("%d\n",dir);}		
		if (move_h>50 && dir==2 && move_v<150)
			{//printf("it's a left move!\n");
				flag=1;}
			else if (move_h>50 && dir==1 && move_v<150)
			{//printf("it's a right move!\n");
				flag=2;}
			else if (move_v>150 && dir==4 && move_h<50)
			{//printf("it's a left move!\n");
				flag=3;}
			else if (move_v>150 && dir==3 && move_h<50)
			{//printf("it's a right move!\n");
				flag=4;}	
		
		
			if (flag==1)
			{printf("Left option: Today's weather: Sunny!\n");}
			if (flag==2)
			{printf("Right option: Tomorrow's weather: Cloudy!\n");}
			if (flag==3)
			{printf("Up option: Note: Remember to submit final report before 11:55 pm Dect 4th\n");}
			if (flag==4)
			{printf("Down option: The alarm is : 11:30 pm Dect 4th\n");}
			
			
		prev_locx=curr_locx;
		prev_locy=curr_locy;
		*/
		if (curr_locy>540 && curr_locx>100 && curr_locx<380)
			{printf("Left option: Hi, I'm hungry!\n");}
				//printf("Left option:Today's weather: Sunny!\n");
			else if (curr_locy<100 && curr_locx>100 && curr_locx<380)
			printf("Right option: Hi, I'm thirsty!\n");
			//printf("Right option: Tomorrow's weather: Cloudy!\n");
			else if (curr_locx<100 && curr_locy>100 && curr_locy<540)
			printf("Up option: Hi, I'm sad!\n");
			//printf("Up option: Note: Have a presentation on Dec 10 am !\n");
			else if (curr_locx>380 && curr_locy>100 && curr_locy<540)
			printf("Down option: Just kidding, check if you are there.\n");
			//printf("Down option: I don't know what to do here !\n");
			
	//timing analysis
  	//printf("Frame @ %u sec, %lu nsec, dt=%5.2lf msec, avedt=%5.2lf msec, rate=%5.2lf fps\n", 
    //              (unsigned)frame_time.tv_sec, 
    //              (unsigned long)frame_time.tv_nsec,
     //             framedt, ave_framedt, ave_frame_rate);
	 //   framedt=curr_frame_time - prev_frame_time;
     //       prev_frame_time=curr_frame_time;

    //	usleep(100000);

    
     }  
    capture.release();
    cvDestroyWindow("Color Recognition");
};

