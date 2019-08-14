#ifndef BLOCKS_H
#define BLOCKS_H

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;

struct Pos{
    int row;
    int col;
};
struct block{
    Pos blocksPos;
    vector<Pos> pixelPos;
};

class blocks
{
public:
    cv::Mat img;
    vector<block> multiblock;

public:
    blocks(cv::Mat img);


private:
    block growBlock(cv::Mat img, Pos pos, cv::Mat &isinblock);
    vector<block> findBlocks(cv::Mat img);


};

#endif // BLOCKS_H
