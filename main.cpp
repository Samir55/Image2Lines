#include <iostream>
#include "src/LineSegmentation.h"

int main() {
//    LineSegmentation ls("../src/Data/input/1min.png");
//    LineSegmentation ls("../src/Data/input/2min.png");
//    LineSegmentation ls("../src/Data/input/3min.png");
//    LineSegmentation ls("../src/Data/input/4min.png");
    LineSegmentation ls("../src/Data/input/A.jpg");
//    LineSegmentation ls("../src/Data/input/5min.png");
    ls.segment();

    return 0;
}