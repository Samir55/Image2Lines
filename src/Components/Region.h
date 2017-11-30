//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_REGION_H
#define IMAGE2LINES2_REGION_H

#include "Line.h"

/// A class representing the line regions.
class Region {
    friend class LineSegmentation;

private:
    int region_id;
    ///< Region id
    Mat region;
    ///< 2D matrix representing the region.
    Line *top;
    ///< Region top boundaries;
    Line *bottom;
    ///< Region bottom boundaries;
    int row_offset;
    ///< A vector containing the offset of each col to the original image matrix.
    Mat covariance;
    ///< The covariance of the matrix.
    Vec2f mean;
    ///< The mean of the matrix.

    Region(Line *top, Line *bottom);

    /// Get the region matrix
    /// \param img
    /// \param region_id
    /// \return
    bool update_region(Mat &img, int);

    /// Calculate region black pixels covariance relative to the whole image dimensions
    void
    calculate_mean();

    /// Calculate region black pixels mean relative to the whole image dimensions
    void
    calculate_covariance();

    /// Calculate bi-variate Gaussian density given a point.
    double
    bi_variate_gaussian_density(Mat point);
};


#endif //IMAGE2LINES2_REGION_H
