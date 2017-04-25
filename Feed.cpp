

#include "Feed.h"



//#define SHOW_STEPS            // un-comment or comment this line to show steps or not


Feed::Feed() { //brings in the number of the window
					///////////////////////////////////////////////////////////////////////////////////////////////////



	cam_name = "";





	//capVideo.open("rtsp://tech:tech@192.168.1.194:556/cam/realmonitor?channel=8&subtype=0");  //@CW opens the RTSP feed as the feed, i'd like to eventually not make this hard coded.
	capVideo.open("rtsp://tech:tech@192.168.1.194:556/cam/realmonitor?channel=8&subtype=0");


	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);


	//int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35); //@CW - position of the line cars going left to right need to hit to increment carCount. It says horizonrl position, but it isn't, I just didn't feel like changing the name everywhere
	//int intVerticalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35);
	//@CW - position of the line cars going right to left need to hit to increment carCount1. 

	//@CW These are just the positions for the visual lines on the screen. crossingLine was originally a horizontal line, so I just switched the x and y values. imgFrame1.cols and imageFrame.rows can be used to change the 
	intHorizontalLinePosition = imgFrame1.cols / 4;
	//int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35); //@CW - position of the line cars going left to right need to hit to increment carCount. It says horizonrl position, but it isn't, I just didn't feel like changing the name everywhere
	//int intVerticalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35);
	intVerticalLinePosition = imgFrame1.cols - (imgFrame1.cols / 4); //@CW - position of the line cars going right to left need to hit to increment carCount1. 
																	 //std::cout << carCount1 << "\n"; @cw print carCount1, used to determine if carCount1 was working
	crossingLine[0].y = 0;
	crossingLine[0].x = intHorizontalLinePosition;

	crossingLine[1].y = imgFrame1.cols - 1;
	crossingLine[1].x = intHorizontalLinePosition;

	crossingLine1[0].y = 0;
	crossingLine1[0].x = intVerticalLinePosition;

	crossingLine1[1].y = imgFrame1.cols - 1;
	crossingLine1[1].x = intVerticalLinePosition;





}

Feed::Feed(cv::String video, cv::String name, cv::String direction_of_enterance) { //brings in the number of the window

	direction = direction_of_enterance;								///////////////////////////////////////////////////////////////////////////////////////////////////
	cam_name = name;
	//capVideo.open("rtsp://tech:tech@192.168.1.194:556/cam/realmonitor?channel=8&subtype=0");  //@CW opens the RTSP feed as the feed, i'd like to eventually not make this hard coded.
	capVideo.open(video);

	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);
		
	//int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35); //@CW - position of the line cars going left to right need to hit to increment carCount. It says horizonrl position, but it isn't, I just didn't feel like changing the name everywhere
	//int intVerticalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35);
	 //@CW - position of the line cars going right to left need to hit to increment carCount1. 

																		 //@CW These are just the positions for the visual lines on the screen. crossingLine was originally a horizontal line, so I just switched the x and y values. imgFrame1.cols and imageFrame.rows can be used to change the 
	 intHorizontalLinePosition = imgFrame1.cols / 4;
	//int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35); //@CW - position of the line cars going left to right need to hit to increment carCount. It says horizonrl position, but it isn't, I just didn't feel like changing the name everywhere
	//int intVerticalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35);
	 intVerticalLinePosition = imgFrame1.cols - (imgFrame1.cols / 4); //@CW - position of the line cars going right to left need to hit to increment carCount1. 
																	  //std::cout << carCount1 << "\n"; @cw print carCount1, used to determine if carCount1 was working
	 crossingLine[0].y = 0;
	 crossingLine[0].x = intHorizontalLinePosition;

	 crossingLine[1].y = imgFrame1.cols - 1;
	 crossingLine[1].x = intHorizontalLinePosition;

	 crossingLine1[0].y = 0;
	 crossingLine1[0].x = intVerticalLinePosition;

	 crossingLine1[1].y = imgFrame1.cols - 1;
	 crossingLine1[1].x = intVerticalLinePosition;
	

	


}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs) {

	for (auto &existingBlob : existingBlobs) {

		existingBlob.blnCurrentMatchFoundOrNewBlob = false;

		existingBlob.predictNextPosition();
	}

	for (auto &currentFrameBlob : currentFrameBlobs) {

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobs.size(); i++) {

			if (existingBlobs[i].blnStillBeingTracked == true) {

				double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

				if (dblDistance < dblLeastDistance) {
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5) {
			addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
		}
		else {
			addNewBlob(currentFrameBlob, existingBlobs);
		}

	}

	for (auto &existingBlob : existingBlobs) {

		if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
			existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
		}

		if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
			existingBlob.blnStillBeingTracked = false;
		}

	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex) {

	existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
	existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

	existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

	existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
	existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

	existingBlobs[intIndex].blnStillBeingTracked = true;
	existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs) {

	currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

	existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double Feed::distanceBetweenPoints(cv::Point point1, cv::Point point2) {

	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);

	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName) {

	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	std::vector<std::vector<cv::Point> > contours;

	for (auto &blob : blobs) {
		if (blob.blnStillBeingTracked == true) {
			contours.push_back(blob.currentContour);
		}
	}

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Feed::checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount) {
	bool blnAtLeastOneBlobCrossedTheLine = false;

	for (auto blob : blobs) {

		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;

			if (blob.centerPositions[prevFrameIndex].x > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x <= intHorizontalLinePosition) {
				carCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
			}
		}

	}

	return blnAtLeastOneBlobCrossedTheLine;
}

