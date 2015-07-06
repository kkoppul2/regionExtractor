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
    Mat img = imread("/home/jkaikaus/Lab/regionExtractor/Samples/2.jpg");
    Mat box, fin, temp;
    cvtColor(img, box, COLOR_BGR2GRAY);
    Size s = img.size(); //obtain image dimensions
	int rows = s.height;
	int cols = s.width;
    Mat mask = Mat::zeros(rows, cols, CV_8UC1); //make mask based off of image dimensions
    RNG rng(12345);
    MSER ms(5, 60, 14400, .25, .2, 200, 1.01, .003, 5);
    std::vector<vector<Point>> regions, contours, onehalf, two, three;
    vector<Vec4i> hierarchy;
    ms(box, regions, Mat());
    Point org;
    org.x = s.width/3+90;
    org.y = 50;
    for (size_t i = 0; i < regions.size(); i++)
   	{
        drawContours(mask,regions,i,Scalar::all(255),1,8, vector<Vec4i>(), 0, Point());
        findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        vector<vector<Point>> hull(contours.size());
        for (size_t j = 0; j < contours.size(); j++)
   	    {
            convexHull(Mat(contours[j]), hull[j], false);
   	    }
        std::vector<int>::iterator z = hull.begin();
        while(!hull.end())
        {
            onehalf = 1.5*(hull[z] - mean(hull));
            two = 2*(hull[z] - mean(hull));
            three = 3*(hull[z] - mean(hull));
        }
        for (size_t k = 0; k < contours.size(); k++)
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(img,contours,k,color,2,8, hierarchy, 0, Point());
            drawContours(img,hull,k, Scalar(0,255,255), 2,8,vector<Vec4i>(), 0);
        }
   	}
    // sprintf(str, "%d", tags[i]);
    //putText(img, "Standard Parameters\n MSER ms(10, 60, 14400, .25, .2, 200, 1.01, .003, 5)", org, FONT_HERSHEY_PLAIN, 1.5,  Scalar::all(0));
    imshow("/home/jkaikaus/Lab/regionExtractor/output/Samples/4.jpg", img);
    waitKey(0);
   	return 0;
}



