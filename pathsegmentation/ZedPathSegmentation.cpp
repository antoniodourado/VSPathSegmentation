#include "ZedPathSegmentation.h"

ZedPathSegmentation::ZedPathSegmentation(){

}

ZedPathSegmentation::~ZedPathSegmentation(){

}

list<pair<int,int>> ZedPathSegmentation::safePathList(cv::Mat img,cv::Mat distMat){
    int const larg = 100;
    float navdist = 1;
    cv::Mat diff;
    list<pair<int,int>> pathxcoord;
    cv::Mat imgbit =  img.clone();
    IMGWIDTH = img.size().width;
    IMGHEIGHT = img.size().height;
    /*cv::Mat sEl = getStructuringElement(MORPH_RECT,Size(21,21));
        morphologyEx(imgbit,imgbit,MORPH_DILATE,sEl);
        morphologyEx(imgbit,imgbit,MORPH_CLOSE,sEl);
        imgbit.setTo(cv::Scalar(255),imgbit==0);*/


    int slice = IMGHEIGHT / 4;
    cv::Rect recsub(0,(IMGHEIGHT-slice-1),IMGWIDTH,slice);
    cv::Mat bpiece = imgbit(recsub).clone();
    cv::Mat bdist = distMat(recsub).clone();
    cv::Mat allb = cv::Mat::ones(bpiece.rows,bpiece.cols,CV_8UC1);
    allb.setTo(cv::Scalar(255),allb==1);
    compare(bpiece,allb,diff,cv::CMP_NE);
    int nz = countNonZero(diff);
    if(nz == 0)
        return pathxcoord;

    cv::Mat threshOut;
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat bp2 = bpiece.clone();
    //cout << navdist << endl;
    cv::threshold(bpiece,bpiece,navdist,255,CV_THRESH_BINARY); 

    int bcheck = 23;


    cv::Mat cpathw = cv::Mat::ones(bcheck,larg,CV_8UC1);
    cpathw.setTo(cv::Scalar(255),cpathw==1);
    cv::Mat inter = cv::Mat::zeros(bcheck,larg,CV_8UC1);

    pathxcoord.clear();

    cv::Mat cpathb = cv::Mat::zeros(bcheck,1,CV_8UC1);
    for(int i=0;i<IMGWIDTH;i++){
        inter = bpiece(cv::Rect(i,slice-bcheck,1,bcheck)).clone();
        compare(inter,cpathb,diff,cv::CMP_NE);
        int nz = countNonZero(diff);
        if(nz==0)
            bpiece.col(i).setTo(cv::Scalar(0));
		else
			bpiece.col(i).setTo(cv::Scalar(255));
    }
	
	imshow("b", bpiece);


	//cv::bitwise_not(bpiece, bpiece);

    cv::Mat bpcopy = bpiece.clone();
    cv::findContours(bpcopy,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	
    vector<cv::Moments> moments(contours.size());
    for(unsigned int i=0;i<contours.size();i++){
        moments[i] = cv::moments(contours[i],false);
    }
    cv::Mat fpath = cv::Mat::ones(slice,larg,CV_8UC1);
    fpath.setTo(cv::Scalar(255),fpath==1);
    for(unsigned int i=0;i<contours.size();i++){
        int x = (int)(moments[i].m10/moments[i].m00);
        int y = (int)(moments[i].m01/moments[i].m00);
		cout << "Cont X,Y: " << x << " " << y << endl;
        cv::Point2f mc(x,y);
        if(mc.x + larg/2 > IMGWIDTH || mc.x - larg/2 < 0)
            continue;
        inter = bpiece(cv::Rect(mc.x-larg/2,0,larg,slice)).clone();
        compare(inter,fpath,diff,cv::CMP_NE);
        int nz = countNonZero(diff);
        if(nz==0){
            pathxcoord.push_front(pair<int,int>(x,boundingRect(contours[i]).width));
        }
    }
    return pathxcoord;

}

pair<int,int> ZedPathSegmentation::getCentralPath(list<pair<int,int>> paths){
    if(paths.size() < 1){
        cout << "Couldn't find any path" << endl;
        return make_pair(-1,-1);
    }
    pair<int,int> avgmd = paths.front();
    double imgmid = IMGWIDTH/2;
    cout << "paths found: " << paths.size() << endl;
    if(paths.size() > 1)
        for(auto x : paths)
            if(x.first == imgmid ||
                    (x.first < imgmid && x.first+x.second/2 >= imgmid) ||
                    (x.first > imgmid && x.first-x.second/2 <= imgmid))
                avgmd = x;
    cout << "closest path to the center: " << avgmd.first << endl;
    if(avgmd.first-floor(avgmd.second/2)+avgmd.second >= IMGWIDTH){
        avgmd.second -= abs((avgmd.first-floor(avgmd.second/2)+avgmd.second) - IMGWIDTH)*2;
    }
    else if(avgmd.first-floor(avgmd.second/2) < 0)
        avgmd.first = floor(avgmd.second/2);
    cout << "x: " << avgmd.first-floor(avgmd.second/2) << " | Width: " << avgmd.second << endl;

    return avgmd;
}

cv::Mat ZedPathSegmentation::markSafePaths(cv::Mat img,list<pair<int,int>> paths){
    int slice = IMGHEIGHT / 4;
    cv::Mat chosenPath = img;
    for(auto path : paths){
        cv::Rect imgpath(path.first-floor(path.second/2),IMGHEIGHT-slice,path.second,slice);
        rectangle(chosenPath,imgpath,cv::Scalar(0,0,255),1,8);
    }
    return chosenPath;
}

void ZedPathSegmentation::applyMorph(cv::Mat* imgbit){
    cv::Mat sEl = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
    //morphologyEx(*imgbit,*imgbit,cv::MORPH_DILATE,sEl);
    morphologyEx(*imgbit,*imgbit,cv::MORPH_CLOSE,sEl);
//    morphologyEx(*imgbit,*imgbit,cv::MORPH_OPEN,sEl);
//    morphologyEx(*imgbit,*imgbit,cv::MORPH_DILATE,sEl);
//    morphologyEx(*imgbit,*imgbit,cv::MORPH_DILATE,sEl);





    //imgbit->setTo(cv::Scalar(255),*imgbit==0);
}

