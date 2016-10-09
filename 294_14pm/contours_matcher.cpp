/* This program is for processing an input image to extract information of water level
* @brief File contain main function of program that take an input picture have information of water level
* and use some image processing algorithm to extract this information
*
* @author Dao Minh Thuc
*/
//! [includes]
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include <string>
#include "defines.h"
//! [includes]

using namespace cv;

void match_contours()
{
	vector<vector<Point>> input_contour;
	vector<vector<Point>> template_contour;
	//findContour(input_image, input_contour, "canny_input_image", "draw_input_image");
	//findContour(template_image, template_contour, "canny_template_image", "draw_template_image");
	double result = matchShapes(input_contour[0], template_contour[0], CV_CONTOURS_MATCH_I1, 1);
	//if result lower than 0.05 then the result is ok
	std::cout << result;
}