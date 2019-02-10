#include <Image.h>

using namespace cv;
using namespace std;

//Declaration of filter functions
void threshold_filter(int, int, Mat);
void averaging_filter(int, int, Mat);
void Laplacian_filter(int, int, Mat);
void median_filter(int, int, Mat);
void Gaussianblur(int, int, Mat);


int main(int argv, char** argc) {

	//Create an object of class "BPMImage" or "PngImage" or "JpegImage".
	//The object created should match the image type to be filtered.
	BMPImage picture;

	//Reads the picture to be filtered using the OpenCV library and save it as Opencv Mat(Matrix) type.
	//This matrix conatains the pixel values of the image loaded.
	//The first argument of the imread function is the <ImageName.ImageType>
	//The second argument is how you want to load the image. For instance, 
	//you might load a colored image as greysclae using CV_LOAD_IMAGE_GRAYSCALE.
	cv::Mat pix = cv::imread("medical_image1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	
	//This checks if image found in folder and opened successfully
	if (pix.empty())
		std::cout << "Image failed to open, Image not found! Put image in project folder." << std::endl;
	else
		std::cout << "Image opened fine. You can continue" << std::endl;

	//Getting the image height and width from the class. These values will be needed by our filters.
	int h = picture.get_height(pix);
	int w = picture.get_width(pix);

	//Getting the type of Image loaded, using the get_type() member of the class of objected created
	string type = picture.get_type();


	//User-Interface guide
	std::cout << "\n" <<"Select the type of filter you want" << std::endl;
	std::cout << "Enter 1 for Threshold filter" << std::endl;
	std::cout << "Enter 2 for Four-point averaging filter" << std::endl;
	std::cout << "Enter 3 for Laplacian filter" << std::endl;
	std::cout << "Enter 4 for Median filter" << std::endl;
	std::cout << "Enter 5 for GuassianBlur filter" << std::endl;

	//Declaring integer to be used by switch conditional statement.
	int x;
	
	std::cin >> x;

	//Switch conditional- to select and execute the user defined filter
	switch (x) {
	//This case is executed if the user enters "1"
	case 1:
		threshold_filter(h, w, pix);
		std::cout << "type of picture is: " << type << std::endl;
		break;
	//This case is executed if the user enters "2"
	case 2:
		averaging_filter(h, w, pix);
		std::cout << "type of picture is: " << type << std::endl;
		break;
	//This case is executed if the user enters "3"
	case 3:
		Laplacian_filter(h, w, pix);
		std::cout << "type of picture is: " << type << std::endl;
		break;
	//This case is executed if the user enters "4"
	case 4:
		median_filter(h, w, pix);
		cout << "type of picture is: " << type << std::endl;
		break;
	//This case is executed if the user enters "5"
	case 5:
		Gaussianblur(h, w, pix);
		std::cout << "type of picture is: " << type << std::endl;
		break;
	//This case is executed if the user enters any number outside of 1-5
	default:
		std::cout << "wrong value entered, please enter numbers between 1 and 5" << std::endl;

	}


	//The waitkey() function ensures the image remains on the screen until the user closes it.
	//The number of milliseconds could be specified in the bracket. For instance waitkey(100)
	//for 100 milliseconds.
	cv::waitKey();

	system("pause");
}

void save_copy(Mat filt_pic) {
	//Mat filt_pics = imread("filt_pic");
	imwrite("filtered_copy.jpg", filt_pic);
}



//A Function for threshold filter
/*The filter sets the value of all pixel values less than 128 to 0(black), and pixel values otherwise to 255(white).
128 is used, because it is an average value for the pixel values. 

This function and others use the .at function in open cv, which allows us to access the pixel values in the matrix of an image.
The .at function is templated. Hence, we used the uint8_t which is capable of storing integer values between 0-255
This was why we #included stdint.h in the Image.h file."unsigned char" could be use in place of "uint8_t"
*/
void threshold_filter(int h, int w, Mat pic) {
	//clone the input image to be shown later as original
	Mat original = pic.clone();

	//iterate over the rows of the image
	for (int r = 0; r < w; r++) 
	{
		//iterate over the column of the image
		for (int c = 0; c < h; c++) 
		{
			//check if pixel value is less than 128 and set to black
			if (pic.at<uint8_t>(r, c) < 128) {
				pic.at<uint8_t>(r, c) = 0;
			}
			else
			{
			//if pixel value is not less than 128, set as white
				pic.at<uint8_t>(r, c) = 255;
			}
			
		}
	}

	//show filtered copy.
	cv::imshow("filtered_copy", pic);
	//show original copy for comparison.
	cv::imshow("original_copy", original);
	//save filtered copy in our project folder.
	cv::imwrite("threshold_filtered_copy.jpg", pic);
	
}


//A function for the Four-Point average threshold filter.
/*This is a four point neighbouring average technique. The contribution of the four neighbours of a given pixel
	are used to redefine the value of the pixel. A 3x3 widow is used. The convolution kernel h(m,n) is a shown below;

	|0    1/4    0|
	|1/4   0   1/4|
	|0    1/4    0|

we therefore obtain;
	g(i,j) = 1/4[f(i-1,j) + f(i+1,j) + f(i,j-1) + f(i,j+1)]

g(i,j) value being the new value for f(i,j).

*/
void averaging_filter(int h, int w, Mat pic) {
	//clone the input image to be used in filtering process
	Mat pic_4 = pic.clone();

	//clone the input image to be shown later as original
	Mat original = pic.clone();

	//iterate over the rows of image matrix. 
	//The last rows and columns on both sides are avoided, we assume non periodicity for simplicity.
	// Moreover, the image matrix is so large that it is save to avoid the last rows and columns.

	for (int r = 1; r < w-1; r++)
	{
		//iterate over columns of image matrix
		for (int c = 1; c < h-1; c++)
		{
			//Apply the convolution kernel to re-define the pixel vale being considered.
			 int sum = (1 / 4)*pic.at<uint8_t>(r - 1, c) + (1 / 4)*pic.at<uint8_t>(r + 1, c) +
				(1/4)* pic.at<uint8_t>(r, c - 1) + (1.4)*pic.at<uint8_t>(r, c + 1);

			 //set the new value for the pixel being considered.
			 //saturate cast ensures there is no overflow, i.e no value greater than 255 or less than 0 are set as 
			 //pixel value. If sum>255, it sets it to 255 and if sum<0, it sets it to 0.
			pic_4.at<uint8_t>(r, c) = cv::saturate_cast<uint8_t>(sum);

		}
	}
	//show filtered copy.
	cv::imshow("filtered_copy.jpg", pic_4);
	//show original copy for comparison.
	cv::imshow("original_copy", original);
	//save filtered copy in our project folder.
	cv::imwrite("averaging_filtered_copy.jpg", pic_4);
}

//Function to define Laplacian filter.
/*This method of filtering is arrived at by taking the second partial derivative(Laplacian) of the pixel value
	location f(x,y), then discretizing the Laplacian. One therefore arrives at the convolution kernel for the Laplacian
	operator as follows;

	|0    1   0|
	|1   -4   1|
	|0    1   0|

we therefore obtain;
	g(i,j) = f(i-1,j) + f(i+1,j) + f(i,j-1) + f(i,j+1) - 4*f(i,j)
g(i,j) value being the new value for f(i,j).

*/
void Laplacian_filter(int h, int w, Mat pic)
{
	//clone the input image to be used in filtering process
	Mat result = pic.clone();

	//clone the input image to be shown later as original
	Mat original = pic.clone();

	//iterate over rows of image matrix, avoiding the egdes
	for (int r = 1; r < w - 1; r++) {

		//iterate over columns of image matrix, avoiding the edges
		for (int c = 1; c < h - 1; c++) {
			//calculate the "Laplacian Value"
			int lap = pic.at<uint8_t>(r - 1, c) + pic.at<uint8_t>(r, c + 1) + pic.at<uint8_t>(r + 1, c)
				+ pic.at<uint8_t>(r, c - 1) - 4 * pic.at<uint8_t>(r, c);

			//set the new value for the pixel being considered.
			//saturate cast ensures there is no overflow, i.e no value greater than 255 or less than 0 are set as 
			//pixel value. If sum>255, it sets it to 255 and if sum<0, it sets it to 0.
			result.at<uint8_t>(r, c) = cv::saturate_cast<uint8_t>(lap);
		}
	}
	//show filtered copy.
	cv::imshow("Laplacian_filtered.jpg", result);
	//show original copy for comparison.
	cv::imshow("original_copy", original);
	//save filtered copy in our project folder.
	cv::imwrite("Laplacian_filtered_copy.jpg", result);
}


//Function to define Median Filter
/*Median filtering is a nonlinear smoothing method. It sorts the pixel values in the neighbourhood window of the
 reference pixel and selects the median value of the sorted data. The original value at the reference
pixel is then replaced by the median value.

For the grayscale image being considered, which is a two dimensional image processing. A 3x3 processing window 
was used. For instance, consider the image matrix below;

    |13    61   70   45   67|
	|46    64   61   56   78|
f =	|60    100  90   45   23| consider the 90 pixel value, the 3x3 widow becomes; [64,61,56,100,90,45,105,94,11]
	|70    105  94   11   12| After sorting, the widow is: [11,45,56,61,64,90,94,100,105], Threrefore, the pixel
	|91    101  96   45   34| value 90 is replaced by the median value 64.

*/
void median_filter(int h, int w, Mat pic) {
	//clone the input image to be used in filtering process
	Mat result = pic.clone();

	//clone the input image to be shown later as original
	Mat original = pic.clone();

	//Declare an array to hold 9 integers
	int med_arr[9];

	//Determine integer n, required for sorting using std::sort
	int n = sizeof(med_arr) / sizeof(med_arr[0]);

	//iterate over rows of image matrix, avoiding the edges
	for (int r = 1; r < w - 1; r++) {
		//iterate over columns of image matrix, avoiding the edges
		for (int c = 1; c < h - 1; c++) {
			//fill the array with values to be sorted
			med_arr[0] = pic.at<uint8_t>(r-1, c-1);
			med_arr[1] = pic.at<uint8_t>(r , c-1);
			med_arr[2] = pic.at<uint8_t>(r + 1, c - 1);
			med_arr[3] = pic.at<uint8_t>(r - 1, c );
			med_arr[4] = pic.at<uint8_t>(r, c);
			med_arr[5] = pic.at<uint8_t>(r+1, c);
			med_arr[6] = pic.at<uint8_t>(r - 1, c + 1);
			med_arr[7] = pic.at<uint8_t>(r, c + 1);
			med_arr[8] = pic.at<uint8_t>(r + 1, c+1);

			//sort the values
			std::sort(med_arr, med_arr+n, greater<int>());

			//replace the pixel under consideration with the median value
			result.at<uint8_t>(r, c) = med_arr[4];

		}
	}
	
	//show filtered copy.
	imshow("median_filtered.jpg", result);
	//show original copy for comparison.
	imshow("original_copy", original);
	//save filtered copy in our project folder.
	imwrite("median_filtered_copy.jpg", result);
}

//Function to define Gaussian Blur Filter.
/*Gaussian smoothing filtering is another linear filtering method used in the spatial domain. It is mainly used to
blur images and remove noise.
Its convolution kernel is given as;

G(x,y) = (1/2xpixsigma^2) x exp-(((x-x0)^2 + (y-y0)^2)/2xsigma^2)

where sigma is a smoothing parameter used to control the extent of smoothing. The larger the value of sigma, 
the greater is the extent of smoothing.

Here, the Gaussian Kernel is transformed to a 5x5 Matrix. The kernel elements are then used to multiply
the corresponding elements in 5x5 matrix of a reference pixel value and its neighors. The sum of all
the values gives the new value for the referenced pixel.


*/
void Gaussianblur(int h, int w, Mat pic) {
	//clone the input image to be used in filtering process
	Mat result = pic.clone();

	//clone the input image to be shown later as original
	Mat original = pic.clone();

	/*Start to create Guassian Kernel*/
	//Declaring and initializing variables to create Guassian kernel
	int sigma = 2;
	const int kernel_width = 5;
	const int kernel_height = 5;
	const double pi = 3.14159;

	//Initialize kernel Matrix(array of arrays)
	double kernel_arr[kernel_height][kernel_width];
	//Initialize sum to be used for normalizing kernel
	double sum = 0.0;


	//iterate over row of kernel
	for (int row = 0; row < kernel_width; row++) {
		//iterate over column of kernel
		for (int col = 0; col < kernel_height; col++)
		{
			//calculate kernel values
			double amplitude = 1 / (2 * pi* pow(sigma, 2));
			float val = amplitude * exp(-(pow(row - kernel_width / 2, 2) + pow(col - kernel_height / 2, 2)))/
				(2*pow(sigma,2));
			//insert value into position in the matrix
			kernel_arr[row][col] = val;
			//sum values
			sum += val;
		}
	}

	//Normalize Guassian kernel by dividing through by sum of all elements
	for (int row = 0; row < kernel_width; row++) {
		for (int col = 0; col < kernel_height; col++)
		{
			kernel_arr[row][col] = kernel_arr[row][col]/sum;
		}
	}
	/*Guassian kernel created*/

	/*Apply Guassian Kernel to Image matrix*/

	//iterate over rows of image matrix, avoiding the 2 layer egdes, since we are using a 5x5 matrix
	for (int row = 2; row < w-2; row++) {
		//iterate over columns of image matrix, avoiding the 2 layer egdes, since we are using a 5x5 matrix
		for (int col = 2; col < h-2; col++)
		{
			//initiate new pixel value
			float val = 0.0;

			//iterate over row of kernel
			for (int kernel_row = 0; kernel_row < kernel_width; kernel_row++) {
				//iterate over column of kernel
				for (int kernel_col = 0; kernel_col < kernel_height; kernel_col++) {
					//calculate new pixel value
					float pix = pic.at<uint8_t>(kernel_row + row - 2, kernel_col + col - 2)*
						kernel_arr[kernel_row][kernel_col];
					val += pix;
				}
			}
			//insert value into appropriate position. Cvround is used to roundoff values to a whole number
			result.at<uint8_t>(row, col) = cvRound(val);
		}
	}
	
	//show filtered copy.
	cv::imshow("Gaussian_filtered.jpg", result);
	//show original copy for comparison.
	cv::imshow("original_copy", original);
	//save filtered copy in our project folder.
	cv::imwrite("Gaussian_filtered_copy.jpg", result);

}