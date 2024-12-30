#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
int main()
{
std::string image_path = samples::findFile("C:/Users/srika/Downloads/rose4.jpg");
Mat img = imread(image_path, IMREAD_COLOR);
if (img.empty())
{
std::cout << "Could not read the image: " << image_path << std::endl;
return 1;
}
imshow("Display window", img);
int k = waitKey(0); // Wait for a keystroke in the window
// if key equal to q exit from the window
if (k == 'q') {
exit;
}
// if key equal to s we are saving the image to the below mentioned location
if (k == 's')
{
imwrite("rose1.png", img);
}
return 0;
} 