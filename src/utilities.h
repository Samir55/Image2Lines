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
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

typedef int valley_id;

using namespace cv;
using namespace std;

/// A class representing the separator between line regions.
struct Line {
    int index; ///< Index of the line in the vector.
    vector<valley_id> valleys_ids; ///< The ids of the valleys.
    vector<Point> points; ///< The points representing the line.
    int start_row_position; ///< The row at which the region starts.
    int height; ///< The height of the line region above this line separator.
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
    cv::Mat covariance;
    cv::Mat mean;

    Region(cv::Mat a, cv::Mat b, cv::Mat c) {
        region = a.clone();
    }

    ///
    void
    calculate_mean() {


    }

    ///
    void
    calculate_covariance() {
        Mat m1 = Mat::zeros(2, region.rows * region.cols, CV_32F), m2(region.rows * region.cols, 2, CV_32F);
        int n = 0;
        for (int i = 0; i < region.rows; ++i) {
            for (int j = 0; j < region.cols; ++j) {
                if (region.at(i, j) == 0) {
                    m1.at(n, 0) = i - mean.at(0);
                    m1.at(n, 1) = j - mean.at(1);

                    m2.at(0, n) = i - mean.at(0);
                    m2.at(1, n) = j - mean.at(1);

                    n++;
                }
            }
        }
        m1 = Mat(m1, 2, Range(0, n));
        m2 = Mat(m21, Range(0, n), 2);

        covariance = ((m1 * m2) / n);
        for (int i = 0; i < covariance.rows; ++i) {
            for (int j = 0; j < covariance.cols; ++j) {
                cout << covariance.at(i, j) << " ";
            }
            cout << endl;
        }
        this.covariance = covariance.clone();
    }

};

#endif //IMAGE2CHAR_UTILITIES_H_H
