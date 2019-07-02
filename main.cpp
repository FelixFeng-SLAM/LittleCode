#include <iostream>
#include "gms_matcher.h"

using namespace std;

void GmsMatch(Mat &img1, Mat &img2, vector<cv::KeyPoint> &kp1, vector<cv::KeyPoint> &kp2,
              vector<cv::DMatch> &matches_gms) {
    Mat d1, d2;
    vector<DMatch> matches_all;

    Ptr<ORB> orb = ORB::create(500);
    orb->setFastThreshold(0);

    orb->detectAndCompute(img1, Mat(), kp1, d1);
    orb->detectAndCompute(img2, Mat(), kp2, d2);

    BFMatcher matcher(NORM_HAMMING);
    matcher.match(d1, d2, matches_all);

    // GMS filter
    std::vector<bool> vbInliers;
    gms_matcher gms(kp1, img1.size(), kp2, img2.size(), matches_all);
    int num_inliers = gms.GetInlierMask(vbInliers, false, false);
    cout << "Get total " << num_inliers << " matches." << endl;

    // collect matches
    for (size_t i = 0; i < vbInliers.size(); ++i)
    {
        if (vbInliers[i] == true)
        {
            matches_gms.push_back(matches_all[i]);
        }
    }

    // draw matching
//	Mat show = DrawInlier(img1, img2, kp1, kp2, matches_gms, 1);
    Mat show;
//    drawMatches(img1,kp1,img2,kp2,matches_gms,show);
    vector<DMatch> match_inv;
    for(int k=0;k<matches_gms.size();k++){
        DMatch ma;
        ma.queryIdx=matches_gms[k].trainIdx;
        ma.trainIdx=matches_gms[k].queryIdx;
        match_inv.push_back(ma);
    }
    drawMatches(img2,kp2,img1,kp1,match_inv,show);
    namedWindow("show",0);
    resizeWindow("show",1024,512);
    imshow("show", show);
    waitKey(0);

}


int main()
{
    cv::Mat img_1 = cv::imread("../GMSmatcher/data/01.png");
    cv::Mat img_2 = cv::imread("../GMSmatcher/data/02.png");


    vector<cv::KeyPoint> keypoints_1, keypoints_2;
    vector<cv::DMatch> matches;
    cv::Mat descriptor01,descriptor02;
    GmsMatch(img_1,img_2, keypoints_1, keypoints_2, matches);
    return 0;
}
