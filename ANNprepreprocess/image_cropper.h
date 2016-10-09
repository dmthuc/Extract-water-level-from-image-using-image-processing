#ifndef IMAGE_CROPPER
#define IMAGE_CROPPER
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include <iostream>
#include <string>
#include "defines.h"
/*
 * 
 */
void crop_image(cv::Mat& input_image, cv::Mat& output_image, Program_mode program_mode = IMPLICIT);
void crop_vector_of_matrices(std::vector<cv::Mat> &extracted_matrices,std::vector<cv::Mat> &cropped_matrices,const Program_mode program_mode= IMPLICIT);

#endif