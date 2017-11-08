#include "utilities.h"
#define CHUNKS_NUMBER 20

// Note you can use C++11 new syntax for loops/etc.
// ToDo @Samir55 add initializations.
// ToDo @Samir55 Document any function you implement.
// ToDo @TheAbzo Document any function you implement.

/// Contour(Region) class represents a connected region in the image.
class Contour {
public:
    int x, y;
    ///< Position of top left pixel of this region.
    vector<int> code_chain; ///< The code chain representation (8- neighbourhood).
    vector<pair<int, int>> y_projection_profile; ///<

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
    vector<int> histogram;
    vector<int> peaks_positions;
    vector<int> valleys_positions;

    // ToDo @TheAbzo.
    ///
    void find_contours();

    void calculate_histogram();

};

// WIP Line Segmenter class.
class LineSegmenter {
public:
    cv::Mat org_img;
    cv::Mat img;
    vector<Chunk> chunks;

///
/// \param img
/// \return
    void preprocess();

///
/// \param img
/// \return
    void generate_chunks();
};
