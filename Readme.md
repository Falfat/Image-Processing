## Image Processing Using C++
=============================

Creator: Yusuf Falola

This is a C++ software created for enhancing and modifying medical images in different ways using filters. Five filters were created in this study, namely; Threshold filter, Four-point averaging filter, Laplacian filter, Median filter and Gaussian filter. Grayscale medical images were considered in this work. An external C++ library named OpenCV was employed to load, open and save images. This library also helped to generate the image matrix, which is needed for the filtering algorithms to process the images. 

## Filters Considered

- Thresholding filter

- Four-point averaging filter

- Laplacian Filter

- Median Filter

- Gaussian Blur filter


## Usage

- The OpenCV-3.1.0 library used can be downloaded here https://opencv.org/releases.html. The installation guide for this library is well explained in this video https://www.youtube.com/watch?v=l4372qtZ4dc. 

- This GitHub page should be cloned and Then folder directory should be included after opening the project, by right clicking on project >> properties >> C/C++ >> General >> Additional Include Directories. 

- This code is executed by first creating an object of class BMPImage, PngImage or JpegImage to be processed. User should type the < imagename.imagetype> in the imread() function, then run.

- The code should be run in x64 mode which is compatible with the library used. Then the user interface will pop up, requiring the user to choose the type of filter desired, by entering numbers 1-5.

- Then two images will be shown; original and filtered. Also, a jpeg format of the filtered image will be saved in the project folder. Also, the format of the original image will be displayed on the console. 

- If a wrong number is typed, an error message will be shown telling the user to type a correct number. Typing an image name not saved in the project folder will lead to display of an error notification on the interface. Three images of .jpg, .png and .bmp format have been provided for testing.

## References

- https://en.wikipedia.org/wiki/Thresholding_(image_processing), Accessed 10th, February 2019.


- 2.	Choi-hong, L & Meiqing, W (2009). A Concise Introduction to Image Processing using C++. (1st ed.). London, UK: CRC press.










