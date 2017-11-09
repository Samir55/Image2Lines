#include "utilities.h"
#include "LineSegmenter.hpp"

int main() {
    LineSegmenter line_segmenter;
    line_segmenter.org_img = imread("../img/3min.png", CV_LOAD_IMAGE_GRAYSCALE);

    line_segmenter.preprocess();

    line_segmenter.generate_chunks();

    line_segmenter.find_initial_lines();

    return 0;

}