#include <opencv/cv.hpp>
#include "utilities.h"

using namespace cv;

/// Contour(Region) class represents a connected region in the image.
class Contour {
public:
    int x, y;
    ///< Position of top left pixel of this region.
    vector<int> code_chain; ///< The code chain representation (8- neighbourhood).

    ///
    /// \param img
    void generate_code_chain(cv::Mat &img) {
        // Border tracing.
    }
};

int main() {
    // Read image and convert it to greyscale.
    cv::Mat im_gray = cv::imread("../img/1.png",CV_LOAD_IMAGE_GRAYSCALE);

    // Result.
    cv::Mat img_bw,filtered;
    // Apply filters
    blur( im_gray, filtered, Size(3, 3 ), Point(-1,-1) );
    cv::imwrite("img_blurred.jpg", filtered);
    cv::threshold(filtered, img_bw, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("img_bw.jpg", img_bw);
    //contours detection

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours( img_bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE,Point(0,0) );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() -1);

    for( size_t i = 0; i < contours.size()-1; i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 1, true );//apply approximation to polygons with accuracy +-3
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    }
                //merging the intersection/union rectangles
                //loop on all of rectangles
                Rect2d rectangle3 ;
                vector<Rect> merged_rectangles;
    bool is_repeated=false;
    Mat drawing = Mat::zeros( img_bw.size(), CV_8UC3 );
                for(int i=0; i<boundRect.size(); i++)
                {is_repeated=false;
                    for(int j=i+1; j<boundRect.size(); j++) {
                        rectangle3 = boundRect[i] & boundRect[j];
                        //check for intersection/union
                        if (rectangle3.area() > 0) {is_repeated=true;
                          rectangle3=boundRect[i] | boundRect[j];

                            Rect2d merged_rectangle(rectangle3.tl().x, rectangle3.tl().y, rectangle3.width, rectangle3.height);
                            //push in merged rectangles
                        cout<<rectangle3.width<<" "<<i<<" "<<j <<endl;
                            if(j==boundRect.size()-2)
                            merged_rectangles.push_back(merged_rectangle);
                            //update the current vector
                            boundRect[j]=merged_rectangle;

                        }

                    }
                    if(!is_repeated)
                        merged_rectangles.push_back(boundRect[i]);
                }



    //groupRectangles(boundRect,0,0.0001);

    for( size_t i = 0; i< merged_rectangles.size(); i++ )
    {
        //drawContours( drawing, contours_poly, (int)i, Scalar(255), 1, 8, vector<Vec4i>(), 0, Point() );
      // rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255), 2, 8, 0 );
       rectangle( drawing, merged_rectangles[i].tl(), merged_rectangles[i].br(), Scalar(255), 2, 8, 0 );

    }
    //rectangle( drawing, merged_rectangles[1].tl(), merged_rectangles[1].br(), Scalar(255), 2, 8, 0 );
    //rectangle( drawing, merged_rectangles[0].tl(), merged_rectangles[0].br(), Scalar(255), 2, 8, 0 );
  //  rectangle( drawing, merged_rectangles[3].tl(), merged_rectangles[0].br(), Scalar(255), 2, 8, 0 );

//    Rect2d testing(0,0, 100,100);//assuming it the left corner point
//    rectangle( drawing, testing.tl(), testing.br(), Scalar(255), 2, 8, 0 );

    cv::imwrite("contours.jpg", drawing);





//    // you could also reuse img1 here
//    Mat mask = Mat::zeros(im_gray.rows, im_gray.cols, CV_8UC1);
//
//    // CV_FILLED fills the connected components found
////    for( size_t i = 0; i< contours.size()-1; i++ )
////    {
////        drawContours( mask, contours, (int)i, Scalar(255), 1, 8, hierarchy, 0, Point() );
////    }
//
//    cv::imwrite("img_bw2.jpg", mask);
////    /// Draw contours
////    for(int i= 0; i < contours.size(); i++)
////    {
////        for(int j= 0; j < contours[i].size();j++) // run until j < contours[i].size();
////        {
////            cout << contours[i][j] << endl; //do whatever
////        }
////        cout <<"hi"<<endl;
////    }
////    RNG rng(12345);
////
////    for( int i = 0; i< contours.size(); i++ )
////    {
////        if(abzo)
////           color = (0,0,255);
////        drawContours( img_bw, contours, i, color, 2, 8, hierarchy, 0, Point() );
////    }
////
////    imshow( "Result window", img_bw );
////    waitKey(0);
    return 0;

}