#include <iostream>
#include "image_cropper.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include "image_cropper.h"
#include "defines.h"
/**
* Rotate an image
*/
static void rotate(cv::Mat& src, double angle, cv::Mat& dst)
{
	int len = std::max(src.cols, src.rows);
	cv::Point2f pt(len / 2., len / 2.);
	cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

	cv::warpAffine(src, dst, r, cv::Size(len, len));
}
void crop_image(cv::Mat& originalImage, cv::Mat& croppedImage, int program_mode)
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
/*
 * @brief Cropping for vector of matrices
 */
void crop_vector_of_matrices(std::vector<cv::Mat> &extracted_matrices, std::vector<cv::Mat> &cropped_matrices, const int program_mode)
{
	static int index_image = 0;
	for (int i = 0; i < extracted_matrices.size(); i++)
	{
		cv::Mat img = extracted_matrices[i].clone();
		cv::Mat output;
		//Applying gaussian blur to remove any noise
		cv::GaussianBlur(img, output, cv::Size(5, 5), 0);
		//thresholding to get a binary image
		//cv::adaptiveThreshold
		cv::threshold(output, output, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		//cv::threshold(output, output, BINARY_THRESHOLD, 255, 0);
		crop_image(output, output, program_mode);
		cv::Mat *output1 = new cv::Mat(output.cols, output.rows, CV_8UC1);
		if (output.cols>output.rows)//if image number is rotated then rerotated it
		{
			transpose(output, *output1);
			flip(*output1, *output1,0);
			cropped_matrices.push_back(*output1);
			if (program_mode &SHOW_IMAGE)
			{
				std::stringstream sttm;
				sttm << "cropped_image_" << index_image++;
				std::string window_name = sttm.str();
				cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
				cv::imshow(window_name, *output1);
				if (program_mode & SHOW_DEBUG)
				{
					std::cout << "cropped_image_" << index_image << "with column is:" << output.cols << " and with row is:" << output.rows << std::endl;
				}
				

			}
			delete output1;
		}
		else
		{
			cropped_matrices.push_back(output);
			if (program_mode & SHOW_IMAGE)
			{
				std::stringstream sttm;
				sttm << "cropped_image_" << index_image++;
				std::string window_name = sttm.str();
				cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
				cv::imshow(window_name, output);
				if (program_mode & SHOW_DEBUG)
				{
					std::cout << "cropped_image_" << index_image << "with column is:" << output.cols << " and with row is:" << output.rows << std::endl;
				}
				

			}
		}
		
	}

}
