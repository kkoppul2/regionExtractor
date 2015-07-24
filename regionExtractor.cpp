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
std::vector<vector<Point>> scaleHull(float scale, Point2f mean, std::vector<std::vector<Point>>& hull, uint64_t idx);

Mat collapseContour(std::vector<Point> specificContour, Point variance, Mat img);

void printContours(Mat& img, std::vector<std::vector<Point>>& hull, std::vector<Point2f>& mc, int type);

int main(int argc, char *argv[])
{
    //Include agrument usage and thrown error for improper use

    Mat img = imread(argv[1]);
    Mat box;
    cvtColor(img, box, COLOR_BGR2GRAY);
    MSER ms(5, 1000, 14400, .25, .2, 200, 1.01, .003, 5);                   //Where do these specifications come from?
    std::vector<std::vector<Point>> regions;
    ms(box, regions, Mat());
    std::vector<std::vector<Point>> hull(regions.size());

    //Extract convex hull from each contour and store in hull[] for contour scaling
    for (size_t i = 0; i < regions.size(); i++)
        convexHull(Mat(regions[i]), hull[i], false);
   	
    
    std::vector<Moments> mu(hull.size());
    std::vector<Point2f> mc(hull.size()); //center of masses (mean)     Figure out how each of these determined from moments
    std::vector<Point2f> mv(hull.size()); //variances
    for (size_t j = 0; j < hull.size(); j++)
    {                
        mu[j] = moments(hull[j], false);                                    //Create moments vector to extract center of mass and variance
        mc[j] = Point2f( mu[j].m10/mu[j].m00 , mu[j].m01/mu[j].m00 );       //Center of mass for each convexHull of each contour
        mv[j] = Point2f( mu[j].m20/mu[j].m00 , mu[j].m02/mu[j].m00 );       //Variance of each convexHull of each contour
    }

    //All scaling operations
    scaleHull(1.5, m_[i], hull, i)
    scaleHull(2, m_[i], hull, i)
    scaleHull(3, m_[i], hull, i)


    printContours(img, hull, mc, 0);
   	return 0;
}

std::vector<vector<Point>> scaleHull(float scale, Point2f mean, std::vector<std::vector<Point>>& hull, uint64_t idx)

Mat collapseContour(std::vector<Point> specificContour, Point variance, Mat img) //subtract std dev from x and y of contour
{ 
    Size s = img.size(); //obtain image dimensions
    int rows = s.height;
    int cols = s.width;
    Mat temp = Mat::zeros(rows, cols, CV_8UC1 );
    int stddevx = sqrt(variance.x);
    int stddevy = sqrt(variance.y);
    std::vector<Point> vector(specificContour.size());
    std::vector<std::vector<Point>> newcontours(1);
    std::vector<std::vector<Point>> oldcontours(1);
    for (size_t i = 0; i < specificContour.size(); i++)
    {
        int tempx, tempy;
        tempx = specificContour[i].x - stddevx;
        tempy = specificContour[i].y - stddevy;
        vector.push_back(Point(tempx,tempy));
    }
    for (size_t i = 0; i < vector.size(); i++)
    {
        Point p = vector[i];
        Point d = specificContour[i];
        if (p.x != 0 && p.y != 0 )
        {
            newcontours[0].push_back(p);
        }
        if (d.x != 0 && d.y != 0 )
        {
            oldcontours[0].push_back(p);
        }          
    }
    for (size_t i = 0; i < newcontours.size(); i++)
    {
        drawContours(temp,newcontours,i,Scalar(255,255,255),2,8, std::vector<Vec4i>(), 0, Point());
        //drawContours(temp, oldcontours, i, Scalar(255,255, 0), 2, 8, std::vector<Vec4i>(), 0, Point());
    }
    /*for (size_t i = 0; i < (size_t)rows; i++)
    {
        for (size_t j = 0; j < (size_t)cols; j++)
        {
            if(!(img.at<double>(i,j) == 1))
            {
                img.at<double>(i,j) = 0;
            }
        }
    }*/
    return temp;
}

void printScaleContours(Mat img, std::vector<std::vector<Point>>& hull, std::vector<std::vector<Point>>& hull15, std::vector<std::vector<Point>>& hull2, std::vector<std::vector<Point>>& hull3)
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

void printCollapseContours(Mat img, std::vector<std::vector<Point>>& hull, std::vector<std::vector<Point>>& hullcollapse)
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
std::vector<vector<Point>> scaleHull(float scale, Point2f mean, std::vector<std::vector<Point>>& hull, uint64_t idx)
{
    std::vector<std::vector<Point>> newcontours(hull.size());
    float scalex, scaley, tempx, tempy;
    for (size_t i = 0; i < hull[idx].size(); ++i)
    {  
        tempx = hull[idx][i].x - mean.x;
        tempy = hull[idx][i].y - mean.y;
        scalex = scale*tempx;
        scaley = scale*tempy;
        scalex+= mean.x;
        scaley+= mean.y;
        if (scaley != 0 && scalex != 0)
            newcontours[idx].push_back(Point((int)scalex,(int)scaley));
    }
    return newcontours;
}

