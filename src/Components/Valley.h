//
// Created by Ibrahim Radwan on 11/29/17.
//

#ifndef IMAGE2LINES2_VALLEY_H
#define IMAGE2LINES2_VALLEY_H

class Line;

class Valley {
public:
    static int ID;

    int chunk_index;
    ///< The index of the chunk in the chunks vector.
    const int valley_id;
    ///< The valley id.
    int position;
    ///< The row position.
    bool used;
    /// Whether it's used by a line or not.
    Line* line;
    /// The line that this valley is connected to

    Valley() : valley_id(ID++), used(false) {}

    Valley(int c_id, int p) : chunk_index(c_id), valley_id(ID++), position(p), used(false) {}
};


#endif //IMAGE2LINES2_VALLEY_H
