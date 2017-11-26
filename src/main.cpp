#include "LineSegmentation.hpp"

int main() {
    string img_path = "../img/1min.png";
    LineSegmentation line_segmentation(img_path);
    line_segmentation.get_lines();


    // Testing
//    cv::Mat m = Mat::ones(50, 50, CV_8U) * 255;
//
//    for (int i = 0; i < 50; ++i) {
//        m.at<uchar>(i, i) = 0;
//    }
//
//    Region r(m, cv::Mat(), cv::Mat());
//    cout << r.covariance.at<float>(0,0) << " " << r.covariance.at<float>(0,1) << endl
//         << r.covariance.at<float>(1,0) << " " << r.covariance.at<float>(1,1) << endl;
    return 0;
}