//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_PEAK_H
#define IMAGE2LINES2_PEAK_H


class Peak {
public:
    int position;
    ///< The row position.

    int value;
    ///< The number of foreground pixels.

    Peak() {}

    Peak(int p, int v) : position(p), value(v) {}

    /// Compare according to the value.
    static bool
    comp_value(const Peak &a, const Peak &b);

    /// Compare according to the row position
    static bool
    comp_position(const Peak &a, const Peak &b);
};


#endif //IMAGE2LINES2_PEAK_H
