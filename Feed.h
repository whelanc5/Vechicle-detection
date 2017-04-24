#pragma once
#ifndef MY_FEED
#define MY_FEED

#include<iostream>
//#include<conio.h>  
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "Blob.h"
class Feed {

public:
	

	// global variables ///////////////////////////////////////////////////////////////////////////////
	 const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
	 const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
	 const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
	 const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
	 const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);
	 cv::String cam_name;
	 //holds car count from left 
	 int carCount = 0;
	 //holds car count from right
	 int carCount1 = 0;
	 //holds the total count, will be carCount - carCount1 or vice versa
	 int totalCount = 0;
	 //mat for holding the first frame
	 cv::Mat imgFrame1;
	 //mat for holding the 2nd frame
	 cv::Mat imgFrame2;
	 //vector of last 5 blob positions
	 std::vector<Blob> blobs;
	 //line for detecting a blob
	 cv::Point crossingLine[2];
	 //2nd line for detecting a blob
	 cv::Point crossingLine1[2];
	 cv::VideoCapture capVideo;
	 //int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35); //@CW - position of the line cars going left to right need to hit to increment carCount. It says horizonrl position, but it isn't, I just didn't feel like changing the name everywhere
	 int intHorizontalLinePosition = imgFrame1.cols / 4;	 
	 //@CW - position of the line cars going right to left need to hit to increment carCount1. 
	 int intVerticalLinePosition = imgFrame1.cols - (imgFrame1.cols / 4); 
	 char chCheckForEscKey = 0;
	 //holds direction that the car are going to enter from
	 cv::String direction;
	

	 

	 

	 bool blnFirstFrame = true;

	 int frameCount = 2;
	 //@CW These are just the positions for the visual lines on the screen. crossingLine was originally a horizontal line, so I just switched the x and y values. imgFrame1.cols and imageFrame.rows can be used to change the 


	// function prototypes ////////////////////////////////////////////////////////////////////////////
	 Feed();
	 //void createFeed(cv::String video, cv::String name);
	 void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
	 void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
	 void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
	 double distanceBetweenPoints(cv::Point point1, cv::Point point2);
	 void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
	 void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
	 bool checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount);
	 bool checkIfBlobsCrossedTheLineFromLeft(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount);
	 void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy);
	 void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy);
	 void drawCarCountOnImage1(int &carCount, cv::Mat &imgFrame2Copy);
	 //feed constrcutor. video takes a string for the rtsp feed, name takes the name of the feed, direction_of_enterence takes the direction the car enter from
	 Feed(cv::String video, cv::String name, cv::String direction_of_enterance);
	 //displays camera feeds and counts cars going by
	 void doStuff();
	 //returns total number of cars that have gone in or out
	 int getTotal();
};

#endif
