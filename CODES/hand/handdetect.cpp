/*
 * Baozhi Yu
 * Hand detection
 * CSCI5722
 * 
 * 
 */

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
//#include <hmm.h>
#include <iostream>
#include <iomanip>
#include <math.h>

void skinDetect( IplImage* img,IplImage* tmpH2);
int quantAngle(int x,int y,int z);

int main( int argc, char** argv )
{
  //
  IplImage* pFrame = NULL; 
  IplImage* pFrImg = NULL;
  IplImage* pBkImg = NULL;
  IplImage* H;
  IplImage* tmpImg;
  IplImage* dst;
  CvMat* pFrameMat = NULL;
  CvMat* pFrMat = NULL;
  CvMat* pBkMat = NULL;
  
  CvCapture* pCapture = NULL;
  CvMoments moments;
  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  int mode = CV_RETR_EXTERNAL;
  CvMat* region; 
  CvPoint pt1,pt2; 
  double contArea, imgArea,skinArea;
  double m00 = 0, m10, m01, mu20, mu11, mu02, inv_m00; 
  double a, b, c; 
  int xc, yc; 
  int x1,y1,qA;
  int a1[1000],b1[1000];

  int nFrmNum = 0;
  
    
    if( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
        pCapture = cvCaptureFromCAM( argc == 2 ? argv[1][0] - '0' : 0 );
    else if( argc == 2 )
        pCapture = cvCaptureFromAVI("11.avi"); 

    if( !pCapture )
    {
        fprintf(stderr,"Could not initialize capturing...\n");
        return -1;
    }

  //
  cvNamedWindow("video", 1);
  cvNamedWindow("background",1);
  cvNamedWindow("foreground",1);
  cvNamedWindow("skin",1);
  //
  cvMoveWindow("video", 30, 0);
  cvMoveWindow("background", 360, 0);
  cvMoveWindow("foreground", 690, 0);
  cvMoveWindow("skin", 720, 0);
   
  printf( "Hot keys: \n"
        "\tESC - quit the program\n"
        "\tc - stop the tracking\n"
        "\tb - start tracking\n" );

  //pCapture = cvCaptureFromFile("11.avi");
  for(;;)
  {
      pFrame = cvQueryFrame( pCapture );//
     if (!pFrame)
           break;
      nFrmNum=nFrmNum+1;
	  tmpImg = cvCloneImage(pFrame);
	  //cvShowImage("video", pFrame);
      H = cvCreateImage(cvSize(tmpImg->width, tmpImg->height), IPL_DEPTH_8U,1);
	  dst=cvCreateImage(cvSize(tmpImg->width, tmpImg->height),  IPL_DEPTH_8U,3);
	  skinDetect(tmpImg,H);

     
      if(nFrmNum == 1)
	{
      pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
	  pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
	  
	  //pBkImg1 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,3);
      //pFrImg1 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,3);
	  

	  pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
	  pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
	  pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

	  //

	  cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
	  cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
	  //cvConvert(pFrame, pFrameMat);
	  //cvConvert(pFrame, pFrMat);
	  //cvConvert(pFrame, pBkMat);

	  cvConvert(pFrImg, pFrameMat);
	  cvConvert(pFrImg, pFrMat);
	  cvConvert(pFrImg, pBkMat);
	}
      else
	{
	  cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
	  cvConvert(pFrImg, pFrameMat);
        //cvConvert(pFrame, pFrameMat);

	  //
	  cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);
	  
	  //
	  cvAbsDiff(pFrameMat, pBkMat, pFrMat);

	  //
	  cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);
	  //cvThreshold(pFrMat, H, 50, 255.0, CV_THRESH_BINARY);

	  //  
	  cvErode(pFrImg, pFrImg, 0, 1);
	  cvDilate(pFrImg, pFrImg, 0, 1);
     


	  //
	  cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);
	  //
	  cvConvert(pBkMat, pBkImg);

      cvOr(pFrImg,H,H,0);
	  
	  cvFindContours( H, storage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE);
      cvDrawContours( H, contour, CV_RGB(0,0,255), CV_RGB(255,0,0),  2, 2, 8);
      for( ; contour != 0; contour = contour->h_next ) 
	  { 
	 contour=cvApproxPoly( contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP,3,1 );
	 CvScalar color = CV_RGB( 255,0,0 );
	 contArea = fabs(cvContourArea( contour, CV_WHOLE_SEQ ));
	 imgArea = pFrame->width*pFrame->height;
    // Ignore very small contours
        if ( contArea/imgArea >= 0.05 )
		{
          cvDrawContours( dst, contour, color, color, -1, CV_FILLED, 8 );
          region=(CvMat*)contour;
          cvMoments( region, &moments,0 );
          m00 = moments.m00; 
          m10 = moments.m10; 
          m01 = moments.m01; 
          mu11 = moments.mu11; 
          mu20 = moments.mu20; 
          mu02 = moments.mu02;
          inv_m00 = 1. / m00; 
          xc = cvRound( m10 * inv_m00 ); 
          yc = cvRound( m01 * inv_m00 ); 
          a = mu20 * inv_m00; 
          b = mu11 * inv_m00; 
          c = mu02 * inv_m00;
          pt1.x=xc-1;pt1.y=yc; 
          pt2.x=xc+1;pt2.y=yc; 
          cvLine( dst, pt1, pt2, CV_RGB(0,255,0), 2, CV_AA, 0 ); 
          pt1.x=xc;pt1.y=yc-1; 
          pt2.x=xc;pt2.y=yc+1; 
          cvLine( dst, pt1, pt2, CV_RGB(0,255,0), 2, CV_AA, 0 ); 
	      //printf("nFrmNum=%d",nFrmNum);

          //printf("yc=%d",yc);
          //printf("xc=%d\n",xc);
		  if (xc>550 && yc<380 && yc>100)
			printf("Left option: Today's weather: Sunny!\n");
			else if (xc<100 && yc<380 && yc>100)
			
			printf("Right option: Tomorrow's weather: Cloudy!\n");
			else if (yc<100 && xc<550 && xc>100)
			printf("Up option: Note: Remember to submit final report before 11:55 pm Dect 4th\n");
			else if (yc>380 && xc<550 && xc>100)
			printf("Down option: The alarm is : 11:30 pm Dect 4th\n");
	     int o=nFrmNum-1;
	     a1[o]=xc;
       	 b1[o]=yc;
	     if(nFrmNum==2)
		 {  x1=xc;
	     y1=yc;
		 }
         x1=xc-x1;
         y1=yc-y1;
         qA=quantAngle(x1,y1,qA);
         //printf("qA=%d\n",qA);
		}
	  } 
	  
	 //cvLine( dst, pt1, pt2, CV_RGB(0,255,0), 2, CV_AA, 0 );
	// cvLine( dst, pt1, pt2, CV_RGB(0,255,0), 2, CV_AA, 0 ); 

     
	  //
	  
      //cvFlip(pBkImg, pBkImg, 0);
	  //cvFlip(pFrImg, pFrImg, 0);
      //cvFlip(H, H, 0);
	  //cvFlip(dst, dst, 0);
      cvShowImage("video", pFrame);
      cvShowImage("skin", H);
	  cvShowImage("background", pBkImg);
	  cvShowImage("foreground", dst);

	  //
	  //
	  //
	  



	}
   
     c = cvWaitKey(10);
        if( (char) c == 27 )
            break;
        switch( (char) c )
        {
        case 'b':
            //backproject_mode ^= 1;
            break;
        case 'c':
           //track_object = 0;
           // cvZero( histimg );
            break;
        default:
            ;
        }
    
}
	
	//for(int i=0;i<50;i++)
	// {
	 //     printf("a1[%d]=%d\n",i,a1[i]);
	//	  printf("b1[%d]=%d\n",i,b1[i]);
	//}



  //
  cvDestroyWindow("video");
  cvDestroyWindow("background");
  cvDestroyWindow("foreground");
  cvDestroyWindow("skin");

  //
  //cvReleaseImage(&pFrImg);
  cvReleaseImage(&pBkImg);
  cvReleaseMat(&pFrameMat);
  cvReleaseMat(&pFrMat);
  cvReleaseMat(&pBkMat);
  cvReleaseImage(&dst);


  return 0;
}


