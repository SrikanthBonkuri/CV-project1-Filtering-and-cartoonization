
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdint.h>
int greyscale(cv::Mat& src, cv::Mat& dst);
int blur5x5(cv::Mat& src, cv::Mat& dst);
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);
int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold);
using namespace cv;
int main()
{
cv::VideoCapture video(0);
if (!video.isOpened()) { return -1; }
cv::Mat frame;
int frame_width = (int)video.get(cv::CAP_PROP_FRAME_WIDTH);
int frame_height = (int)video.get(cv::CAP_PROP_FRAME_HEIGHT);
cv::VideoWriter output("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(frame_width, frame_he
ight));
char k = 'g';
while (video.read(frame)) {
if (frame.empty()) {
printf("frame is empty\n");
break;
}
//cvtColor(frame, frame, COLOR_BGR2GRAY);
cv::imshow("Video feed", frame);
// pick another effect to implement on your video, task 10 xtra features implementation.
{
//sparkles
//increase brightness to images
Mat imageBrighnessHigh50;
frame.convertTo(imageBrighnessHigh50, -1, 1, 50); //increase the brightness by 50
cv::imwrite("imageBrighnessHigh50.jpg", imageBrighnessHigh50);
//stretch or wrap the image
//change around the color palette or make the image negative of itself
Mat negativeimage;
bitwise_not(frame, negativeimage);
cv::imwrite("negativeimage.jpg", negativeimage);
//other types of blur filters
Mat blurimage,blurimage1;
int sigma = 3; int ksize = (sigma * 5) | 1;
//GaussianBlur(frame, blurimage, Size(ksize, ksize), sigma, sigma);
blur(frame, blurimage, Size(ksize, ksize));
medianBlur(frame, blurimage1, ksize);
cv::imwrite("blurimage.jpg", blurimage);
cv::imwrite("blurimage1.jpg", blurimage1);
}
if (k == 's') { output.write(frame); } // save video
char key = cv::waitKey(25);
if (key == 'q') {
break;
}
if (key == 's') {
k = key;
cv::imwrite("frame.jpg", frame); // save image
}
if (key == 'g') {
// converting frame to grayscale and saving original and grayscale images ==> 3
Mat grayframe;
cvtColor(frame, grayframe, COLOR_BGR2GRAY);
cv::imwrite("grayscale.jpg", grayframe);
cv::imwrite("originalframe.jpg", frame);
}
if (key == 'h') {
//alternate greyscale video ==> 4
Mat graysrc, graydst;
frame.copyTo(graysrc); frame.copyTo(graydst);
greyscale(graysrc, graydst);
}
if (key == 'b') {
//gaussian filter, show blurred image if user types b ==> 5
Mat gaussiansrc, gaussiandst;
frame.copyTo(gaussiansrc); frame.copyTo(gaussiandst);
blur5x5(gaussiansrc, gaussiandst);
}
if (key == 'm') {
// sobel Support GrayScale images
Mat sobeldstx, sobeldsty, grayframe;
cvtColor(frame, grayframe, COLOR_BGR2GRAY);
grayframe.copyTo(sobeldstx); grayframe.copyTo(sobeldsty);
sobelX3x3(grayframe, sobeldstx);
sobelY3x3(grayframe, sobeldsty);
// magnitude
magnitude(sobeldstx, sobeldsty, grayframe);
}
if (key == 'l') {
// blur and quantize
Mat blursrc, blurdst;
frame.copyTo(blursrc); frame.copyTo(blurdst);
blurQuantize(blursrc, blurdst, 10);
}
if (key == 'c') {
// cartoon
{
Mat cartoonsrc, cartoondst;
frame.copyTo(cartoonsrc); frame.copyTo(cartoondst);
cartoon(cartoonsrc, cartoondst, 10, 15);
}
}
}
output.release();
video.release();
cv::destroyAllWindows();
return 0;
}