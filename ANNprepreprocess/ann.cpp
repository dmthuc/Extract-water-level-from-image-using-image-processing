#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include "opencv2/ml/ml.hpp"          // opencv machine learning include file
#include <fstream>
#include "ann.h"
#include "defines.h"

/* declare of helper function*/
void scaleDownImage(cv::Mat &originalImg, cv::Mat &scaledDownImage);
void convertTo1DimensionMatrix(cv::Mat &img, cv::Mat &imgarray);
/*initilize nnetwork*/
ANN::ANN(const char* model_path, const char* model_name)
{
	CvFileStorage* storage = cvOpenFileStorage(model_path, 0, CV_STORAGE_READ);
	CvFileNode *n = cvGetFileNodeByName(storage, 0, model_name);
	nnetwork.read(storage, n);
	cvReleaseFileStorage(&storage);
}
/*
 *@brief classify input_image 
 *@param input_image is pre-crop image
 */
int ANN::classify(const cv::Mat &input_image,Program_mode program_mode)
{
	cv::Mat img = input_image.clone();
	cv::Mat scaledDownImage(16, 16, CV_8U, cv::Scalar(0));
	scaleDownImage(img, scaledDownImage);
	cv::Mat data(1, ATTRIBUTES, CV_32F);// which will contain the pixel data for the digit to be recognized
	
	convertTo1DimensionMatrix(scaledDownImage, data);
	std::cout << data << std::endl;
	cv::Mat classOut(1, CLASSES, CV_32F);/*matrix that contain result*/
	nnetwork.predict(data, classOut);
	/* extract result from result matrix*/
	int maxIndex = 0;
	float value;
	//float maxValue = classOut.at<float>(0, 0);
	float maxValue = 0;
	//std::cout << maxValue;
	for (int index = 0; index<CLASSES; index++)
	{
		value = classOut.at<float>(0, index);
		if (value>maxValue)
		{
			maxValue = value;
			maxIndex = index;
		}
	}
	if (program_mode == EXPLICIT)
	{
		std::cout << "max value of nnetwork is:" << maxValue << std::endl;
		std::cout << "max index correspond is:" << maxIndex << std::endl;
	}
	//maxIndex is the predicted class.
	return maxIndex;/**/
}
//hepler function that help scaleDown image
void scaleDownImage(cv::Mat &originalImg, cv::Mat &scaledDownImage)
{
	for (int x = 0; x<16; x++)
	{
		for (int y = 0; y<16; y++)
		{
			int yd = ceil((float)(y*originalImg.cols / 16));
			int xd = ceil((float)(x*originalImg.rows / 16));
			scaledDownImage.at<uchar>(x, y) = originalImg.at<uchar>(xd, yd);

		}
	}
}

void convertTo1DimensionMatrix(cv::Mat &img, cv::Mat &imgarray)
{
	int i = 0;
	for (int x = 0; x<16; x++)
	{
		for (int y = 0; y<16; y++)
		{
			imgarray.at<float>(0, i) = (img.at<uchar>(x, y) == 255) ? 1 : 0;
			i++;

		}

	}
}
