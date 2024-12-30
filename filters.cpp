
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdint.h>
#include <iostream>
#include <math.h>
#include <typeinfo>
using namespace cv;
int greyscale(cv::Mat& src, cv::Mat& dst) {
for (int i = 0; i < src.rows; i++) {
for (int j = 0; j < src.cols; j++) {
int bv = src.at<Vec3b>(i, j)[0];
int gv = src.at<Vec3b>(i, j)[1];
int rv = src.at<Vec3b>(i, j)[2];
// adding green channel value to other 2 channels
dst.at<Vec3b>(i, j)[0] = dst.at<Vec3b>(i, j)[0] + gv;
dst.at<Vec3b>(i, j)[2] = dst.at<Vec3b>(i, j)[2] + gv;
}
}
cvtColor(dst, dst, COLOR_BGR2GRAY);
imwrite("greyscale-original.jpg", src);
imwrite("greyscale-alternativegreen.jpg", dst);
cv::imshow("feed1", dst);
waitKey(0);
return 0;
}
int blur5x5(cv::Mat & src, cv::Mat & dst) {
int filter[1][5] = { 1, 2, 4, 2, 1 };
int sum = 10;
//horizontal filter
for (int i = 0; i < src.rows; i++) {
for (int j = 0; j < src.cols; j++) {
int value = 0,k=0,f=0;
k = j; f = 2;
while (f < 5 && k < dst.cols) {
value = value + filter[0][f] * src.at<uint8_t>(i, k); k++; f++;
}
k = j-1; f = 1;
while (f >= 0 && k >=0) {
value = value + filter[0][f] * src.at<uint8_t>(i, k); k--; f--;
}
value = value / sum;
uint8_t y = value;
dst.at<uint8_t>(i, j) = y;
}
}
Mat src1;
dst.copyTo(src1);
src1 = abs(src1);
//vertical filter
for (int i = 0; i < src1.cols; i++) {
for (int j = 0; j < src1.rows; j++) {
int value = 0, k = 0, f = 0;
k = j; f = 2;
while (f < 5 && k < dst.rows) {
value = value + filter[0][f] * src1.at<uint8_t>(k, i); k++; f++;
}
k = j-1; f = 1;
while (f >= 0 && k >= 0) {
value = value + filter[0][f] * src1.at<uint8_t>(k ,i); k--; f--;
}
value = value / sum;
uint8_t y = value;
dst.at<uint8_t>(j, i) = y;
}
}
dst = abs(dst);
imwrite("gaussian-original.jpg", src);
imwrite("gaussian-filter.jpg", dst);
cv::imshow("feed1", dst);
waitKey(0);
return 0;
}
int sobelX3x3(cv::Mat& src, cv::Mat& dst) {
// Define Kernel
std::vector<int> kernel({ 1,0,-1,2,0,-2,1,0,-1 });
int kernel_size = 3;
unsigned char* data_in = (unsigned char*)(src.data);
unsigned char* data_out = (unsigned char*)(dst.data);
for (int i = 0; i < src.rows; i++) {
for (int j = 0; j < src.cols; j += 1) {
if (i <= kernel_size / 2 || i >= src.rows - kernel_size / 2 ||
j <= kernel_size / 2 || j >= src.cols - kernel_size / 2) {
data_out[dst.step * i + j] = 0;
continue;
}
int sum = 0;
int k_ind = 0;
for (int k_row = -kernel_size / 2; k_row <= kernel_size / 2; ++k_row) {
for (int k_col = -kernel_size / 2; k_col <= kernel_size / 2; ++k_col) {
sum += kernel[k_ind] * data_in[src.step * (i + k_row) + j + k_col];
k_ind++;
}
}
data_out[dst.step * i + j] = std::max(std::min(sum, 255), 0);
}
}
imwrite("sobel-X-original.jpg", src);
imwrite("sobel-x-filter.jpg", dst);
cv::imshow("feed1", dst);
waitKey(0);
return 0;
}
int sobelY3x3(cv::Mat& src, cv::Mat& dst) {
// Define Kernel
std::vector<int> kernel({ 1,2,1,0,0,0,-1,-2,-1 });
int kernel_size = 3;
unsigned char* data_in = (unsigned char*)(src.data);
unsigned char* data_out = (unsigned char*)(dst.data);
for (int i = 0; i < src.rows; i++) {
for (int j = 0; j < src.cols; j += 1) {
if (i <= kernel_size / 2 || i >= src.rows - kernel_size / 2 ||
j <= kernel_size / 2 || j >= src.cols - kernel_size / 2) {
data_out[dst.step * i + j] = 0;
continue;
}
int sum = 0;
int k_ind = 0;
for (int k_row = -kernel_size / 2; k_row <= kernel_size / 2; ++k_row) {
for (int k_col = -kernel_size / 2; k_col <= kernel_size / 2; ++k_col) {
sum += kernel[k_ind] * data_in[src.step * (i + k_row) + j + k_col];
k_ind++;
}
}
data_out[dst.step * i + j] = std::max(std::min(sum, 255), 0);
}
}
imwrite("sobel-Y-original.jpg", src);
imwrite("sobel-Y-filter.jpg", dst);
cv::imshow("feed1", dst);
waitKey(0);
return 0;
}
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst) {
for (int i = 0; i < sx.rows; i++) {
for (int j = 0; j < sx.cols; j++) {
int sx_v = sx.at<uint8_t>(i, j);
int sy_v = sy.at<uint8_t>(i, j);
int value = sqrt(sx_v * sx_v + sy_v * sy_v);
int8_t y = value;
dst.at<int8_t>(i, j) = value;
}
}
imwrite("magnitude-sx-original.jpg", sx);
imwrite("magnitude-sy-original.jpg", sy);
imwrite("magnitude-filter.jpg", dst);
cv::imshow("feed1", dst);
waitKey(0);
return 0;
}
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels) {
// using gaussian blurred image from the above functions
cv::GaussianBlur(src, src, Size(5, 5), 0, 0);
int b = (int)255 / levels;
for (int i = 0; i < src.rows; i++) {
for (int j = 0; j < src.cols; j++) {
dst.at<Vec3b>(i, j)[0] = ((src.at<Vec3b>(i, j)[0] / b) * b);
dst.at<Vec3b>(i, j)[1] = ((src.at<Vec3b>(i, j)[1] / b) * b);
dst.at<Vec3b>(i, j)[2] = ((src.at<Vec3b>(i, j)[2] / b) * b);
}
}
imwrite("blurQuantize-original.jpg", src);
imwrite("blurQuantize-filter.jpg", dst);
cv::imshow("feed1", dst);
waitKey(0);
return 0;
}
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold) {
Mat sobeldstx, sobeldsty, sobelsrcx, sobelsrcy, grayframe;
cvtColor(src, grayframe, COLOR_BGR2GRAY);
grayframe.copyTo(sobeldstx); grayframe.copyTo(sobeldsty);
grayframe.copyTo(sobelsrcx); grayframe.copyTo(sobelsrcy);
sobelX3x3(sobelsrcx, sobeldstx); //calling sobel
sobelY3x3(sobelsrcy, sobeldsty); //calling sobel
Mat magnitudedst, magnitudesrc;
magnitude(sobeldstx, sobeldsty, grayframe); //calling magnitude
Mat blursrc, blurdst;
src.copyTo(blursrc); src.copyTo(blurdst);
blurQuantize(blursrc, blurdst, levels); //calling blurQuantize
for (int i = 0; i < grayframe.rows; i++) {
for (int j = 0; j < grayframe.cols; j++) {
int value = grayframe.at<uint8_t>(i, j);
if (value >= magThreshold) {
blurdst.at<uint8_t>(i, j) = 0;
}
}
}
imwrite("Cartoon-original.jpg", src);
//imwrite("Cartoon-mag.jpg", grayframe);
imwrite("Cartoon-blur.jpg", blurdst);
cv::imshow("feed1", blurdst);
waitKey(0);
return 0;
} 