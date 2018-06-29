#include "LineSegmentation.hpp"

int main(int argc, char *argv[]) {
	cout << argv[1] << endl;
    cout << argv[2] << endl;

    string img_path = argv[1];
    LineSegmentation line_segmentation(img_path, argv[2]);
    vector<cv::Mat> lines = line_segmentation.segment();
    // Save lines to path.
    line_segmentation.save_lines_to_file(lines);
    return 0;
}