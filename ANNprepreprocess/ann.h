/* This program is for processing an input image to extract information of water level
* @brief File contain artifical object that take 
*
* @author Dao Minh Thuc
*/
#ifndef ANN_H
#define ANN_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include "opencv2/ml/ml.hpp"          // opencv machine learning include file
#include <fstream>
#include "defines.h"
class ANN{
private:
	CvANN_MLP nnetwork;
public:
	/*initilize nnetwork*/
	ANN(const char* model_path, const char* model_name);
	/*classify input_image */
	int classify(const cv::Mat &input_image, Program_mode program_mode = IMPLICIT);

};

#endif
