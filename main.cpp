#include <iostream>
#include "src/LineSegmentation.h"

int main() {
    int c;
    cin >> c;

    LineSegmentation ls("../src/Data/input/" + to_string(c) + ".png");
    ls.segment();

    return 0;
}