#include <iostream>
#include "src/LineSegmentation.h"

int main() {
    LineSegmentation ls("../src/Data/input/1min.png");
    ls.segment();

    return 0;
}