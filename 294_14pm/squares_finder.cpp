/* This find is a part of program that take an input picture contain information about water level and extract it
 * @brief This find contain method that help find square contours in a given grayscale image
 * @author Dao Minh Thuc
 * @date April 20, 2015
 */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>//for convert RGB image to GrayScale
#include <string>
#include "defines.h"
#include "squares_finder.h"
using namespace cv;


/*
 * helper function:
 * finds a cosine of angle between vectors
 * from pt0->pt1 and from pt0->pt2
 */
static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
/*******************************************************************/

/*
 * @brief the function draws all the squares in the image
 *
 */
static void drawSquares(Mat& image, const vector< vector<Point> >& squares)
{
	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, CV_AA);
	}
	namedWindow("rectangledetector", CV_WINDOW_AUTOSIZE);
	imshow("rectangledetector", image);
}
/*****************************************************************************************************/

/* @brief This function take grayscale image as input then find squares contour in that image
 * @param squares : vector of contour that caller provide for function as a place to store contours that have square shape
 * @param program_mode : to select if the program will run implicitly or not- refer to
 */
void findSquares(const Mat& gray_image, vector<vector<Point> >& squares, \
	const int low_threshold, const int max_threshold, int program_mode)
{
	double rightArea = MIN_AREA_IN_A_SQUARE;
	double size_of_contour = 0;
	squares.clear();
	vector<vector<Point> >contours;//storage contours
	vector<Point> approx;//storage approximate shape--store vertexs
	vector<Vec4i> hierarchy;
	/******************************** end of declaration ****************************************/

	/* get a random color for rotated boxes and save in [Scalar color] */
	RNG rng(12345);
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	/**************************************************************************************/

	/* Detecting edge using Canny method*/
	Mat edge_detected_image;
	Canny(gray_image, edge_detected_image, low_threshold, max_threshold);
	//add new line
	//dilate(edge_detected_image, edge_detected_image, Mat(), Point(-1, -1));
	if (program_mode & SHOW_IMAGE)
	{
		namedWindow("Canny edge detection window", CV_WINDOW_AUTOSIZE);
		imshow("Canny edge detection window", edge_detected_image);
	}
	/*************** End of edge detecting - edge detected image storage in [Mat edge_detected_image]**************/

	/* Finding contour in edge detected image and draw them- contours return is save in [vector<vector<Point> >contours ] ***/
	findContours(edge_detected_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	if (program_mode & SHOW_IMAGE)
	{
		Mat drawing = Mat::zeros(edge_detected_image.size(), CV_8UC1);/* create an empty matrix to draw contours */

		/* Drawing*/
		for (size_t i = 0; i < contours.size(); i++)
		{
			drawContours(drawing, contours, (int)i, color, 1, 8, hierarchy, 0, Point());
		}
		namedWindow("Contours window", WINDOW_AUTOSIZE);
		imshow("Contours window", drawing);
	}
	/*******************End of finding contour- find contours is storage in [vector<vector<Point> >contours] ****/

	/*Finding rectangle in vector contours- each of contour is appoximate into [vector<Point> appox] then test if
	each appox is a square. If it is a square then push it into [vector<vector<Point>> squares ] */
	for (size_t i = 0; i < contours.size(); i++)
	{
		// approximate contour with accuracy proportional
		// to the contour perimeter
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		//approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, false);
		//find area of right square to filter square inside 8 figure
		if (program_mode & SHOW_DEBUG)
		{
			if (fabs(contourArea(Mat(approx))) > MIN_AREA_IN_A_SQUARE &&
				isContourConvex(Mat(approx)))
			{
				std::cout << "number of side" << approx.size() << std::endl;
			}
			if (isContourConvex(Mat(approx)))
			{
				//std::cout << "number of side" << approx.size() << std::endl;
				//std::cout << "area of contour satify 1 criation " << fabs(contourArea(Mat(approx))) << std::endl;
			}
		}
		if (approx.size() == NUMBBER_OF_SIDE_IN_A_SQUARE &&
			fabs(contourArea(Mat(approx))) > MIN_AREA_IN_A_SQUARE &&
			isContourConvex(Mat(approx)))
		{
			double maxCosine = 0;
			if (program_mode & SHOW_DEBUG)
			{
				std::cout << "area of each rectangle satify 3 criterions :" << fabs(contourArea(Mat(approx)))<<std::endl;/*for debug*/
			}

			for (int j = 2; j < 5; j++)
			{
				// find the maximum cosine of the angle between joint edges
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}

			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence
			if (maxCosine < MAX_COSIN_OF_THE_ANGLE_BETWEEN2EDGE)
			{
				if (fabs(contourArea(Mat(approx))) > rightArea)
				{
					rightArea = fabs(contourArea(Mat(approx)));
				}
			}

		}
	}
	if (program_mode & SHOW_DEBUG)
	{
		std::cout << "right area is:" << rightArea/1.5;
	}
	for (size_t i = 0; i < contours.size(); i++)
	{
		// approximate contour with accuracy proportional
		// to the contour perimeter
		//approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		//approxPolyDP(Mat(contours[i]), approx, 3, true);
		//find square base on addition area fingure
		if (approx.size() == NUMBBER_OF_SIDE_IN_A_SQUARE &&
			fabs(contourArea(Mat(approx))) > (rightArea / 1.5) &&
			isContourConvex(Mat(approx)))
		{
			double maxCosine = 0;

			for (int j = 2; j < 5; j++)
			{
				// find the maximum cosine of the angle between joint edges
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}

			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence
			if (maxCosine < MAX_COSIN_OF_THE_ANGLE_BETWEEN2EDGE)
			{
				if (program_mode & SHOW_DEBUG)
				{
					std::cout << "size of right contour is " << contours[i].size() << std::endl;
					std::cout << "length of right contour is " << arcLength(contours[i], true) << "and area correspond is" << fabs(contourArea(Mat(approx))) << std::endl;
					
				}
				squares.push_back(approx);
				if (contours[i].size()>size_of_contour)
					size_of_contour = contours[i].size();
			}
				
		}
	}
	if (program_mode & SHOW_IMAGE)
	{
		Mat rectangledetector = edge_detected_image.clone();
		for (size_t i = 0; i < squares.size(); i++)
		{
			const Point* p = &squares[i][0];
			int n = (int)squares[i].size();
			polylines(rectangledetector, &p, &n, 1, true, color, 3, CV_AA);
		}
		namedWindow("rectangledetector");
		imshow("rectangledetector", rectangledetector);
	}
	/*************************************************************************************/

	/*more way to find rectangle, using min rotated rectangle*/
	if (squares.size() == 0)
	{
		return;
	}
	/// Find the rotated rectangles for each contour
	vector<RotatedRect> minRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(contours[i]));
	}
	/// Draw contours + rotated rects + ellipses
	Mat drawing = gray_image.clone();;/* create an empty matrix to draw contours */
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 0, 255);
		// contour
		//drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		// rotated rectangle
		
		
		Size2f size = minRect[i].size;
		double ratio = std::abs(1 - (double)size.width / size.height);// calculate ratio between width and height of rectangle
		if (ratio < SIDE_RATIO)//check if rectangle is square or not
		{
			/* convert Rotated rect to polygon*/
			vector<Point> appox1;
			Point2f rect_points[4];
			minRect[i].points(rect_points);
			for (int i = 0; i < 4; i++)
			{
				appox1.push_back(rect_points[i]);
			}
			//test if it square;
			double side_length = arcLength(contours[i], false) / 4;
			double area = fabs(contourArea(Mat(appox1)));

			
			//if (area>(side_length*side_length / 2) && area > MIN_AREA_IN_A_SQUARE)
			if (fabs(contourArea(Mat(appox1))) >(rightArea / 1.5) && fabs(contourArea(Mat(appox1))) <(rightArea *1.3) && contours[i].size() <(size_of_contour*3))
			{
				squares.push_back(appox1);
				if (program_mode & SHOW_IMAGE)
				{
					for (int j = 0; j < 4; j++)
						line(drawing, appox1[j], appox1[(j + 1) % 4], color,2, 8);
				}

				if (program_mode & SHOW_DEBUG)
				{
					std::cout << "size of right contour is " << contours[i].size() << std::endl;

				}
			}
		}



	}

	
	if (program_mode & SHOW_IMAGE)
	{
		namedWindow("rectangle detector 2", WINDOW_AUTOSIZE);
		imshow("rectangle detector 2", drawing);
	}
}


