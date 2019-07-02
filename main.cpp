#include <iostream>

#include "opencv2/opencv.hpp"

using namespace std;

int main()
{
    ///REFERENCE:https://docs.opencv.org/master/d2/d1d/samples_2cpp_2lkdemo_8cpp-example.html#a20
    ///

    cv::Mat img1 = cv::imread("../KLTtracking/data/01.png");
    cv::Mat img2 = cv::imread("../KLTtracking/data/02.png");
    cv::Mat img20 = img2.clone();
    cv::cvtColor(img1,img1,CV_BGR2GRAY);
    cv::cvtColor(img2,img2,CV_BGR2GRAY);

    vector<cv::Point2f> pts1, pts2, pts11;
    cv::TermCriteria termcrit(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
    cv::Size subPixWinSize(10,10), winSize(31,31);

//    const int MAX_COUNT = 50;
//    cv::goodFeaturesToTrack(img1, pts1, MAX_COUNT, 0.01, 10, cv::Mat(), 3, 3, 0, 0.04);

    vector<cv::KeyPoint> kpstemp;
    cv::FAST(img1,kpstemp,20);

    for (int i = 0; i < kpstemp.size(); i++) {
        pts1.push_back(kpstemp[i].pt);
    }

    cv::cornerSubPix(img1, pts1, subPixWinSize, cv::Size(-1,-1), termcrit);

    vector<uchar> status;
    vector<float> err;
    cv::calcOpticalFlowPyrLK(img1,img2, pts1, pts2, status, err, winSize,
                                     3, termcrit, 0, 0.001);

//    vector<cv::DMatch> matches;
    size_t i, k;
    for( i = k = 0; i < pts1.size(); i++ )
    {
        if( !status[i] )
            continue;

        pts2[k++] = pts2[i];
        pts11.push_back(pts1[i]);

//        cv::DMatch matchtemp;
//        matchtemp.queryIdx = i;
//        matchtemp.trainIdx = k-1;
//        matches.push_back(matchtemp);

    }
    pts2.resize(k);

    cv::Mat dst=img20.clone();
    cv::RNG rng;
    for (int i = 0; i < pts2.size(); i++) {
        cv::line(dst, pts11[i], pts2[i],
                cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)),1,8,0);
        cv::circle(dst, pts2[i],
                2, cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), -1);
    }

    cv::namedWindow("dst1",0);
    cv::resizeWindow("dst1",1024,512);
    imshow("dst1", dst);

//    vector<cv::KeyPoint> kps1, kps2;
//    cv::KeyPoint::convert(pts1, kps1, 1, 1, 0, -1);
//    cv::KeyPoint::convert(pts2, kps2, 1, 1, 0, -1);
//    cv::Mat dst2;
//    cv::drawMatches(img1,kps1,img2,kps2,matches,dst2);
//    cv::namedWindow("dst",0);
//    cv::resizeWindow("dst",1024,512);
//    imshow("dst", dst2);

    cv::waitKey(0);

    return 0;
}
