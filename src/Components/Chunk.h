//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_CHUNK_H
#define IMAGE2LINES2_CHUNK_H

#include "Peak.h"
#include "Valley.h"
#include "../Utilities/Utilities.h"

/// Image Chunk.
class Chunk {
    friend class LineSegmentation;

    /// Valleys and peaks detection in this image chunk.
    /// \param map_valley to fill it
    /// \return int the average line height in this chunk.
    int
    find_peaks_valleys(map<int, Valley *>& map_valley);

private:
    int index;
    /// The index of the chunk.
    int start_col;
    ///< The start column position.
    int width;
    ///< The width of the chunk.
    Mat img;
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

    Chunk(int o, int c, int w, Mat i);

    /// Calculate the chunk histogram (Y projection profile).
    /// This function is called by find_peaks_valleys.
    void calculate_histogram();
};


#endif //IMAGE2LINES2_CHUNK_H
