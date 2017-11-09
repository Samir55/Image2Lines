#include "utilities.h"
#include "LineSegmenter.hpp"

/// Debugging.
void testing() {
    // Read image and convert it to greyscale.
    cv::Mat im_gray = cv::imread("../img/1.png",CV_LOAD_IMAGE_GRAYSCALE);

}

int main() {
    LineSegmenter line_segmenter;
    line_segmenter.org_img = imread("../img/3min.png", CV_LOAD_IMAGE_GRAYSCALE);

    line_segmenter.preprocess();

    line_segmenter.generate_chunks();

    line_segmenter.find_initial_lines();

    return 0;

}