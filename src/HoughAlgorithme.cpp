#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

/** In this script we use the Standard and Probabilistic Hough Line Transform
**  1 - Loading an image.
**  2 - Retierve edges
**  3 - Apply the Transform, draw the lines then export to a new image.  
**/

int main(int argc, char** argv)
{
    /** Declare the output variables **/
    Mat dst, cdst, cdstP;

    /** Load Data base images **/
    Mat src = imread( "/home/lbenboudiaf/Bureau/ProjetImagerie/bdd/esc5.jpg", IMREAD_GRAYSCALE );

    /** Check if image is loaded fine **/
    if(src.empty()){
        printf(" Error opening image \n");
        return -1;
    }

    /**Edge detection by using a Canny detector **/
    Canny(src, dst, 50, 200, 3);

    /** Copy edges to the images that will display the results in BGR **/
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    /** Apply Standard Hough Line Transform
    ** and stock the detected lines into a vector
    **
    ** @params
      - dst : Edges which are binarized
      - lines : vector will store the polar parameters rho and theta.
      - rho : 1 pixel
      - theta : in radian we use 1 degree.
      - threshold : minimum number of intersections to detect a line.
    **/
    vector<Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection

    /** Then we display the lines by drawing them **/

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }

    /** Apply Probabilistic Hough Line Transform
    ** @params
    ** - dst : Edges which are binarized
    ** - lines : vector will store the xStart,YStart | xEnd, YEnd.
    ** - rho : 1 pixel.
    ** - theta : in radian we use 1 degree.
    **/

    vector<Vec4i> linesP;
    HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10 );

    /** Then we display the lines by drawing them **/
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
    }

    /** Display Original Image , Standard and Probabilistic Hough Transform **/
    imshow("Original image ", src);
    imshow("Standard Hough Line Transform", cdst);
    imshow("Probabilistic Line Transform", cdstP);

    /** Save results sample **/
    imwrite("../bdd/HoughResults/esc5-Standard.jpg",cdstP);
    imwrite("../bdd/HoughResults/esc5-Prob.jpg",cdstP);

    // Wait and Exit
    waitKey();
    return 0;
}