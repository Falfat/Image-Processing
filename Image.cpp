#include <Image.h>
using namespace cv;
using namespace std;

int Image::get_width(Mat pix) {
	return pix.rows;
}

int Image::get_height(Mat pix) {
	return pix.cols;
}



