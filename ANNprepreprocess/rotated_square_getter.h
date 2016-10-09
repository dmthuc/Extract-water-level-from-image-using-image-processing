#ifndef ROTATED_SQUARE_GETTER_H
#define ROTATED_SQUARE_GETTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale

using namespace cv;

/* @brief Extract a matrix image from a given matrix image with size of given Rotated Rectangle
*
*
*/
void extract_image_from_square_contours(const Mat& gray_image, vector<vector<Point> >& squares, vector<Mat>& extracted_matrices, Program_mode program_mode);

#endif
