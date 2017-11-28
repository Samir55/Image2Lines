#include "LineSegmentation.hpp"

int main() {
    string img_path = "../img/1min.png";
    LineSegmentation line_segmentation(img_path);
    cv::Mat lines = line_segmentation.get_lines();
    return 0;
}