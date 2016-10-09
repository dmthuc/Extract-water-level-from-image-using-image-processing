/* This program is for processing an input image to extract information of water level
* @brief File contain main function of program that take an input picture have information of water level
* and use some image processing algorithm to extract this information
*
* @author Dao Minh Thuc
*/
#ifndef _DEFINES_H
#define _DEFINES_H

/* user define type that indicate the mode that the program will run
 * please refer to top comments of main.cpp for more information
 */
enum Program_mode { IMPLICIT = 0, EXPLICIT, DEBUG_MODE };

/* values provide for Canny detector*/
const int CANNY_LOW_THRESHOLD = 0;
const int CANNY_MAX_THRESHOLD = 255;

/* value provide for finding square process */
const int NUMBBER_OF_SIDE_IN_A_SQUARE = 4;
const int MIN_AREA_IN_A_SQUARE = 1000;
const double MAX_COSIN_OF_THE_ANGLE_BETWEEN2EDGE = 0.25;

/* number of sample images for contours matching*/
const int NUMBER_OF_SAMPLE_IMAGES = 6;

/* percent of image that will be discard */
const int DISCARD_PERCENT_OF_IMAGE = 7;

/* relative path to store extracted_image */
const std::string RELATIVE_DIRECTORY_OF_EXTRACTED_IMAGE = ".\\extract_images\\extract_of_image";

/* relative path to store sample_image */
const std::string RELATIVE_DIRECTORY_OF_SAMPLE_IMAGE = ".\\sample_images\\image_sample";

/* define window parameters for shape matching*/
const double TOP_WINDOW = 2.0;
const double BOT_WINDOW = 0.0001;
const double WINDOW_THRESHOLD = 0.09;

/*define start number of sample image*/
const int START_NUMBER = 3;

/* !define model file path for neutron network*/
const char MODEL_PATH[] = "C:\\Users\\Thuc\\Documents\\Visual Studio 2013\\Projects\\Project1\\Debug\\neutron_network\\param.xml";
/* !define model name*/
const char MODEL_NAME[] = "DigitOCR";

/* !define parameter for matrix input that will be directly processed by nnetwork */
#define ATTRIBUTES 256  //Number of pixels per sample 16X16
#define CLASSES 10      //Number of distinct labels.

/* define binary threshold use in image_cropper*/
const int BINARY_THRESHOLD = 140;

//const std::string PATH_OF_GEN_IMAGE = "C:\\Users\\Thuc\Documents\\Visual Studio 2013\\Projects\\Project1\\Debug\\neutron_network\\genimg\\";
#endif
