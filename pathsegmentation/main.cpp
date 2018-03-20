#include <iostream>
#include "ZedPathSegmentation.h"
#include "KinectPathSegmentation.h"
#include <opencv2/core/core.hpp>
#include <list>
//#include <opencv2/highgui/highgui.hpp>


using namespace std;

cv::Mat img;



void mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
	if (event == cv::EVENT_MOUSEMOVE) {
		cout << "(" << x << ", " << y << ")" << " | " << (int)img.at<uchar>(y,x) << endl;
	}
}

int main(){

    ZedPathSegmentation segmentador;
	cv::Mat img3;
	const char* imname = "zed3.jpg";
    img = cv::imread(imname);
	img3 = cv::imread("zedRGB3.jpg");
    cv::cvtColor(img,img,CV_BGR2GRAY);
    segmentador.applyMorph(&img);
	cv::bitwise_not(img,img);
	list<pair<int, int>> paths = segmentador.safePathList(img, img);
	cout << "Caminhos: " << paths.size() << endl;
    cv::Mat img2 = segmentador.markSafePaths(img3,paths);

	cvNamedWindow("image");
	cvSetMouseCallback("image", mouse_callback,&img);

	cv::imwrite("imout.jpg", img2);


    while(1){
        cv::imshow("image",img2);
        int k = cv::waitKey(30);
		if (k == 27)
			break;
    }


    return 0;
}
