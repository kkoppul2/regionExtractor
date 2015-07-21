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
    Mat g;
    //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
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
            circle( g, mc[j], 4, Scalar::all(255), -1, 8, 0 );
            std::vector<Point> scaleone(contours[j].size());
            std::vector<Point> scaletwo(contours[j].size());
            std::vector<Point> scalethree(contours[j].size());
            std::vector<Point> intmc(mc.size());
            std::vector<vector<Point>> newcontours1(contours.size());
            std::vector<vector<Point>> newcontours2(contours.size());
            std::vector<vector<Point>> newcontours3(contours.size());
            intmc[j]= Point((int)mc[j].x,(int)mc[j].y);
       	    for (size_t k = 0; k < contours[j].size(); k++)
			{   
                int onehalfx, onehalfy, twox, twoy, threex, threey;
				int tempx = contours[j][k].x - intmc[j].x;
                int tempy = contours[j][k].y - intmc[j].y;
                onehalfx = 1.5*tempx;
                onehalfy = 1.5*tempy;
                onehalfx+= intmc[j].x;
                onehalfy+= intmc[j].y;
                twox = 2*tempx;
                twoy = 2*tempy;
                twox+= intmc[j].x;
                twoy+= intmc[j].y;
                threex = 3*tempx;
                threey = 3*tempy;
                threex += intmc[j].x;
                threey += intmc[j].y;
                scaleone.push_back(Point(onehalfx,onehalfy));
                
                scaletwo.push_back(Point(twox,twoy));
                scalethree.push_back(Point(threex,threey));             
			}
            for (size_t k = 0; k < newcontours1.size(); k++)
            {
                for (size_t d = 0; d < scaleone.size(); d++)
                {
                    Point p = scaleone[d];
                    if (p.x != 0 && p.y != 0 ){
                        newcontours1[k].push_back(p);
                    }
                }
                for (size_t d = 0; d < scaletwo.size(); d++)
                {
                    Point p = scaletwo[d];
                    if (p.x != 0 && p.y != 0 ){
                        newcontours2[k].push_back(p);
                    }
                }
                for (size_t d = 0; d < scalethree.size(); d++)
                {
                    Point p = scalethree[d];
                    if (p.x != 0 && p.y != 0 ){
                        newcontours3[k].push_back(p);
                    }
                }
            }
            
            img.copyTo(g);
            drawContours(g,contours,j,Scalar(0,255,255),2,8, vector<Vec4i>(), 0, Point());
            drawContours(g,newcontours1,j,Scalar(0,255,0),2,8, hierarchy, 0, Point()); 
            drawContours(g,newcontours2,j,Scalar(0,0,255),2,8, vector<Vec4i>(), 0, Point());
            drawContours(g,newcontours3,j,Scalar(255,0,0),2,8, vector<Vec4i>(), 0, Point());
            imshow("/home/jkaikaus/Lab/regionExtractor/output/Samples/4.jpg", g);
            waitKey(0);
   	    }
   	}

    //imshow("/home/jkaikaus/Lab/regionExtractor/output/Samples/4.jpg", img);
    //waitKey(0);
   	return 0;
}



