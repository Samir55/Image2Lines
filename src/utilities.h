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
    vector<valley_id> valleys_ids; ///<
    vector<Point> points; ///< The points representing the line.
    int height; ///< The height of the line region above this line separator.
};

struct Peak {
    int position;
    int value;
    int range_start;
    int range_end;

    Peak () {}

    Peak (int p, int v) : position(p), value(v){
        range_end = range_start = -1;
    }
    Peak (int p, int v, int s, int e) : position(p), value(v), range_start(s), range_end(e) {}
    void print_peak () {
        cout << "Position: " << position << " , Value: " << value << " , Range Start: " << range_start <<  " , Raneg End: " << range_end << endl;
    }

    bool operator < (const Peak &p) const {
        return value > p.value;
    }

    static bool comp (const Peak &a, const Peak &b)  {
        return a.position < b.position;
    }

};

struct Valley {
    int chunk_order;
    int valley_id;
    int position;
    int value;
    bool used;

    Valley (int v_id) : valley_id(v_id), used(false) {}

    Valley (int c_id, int v_id, int p, int v) : chunk_order(c_id), valley_id(v_id), position(p), value(v), used(false) {
    }

    static bool comp (const Valley *a, const Valley *b)  {
        return a->position < b->position; // Write here the condition that must hold for all valleys.
    }
};

struct Region {
    cv::Mat region;
    cv::Mat covar;
    cv::Mat mean;
    Region(cv::Mat a, cv::Mat b, cv::Mat c) : region(a), covar(b), mean(c) {
    }
};
#endif //IMAGE2CHAR_UTILITIES_H_H