bool Feed::checkIfBlobsCrossedTheLineFromLeft(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount) //@CW checks if blobs cross the line from right side, needed to change the greater than and less than signs
{
	bool blnAtLeastOneBlobCrossedTheLine = false;

	for (auto blob : blobs) {

		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;

			if (blob.centerPositions[prevFrameIndex].x < intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x >= intHorizontalLinePosition) {
				carCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
			}
		}

	}

	return blnAtLeastOneBlobCrossedTheLine;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy) {

	for (unsigned int i = 0; i < blobs.size(); i++) {

		if (blobs[i].blnStillBeingTracked == true) {
			cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

			int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
			double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
			int intFontThickness = (int)std::round(dblFontScale * 1.0);

			cv::putText(imgFrame2Copy, std::to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Feed::drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy) {

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	//double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	double dblFontScale = (1);
	int intFontThickness = (int)std::round(dblFontScale * 1.5);

	cv::Size textSize = cv::getTextSize(std::to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

	cv::Point ptTextBottomLeftPosition;

	ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
	ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);

	cv::putText(imgFrame2Copy, std::to_string(carCount), ptTextBottomLeftPosition, intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);

}

void Feed::drawCarCountOnImage1(int &carCount, cv::Mat &imgFrame2Copy) //@CW draws count on image, needed to create a seperate function to change where the count is displayed
{

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	double dblFontScale = (1);
	//double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	int intFontThickness = (int)std::round(dblFontScale * 1.5);

	cv::Size textSize = cv::getTextSize(std::to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

	cv::Point ptTextBottomLeftPosition;

	ptTextBottomLeftPosition.x = 1;
	ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);

	cv::putText(imgFrame2Copy, std::to_string(carCount), ptTextBottomLeftPosition, intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);

}

void Feed::doStuff() //creates a window and does all the blob detection
{
	

	std::vector<Blob> currentFrameBlobs;

	cv::Mat imgFrame1Copy = imgFrame1.clone();
	cv::Mat imgFrame2Copy = imgFrame2.clone();

	cv::Mat imgDifference;
	cv::Mat imgThresh;

	cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
	cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

	cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
	cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

	cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

    cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

	//cv::imshow("imgThresh", imgThresh); //@CW shows the threshold window, not needed for what we need to do

	cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

	for (unsigned int i = 0; i < 2; i++) {
		cv::dilate(imgThresh, imgThresh, structuringElement5x5);
		cv::dilate(imgThresh, imgThresh, structuringElement5x5);
		cv::erode(imgThresh, imgThresh, structuringElement5x5);
	}

	cv::Mat imgThreshCopy = imgThresh.clone();

	std::vector<std::vector<cv::Point> > contours;

	cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	//drawAndShowContours(imgThresh.size(), contours, "imgContours");

	std::vector<std::vector<cv::Point> > convexHulls(contours.size());

	for (unsigned int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours[i], convexHulls[i]);
	}

	//	drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

	for (auto &convexHull : convexHulls) {
		Blob possibleBlob(convexHull);

		if (possibleBlob.currentBoundingRect.area() > 400 &&
			possibleBlob.dblCurrentAspectRatio > 0.2 &&
			possibleBlob.dblCurrentAspectRatio < 4.0 &&
			possibleBlob.currentBoundingRect.width > 30 &&
			possibleBlob.currentBoundingRect.height > 30 &&
			possibleBlob.dblCurrentDiagonalSize > 60.0 &&
			(cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) {
			currentFrameBlobs.push_back(possibleBlob);
		}
	}

	//	drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

	if (blnFirstFrame == true) {
		for (auto &currentFrameBlob : currentFrameBlobs) {
			blobs.push_back(currentFrameBlob);
		}
	}
	else {
		matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
	}

	//	drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

	imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

												//drawBlobInfoOnImage(blobs, imgFrame2Copy);

	bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount); //@CW checks if blobs passed the correct point going from left to right, this was the original function that was used

	if (blnAtLeastOneBlobCrossedTheLine == true) {
		cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
	}
	else {
		cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
	}

	bool blnAtLeastOneBlobCrossedTheLine1 = checkIfBlobsCrossedTheLineFromLeft(blobs, intVerticalLinePosition, carCount1); //@CW checks if blobs passed the correct point going from right to left. just changed the greater to and less than symbols to make it look for objects going the other direction.
	if (blnAtLeastOneBlobCrossedTheLine1 == true) {
		cv::line(imgFrame2Copy, crossingLine1[0], crossingLine1[1], SCALAR_GREEN, 2);
	}
	else {
		cv::line(imgFrame2Copy, crossingLine1[0], crossingLine1[1], SCALAR_RED, 2);
	}
	drawCarCountOnImage(carCount, imgFrame2Copy);
	drawCarCountOnImage1(carCount1, imgFrame2Copy);

	cv::String abc =  cam_name;

//	cv::imshow(abc, imgFrame2Copy);

	//cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

	// now we prepare for the next iteration

	currentFrameBlobs.clear();

	imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is
	capVideo.read(imgFrame2); //@CW this line needed to be added for RTSP to work, since it will only have one frame by default, the next if statement needs to be removed also.
							  /*if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT)) {
							  capVideo.read(imgFrame2);
							  }
							  else {
							  std::cout << "end of video\n";
							  break;
							  } */

	blnFirstFrame = false;
	frameCount++;
	if (direction == "East") {
		totalCount = carCount - carCount1;
//		std::cout << direction;
	}
	else {
//		std::cout << direction;
		totalCount = carCount1 - carCount;
	}
}

int Feed::getTotal() {
	int temp = totalCount;
	carCount = 0;
	carCount1 = 0;
	totalCount = 0;
	return temp;

}
