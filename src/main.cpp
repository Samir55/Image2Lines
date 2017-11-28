#include "LineSegmentation.hpp"

int main() {
    string img_path = "../img/3min.png";
    LineSegmentation line_segmentation(img_path);
    line_segmentation.get_lines();
    return 0;
}