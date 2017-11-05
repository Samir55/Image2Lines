// WIP Line Segmentation, To be later added to dedicated class.
#include "utilities.h"
#define CHUNKS_NUMBER 20

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
struct Chunk {
    cv::Mat img;
    int order;
    int start_col;
    int width;
    vector<Contour> contours;
};

///
/// \param img
/// \return
cv::Mat preprocess(const cv::Mat img) {
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
vector<Chunk> get_chunks(const cv::Mat img) {

    int width = img.cols;
    int chunk_width = width / CHUNKS_NUMBER;

    vector<Chunk> chunks(CHUNKS_NUMBER);
    for (int i_chunk = 0, start_pixel = 0; i_chunk < CHUNKS_NUMBER; ++i_chunk) {
        chunks[i_chunk].order = i_chunk + 1;
        chunks[i_chunk].start_col = start_pixel;
        chunks[i_chunk].width = chunk_width;
//        cout << chunk_width <<  " " << start_pixel << " " << img.rows << " " << img.cols << endl; // For debugging.
        chunks[i_chunk].img = cv::Mat (img,
                                       cv::Range( 0, img.rows ), // rows
                                       cv::Range( start_pixel, start_pixel + chunk_width )); // cols
        start_pixel += chunk_width;
//        cv::imwrite(to_string(i_chunk + 1) + ".jpg", chunks[i_chunk].img); // For debugging.
    }
    return chunks;
}

