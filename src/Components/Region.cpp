//
// Created by Ibrahim Radwan on 11/29/17.
//

#include "Region.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

bool
Region::update_region(Mat &binary_image, int region_id) {
    this->region_id = region_id;

    int min_region_row = row_offset = (top == nullptr) ? 0 : top->min_row_position;
    int max_region_row = (bottom == nullptr) ? binary_image.rows : bottom->max_row_position;

    region = Mat::ones(max_region_row - min_region_row, binary_image.cols, CV_8U) * 255;

    // Fill region.
    if (bottom != nullptr) {
        for (int c = 0; c < binary_image.cols; c++) {
            int start = ((top == nullptr) ? 0 : top->points[c].x);
            for (int i = start; i < bottom->points[c].x; i++) {
                region.at<uchar>(i - min_region_row, c) = binary_image.at<uchar>(i, c);
            }
        }
    }
    calculate_mean();
    calculate_covariance();

    imwrite("out/" + string("Region") + to_string(region_id) + ".jpg",
            region);

    return countNonZero(region) == region.cols * region.rows;
}


Region::Region(Line *top, Line *bottom) {
    this->top = top;
    this->bottom = bottom;
}

void
Region::calculate_mean() {
    mean[0] = mean[1] = 0.0f;

    int n = 0;

    for (int i = 0; i < region.rows; i++) {
        for (int j = 0; j < region.cols; j++) {
            // if white pixel continue.
            if (region.at<uchar>(i, j) == 255) continue;

            if (n == 0) {
                n++;
                mean = Vec2f(i + row_offset, j);
            } else {
                mean = (n - 1.0) / n * mean + 1.0 / n * Vec2f(i + row_offset, j);
                n++;
            }
        }
    }
}

void
Region::calculate_covariance() {
    Mat covariance = Mat::zeros(2, 2, CV_32F);

    int n = 0; // # of points
    float sum_i_squared = 0, sum_j_squared = 0, sum_i_j = 0;

    for (int i = 0; i < region.rows; i++) {
        for (int j = 0; j < region.cols; j++) {
            // if white pixel continue.
            if ((int) region.at<uchar>(i, j) == 255) continue;

            float new_i = i + row_offset - mean[0];
            float new_j = j - mean[1];

            sum_i_squared += new_i * new_i;
            sum_i_j += new_i * new_j;
            sum_j_squared += new_j * new_j;

            n++;
        }
    }
    if (n) {
        covariance.at<float>(0, 0) = sum_i_squared / n;
        covariance.at<float>(0, 1) = sum_i_j / n;
        covariance.at<float>(1, 0) = sum_i_j / n;
        covariance.at<float>(1, 1) = sum_j_squared / n;

    }
    this->covariance = covariance.clone();
}

double
Region::bi_variate_gaussian_density(Mat point) {
    point.at<float>(0, 0) -= this->mean[0];
    point.at<float>(0, 1) -= this->mean[1];

    Mat point_transpose;
    transpose(point, point_transpose);

    // Calculate bi_variate probability equation
    Mat ret = ((point * this->covariance.inv() * point_transpose));
    ret *= sqrt(determinant(this->covariance * 2 * M_PI));

    return ret.at<float>(0, 0);
}
