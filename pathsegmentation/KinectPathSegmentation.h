#include <iostream>
#include <list>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

class KinectPathSegmentation{

public:

    KinectPathSegmentation();
    ~KinectPathSegmentation();
    list<pair<int,int>> safePathList(cv::Mat,cv::Mat);
    pair<int,int> getCentralPath(list<pair<int,int>>);
    cv::Mat markSafePaths(cv::Mat,list<pair<int,int>>);
    void applyMorph(cv::Mat*);

private:

    int IMGWIDTH = 640;
    int IMGHEIGHT = 480;


};