void skinDetect( IplImage*img,IplImage*tmpH2)
{   
	               //声明IplImage指针
    IplImage* H;
    IplImage* S;
    IplImage* V;
	IplImage* tmpH1;
    IplImage* tmpS1;
    IplImage* tmpS2;
    IplImage* tmpH3;
    IplImage* tmpS3;
    IplConvKernel *erosionElement, *dilationElement;

    cvSmooth(img, img, CV_GAUSSIAN, 3, 0, 0);//高斯滤波先，以平滑图像
    H=     cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
	S=     cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
	V=     cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
    tmpH1= cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
	tmpS1= cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
	tmpS2= cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
	tmpH3= cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
	tmpS3= cvCreateImage(cvSize(img->width, img->height),  IPL_DEPTH_8U,1);
		
		
        cvCvtColor( img, img, CV_BGR2HSV );

     // Split to HSV planes
        cvCvtPixToPlane(img,H,S,V,0);
     // Detect skin tone Hues and Saturations
     // NOTE: Hue values need to be doubled for actual
     
     // Red to Orange Hue with High Saturation
     // Hue 0 to 28 degree and Sat 190 to 200
     cvInRangeS(H,cvScalar(0.0,0.0,0,0),cvScalar(14.0,0.0,0,0),tmpH1);
     cvInRangeS(S,cvScalar(60.0,0.0,0,0),cvScalar(200.0,0.0,0,0),tmpS1);
     cvAnd(tmpH1,tmpS1,tmpH1,0);
	 // Red Hue with Low Saturation
     // Hue 0 to 26 degree and Sat 20 to 90
     cvInRangeS(H,cvScalar(0.0,0.0,0,0),cvScalar(30.0,0.0,0,0),tmpH2); 
     cvInRangeS(S,cvScalar(20.0,0.0,0,0),cvScalar(100.0,0.0,0,0),tmpS2);
	 //cvNot(tmpS2,tmpS2);
     cvAnd(tmpH1,tmpS2,tmpH1,0);
     
     // Red Hue to Pink with Low Saturation
     // Hue 340 to 360 degree and Sat 15 to 90
     cvInRangeS(H,cvScalar(170.0,0.0,0,0),cvScalar(180.0,0.0,0,0),tmpH3);
     cvInRangeS(S,cvScalar(15.0,0.0,0,0),cvScalar(90.0,0.0,0,0),tmpS3);
     cvAnd(tmpH3,tmpS3,tmpH3,0);
     
     // Combine the Hue and Sat detections
     cvOr(tmpH3,tmpH2,tmpH2,0);
     cvOr(tmpH1,tmpH2,tmpH1,0);
	 //  
	 cvSmooth(tmpH1, tmpH1, CV_GAUSSIAN, 9, 0, 0);
	 
	 dilationElement = cvCreateStructuringElementEx( 5,5,3,3, CV_SHAPE_RECT , 0 );
     erosionElement = cvCreateStructuringElementEx( 5,5,3,3, CV_SHAPE_RECT , 0 );
     
	 
	 cvErode(tmpH1, tmpH1, dilationElement, 1);
	 cvDilate(tmpH1, tmpH2,erosionElement, 1);
      //
     
     cvReleaseImage(&tmpH1);
     cvReleaseImage(&tmpS1);
     cvReleaseImage(&tmpS2);
     cvReleaseImage(&tmpH3);
     cvReleaseImage(&tmpS3);
     cvReleaseImage(&H);
     cvReleaseImage(&S);
     cvReleaseImage(&V);
}


int quantAngle(int x,int y,int z)
{
    if(x!=0)
	{
		int t=y/x;
		if(x>0&&y>=0)
		{
			if(t>=0&&t<0.57735)
				z=0;
			else if(t>0.57735&&t<=1.73205)
				z=1;
			else if(t>1.73205)
				z=2;
		}
		else if(x<0&&y>0)
		{
			if(t<0&&t>-0.57735)
				z=5;
			else if(t<-0.57735&&t>=-1.73205)
				z=4;
			else if(t<-1.73205)
				z=3;
		}
		else if(x<0&&y<=0)
		{
			if(t>=0&&t<0.57735)
				z=6;
			else if(t>0.57735&&t<=1.73205)
				z=7;
			else if(t>1.73205)
				z=8;
		}
		else if(x>0&&y<0)
		{
			if(t<0&&t>-0.57735)
				z=11;
			else if(t<-0.57735&&t>=-1.73205)
				z=10;
			else if(t<-1.73205)
				z=9;
		}
	}
	else if(x==0)
	{
	     if(y>0)
		   z=3;
		 else z=9;
	}

return z;

	
}




	
