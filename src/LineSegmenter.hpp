#include "utilities.h"
#define CHUNKS_NUMBER 20

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
class Chunk {
public:
    cv::Mat img;
    int order;
    int start_col;
    int width;
    vector<Contour> contours;
    vector<vector<Point>> alt_contours; ///< Used as a temproray alternative to our generated contours.
};

// WIP Line Segmenter class.
class LineSegmenter {
public:
///
/// \param img
/// \return
    cv::Mat preprocess(const cv::Mat img);

///
/// \param img
/// \return
    vector<Chunk> get_chunks(const cv::Mat img);
};
