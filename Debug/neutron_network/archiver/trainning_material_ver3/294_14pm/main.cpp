/*!This program is for processing an input image to extract information of water level
 * @brief file contains main function
 * @author Dao Minh Thuc
 * @note i note here because it's hard to remember it
 * vector<Point> represent a contour
 * vector<vector<Point>> represent a vector of contour
 * vector< vector<Point> > *contours represent array of vector of contour
 */
#include <iostream>
#include "getopt.h"
#include "defines.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include <string>
#include <stdio.h>
#include "squares_finder.h"
#include "rotated_square_getter.h"
#include "image_cropper.h"
#include "ann.h"

using namespace cv;
/* !Program run with following options:
 * -i for showing image while running
 * -d for showing debug message

*/
int main(int argc, char ** argv)
{
	/* process options for program */
	int opt;
	int program_mode = NORMAL;
	char * input_image_name = (char*)0;
	while ((opt = getopt(argc,argv,":idf:")) !=-1)
	{
		switch (opt)
		{
		case 'i':
			program_mode |= SHOW_IMAGE;
			break;
		case 'd':
			std::cout << "program run with showing debug message" << std::endl;
			program_mode |= SHOW_DEBUG;
			break;
		case 'f':
			input_image_name = optarg;
			break;
		case ':':
			std::cerr << "err: option need a value" << std::endl;
			break;
		case '?':
			std::cerr << "err: unknown option:" << optopt << std::endl;
			break;
		}
	}
	for (; optind < argc; ++optind)
	{
		std::cerr << "err: argument:" << argv[optind] << std::endl;
	}
	if (program_mode& SHOW_DEBUG&SHOW_IMAGE)
	{
		std::cout <<"program run with showing image\n";
	}
	if (!input_image_name)
	{
		std::cerr<<"err: please specify input image\n";
		return 0;
	}
	/* print result of total process */

	/* Read image*/
	Mat input_image_mat = imread(input_image_name, 1);
	if (input_image_mat.empty())
	{
		std::cerr << "err: input image is empty";
	}
	if (program_mode & SHOW_IMAGE)
	{
		namedWindow("input image window", CV_WINDOW_AUTOSIZE);
		imshow("input image window", input_image_mat);
	}

	/* Convert to grayscale*/
	Mat grayscale_image_mat;
	cvtColor(input_image_mat, grayscale_image_mat, CV_BGR2GRAY);
	if (program_mode & SHOW_IMAGE)
	{
		namedWindow("grayscale image window", CV_WINDOW_AUTOSIZE);
		imshow("grayscale image window", grayscale_image_mat);
	}
	/* Smoothing image */
	Mat blurred_image_mat;
	GaussianBlur(grayscale_image_mat, blurred_image_mat, Size(3, 3), 0, 0); /*Gaussian mask have size (3,3)*/
	if (program_mode & SHOW_IMAGE)
	{
		namedWindow("blur image window", CV_WINDOW_AUTOSIZE);
		imshow("blur image window", blurred_image_mat);
	}
	/* Find square in blurred image*/
	vector<vector<Point> > squares;
	findSquares(blurred_image_mat, squares, CANNY_LOW_THRESHOLD, CANNY_MAX_THRESHOLD, program_mode);
	/****square contours now save in [vector<vector<Point> > squares] *********************************************/
	vector<Mat> extracted_matrices;
	extract_image_from_square_contours(blurred_image_mat, squares, extracted_matrices, program_mode);
	/*********** Extracted_matrices now store in [vector<Mat> extracted_matrices] *********************************************/
	/*Get cropped image from [vector<Mat> extracted_matrices]*/
	vector<Mat> cropped_images;
	crop_vector_of_matrices(extracted_matrices, cropped_images, program_mode);
	/*** now cropped image is store in [vector<Mat> cropped_images]**/
	/**classify*/
	ANN nn = ANN(MODEL_PATH, MODEL_NAME);
	for (int i = 0; i < cropped_images.size(); i++)
	{
		int result = nn.classify(cropped_images[i], program_mode);
		if (program_mode & SHOW_DEBUG)
		{
			std::cout << "cropped_image" << i << "contain" << result << std::endl;
		}
		else
		{
			std::cout << result << " ";
		}
		
	}
	if (program_mode & SHOW_IMAGE)
	{
		waitKey(0);
	}
	//return result for caller program
	return 0;
}