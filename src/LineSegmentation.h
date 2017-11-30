//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_LINESEGMENTATION_H
#define IMAGE2LINES2_LINESEGMENTATION_H

#include "Components/Chunk.h"
#include "Components/Region.h"

class LineSegmentation {
public:
    LineSegmentation(string path_of_image);

    /// Get the lines found in the saved image/
    /// \return vector<Mat> a vector containing each line as a 2D mat.
    vector<Mat> segment();

private:
    string image_path;
    ///< Image path
    map<int, Chunk *> chunk_map;
    ///< Map to map the Chunk id and its corresponding Chunk pointer
    map<int, Valley *> map_valley;
    ///< Map to map the Valley id and its corresponding Valley pointer
    Mat color_img;
    ///< Used for debugging only.
    Mat grey_img;
    ///< The grey image.
    Mat binary_img;
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
    int chunk_width;
    ///< Chunk width
    /// Read the image file into CV matrix
    void read_image();

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

    vector<Mat>
    get_regions();

    /// Use statistical approach to repair the initial lines (refer to the paper).
    void
    repair_lines();

    /// Check if the component belongs to the above region
    bool component_belongs_to_above_region(Line*, Rect);

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


#endif //IMAGE2LINES2_LINESEGMENTATION_H
