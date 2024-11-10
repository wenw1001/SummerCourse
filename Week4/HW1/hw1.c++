#include <iostream>
// g++ -std=c++14 -ggdb hw1.c++ -o hw1.out `pkg-config --cflags --libs opencv4`
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp" // Color convert functions
#include <opencv2/imgcodecs.hpp> // provides functions for reading and writing
#include <opencv2/highgui.hpp> //contains the functions to show an image in a window
#include <unistd.h>
#include <cmath>
#include<limits>

using namespace cv;
using namespace std;


Mat src;
vector<Point> click_points;

void onMouse(int evt, int x, int y, int flags, void* param) {
    if(evt == EVENT_LBUTTONDOWN) {
        cout<<"click!   ";
        vector<Point>* ptPtr = (vector<Point>*)param;
        ptPtr->push_back(Point(x,y));
        cout<<"x="<<x<<"\ty="<<y<<endl; 
        circle( src, Point(x,y), 4, Scalar( 0, 0, 0 ), FILLED, LINE_8 );
        imshow("Result", src);
    }
}

void drawCurve(Mat image,vector<Point> &points){
    Mat src = image.clone();
	for (int i = 0; i < points.size(); i++)
	{
		int j = (i + 1) % points.size();
        circle( src, points[i], 4, Scalar( 0, 0, 0 ), FILLED, LINE_8 );
		line(src, points[i], points[j], CV_RGB(255, 0, 0), 2, 8, 0);
	}
	imshow("Result", src);
	waitKey(30);
}

vector<Point> ACTIVE_CONTOUR(Mat image, Mat gradient_magnitude,vector<Point> points){
    float econt, ecurv, eimg;
    float etotal;
    vector<Point> result;

    double alpha = 0.8;  // 控制平滑性
    double beta = 1.0;   // 控制彈性
    double gamma = 1.0; // 控制外部能量
    

    int n = points.size();
    int M = 5; //Set search region 5x5，即在該點5x5以內的範圍計算最佳點

    // float d = 0.0; // d is the average distance between the points of the snake
    // for (int i = 1; i < n; ++i) {
    //     d += points[i] - points[i - 1];
    // }
    // d /= (n - 1);

    for (int i=0; i<n; i++){
        Point bestPoint = points[i];
        double emin = numeric_limits<double>::infinity();
        for (int dx = -M; dx<=M ; ++dx){
            for(int dy = -M; dy<=M; ++dy){

                Point prev_point = points[(i-1+n)%n];
                Point curr_point = points[i] + Point(dx, dy);
                Point next_point = points[(i+1)%n];

                if (curr_point.x < 0 || curr_point.x >= image.cols || curr_point.y < 0 || curr_point.y >= image.rows)
                    continue;

                econt = pow((curr_point.x - prev_point.x), 2)+pow((curr_point.y - prev_point.y), 2);
                ecurv = pow((prev_point.x - 2 * curr_point.x + next_point.x), 2)+pow((prev_point.y - 2 * curr_point.y + next_point.y), 2);

                if (curr_point.x >= 0 && curr_point.x < gradient_magnitude.cols && curr_point.y >= 0 && curr_point.y < gradient_magnitude.rows) {
                    eimg = -pow((gradient_magnitude.at<float>(curr_point.y,curr_point.x)), 2);
                }
                else{eimg =  0.0;}

                etotal = alpha * econt + beta * ecurv + gamma * eimg;
                // cout<<"econt: "<<econt<<", ecurv: "<<ecurv<<", eimg: "<<eimg<<endl;
                // cout<<"etotal: "<<etotal<<", emin: "<<emin<<endl;
                if (etotal < emin) {
                    // cout<<"etotal: "<<etotal<<", emin: "<<emin<<endl;
                    // cout<<points[i]<<" --> "<<curr_point<<endl<<endl;
                    // cout<<"update! ("<<dx<<", "<<dy<<")"<<endl;
                    emin = etotal;
                    bestPoint = curr_point;
                    }

            }
        }
        points[i] = bestPoint;
        result.push_back(bestPoint);
    }
    return points;

}


