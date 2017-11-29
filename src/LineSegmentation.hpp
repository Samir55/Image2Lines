#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
#include <cstring>
#include <cmath>
#include <cv.h>
#include <math.h>
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#define CHUNKS_NUMBER 20
#define CHUNKS_TO_BE_PROCESSED 7
#define TEST_LINE_COLOR cv::Vec3b(255, 0, 255) // Magenta color.

typedef int valley_id;

using namespace cv;
using namespace std;

class LineSegmentation;

/// A class representing the separator between line regions.
struct Line {
    friend class LineSegmentation;

private:
    int index;
    ///< Index of the line in the vector.
    vector<valley_id> valleys_ids;
    ///< The ids of the valleys.
    int start_row_position;
    ///< The row at which the region starts.
    int end_row_position;
    ///< The row at which the region ends.
    int height;
    ///< The height of the line region above this line separator.
    vector<Point> points;
    ///< The points representing the line.

    Line(int idx, int initial_valley_id);
};

/// A class representing the peaks (local maximum points in the histogram).
class Peak {
public:
    int position;
    ///< The row position.
    int value;
    ///< The number of foreground pixels.

    Peak() {}

    Peak(int p, int v) : position(p), value(v) {}

    Peak(int p, int v, int s, int e) : position(p), value(v) {}

    /// Compare according to the value.
    bool
    operator<(const Peak &p) const;

    /// Compare according to the row position
    static bool
    comp(const Peak &a, const Peak &b);
};

/// A class representing the valleys (local minimum points in the histogram)
class Valley {
public:
    int chunk_order;
    ///< The index of the chunk in the chunks vector.
    int valley_id;
    ///< The valley id.
    int position;
    ///< The row position.
    bool used;
    /// Whether it's used by a line or not.

    Valley(int v_id) : valley_id(v_id), used(false) {}

    Valley(int c_id, int v_id, int p, int v) : chunk_order(c_id), valley_id(v_id), position(p), used(false) {}

    static bool
    comp(const Valley *a, const Valley *b);
};

/// A class representing the line regions.
class Region {
    friend class LineSegmentation;

private:
    cv::Mat region;
    ///< 2D matrix representing the region.
    int start_row;
    ///<
    int end_row;
    ///<
    vector<int> row_offset;
    ///< A vector containing the offset of each col to the original image matrix.
    cv::Mat covariance;
    ///< The covariance of the matrix.
    cv::Vec2f mean;
    ///< The mean of the matrix.

    Region(cv::Mat a, vector<int> ro);

    void
    calculate_mean();

    void
    calculate_covariance();

    /// Calculate bi-variate Gaussian density given a point.
    double
    bi_variate_gaussian_density(Mat point);
};

/// Image Chunk.
class Chunk {
    friend class LineSegmentation;

    /// Valleys and peaks detection in this image chunk.
    /// \return int the average line height in this chunk.
    int
    find_peaks_valleys();

private:
    int order;
    /// The index of the chunk.
    int start_col;
    ///< The start column position.
    int width;
    ///< The width of the chunk.
    cv::Mat img;
    ///< The grey level image
    vector<int> histogram;
    ///< The values of the y histogram projection profile.
    vector<Peak> peaks;
    ///< The found peaks in this chunk.
    vector<Valley *> valleys;
    ///< The found valleys in this chunk.
    int avg_height;
    ///< The average line height in this chunk.
    int lines_count;
    ///< The estimated number of lines in this chunk.

    Chunk(int o, int c, int w, cv::Mat i);

    /// Calculate the chunk histogram (Y projection profile).
    /// This function is called by find_peaks_valleys.
    void calculate_histogram();
};

/// Line Segmentation class.
class LineSegmentation {
public:
    LineSegmentation(string path_of_image);

    /// Generate the lines found in the saved image/
    /// \return vector<cv::Mat> a vector containing each line as a 2D mat.
    vector<cv::Mat>
    get_lines();

private:
    cv::Mat color_img;
    ///< Used for debugging only.
    cv::Mat grey_img;
    ///< The grey image.
    cv::Mat binary_img;
    ///< The preprocessed image.
    vector<Chunk> chunks;
    ///< The image chunks.
    vector<Line> initial_lines;
    ///< The initial lines.
    vector<Region> line_regions;
    ///< The regions of all found initial lines in the image.
    vector<Rect> contours;
    /// The handwritten components found in the binary image.
    int avg_line_height;
    ///< The average height of lines in the image.

    /// Apply OTSU threshold and Binarization to the grey image.
    void
    pre_process_image();

    /// Find handwritten components found in the binary image.
    void
    find_contours();

    /// Generate image chunks according to the current CHUNKS_NUMBER.
    void
    generate_chunks();

    /// Get the initial (candidate lines).
    void
    get_initial_lines();

    /// Generate the initial line points.
    void
    generate_initial_points();

    /// Update the lines regions (A 2D mat describing each line in the image).
    void
    update_regions();

    vector<cv::Mat>
    get_regions();

    /// Use statistical approach to repair the initial lines (refer to the paper).
    void
    repair_lines();

    /// Draw the lines on the original color image for debugging.
    /// \param path string the path of the output image.
    void
    show_lines(string path);

    /// Connect the nearest valleys found in image chunks to form an initial line in a recursive manner.
    /// This function is called by find_initial_lines.
    /// \param i integer The index of the chunk.
    /// \param current_valley Valley The current valley.
    /// \return Line a candidate(initial line)
    Line
    connect_valleys(int i, Valley *current_valley, Line &line, int valleys_min_abs_dist);
};
