/* This program is for processing an input image to extract information of water level
* @brief File contain function that extract a matrix image from a given matrix image with size of given Rotated Rectangle
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

/* @brief Helper function support for [myGetQuadrangleSubPix]
 */
static void getQuadrangleSubPix_8u32f_CnR(const uchar* src, size_t src_step, Size src_size,
	float* dst, size_t dst_step, Size win_size,
	const double *matrix, int cn)
{
	int x, y, k;
	double A11 = matrix[0], A12 = matrix[1], A13 = matrix[2];
	double A21 = matrix[3], A22 = matrix[4], A23 = matrix[5];

	src_step /= sizeof(src[0]);
	dst_step /= sizeof(dst[0]);

	for (y = 0; y < win_size.height; y++, dst += dst_step)
	{
		double xs = A12*y + A13;
		double ys = A22*y + A23;
		double xe = A11*(win_size.width - 1) + A12*y + A13;
		double ye = A21*(win_size.width - 1) + A22*y + A23;

		if ((unsigned)(cvFloor(xs) - 1) < (unsigned)(src_size.width - 3) &&
			(unsigned)(cvFloor(ys) - 1) < (unsigned)(src_size.height - 3) &&
			(unsigned)(cvFloor(xe) - 1) < (unsigned)(src_size.width - 3) &&
			(unsigned)(cvFloor(ye) - 1) < (unsigned)(src_size.height - 3))
		{
			for (x = 0; x < win_size.width; x++)
			{
				int ixs = cvFloor(xs);
				int iys = cvFloor(ys);
				const uchar *ptr = src + src_step*iys;
				float a = (float)(xs - ixs), b = (float)(ys - iys), a1 = 1.f - a, b1 = 1.f - b;
				float w00 = a1*b1, w01 = a*b1, w10 = a1*b, w11 = a*b;
				xs += A11;
				ys += A21;

				if (cn == 1)
				{
					ptr += ixs;
					dst[x] = ptr[0] * w00 + ptr[1] * w01 + ptr[src_step] * w10 + ptr[src_step + 1] * w11;
				}
				else if (cn == 3)
				{
					ptr += ixs * 3;
					float t0 = ptr[0] * w00 + ptr[3] * w01 + ptr[src_step] * w10 + ptr[src_step + 3] * w11;
					float t1 = ptr[1] * w00 + ptr[4] * w01 + ptr[src_step + 1] * w10 + ptr[src_step + 4] * w11;
					float t2 = ptr[2] * w00 + ptr[5] * w01 + ptr[src_step + 2] * w10 + ptr[src_step + 5] * w11;

					dst[x * 3] = t0;
					dst[x * 3 + 1] = t1;
					dst[x * 3 + 2] = t2;
				}
				else
				{
					ptr += ixs*cn;
					for (k = 0; k < cn; k++)
						dst[x*cn + k] = ptr[k] * w00 + ptr[k + cn] * w01 +
						ptr[src_step + k] * w10 + ptr[src_step + k + cn] * w11;
				}
			}
		}
		else
		{
			for (x = 0; x < win_size.width; x++)
			{
				int ixs = cvFloor(xs), iys = cvFloor(ys);
				float a = (float)(xs - ixs), b = (float)(ys - iys), a1 = 1.f - a, b1 = 1.f - b;
				float w00 = a1*b1, w01 = a*b1, w10 = a1*b, w11 = a*b;
				const uchar *ptr0, *ptr1;
				xs += A11; ys += A21;

				if ((unsigned)iys < (unsigned)(src_size.height - 1))
					ptr0 = src + src_step*iys, ptr1 = ptr0 + src_step;
				else
					ptr0 = ptr1 = src + (iys < 0 ? 0 : src_size.height - 1)*src_step;

				if ((unsigned)ixs < (unsigned)(src_size.width - 1))
				{
					ptr0 += ixs*cn; ptr1 += ixs*cn;
					for (k = 0; k < cn; k++)
						dst[x*cn + k] = ptr0[k] * w00 + ptr0[k + cn] * w01 + ptr1[k] * w10 + ptr1[k + cn] * w11;
				}
				else
				{
					ixs = ixs < 0 ? 0 : src_size.width - 1;
					ptr0 += ixs*cn; ptr1 += ixs*cn;
					for (k = 0; k < cn; k++)
						dst[x*cn + k] = ptr0[k] * b1 + ptr1[k] * b;
				}
			}
		}
	}

}
/****************************************************************************/

/* @brief helper function support for function [getRotRectImg]
 */
