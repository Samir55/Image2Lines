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

/// Calculate bi-variate Gaussian density
float biVarGaussianDensity(cv::Mat point, Vec2f mean, cv::Mat coVariance) {
    point(0, 0) -= mean[0];
    point(1, 0) -= mean[1];

    Mat pointTrans;
    transpose(point, pointTrans);

    return sqrt(determinant(coVariance) * 2 * M_PI) * (point * coVariance.inv() * pointTrans);
}

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
    cv::Vec2f mean;

    Region(cv::Mat a, cv::Mat b, cv::Mat c) {
        region = a.clone();
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
                    mean = Vec2f(i, j);
                } else {
                    mean = (n - 1.0) / n * mean + 1.0 / n * Vec2f(i, j);
                    n = n + 1.0;
                }
            }
        }
        cout << mean[0] << " " << mean[1] << endl;
    }

    ///
    void
    calculate_covariance() {
        covariance = Mat::zeros(2, 2, CV_32F);
        int n = 0;

        for (int i = 0; i < region.rows; i++) {
            for (int j = 0; j < region.cols; j++) {
                // if white pixel continue.
                if (region.at<uchar>(i, j) == 255) continue;

                Mat point = Mat::zeros(2, 1, CV_32F);
                point.at<float>(0, 0) = i - mean[0];
                point.at<float>(1, 0) = j - mean[1];

                Mat pointTrans;
                transpose(point, pointTrans);

                covariance = ((n - 2.0) / (n - 1)) * covariance + (1.0 / (n - 1)) * (point * pointTrans);

                n++;
                if (n == 1) n++;
            }
        }
    }

};

#endif //IMAGE2CHAR_UTILITIES_H_H
