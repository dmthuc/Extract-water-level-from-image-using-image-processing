#ifndef DEFINES_H
#define DEFINES_H
#include <string>
using namespace std;
/* !define path to training set*/
const string TRAINING_PATH = "C:\\Users\\Thuc\\Documents\\Visual Studio 2013\\Projects\\Project1\\Debug\\neutron_network\\";
const int NUMBER_OF_SAMPLE_IN_EACH_SET = 90;

/* user define type that indicate the mode that the program will run
* please refer to top comments of main.cpp for more information
*/
enum Program_mode { QUIET = 0, GRAPHIC, DEBUG };

/* !Define parameters for training set*/
const int START_NUMBER_IN_TRAINNINGSET = 0;
const int END_NUMBER_IN_TRAINNINGSET = 9;
#endif