static void myGetQuadrangleSubPix(const Mat& src, Mat& dst, Mat& m)
{
	CV_Assert(src.channels() == dst.channels());

	cv::Size win_size = dst.size();
	double matrix[6];
	cv::Mat M(2, 3, CV_64F, matrix);
	m.convertTo(M, CV_64F);
	double dx = (win_size.width - 1)*0.5;
	double dy = (win_size.height - 1)*0.5;
	matrix[2] -= matrix[0] * dx + matrix[1] * dy;
	matrix[5] -= matrix[3] * dx + matrix[4] * dy;

	if (src.depth() == CV_8U && dst.depth() == CV_32F)
		getQuadrangleSubPix_8u32f_CnR(src.data, src.step, src.size(),
		(float*)dst.data, dst.step, dst.size(),
		matrix, src.channels());
	else
	{
		CV_Assert(src.depth() == dst.depth());
		cv::warpAffine(src, dst, M, dst.size(),
			cv::INTER_LINEAR + cv::WARP_INVERSE_MAP,
			cv::BORDER_REPLICATE);
	}

}
/*
 * @brief Extract a matrix image from a given matrix image with size of given Rotated Rectangle
 * @param rr given Rotated Rectangle
 * @param img given matrix image to extract from
 */
static void getRotRectImg(cv::RotatedRect rr, Mat &img, Mat& dst)
{

	Mat m(2, 3, CV_64FC1);
	float ang = rr.angle*CV_PI / 180.0;
	//std::cout <<"angle is "<< ang<<std::endl;
	m.at<double>(0, 0) = cos(ang);
	m.at<double>(1, 0) = sin(ang);
	m.at<double>(0, 1) = -sin(ang);
	m.at<double>(1, 1) = cos(ang);
	m.at<double>(0, 2) = rr.center.x;
	m.at<double>(1, 2) = rr.center.y;
	myGetQuadrangleSubPix(img, dst, m);
}
/* @brief Extract a matrix image from a given matrix image with size of given Rotated Rectangle 
 * and save that extracted image to a vector of matrix
 * @param gray_image a given image to extract from
 * @param squares given vector a contours to extract from
 * @param extracted_matrices a vector of matrices that caller provide for storage
 */
void extract_image_from_square_contours(const Mat& gray_image, vector<vector<Point> >& squares, vector<Mat> &extracted_matrices, int program_mode)
{
	/* get a random color for rotated boxes and save in [Scalar color] */
	RNG rng(12345);
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	/**************************************************************************************/

	/*create bounding rotated boxes for each contours*/
	vector<RotatedRect> minRect(squares.size());
	for (size_t i = 0; i < squares.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(squares[i]));
	}
	if (program_mode & SHOW_IMAGE)
	{
		Mat boxed_image = gray_image.clone();
		for (int i = 0; i < squares.size(); i++)
		{
			//draw rotated rectangle
			Point2f rect_points[4]; minRect[i].points(rect_points);
			for (int j = 0; j < 4; j++)
			{
				line(boxed_image, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
			}
		}
		namedWindow("rotated_rectangle", CV_WINDOW_AUTOSIZE);
		imshow("rotated_rectangle", boxed_image);
	}
	/*** rotatedRect is now save in [vector<RotatedRect> minRect(squares.size())]********************************/

	// extract each rotated image
	Mat clone_of_gray_image = gray_image.clone();
	for (int i = 0; i < minRect.size(); i++)
	{

		Mat extracted_image = Mat(minRect[i].size, CV_8UC1);
		getRotRectImg(minRect[i], clone_of_gray_image, extracted_image);
		if (program_mode & SHOW_DEBUG)
		{
			std::cout << "numberofrow" << extracted_image.rows << "numberofcol" << extracted_image.cols;
		}
		
		/*discard pad of image*/
		int pad_of_image_in_row = extracted_image.rows / DISCARD_PERCENT_OF_IMAGE;
		int pad_of_image_in_col = extracted_image.cols / DISCARD_PERCENT_OF_IMAGE;
		/*end of discarding pad of image*/
		Mat partofextracted_image = extracted_image(Range(pad_of_image_in_row, extracted_image.rows - pad_of_image_in_row), Range(pad_of_image_in_col, extracted_image.cols - pad_of_image_in_col));
		extracted_matrices.push_back(partofextracted_image);/*storage extracted image in vector*/
		//std::cout << "push_back one more matrix";
		if (program_mode & SHOW_IMAGE)
		{
			/* get new name for each windows */
			std::stringstream sstm;
			sstm << "extract of image" << i;
			std::string windowsname = sstm.str();
			std::stringstream sstm1;
			sstm1 << RELATIVE_DIRECTORY_OF_EXTRACTED_IMAGE << i;
			std::string pathname = sstm1.str();
			/* show and save image*/
			namedWindow(windowsname, CV_WINDOW_AUTOSIZE);
			imshow(windowsname, partofextracted_image);
			imwrite(pathname + ".jpg", partofextracted_image);
		}
		
		
	}
}