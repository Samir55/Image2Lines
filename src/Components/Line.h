//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_LINE_H
#define IMAGE2LINES2_LINE_H

#include "Valley.h"
#include "../Utilities/Utilities.h"

class Region;

/// A class representing the separator between line regions.
class Line {
    friend class LineSegmentation;

    friend class Region;

private:
    ///< Index of the line in the vector.
    Region *above;
    ///< Region above the line
    Region *below;
    ///< Region below the line
    int min_row_position = 0, max_row_position = 0;
    ///< Minimum/Maximum row point on this line
    vector<int> valleys_ids;
    ///< The ids of the valleys.
    vector<Point> points;
    ///< The points representing the line.

    Line(int initial_valley_id);

    /// Generate the initial line points.
    void
    generate_initial_points(int chunk_width, int img_width, map<int, Valley *> map_valley);
};


#endif //IMAGE2LINES2_LINE_H
