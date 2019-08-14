#include <iostream>
#include "opencv2/opencv.hpp"

#include "blocks.h"

using namespace std;

int main(int argc, char *argv[])
{

    cv::Mat img = cv::imread("../findBlocks/data/data.png");
    cv::imshow("src",img);
    cv::cvtColor(img,img,CV_BGR2GRAY);
    blocks bls = blocks(img);

    cv::waitKey(0);

}
