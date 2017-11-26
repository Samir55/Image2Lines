#include "utilities.h"

#define CHUNKS_NUMBER 20
#define CHUNKS_TO_BE_PROCESSED 5
#define TEST_LINE_COLOR cv::Vec3b(255, 0, 255)

/// Image Chunk.
class Chunk
{
public:
    int order;
    /// The index of the chunk.
    int start_col;
    ///< The start column position.
    int width;
    ///< The width of the chunk.
    cv::Mat img;
    ///< //grey level image
    vector<int> histogram;
    ///< The values of the y histogram projection profile.
    vector<Peak> peaks;
    ///< The found peaks in this chunk.
    vector<Valley *> valleys;
    ///< The found valleys in this chunk.

    Chunk(int o, int c, int w, cv::Mat i);

    /// Valleys and peaks detection in this image chunk.
    /// \return int the average line height in this chunk.
    int
    find_peaks_valleys();
};

// Line Segmentation class.
class LineSegmentation
{
public:
    LineSegmentation(string path_of_image);

    /// Generate the lines found in the saved image/
    /// \return vector<cv::Mat> a vector contining each line as a 2D mat.
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
    vector<Rect> contours; /// The handwritten components found in the binary image.

    /// Apply OTSU thresholding and Binarization to the grey image.
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

    /// ToDo @Samir55
    void
    draw_image_with_lines(bool save_img = true);

    /// Get the lines regions ( A 2D mat describing each line in the image).
    void
    get_regions();

    /// Use Statistical approach to repair the initial lines.
    // When hitting get the hit component and get the above and the below line regions.
    // Apply to each pixel in each line region P(p |μ,Σ) = |2πΣ|1 (p − μ)Σ−1(p − μ)T to get 2 probabilities.
    // Assign the region to the correct line region and update the points of the separator line.
    void
    repair_lines();

    /// Draw the lines on the original color image for debugging.
    void
    show_lines();

    /// Connect the nearest valleys found in image chunks to form an initial line in a recursive manner.
    /// This function is called by find_initial_lines.
    /// \param i integer The index of the chunk.
    /// \param current_valley Valley The current valley.
    /// \return Line a candidate(initial line)
    Line
    connect_valleys(int i, Valley *current_valley, Line &line, int valleys_min_abs_dist);
};
