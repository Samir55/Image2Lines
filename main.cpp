#include <iostream>
#include "src/LineSegmentation.h"

int main() {
    LineSegmentation ls("../src/Data/input/2min.png");
    ls.segment();

    return 0;
}