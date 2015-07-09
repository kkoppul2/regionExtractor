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
    Mat box, fin;
    cvtColor(img, box, COLOR_BGR2GRAY);
    Size s = img.size(); //obtain image dimensions
	int rows = s.height;
	int cols = s.width;
    Mat mask = Mat::zeros(rows, cols, CV_8UC1); //make mask based off of image dimensions
    RNG rng(12345);
    MSER ms(5, 60, 14400, .25, .2, 200, 1.01, .003, 5);
    std::vector<vector<Point>> regions, contours, onehalf, two, three;
    std::vector<Vec4i> hierarchy;
    ms(box, regions, Mat());
    
    Point org;
    org.x = s.width/3+90;
    org.y = 50;
    for (size_t i = 0; i < regions.size(); i++)
   	{
        drawContours(mask,regions,i,Scalar::all(255),1,8, vector<Vec4i>(), 0, Point());
        findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        std::vector<std::vector<Point>> hull(contours.size());
        std::vector<Moments> mu(contours.size());
        std::vector<Point2f> mc(contours.size());
        for (size_t j = 0; j < contours.size(); j++)
   	    {
            mu[j]= moments(contours[j], false);
            convexHull(Mat(contours[j]), hull[j], false);
   	    }
        for(size_t a = 0; a < contours.size(); a++)
        {
            mc[a] = Point2f( mu[a].m10/mu[a].m00 , mu[a].m01/mu[a].m00 );
            
        }
        for(
        for (size_t k = 0; k < contours.size(); k++)
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(img,contours,k,color,2,8, hierarchy, 0, Point());
            //drawContours(img,hull,k, Scalar(0,255,255), 2,8,vector<Vec4i>(), 0);
            circle( img, mc[k], 4, Scalar::all(255), -1, 8, 0 );
        }
   	}
    imshow("/home/jkaikaus/Lab/regionExtractor/output/Samples/4.jpg", img);
    waitKey(0);
   	return 0;
}



