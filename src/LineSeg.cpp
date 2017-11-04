// WIP Line Segmentation, To be later added to dedicated class.
#include "utilities.h"

vector<vector<Point>> contours; ///< Used as a temproray alternative to our generated contours.

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

///
struct ImageChunk {
    int start_col, end_col;
    vector<int> contours_ids;
};

///
/// \param img
/// \return
cv::Mat preprocess_img (const cv::Mat img) {
    // More filters are about to be applied. TheAbzo job.
    cv::Mat preprocessed_img, smoothed_img;

    // Noise reduction (Currently a basic filter).
    cv::blur( img, smoothed_img, Size(3, 3 ), Point(-1,-1) );

    // OTSU thresholding and Binarization.
    cv::threshold(smoothed_img, preprocessed_img, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    return preprocessed_img;
}

///
/// \param img
/// \return
vector<ImageChunk> get_img_chunks (cv::Mat img) {
    int width = img.cols;
    int chunk_width = width / 20;
    vector<ImageChunk> chunks(20);
    for (int i_chunk = 0, start_pixel = 1; i_chunk < 20; ++i_chunk) { // ToDo @Samir55 Here check;
        chunks[i_chunk].start_col = start_pixel;
        chunks[i_chunk].end_col = start_pixel + chunk_width;
        start_pixel += chunk_width;
    }
    return chunks;
}