int main( int argc, char** argv ) {

//1. Read the image

    // string image_path = samples::findFile("img.jpg"); //找檔案位置
	const Mat image = imread("images/img.jpg",IMREAD_COLOR);
    if( !image.empty() ){cout << "Image read success\n";}

    // imshow("bulb", image);
    // waitKey(0); 
    // destroyAllWindows();


    
//2. Convert the image to Grayscal
    Mat gray_image;
    Mat blur_image;
    cout << "Convert the image to Grayscal...\n";
    cvtColor(image, gray_image, COLOR_BGR2GRAY); // cvtColor(src, dst, convert)

	// imshow("gray image", gray_image);
	// waitKey(0);
    // destroyAllWindows();


 // 3. Denoising (Use Gaussian blur)
    // reference:https://www.funvisiontutorials.com/2020/04/opencv-c-tutorial-smoothing-blur-noise.html,
    // https://docs.opencv.org/4.x/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html

    GaussianBlur(gray_image, blur_image, Size(7 ,7), 0, 0);
    // imshow("blur", blur_image);
	// waitKey(0);
	// destroyAllWindows();


// 4. Calculate the gradient by using Sobel
    // reference: https://docs.opencv.org/4.x/d2/d2c/tutorial_sobel_derivatives.html
    // 說明可看這：https://blog.csdn.net/m0_51402531/article/details/121065044

    cout << "Uses Sobel OpenCV functions for edge detection...\n";

    int ddepth = CV_32F;

    Mat grad_image;
    Mat grad_x, grad_y; 
    Mat abs_grad_x, abs_grad_y;

    Sobel(blur_image, grad_x, ddepth, 1, 0, 3);
    Sobel(blur_image, grad_y, ddepth, 0, 1, 3);

    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad_image);

    // imshow("Sobel Gradient_X", abs_grad_x);
    // waitKey(0);
    // destroyAllWindows();
    // imshow("Sobel Gradient_Y", abs_grad_x);
    // waitKey(0);
    // destroyAllWindows();
    // imshow("Sobel Gradient Image", grad_image);
    // waitKey(0);
    // destroyAllWindows();




// 5. Generate NUM_POINTS points around the object 
    // reference: https://docs.opencv.org/3.4/db/d75/samples_2cpp_2create_mask_8cpp-example.html#a27,
    //  https://stackoverflow.com/questions/24586013/opencv-storing-the-coordinates-of-the-points-clicked-by-mouse-left-button
    //  https://docs.opencv.org/4.x/d3/d96/tutorial_basic_geometric_drawing.html

    src = image.clone();
    namedWindow("Result");
    setMouseCallback("Result", onMouse, (void*)&click_points);
    
    // cout <<"Click to record the point\n";
    imshow("Result", src);

    int X=0, Y=0; 
    while(click_points.size()<10){
        cout<<"Points Num = "<<click_points.size()<<", Click more.\n";
        waitKey(0);
    // }
    }
    // destroyAllWindows();
    cout<<"The Points are as below:\n"<<click_points<<endl;

    drawCurve(src, click_points);


// 6. For i = 0 to MAX_ITERATION: 
    // reference: https://www.cnblogs.com/luofeiju/p/13360759.html,
    //  https://docs.opencv.org/3.4/da/d32/samples_2cpp_2contours2_8cpp-example.html
    //  https://blog.csdn.net/webzhuce/article/details/100834086
    //  https://www.cse.unr.edu/~bebis/CS791E/Notes/DeformableContours.pdf
    

    int MAX_ITERATION = 100;
    vector<Point> new_points;
    Mat gradient_magnitude;
    magnitude(grad_x , grad_y,gradient_magnitude);

    cout<<"START THE ITERATION\n";

    for (int i=0; i<MAX_ITERATION; ++i){
        cout<<"iteration: "<<i<<endl;
        new_points = ACTIVE_CONTOUR(grad_image, gradient_magnitude, click_points);
        
        if(click_points==new_points){
            cout<<"points are the same, iteration ends"<<endl;
            break;
        }

        drawCurve(image, new_points);
        click_points=new_points;

    }

    waitKey(0);

	return 0;
}