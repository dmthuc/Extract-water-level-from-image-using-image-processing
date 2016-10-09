#ifndef SQUARES_FINDER_H
#define SQUARES_FINDER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale

using namespace cv;
/* @brief This function take grayscale image as input then find squares contour in that image
* @param squares : vector of contour that caller provide for function as a place to store contours that have square shape
* @param program_mode : to select if the program will run implicitly or not- refer to
*/
void findSquares(const Mat& gray_image, vector<vector<Point> >& squares, \
	const int low_threshold, const int max_threshold, Program_mode program_mode = IMPLICIT);

#endif
