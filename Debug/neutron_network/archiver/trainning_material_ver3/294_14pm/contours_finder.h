/* This program is for processing an input image to extract information of water level
* @brief File contain function that find all contours of an image
* and use some image processing algorithm to extract this information
*
* @author Dao Minh Thuc
*/

#ifndef CONTOURS_FINDER_H
#define CONTOUR_FINDER_H
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
void findContour(const Mat& input_image, vector<vector<Point>>& contours, const std::string & canny_window_name, const std::string & draw_window_name, Program_mode program_mode = IMPLICIT);
#endif