#ifndef IMAGE2CHAR_UTILITIES_H_H
#define IMAGE2CHAR_UTILITIES_H_H

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

typedef int valley_id;

using namespace cv;
using namespace std;

/// A class representing the separator between line regions.
struct Line {
    int index;
    ///< Index of the line in the vector.
    vector<valley_id> valleys_ids;
    ///< The ids of the valleys.
    int start_row_position;
    ///< The row at which the region starts.
    int height;
    ///< The height of the line region above this line separator.
    vector<Point> points;
    ///< The points representing the line.

    Line(int idx, int initial_valley_id) : start_row_position(-1), height(0), points(vector<Point>()) {
        this->index = idx;
        valleys_ids.push_back(initial_valley_id);
    }
};

struct Peak {
    int position;
    int value;
    int range_start;
    int range_end;

    Peak() {}

    Peak(int p, int v)
            : position(p), value(v) {
        range_end = range_start = -1;
    }

    Peak(int p, int v, int s, int e)
            : position(p), value(v), range_start(s), range_end(e) {}

    void
    print_peak() {
        cout << "Position: " << position << " , Value: " << value << " , Range Start: " << range_start
             << " , Raneg End: " << range_end << endl;
    }

    bool
    operator<(const Peak &p) const {
        return value > p.value;
    }

    static bool
    comp(const Peak &a, const Peak &b) {
        return a.position < b.position;
    }

};

struct Valley {
    int chunk_order;
    int valley_id;
    int position;
    int value;
    bool used;

    Valley(int v_id)
            : valley_id(v_id), used(false) {}

    Valley(int c_id, int v_id, int p, int v)
            : chunk_order(c_id), valley_id(v_id), position(p), value(v), used(false) {
    }

    static bool
    comp(const Valley *a, const Valley *b) {
        return a->position < b->position; // Write here the condition that must hold for all valleys.
    }
};

struct Region {
    cv::Mat region;
    vector<int> row_offset;
    cv::Mat covariance;
    cv::Vec2f mean;

    Region(cv::Mat a, vector<int> ro) {
        region = a.clone();
        row_offset = ro;
        calculate_mean();
        calculate_covariance();
    }

    ///
    void
    calculate_mean() {
        mean[0] = mean[1] = 0.0f;
        int n = 0;
        for (int i = 0; i < region.rows; i++) {
            for (int j = 0; j < region.cols; j++) {
                // if white pixel continue.
                if (region.at<uchar>(i, j) == 255) continue;
                if (n == 0) {
                    n = n + 1;
                    mean = Vec2f(i+row_offset[j], j);
                } else {
                    mean = (n - 1.0) / n * mean + 1.0 / n * Vec2f(i+row_offset[j], j);
                    n = n + 1.0;
                }
            }
        }
        cout << mean[0] << " " << mean[1] << endl;
    }

    ///
    void
    calculate_covariance() {
        Mat covariance = Mat::zeros(2, 2, CV_32F);
        int n = 0;
        float sumXSquared = 0, sumYSquared = 0, sumXY = 0;

        for (int i = 0; i < region.rows; i++) {
            for (int j = 0; j < region.cols; j++) {
                // if white pixel continue.
                if ((int) region.at<uchar>(i, j) == 255) continue;

                float newI = i+row_offset[j] - mean[0];
                float newJ = j - mean[1];
                sumXSquared += newI * newI;
                sumXY += newI * newJ;
                sumYSquared += newJ * newJ;
                n++;
            }
        }
        covariance.at<float>(0, 0) = sumXSquared / n;
        covariance.at<float>(0, 1) = sumXY / n;
        covariance.at<float>(1, 0) = sumXY / n;
        covariance.at<float>(1, 1) = sumYSquared / n;

        this->covariance = covariance.clone();
    }

    /// Calculate bi-variate Gaussian density.
    float biVarGaussianDensity(Mat point) {
        point.at<float>(0, 0) -= this->mean[0];
        point.at<float>(0, 1) -= this->mean[1];

        Mat pointTrans;
        transpose(point, pointTrans);

        Mat ret = ((point * this->covariance.inv() * pointTrans));
        ret *= sqrt(determinant(this->covariance * 2 * M_PI));

        return ret.at<float>(0, 0);
    }
};

#endif //IMAGE2CHAR_UTILITIES_H_H 3.11*10^11
