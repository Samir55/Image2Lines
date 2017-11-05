#include "utilities.h"
#include "LineSegmenter.hpp"

/// Debugging.
void testing() {
    // Read image and convert it to greyscale.
    cv::Mat im_gray = cv::imread("../img/5circles.jpg",CV_LOAD_IMAGE_GRAYSCALE);

    // Result.
    cv::Mat img_bw,filtered;
    // Apply filters
    blur( im_gray, filtered, Size(3, 3 ), Point(-1,-1) );
    cv::imwrite("img_blurred.jpg", filtered);



    cv::threshold(filtered, img_bw, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    cv::imwrite("img_bw.jpg", img_bw);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours( img_bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    cout<< contours.size();
    // you could also reuse img1 here
    Mat mask = Mat::zeros(im_gray.rows, im_gray.cols, CV_8UC1);

    // CV_FILLED fills the connected components found
    for( size_t i = 0; i< contours.size()-1; i++ )
    {
        drawContours( mask, contours, (int)i, Scalar(255), 1, 8, hierarchy, 0, Point() );
        cout<<"hello"<<endl;
    }

    cv::imwrite("img_bw2.jpg", mask);
//    /// Draw contours
//    for(int i= 0; i < contours.size(); i++)
//    {
//        for(int j= 0; j < contours[i].size();j++) // run until j < contours[i].size();
//        {
//            cout << contours[i][j] << endl; //do whatever
//        }
//        cout <<"hi"<<endl;
//    }
//    RNG rng(12345);
//
//    for( int i = 0; i< contours.size(); i++ )
//    {
//        if(abzo)
//           color = (0,0,255);
//        drawContours( img_bw, contours, i, color, 2, 8, hierarchy, 0, Point() );
//    }
//
//    imshow( "Result window", img_bw );
//    waitKey(0);
}

int main() {

    cv:Mat img = imread("../img/1.png");

//    img = preprocess(img);
//
//    vector<Chunk> ret = get_chunks(img);


    return 0;

}