//
// Created by Beini Fang on 2018-01-25.
// Detects and tags rectangles, triangles, and circles
// Returns vertices of rectangles and triangles, and radius + center of circles
//

#include "shapedetect.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/photo.hpp"

#include <cmath>

#include <iostream>
#include <stdio.h>

const int minTriangleArea = 10;
const int minSquareArea = 10;
const int minCircleArea = 50;


// Finds a cosine of angle between vectors from pt0->pt1 and pt0->pt2
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

//Draws shapes (triangles and rectangles)
static void drawShapes(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares, std::string wndname){
    for(size_t i = 0; i < squares.size(); i++){
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, cvScalar(255,0,0), 3, cv::LINE_AA);
    }

    cv::imshow(wndname, image);
}

// Displays text in the center of a contour
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

static cv::Mat findShapes(const cv::Mat& src, std::vector<std::vector<cv::Point> >& triangles,
                          std::vector<std::vector<cv::Point> >& rectangles, std::vector<std::vector<cv::Point> >& circles)
{
    triangles.clear();
    rectangles.clear();
    circles.clear();

/**************************************************
			       Image Processing
***************************************************/

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY);
    //cv::imshow("1 CvtColor", gray);

    // Use Canny instead of threshold to catch squares with gradient shading
    cv::Mat bw;

    //Removes noise from photo
    cv::fastNlMeansDenoising(gray, bw, 35, 10, 21);	//(input, output, filter strength, template window size, search window size)
    //cv::imshow("2 De-noising", bw);

    //Sharpen image
    // cv::Mat blur;
    // cv::GaussianBlur(gray, blur, cv::Size(0, 0), 3);	//(src, dst, , )
    // cv::imshow("3.1 GaussianBlur", blur);
    // cv::addWeighted(bw, 0.7, blur, 0.3, 0, bw);	//(src1, weight1, src2, weight2, gamma, output)
    // cv::imshow("3.2 Add Weighted (sharpening)", bw);

    //Canny edge detection
    cv::Canny(bw, bw, 0, 50, 5);
    //cv::imshow("3 Canny", bw);

    // Find contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);	//(image, output, mode, method)
    //cv::drawContours(src, contours, -1, cvScalar(255, 0, 0), 2, CV_AA);
    //cv::imshow("4 Draw Contours", src);

    //Close contours (not functioning)
    // std::vector<cv::Point> ConvexHullPoints;
    // cv::convexHull(contours, ConvexHullPoints);
    // cv::Mat drawing = bw.clone();
    //polylines(drawing, ConvexHullPoints, (int)ConvexHullPoints.size(), 1, true, cvScalar(0,0,255), 2, cv::LINE_AA);
    // drawShapes(drawing, ConvexHullPoints, "Contours Convex Hull");
    //imshow("Contours Convex Hull", drawing);

    std::vector<cv::Point> approx;
    cv::Mat dst = src.clone();

/**************************************************
			Shape detection using contours
***************************************************/

    for (int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        // Can adjust accuracy
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

        //Skip small or nonconvex objects
        // if (std::fabs(cv::contourArea(contours[i])) < 8 || !cv::isContourConvex(approx))
        // 	continue;

        //Adjust min shape area to account for smaller robot and shapes
        //TO-DO: Triangle detection is sketchy, straight lines are sometimes labelled as triangles. Add in a maximum height to width ratio for triangle
        if (approx.size() == 3 &&
            (std::fabs(cv::contourArea(contours[i])) > minTriangleArea && cv::isContourConvex(approx)))
        {
            setLabel(dst, "TRI", contours[i]);    // Label triangles
            triangles.push_back(approx);    //Store coordinates of triangles
            std::cout << "Triangle " << i << approx[0] << approx[1] << approx[2] << std::endl;
        }
        else if (approx.size() >= 4 && approx.size() <= 6)
        {
            // Number of vertices of polygonal curve
            int vtc = approx.size();

            // Get the cosines of all corners
            std::vector<double> cos;
            for (int j = 2; j < vtc+1; j++)
                cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

            // Sort ascending the cosine values
            std::sort(cos.begin(), cos.end());

            // Get the lowest and the highest cosine
            double mincos = cos.front();
            double maxcos = cos.back();

            // Use the degrees obtained above and the number of vertices
            // to determine the shape of the contour
            if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3 &&
                (std::fabs(cv::contourArea(contours[i])) > minSquareArea && cv::isContourConvex(approx))){
                setLabel(dst, "RECT", contours[i]);
                rectangles.push_back(approx);   //Store coordinates of detected rectangles
                std::cout << "Rectangle " << i << approx[0] << approx[1] << approx[2] << approx[3] << std::endl;

            }

        }
        else if (approx.size() > 6 && (std::fabs(cv::contourArea(contours[i])) > minCircleArea))
        {
            // Detect and label circles
            double area = cv::contourArea(contours[i]);
            cv::Rect r = cv::boundingRect(contours[i]);	//creates a bounding rectangle around contours
            int radius = r.width / 2;
            // TO-DO: return center and radius of circles
            //cv::Point center = r.tl.x + radius, r.tl.y - radius; 	//Calculates coordinates of center based on top left corner of bounding rectangle

            if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
            {
                setLabel(dst, "CIR", contours[i]);
                circles.push_back(approx);
                //circle(dst, approx.back(), radius, cvScalar(0,255,0), 3, cv::LINE_AA);
            }

        }
    }
    return dst;
}

#include <QDebug>

void Shapedetect::modify(cv::Mat *img) {
    //qDebug() << "I love shapes ecksdee";
    const std::string wndname = "shape-detect";

    std::vector<std::vector<cv::Point> > triangles;
    std::vector<std::vector<cv::Point> > rectangles;
    std::vector<std::vector<cv::Point> > circles;


    *img = findShapes(*img, triangles, rectangles, circles);
    drawShapes(*img, triangles, wndname);		//Outline rectangles and triangles in blue
    drawShapes(*img, rectangles, wndname);

}