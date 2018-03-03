#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>

#include "shapedetect.h"
#include "../utility/logger.h"

const int minTriangleArea = 10;
const int min_square_area = 10;
const int minCircleArea = 50;

// Finds a cosine of angle between vectors from pt0->pt1 and pt0->pt2
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

// Draws shapes (triangles and rectangles)
static void drawShapes(cv::UMat &image, const std::vector<std::vector<cv::Point> > &squares) {
    for (const auto &square : squares) {
        cv::polylines(image, cv::InputArrayOfArrays(square), true, cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
    }
}

// Displays text in the center of a contour
void setLabel(cv::UMat &im, const std::string &label, std::vector<cv::Point> &contour) {
    constexpr int font_face = cv::FONT_HERSHEY_SIMPLEX;
    constexpr double scale = 0.4;
    constexpr int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, font_face, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), cv::Scalar(255, 255, 255),
                  cv::FILLED);
    cv::putText(im, label, pt, font_face, scale, cv::Scalar(0, 0, 0), thickness, 8);
}

static cv::UMat findShapes(
    const cv::UMat &src,
    std::vector<std::vector<cv::Point> > &triangles,
    std::vector<std::vector<cv::Point> > &rectangles,
    std::vector<std::vector<cv::Point> > &circles
) {
    triangles.clear();
    rectangles.clear();
    circles.clear();

    /*
     * Process image to find contours.
     */
    // Convert to grayscale
    cv::UMat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

    // Use Canny instead of threshold to catch squares with gradient shading
    cv::UMat bw;

    //Removes noise from photo
    cv::fastNlMeansDenoising(gray, bw, 35, 10,
                             21);    //(input, output, filter strength, template window size, search window size)

    //Sharpen image
    // cv::Mat blur;
    // cv::GaussianBlur(gray, blur, cv::Size(0, 0), 3);	//(src, dst, , )
    // cv::addWeighted(bw, 0.7, blur, 0.3, 0, bw);	//(src1, weight1, src2, weight2, gamma, output)

    //Canny edge detection
    cv::Canny(bw, bw, 0, 50, 5);

    // Find contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);    //(image, output, mode, method)
    cv::drawContours(src, contours, -1, cv::Scalar(255, 0, 0), 2, CV_AA);

    //Close contours
    // std::vector<cv::Point> ConvexHullPoints;
    // cv::convexHull(contours, ConvexHullPoints);
    // cv::Mat drawing = bw.clone();
    //polylines(drawing, ConvexHullPoints, (int)ConvexHullPoints.size(), 1, true, cvScalar(0,0,255), 2, cv::LINE_AA);
    // drawShapes(drawing, ConvexHullPoints, "Contours Convex Hull");

    std::vector<cv::Point> approx;
    cv::UMat dst(src.clone());

    /*
     * Shape detection using contours.
     */
    for (int i = 0; i < contours.size(); i++) {
        // Approximate contour with accuracy proportional to the contour parameter
        // Using cv::Mat for now as cv::UMat does not have a move constructor for vectors of points
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

        // Skip small or non-convex objects
        // if (std::fabs(cv::contourArea(contours[i])) < 8 || !cv::isContourConvex(approx))
        // 	continue;

        if (approx.size() == 3 &&
            (std::fabs(cv::contourArea(contours[i])) > minTriangleArea && cv::isContourConvex(approx))) {
            setLabel(dst, "TRI", contours[i]);    // Triangles
            triangles.push_back(approx);
            //std::cout << "Triangle " << i << approx[0] << approx[1] << approx[2] << std::endl;
        } else if (approx.size() >= 4 && approx.size() <= 6) {
            // Number of vertices of polygonal curve
            unsigned long vtc = approx.size();

            // Get the cosines of all corners
            std::vector<double> cos;
            for (int j = 2; j < vtc + 1; j++) {
                cos.push_back(angle(approx[j % vtc], approx[j - 2], approx[j - 1]));
            }

            // Sort ascending the cosine values
            std::sort(cos.begin(), cos.end());

            // Get the lowest and the highest cosine
            double min_cos = cos.front();
            double max_cos = cos.back();

            // Use the degrees obtained above and the number of vertices
            // to determine the shape of the contour
            if (vtc == 4 && min_cos >= -0.1 && max_cos <= 0.3 &&
                (std::fabs(cv::contourArea(contours[i])) > min_square_area && cv::isContourConvex(approx))) {
                setLabel(dst, "RECT", contours[i]);
                rectangles.push_back(approx);

                //std::cout << "Rectangle " << i << approx[0] << approx[1] << approx[2] << approx[3] << std::endl;

            }
            // else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
            // 	setLabel(dst, "PENTA", contours[i]);
            // else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
            // 	setLabel(dst, "HEXA", contours[i]);
        } else if (approx.size() > 6 && (std::fabs(cv::contourArea(contours[i])) > minCircleArea)) {
            // Detect and label circles
            double area = cv::contourArea(contours[i]);
            // creates a rectangle around contours
            cv::Rect r = cv::boundingRect(contours[i]);
            int radius = r.width / 2;
            // TODO: return center and radius of circles
            // Calculates coordinates of center based on top left corner of bounding rectangle
            //cv::Point center = r.tl.x + radius, r.tl.y - radius;

            if (std::abs(1 - ((double) r.width / r.height)) <= 0.2 &&
                std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2) {
                setLabel(dst, "CIR", contours[i]);
                circles.push_back(approx);
                //circle(dst, approx.back(), radius, cvScalar(0,255,0), 3, cv::LINE_AA);
            }

        }
    }
    return dst;
}

void ShapeDetect::modify(cv::UMat &img) {
    std::vector<std::vector<cv::Point>> triangles;
    std::vector<std::vector<cv::Point>> rectangles;
    std::vector<std::vector<cv::Point>> circles;

    img = findShapes(img, triangles, rectangles, circles);
    // Outline rectangles and triangles in blue
    //drawShapes(*img, triangles);
    //drawShapes(*img, rectangles);

}
