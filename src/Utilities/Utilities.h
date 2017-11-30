//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_UTILITIES_H
#define IMAGE2LINES2_UTILITIES_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
#include <cstring>
#include <cmath>
#include <math.h>

#include "opencv2/opencv.hpp"
#include "opencv/cv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "Constants.h"

using namespace std;
using namespace cv;

class Utilities {
public:
    static vector<int> primes;

    static bool notPrimesArr[100007];
    ///< Not primes array (flags)

    static void sieve();

    static void addPrimesToVector(int n, vector<int> &probPrimes);
};


#endif //IMAGE2LINES2_UTILITIES_H
