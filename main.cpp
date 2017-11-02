#include "utilities.h"

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
    cv::Mat img_bw;

    // Apply threshold.
    cv::threshold(im_gray, img_bw, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    cv::imwrite("img_bw3.jpg", img_bw);

    return 0;
}