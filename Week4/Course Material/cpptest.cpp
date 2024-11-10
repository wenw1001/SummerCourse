#include<opencv2/opencv.hpp>

using namespace cv;

int main() 
{
	Mat img = imread("images/cat.jpg");

	imshow("cat", img);

	waitKey(0);
	destroyAllWindows();

	return 0;
}