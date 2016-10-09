/* This program is about get cropped image for trainning neutron network
 * It generate binary image using osto binay method.The generated image is stored in the same folder with executable file
 * @author Dao Minh Thuc
 *
 * @note i note here because it's hard to remember it
 * vector<Point> represent a contour
 * vector<vector<Point>> represent a vector of contour
 * vector< vector<Point> > *contours represent array of vector of contour
 */

//! [includes]
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include <string>
#include "defines.h"
#include "squares_finder.h"
#include "rotated_square_getter.h"
#include "contours_finder.h"
#include "contours_matcher.h"
#include "image_cropper.h"
#include "ann.h"
//! [includes]

using namespace cv;
static void get_result(int j, vector<int> &content_result);
static std::string convertInt(const int number)
{
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}
int main(int argc, char ** argv)
{
	/* program_mode use to select if the program run explicitly (display processing image while run) or not
	use as following:
	- "194_20pm input.jpg 2" to select run in debug mode then program will display debug message in command window
	- "194_20pm input.jpg 1" to select run explicitly then program_mode varible will be set to 1-EXPLICIT
	- "194_20pm input.jpg " to select run implicitly then program_mode varible will be set to 0-IMPLICIT as default
	*/

	Program_mode program_mode = IMPLICIT;/* program_mode = 0 to run implicitly and = 1 to run explicitly*/

	for (int ind = 2; ind < 509; ind++)
	{
		/* Read input image */
		std::stringstream sttm1;
		sttm1 <<"C:\\Users\\Thuc\\Pictures\\Debut\\image_(" << convertInt(ind) << ").jpg";
		std::string file_name = sttm1.str();
		//char * input_image_name = argv[1];

		Mat input_image = imread(file_name, 1);

		if (program_mode == EXPLICIT)
		{
			/* If prgram_mode is explixit then create a window and show input image */
			namedWindow("input_image", CV_WINDOW_AUTOSIZE);
			imshow("input_image", input_image);
		}
		/***** End of reading input image - input image is storage at [Mat input_image] ***********/

		/* Convert input_image to grayscale and blur it*/
		Mat gray_image;
		cvtColor(input_image, gray_image, CV_BGR2GRAY);
		GaussianBlur(gray_image, gray_image, Size(3, 3), 0, 0); /*Gaussian mask have size (3,3)*/

		if (program_mode == EXPLICIT)
		{
			/* If prgram_mode is explixit then create a window and show grayscale image */
			namedWindow("gray_image", CV_WINDOW_AUTOSIZE);
			imshow("gray_image", gray_image);
		}
		/*** End of converting to grayscale image - grayscale image is storage at [Mat gray_image] ***/

		/*Find square in gray_image*/
		vector<vector<Point> > squares;
		findSquares(gray_image, squares, CANNY_LOW_THRESHOLD, CANNY_MAX_THRESHOLD, program_mode);
		/****square contours now save in [vector<vector<Point> > squares] *********************************************/
		vector<Mat> extracted_matrices;
		extract_image_from_square_contours(gray_image, squares, extracted_matrices, program_mode);
		/*********** Extracted_matrices now store in [vector<Mat> extracted_matrices] *********************************************/
		/*Get cropped image from [vector<Mat> extracted_matrices]*/
		vector<Mat> cropped_images;
		crop_vector_of_matrices(extracted_matrices, cropped_images, program_mode);
		/*** now cropped image is store in [vector<Mat> cropped_images]**/

		///**classify*/
		//ANN nn = ANN(MODEL_PATH,MODEL_NAME);
		//for (int i = 0; i < cropped_images.size(); i++)
		//{
		//	std::cout <<"cropped_image"<<i<<"contain"<< nn.classify(cropped_images[i],program_mode)<<std::endl;
		//}
		//waitKey(0);
	}
	return 0;
}

