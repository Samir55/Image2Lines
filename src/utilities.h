#ifndef IMAGE2CHAR_UTILITIES_H_H
#define IMAGE2CHAR_UTILITIES_H_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cstring>
#include <cmath>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
using namespace cv;
using namespace std;

struct Peak{
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

struct Valley{
    int position;
    int value;

    Valley () {}

    Valley (int p, int v) : position(p), value(v){
    }
};


#endif //IMAGE2CHAR_UTILITIES_H_H
