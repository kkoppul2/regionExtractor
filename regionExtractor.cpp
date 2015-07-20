#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <limits>
#include <iterator>

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
    std::vector<vector<Point>> regions;
    std::vector<vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    ms(box, regions, Mat());
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    for (size_t i = 0; i < regions.size(); i++)
   	{
        drawContours(mask,regions,i,Scalar::all(255),1,8, vector<Vec4i>(), 0, Point());
        findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        std::vector<vector<Point>> hull(contours.size());
        for (size_t j = 0; j < contours.size(); j++)
   	    {
            convexHull(Mat(contours[j]), hull[j], false);
            std::vector<Moments> mu(hull.size());
            mu[j]= moments(hull[j], false);
            std::vector<Point2f> mc(hull.size());
            mc[j] = Point2f( mu[j].m10/mu[j].m00 , mu[j].m01/mu[j].m00 );
            circle( img, mc[j], 4, Scalar::all(255), -1, 8, 0 );
            std::vector<Point> scaleone(contours[j].size());
            std::vector<Point> scaletwo(contours[j].size());
            std::vector<Point> scalethree(contours[j].size());
            std::vector<Point> intmc(mc.size());
            std::vector<vector<Point>> newcontours(contours.size());
            intmc[j]= Point((int)mc[j].x,(int)mc[j].y);
       	    for (size_t k = 0; k < contours[j].size(); k++)
			{   
				int tempx = contours[j][k].x - intmc[j].x;
                int tempy = contours[j][k].y - intmc[j].y;
                tempx = 1.5*tempx;
                tempy = 1.5*tempy;
                tempx+= intmc[j].x;
                tempy+= intmc[j].y;
                scaleone.push_back(Point(tempx,tempy));             
			}
            for (size_t k = 0; k < newcontours.size(); k++)
            {
                for (size_t d = 0; d < scaleone.size(); d++)
                {
                    Point p = scaleone[d];
                    newcontours[k].push_back(p);
                }
            }
            
            drawContours(img,newcontours,j,color,2,8, hierarchy, 0, Point()); 
   	    }
   	}

    imshow("/home/jkaikaus/Lab/regionExtractor/output/Samples/4.jpg", img);
    waitKey(0);
   	return 0;
}



