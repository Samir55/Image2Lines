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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define CHUNKS_NUMBER 20
#define CHUNKS_TO_BE_PROCESSED 5
#define TEST_LINE_COLOR cv::Vec3b(255, 0, 255) // Magenta color.

typedef int valley_id;

using namespace cv;
using namespace std;

class LineSegmentation;

class Region;

class Valley;

/// A class representing the separator between line regions.
class Line {
    friend class LineSegmentation;

    friend class Region;

private:
    Region *above;
    ///< Region above the line.
    Region *below;
    ///< Region below the line.
    vector<valley_id> valleys_ids;
    ///< The ids of the valleys.
    int min_row_position;
    ///< The row at which the region starts.
    int max_row_position;
    ///< The row at which the region ends.
    vector<Point> points;
    ///< The points representing the line.

    Line(int initial_valley_id);

    /// Generate the initial line points.
    void
    generate_initial_points(int chunk_width, int img_width, map<int, Valley *> map_valley);

    /// Sort accendingly according to the min row position.
    static bool
    comp_min_row_position(const Line *a, const Line *b);
};

/// A class representing the peaks (local maximum points in the histogram).
class Peak {
public:
    int position;
    ///< The row position.
    int value;
    ///< The number of foreground pixels.

    Peak() {}

    Peak(int p, int v)
        : position(p), value(v)
    {}

    Peak(int p, int v, int s, int e)
        : position(p), value(v)
    {}

    /// Compare according to the value. // ToDo @Samir55 change that when finishing.
    bool
    operator<(const Peak &p) const;

    /// Compare according to the row position
    static bool
    comp(const Peak &a, const Peak &b);
};

/// A class representing the valleys (local minimum points in the histogram)
class Valley {
public:
    static int ID;
    ///< Next available id.
    int chunk_index;
    ///< The index of the chunk in the chunks vector.
    int valley_id;
    ///< The valley id.
    int position;
    ///< The row position.
    bool used;
    /// Whether it's used by a line or not.
    Line *line;
    /// The line to which this valley is connected.

    Valley()
        : valley_id(ID++), used(false)
    {}

    Valley(int c_id, int p)
        : chunk_index(c_id), valley_id(ID++), position(p), used(false)
    {}

    static bool
    comp(const Valley *a, const Valley *b);
};

/// A class representing the line regions.
class Region {
    friend class LineSegmentation;

private:
    int region_id;
    ///< Region id.
    cv::Mat region;
    ///< 2D matrix representing the region.
    Line *top;
    ///< Region top boundaries;
    Line *bottom;
    ///< Region bottom boundaries;
    int height;
    ///< Region height (the maximum distance between the above and bottom line)
    int row_offset;
    ///< The offset of each col to the original image matrix.
    cv::Mat covariance;
    ///< The covariance of the matrix.
    cv::Vec2f mean;
    ///< The mean of the matrix.

    Region(Line *top, Line *bottom);

    /// Get the region matrix
    /// \param img
    /// \param region_id
    /// \return
    bool
    update_region(Mat &img, int);

    /// Calculate region black pixels mean relative to the whole image dimensions
    void
    calculate_mean();

    /// Calculate region black pixels covariance relative to the whole image dimensions
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
    /// \param map_valley to fill it
    /// \return int the average line height in this chunk.
    int
    find_peaks_valleys(map<int, Valley *> &map_valley);

private:
    int index;
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
    int avg_white_height;
    ///< The average space height in this chunk.
    int lines_count;
    ///< The estimated number of lines in this chunk.

    Chunk(int o, int c, int w, cv::Mat i);

    /// Calculate the chunk histogram (Y projection profile).
    /// This function is called by find_peaks_valleys.
    void
    calculate_histogram();
};

/// Line Segmentation class.
class LineSegmentation {
private:
    bool not_primes_arr[100007];
    vector<int> primes;

    void
    sieve();

    void
    addPrimesToVector(int, vector<int> &);

public:
    LineSegmentation(string path_of_image);

    /// Generate the lines found in the saved image.
    /// \return vector<cv::Mat> a vector containing each line as a 2D mat.
    vector<cv::Mat>
    segment();

    /// Save current line regions returned from get_regions() or segment() functions in jpg files.
    /// \param lines
    /// \param path
    void save_lines_to_file(const vector<cv::Mat> &lines, string path);

private:
    string image_path;
    ///< Path of the image.
    cv::Mat color_img;
    ///< Used for debugging only.
    cv::Mat grey_img;
    ///< The grey image.
    cv::Mat binary_img;
    ///< The preprocessed image.
    vector<Chunk *> chunks;
    ///< The image chunks.
    map<int, Chunk *> chunk_map;
    ///< Map the Chunk id and its corresponding Chunk pointer
    map<int, Valley *> map_valley;
    ///< Map the Valley id and its corresponding Valley pointer.
    vector<Line *> initial_lines;
    ///< The initial lines.
    vector<Region *> line_regions;
    ///< The regions of all found initial lines in the image.
    vector<Rect> contours;
    /// The handwritten components found in the binary image.
    int avg_line_height;
    ///< The average height of lines in the image.
    int predicted_line_height;
    ///< The predicted height of lines in the image.
    int chunk_width;
    ///< width of the chunk.
    // ToDo @Samir55 add CHUNKS_TO_BE_PROCESSED when needed.

    /// Read the image file into CV matrix
    void
    read_image();

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

    /// Generate the lines regions (A 2D mat describing each line in the image).
    void
    generate_regions();

    /// ToDo @Samir55.
    vector<cv::Mat>
    get_regions();

    /// Use statistical approach to repair the initial lines (refer to the paper).
    void
    repair_lines();

    /// Check if the component belongs to the above region
    bool
    component_belongs_to_above_region(Line &, Rect &);

    /// Draw the lines on the original color image for debugging.
    /// \param path string the path of the output image.
    void
    save_image_with_lines(string path);

    /// Connect the nearest valleys found in image chunks to form an initial line in a recursive manner.
    /// This function is called by find_initial_lines.
    /// \param i integer The index of the chunk.
    /// \param current_valley Valley The current valley.
    /// \return Line a candidate(initial line)
    Line *
    connect_valleys(int i, Valley *current_valley, Line *line, int valleys_min_abs_dist);
};
