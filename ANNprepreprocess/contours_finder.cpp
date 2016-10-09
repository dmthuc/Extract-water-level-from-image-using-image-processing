/* This program is for processing an input image to extract information of water level
* @brief File contain function that find all contours of an image
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
/* @brief Find all contours in a given image and storage found contours in [vector<vector<Point>>& contours]
 * @param input_image a given image to find contours
 * @param contours a vector a contour to save contours
 * @param canny_window_name a name that assign for a new window window to display canny detected image
 * @param draw_window_name a name that assign for a new windows
 */
void findContour(const Mat& input_image, vector<vector<Point>>& contours, const std::string & canny_window_name, const std::string & draw_window_name,Program_mode program_mode)
{
	int low_threshold = 0;
	const int max_threshold = 255;
	Mat edge_detected_image;//check this latter-you have to allocate memory for it
	vector<Vec4i> hierarchy;
	/*detect edge*/
	Canny(input_image, edge_detected_image, low_threshold, max_threshold);
	if (program_mode == EXPLICIT)
	{
		namedWindow(canny_window_name, WINDOW_AUTOSIZE);
		imshow(canny_window_name, edge_detected_image);
	}

	/*find contour */
	findContours(edge_detected_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	if (program_mode == EXPLICIT)
	{
		Mat drawing = Mat::zeros(edge_detected_image.size(), CV_8UC1);

		//get a random color for rotated boxes
		RNG rng(12345);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//
		for (size_t i = 0; i < contours.size(); i++)
		{
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
		}
		/// Show in a window
		namedWindow(draw_window_name, WINDOW_AUTOSIZE);
		imshow(draw_window_name, drawing);
	}


}