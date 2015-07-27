#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <limits>
#include <iterator>
#include <iostream>

using namespace cv;

//Global Variable Declarations

//Helper Function Declarations
std::vector<Point> scaleHull(float scale, Point2f mean, std::vector<Point>& specificHull);

std::vector<Point> collapseContour(Point2f variance, std::vector<Point>& specificHull);

void printScaleContours(Mat img, std::vector<std::vector<Point>>& hull, std::vector<std::vector<Point>>& hull15, std::vector<std::vector<Point>>& hull2, std::vector<std::vector<Point>>& hull3, std::vector<Point2f>& mc);

void printCollapseContours(Mat img, std::vector<std::vector<Point>>& hull, std::vector<std::vector<Point>>& hullcollapse, std::vector<Point2f>& mc);

int main(int argc, char *argv[])
{
    //Include agrument usage and thrown error for improper use
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << "Source Image, Contour Type(1 for scale, 0 for collapse)" << std::endl;
        return 1;
    }

    Mat img = imread(argv[1]);
    Mat box;
    cvtColor(img, box, COLOR_BGR2GRAY);
    MSER ms(5, 1000, 14400, .25, .2, 200, 1.01, .003, 5);
    std::vector<std::vector<Point>> regions;
    ms(box, regions, Mat());
    std::vector<std::vector<Point>> hull(regions.size());

    //Extract convex hull from each contour and store in hull[] for contour scaling
    for (size_t i = 0; i < regions.size(); ++i)
        convexHull(Mat(regions[i]), hull[i], false);
   	
    
    std::vector<Moments> mu(hull.size());
    std::vector<Point2f> mc(hull.size()); //center of masses (mean)
    std::vector<Point2f> mv(hull.size()); //variances
    for (size_t i = 0; i < hull.size(); ++i)
    {                
        mu[i] = moments(hull[i], false);                                    //Create moments vector to extract center of mass and variance
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );       //Center of mass for each convexHull of each contour
        mv[i] = Point2f( mu[i].m20/mu[i].m00 , mu[i].m02/mu[i].m00 );       //Variance of each convexHull of each contour
    }

    //Create and populate scaled contour vectors
    std::vector<std::vector<Point>> hull15(hull.size());
    std::vector<std::vector<Point>> hull2(hull.size());
    std::vector<std::vector<Point>> hull3(hull.size());
    for (size_t i = 0; i < hull.size(); ++i)
    {  
        hull15[i] = scaleHull(1.5, mc[i], hull[i]);
        hull2[i] = scaleHull(2, mc[i], hull[i]);
        hull3[i] = scaleHull(3, mc[i], hull[i]);
    }
    //Create and populate collapsed contour vectors
    std::vector<std::vector<Point>> hullcollapse(hull.size());
    for (size_t i = 0; i < hull.size(); ++i)
        hullcollapse[i] = collapseContour(mv[i], hull[i]);

    //Print all contours 
    if (argv[2])
        printScaleContours(img, hull, hull15, hull2, hull3, mc);
    else
        printCollapseContours(img, hull, hullcollapse, mc);
   	return 0;
}

void printScaleContours(Mat img, std::vector<std::vector<Point>>& hull, std::vector<std::vector<Point>>& hull15, std::vector<std::vector<Point>>& hull2, std::vector<std::vector<Point>>& hull3, std::vector<Point2f>& mc)
{                                                    
    for (uint64_t i = 0; i < hull.size();++i)
    {
        drawContours(img, hull, i, Scalar(0,255,255), 2, 8, vector<Vec4i>(), 0, Point());
        drawContours(img, hull15, i, Scalar(0,255,0), 2, 8, vector<Vec4i>(), 0, Point()); 
        drawContours(img, hull2 ,i, Scalar(0,0,255), 2, 8, vector<Vec4i>(), 0, Point());
        drawContours(img, hull3, i, Scalar(255,0,0), 2, 8, vector<Vec4i>(), 0, Point());
        circle(img, mc[i], 4, Scalar::all(255), -1, 8, 0 );
        namedWindow("Image Window", WINDOW_NORMAL );
        imshow("Image Window", img);
        waitKey(0);
    }
}

void printCollapseContours(Mat img, std::vector<std::vector<Point>>& hull, std::vector<std::vector<Point>>& hullcollapse, std::vector<Point2f>& mc)
{
    for (uint64_t i = 0; i < hull.size();++i)
    {
        drawContours(img, hull, i, Scalar(0,255,255), 2, 8, vector<Vec4i>(), 0, Point());
        drawContours(img, hullcollapse, i, Scalar(0,255,0), 2, 8, vector<Vec4i>(), 0, Point()); 
        circle(img, mc[i], 4, Scalar::all(255), -1, 8, 0 );
        namedWindow("Image Window", WINDOW_NORMAL );
        imshow("Image Window", img);
        waitKey(0);
    }
}

//takes in contour, scales it
std::vector<Point> scaleHull(float scale, Point2f mean, std::vector<Point>& specificHull)
{
    std::vector<Point> newcontour;
    float scalex, scaley, tempx, tempy;
    for (size_t i = 0; i < specificHull.size(); ++i)
    {  
        tempx = specificHull[i].x - mean.x;
        tempy = specificHull[i].y - mean.y;
        scalex = scale*tempx;
        scaley = scale*tempy;
        scalex+= mean.x;
        scaley+= mean.y;
        if (scaley != 0 && scalex != 0)
            newcontour.push_back(Point((int) scalex, (int) scaley));
    }
    return newcontour;
}

std::vector<Point> collapseContour(Point2f variance, std::vector<Point>& specificHull) //subtract std dev from x and y of contour
{ 
    float stddevx = sqrt(variance.x);
    float stddevy = sqrt(variance.y);
    float tempx, tempy;
    std::vector<Point> newcontour;
    for (size_t i = 0; i < specificHull.size(); i++)
    {
        tempx = specificHull[i].x - stddevx;
        tempy = specificHull[i].y - stddevy;
        if (tempx != 0 && tempy != 0 )
        {
            newcontour.push_back(Point((int) tempx, (int) tempy));
        }
    }
    return newcontour;
}

