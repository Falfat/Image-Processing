#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <math.h>

class Image {

public:

	int get_height(cv::Mat pix);
	int get_width(cv::Mat pix);

};

class JpegImage : public Image
{	
public:
	std::string get_type() { return "Jpeg Image"; }
};

class PngImage : public Image
{
public:
	std::string get_type() { return "PNG Image"; }
};

class BMPImage : public Image
{
public:
	std::string get_type() { return "BMP Image"; }
};

