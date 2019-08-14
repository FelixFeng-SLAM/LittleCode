#include "blocks.h"
#include <ctime>

blocks::blocks(cv::Mat img)
{
    this->img = img;
    this->multiblock = findBlocks(img);
}

vector<Pos> getNeighboor(cv::Mat img, Pos pos)
{
    vector<Pos> neighboor;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(1==i && 1==j) continue;
            Pos postemp;
            postemp.row = pos.row+i-1;
            postemp.col = pos.col+j-1;
            if(postemp.row >= 0 && postemp.row <= img.rows
                    && postemp.col >= 0 && postemp.col <= img.cols)
                neighboor.push_back(postemp);
        }
    }
    return neighboor;
}

Pos getPixelsCenter(vector<Pos> pixelpos)
{
    int top = 0;
    int bottom = 0;
    int left = 0;
    int right = 0;
    for(int i=0; i<pixelpos.size(); i++)
    {
        if(pixelpos[i].col > top) top = pixelpos[i].col;
        if(pixelpos[i].col < bottom) bottom = pixelpos[i].col;
        if(pixelpos[i].row > right) right = pixelpos[i].row;
        if(pixelpos[i].row < left) left = pixelpos[i].row;
    }

    Pos pos;
    pos.row = (left+right)/2;
    pos.col = (top+bottom)/2;
    return pos;

}

block blocks::growBlock(cv::Mat img, Pos pos, cv::Mat &isinblock)
{
    // ------- viewer ,delete it! ----------//
    cv::Mat viewer(img.rows, img.cols, CV_8UC3, cv::Scalar(0,0,0));
    cv::RNG rng((unsigned)time(NULL));

    int b = rng.uniform(0,255);
    int g = rng.uniform(0,255);
    int r = rng.uniform(0,255);
    viewer.at<cv::Vec3b>(pos.row, pos.col)[0] = b;
    viewer.at<cv::Vec3b>(pos.row, pos.col)[1] = g;
    viewer.at<cv::Vec3b>(pos.row, pos.col)[2] = r;
    // \------- viewer ,delete it! ----------//

    block result;
    result.blocksPos = pos;

    vector<Pos> pixelpos;
    pixelpos.push_back(pos);

    isinblock.at<uchar>(pos.row,pos.col) = 255;

    vector<Pos> possiblePixel = getNeighboor(img, pos);

    vector<Pos> newpixel;
    while(possiblePixel.size() != 0)
    {
        for(int i=0; i<possiblePixel.size(); i++)
        {
            if(255 == img.at<uchar>(possiblePixel[i].row,possiblePixel[i].col)
                    && 0 == isinblock.at<uchar>(possiblePixel[i].row,possiblePixel[i].col))
            {
                newpixel.push_back(possiblePixel[i]);
                pixelpos.push_back(possiblePixel[i]);
                isinblock.at<uchar>(possiblePixel[i].row,possiblePixel[i].col) = 255;

                // ------- viewer ,delete it! ----------//
                viewer.at<cv::Vec3b>(possiblePixel[i].row,possiblePixel[i].col)[0] = b;
                viewer.at<cv::Vec3b>(possiblePixel[i].row,possiblePixel[i].col)[1] = g;
                viewer.at<cv::Vec3b>(possiblePixel[i].row,possiblePixel[i].col)[2] = r;
                // \------- viewer ,delete it! ----------//

            }

        }


        // ------- viewer ,delete it! ----------//
        cv::imshow("viewer",viewer);
        cv::waitKey(30);
        // \------- viewer ,delete it! ----------//


        vector <Pos>().swap(possiblePixel);

        if(0 == newpixel.size()) continue;

        for(int j=0; j<newpixel.size(); j++)
        {
            vector<Pos> temp = getNeighboor(img, newpixel[j]);
            for(int k=0; k<temp.size(); k++)
            {
                if(0 == isinblock.at<uchar>(temp[k].row,temp[k].col))
                {
                    possiblePixel.push_back(temp[k]);
                }
            }
        }
        vector <Pos>().swap(newpixel);
    }

    result.pixelPos.assign(pixelpos.begin(),pixelpos.end());
    result.blocksPos = getPixelsCenter(result.pixelPos);


    return result;

}

vector<block> blocks::findBlocks(cv::Mat img)
{
    vector<block> result;
    cv::Mat isinblock(img.rows, img.cols, CV_8UC1, cv::Scalar::all(0));
    cv::namedWindow("viewer",0);
    cv::resizeWindow("viewer",512,512);


    for(int i=0; i<img.rows; i++)
    {
        for(int j=0; j<img.cols; j++)
        {
            if(255 == isinblock.at<uchar>(i,j)) continue;

            if(255 == img.at<uchar>(i,j))
            {
                Pos pos;
                pos.row = i; pos.col = j;
                block temp;
                temp = growBlock(img, pos, isinblock);

                // ------- viewer ,delete it! ----------//
                cv::imshow("blocks",isinblock);
                cv::waitKey(50);
                // \------- viewer ,delete it! ----------//

                result.push_back(temp);

            }

        }
    }

    cv::destroyWindow("viewer");
    return result;

}

