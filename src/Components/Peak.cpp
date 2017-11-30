//
// Created by Ibrahim Radwan on 11/29/17.
//

#include "Peak.h"

bool
Peak::comp_value(const Peak &a, const Peak &b) {
    return a.value > b.value;
}

bool
Peak::comp_position(const Peak &a, const Peak &b) {
    return a.position < b.position;
}
