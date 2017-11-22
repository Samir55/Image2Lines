#include "utilities.h"
#define CHUNKS_NUMBER 20
#define TEST_LINE_COLOR cv::Vec3b(255, 0, 255)

// Note you can use C++11 new syntax for loops/etc.
// ToDo @Samir55 add initializations.
// ToDo @Samir55 Document any function you implement.
// ToDo @TheAbzo Document any function you implement.

/// Contour(Region) class represents a connected region in the image.
class Contour
{
public:
    int x, y;
    ///< Position of top left pixel of this region.
    vector<int> code_chain; ///< The code chain representation (8- neighbourhood).
    vector<pair<int, int>> y_projection_profile; ///<

    ///
    /// \param img
    void generate_code_chain(cv::Mat &img) {};
};

///
class Chunk
{
public:
    cv::Mat img; ///< //grey level image
    int order; ///<
    int start_col; ///<
    int width; ///<
    vector<Rect> rectangular_contours; ///<//
    vector<int> histogram; ///<
    vector<Peak> peaks; ///<
    vector<Valley*> valleys; ///<

    ///
    Chunk() {
        valleys = vector<Valley*>();
    };

    ///
    void find_contours();

    /// Contours detection in this image chunk.
    /// \return
    int calculate_histogram();
};

// WIP Line Segmenter class.
class LineSegmenter
{
public:
    cv::Mat color_img; ///< Used for debugging only.
    cv::Mat org_img; ///<
    cv::Mat img; ///<
    vector<Chunk> chunks; ///<
    vector<Line> initial_lines; ///<
    int valleys_min_abs_dist;

    ///
    void preprocess();

    ///
    void generate_chunks();

    ///
    void find_initial_lines();

    ///
    void draw_image_with_lines();

private:
    ///
    /// \param i
    /// \param current_valley
    /// \return
    Line connect_line_valleys (int i, Valley* current_valley, Line& line);
};
