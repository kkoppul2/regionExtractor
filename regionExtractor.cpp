#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <limits>

using namespace cv;

int main(int argc, char *argv[])
{
    Mat img = imread("/home/jkaikaus/Lab/Data/debayer/0000017403.jpg");
    Mat box, fin, temp;
    cvtColor(img, box, COLOR_BGR2GRAY);
    Size s = img.size(); //obtain image dimensions
	int rows = s.height;
	int cols = s.width;
    Mat mask = Mat::zeros(rows, cols, CV_8UC1); //make mask based off of image dimensions
    RNG rng(12345);
    MSER ms(5, 60, 14400, .25, .2, 10, 1.01, .003, 5);
    std::vector<vector<Point>> regions, contours;
    vector<Vec4i> hierarchy;
    ms(box, regions, Mat());
    //printf("%d\n", (int)regions.size());
    for (size_t i = 0; i < regions.size(); i++)
   	{
        drawContours(mask,regions,i,Scalar::all(255),1,8, vector<Vec4i>(), 0, Point());
        findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        for (size_t i = 0; i < contours.size(); i++)
   	    {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(img,contours,i,color,2,8, hierarchy, 0, Point());
   	    }
   	}
    imshow("mser", img);
    waitKey(0);
   	return 0;
}



