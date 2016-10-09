#include <iostream>
#include "opencv2/opencv.hpp"    // opencv general include file
#include "opencv2/ml/ml.hpp"          // opencv machine learning include file
#include <stdio.h>
#include "defines.h"
#include <fstream>
#define ATTRIBUTES 256  //Number of pixels per sample.16X16
#define CLASSES 10                  //Number of distinct labels.

using namespace cv;
void scaleDownImage(cv::Mat &originalImg, cv::Mat &scaledDownImage)
{
	for (int x = 0; x<16; x++)
	{
		for (int y = 0; y<16; y++)
		{
			int yd = (int)ceil((float)(y*originalImg.cols / 16));
			int xd = (int)ceil((float)(x*originalImg.rows / 16));
			scaledDownImage.at<uchar>(x, y) = originalImg.at<uchar>(xd, yd);

		}
	}
}
void cropImage(cv::Mat &originalImage, cv::Mat &croppedImage)
{
	int row = originalImage.rows;
	int col = originalImage.cols;
	int tlx, tly, bry, brx;//t=top r=right b=bottom l=left
	tlx = tly = bry = brx = 0;
	float suml = 0;
	float sumr = 0;
	int flag = 0;

	/**************************top edge***********************/
	for (int x = 1; x<row; x++)
	{
		for (int y = 0; y<col; y++)
		{
			if (originalImage.at<uchar>(x, y) == 0)
			{

				flag = 1;
				tly = x;
				break;
			}

		}
		if (flag == 1)
		{
			flag = 0;
			break;
		}

	}
	/*******************bottom edge***********************************/
	for (int x = row - 1; x>0; x--)
	{
		for (int y = 0; y<col; y++)
		{
			if (originalImage.at<uchar>(x, y) == 0)
			{

				flag = 1;
				bry = x;
				break;
			}

		}
		if (flag == 1)
		{
			flag = 0;
			break;
		}

	}
	/*************************left edge*******************************/

	for (int y = 0; y<col; y++)
	{
		for (int x = 0; x<row; x++)
		{
			if (originalImage.at<uchar>(x, y) == 0)
			{

				flag = 1;
				tlx = y;
				break;
			}

		}
		if (flag == 1)
		{
			flag = 0;
			break;
		}
	}

	/**********************right edge***********************************/

	for (int y = col - 1; y>0; y--)
	{
		for (int x = 0; x<row; x++)
		{
			if (originalImage.at<uchar>(x, y) == 0)
			{

				flag = 1;
				brx = y;
				break;
			}

		}
		if (flag == 1)
		{
			flag = 0;
			break;
		}
	}
	int width = brx - tlx;
	int height = bry - tly;
	cv::Mat crop(originalImage, cv::Rect(tlx, tly, brx - tlx, bry - tly));
	croppedImage = crop.clone();

}
void convertToPixelValueArray(cv::Mat &img, int pixelarray[])
{
	int i = 0;
	for (int x = 0; x<16; x++)
	{
		for (int y = 0; y<16; y++)
		{
			pixelarray[i] = (img.at<uchar>(x, y) == 255) ? 1 : 0;
			i++;

		}

	}
}
void convertToPixelMatrixArray(cv::Mat &img, cv::Mat &imgarray)
{
	int i = 0;
	for (int x = 0; x<16; x++)
	{
		for (int y = 0; y<16; y++)
		{
			imgarray.at<int>(0,i) = (img.at<uchar>(x, y) == 255) ? 1 : 0;
			i++;

		}

	}
}
string convertInt(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

int main(int argc, char** argv)
{
	//read the model from the XML file and create the neural network.
	CvANN_MLP nnetwork;
	CvFileStorage* storage = cvOpenFileStorage("C:\\Users\\Thuc\\Documents\\Visual Studio 2013\\Projects\\Project1\\Debug\\neutron_network\\param.xml", 0, CV_STORAGE_READ);
	CvFileNode *n = cvGetFileNodeByName(storage, 0, "DigitOCR");
	nnetwork.read(storage, n);
	cvReleaseFileStorage(&storage);

	//your code here
	// ...Generate cv::Mat data(1,ATTRIBUTES,CV_32S) which will contain the pixel
	// ... data for the digit to be recognized
	// ...
	//read input image
	//char * input_image_name = argv[1];
	Mat img = imread("C:\\Users\\Thuc\\Documents\\Visual Studio 2013\\Projects\\Project1\\Debug\\target.jpg", 1);
	//imshow("process", img);
	//declaring mat to hold the scaled down image
	cv::Mat scaledDownImage(16, 16, CV_8U, cv::Scalar(0));
	//declaring array to hold the pixel values in the memory before it written into file
	//int pixelValueArray[256];

		



	//reducing the image dimension to 16X16
	scaleDownImage(img, scaledDownImage);
	//reading the pixel values.
	cv::Mat data(1, ATTRIBUTES, CV_32F);// which will contain the pixel data for the digit to be recognized
	convertToPixelMatrixArray(scaledDownImage, data);
	cout << data;
	//end read
	int maxIndex = 0;
	cv::Mat classOut(1, CLASSES, CV_32F);
	//put your code here
	
	// …
	//prediction
	std::cout << "run to here\n";
	nnetwork.predict(data, classOut);
	float value;
	float maxValue = classOut.at<float>(0, 0);
	for (int index = 1; index<CLASSES; index++)
	{
		value = classOut.at<float>(0, index);
		if (value>maxValue)
		{
			maxValue = value;
			maxIndex = index;
		}
	}
	//maxIndex is the predicted class.
	std::cout << maxIndex;

}
