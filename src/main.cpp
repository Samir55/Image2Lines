#include "LineSegmentation.hpp"

int main(int argc, char *argv[]) {
	cout << argv[1] << endl;
    string img_path = argv[1];
    LineSegmentation line_segmentation(img_path);
    vector<cv::Mat> lines = line_segmentation.segment();

    // Save lines to path.
    line_segmentation.save_lines_to_file(lines, "");
    return 0;
}