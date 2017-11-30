//
// Created by Ibrahim Radwan on 11/29/17.
//

#include "Line.h"

Line::Line(int initial_valley_id) : points(vector<Point>()) {
    valleys_ids.push_back(initial_valley_id);
}

void
Line::generate_initial_points(int chunk_width, int img_width, map<int, Valley *> map_valley) {
    int c = 0, previous_row = 0;

    // Sort the valleys according to their chunk number.
    sort(valleys_ids.begin(), valleys_ids.end());

    // Add line points in the first chunks having no valleys.
    if (map_valley[valleys_ids.front()]->chunk_index > 0) {
        previous_row = map_valley[valleys_ids.front()]->position;
        max_row_position = min_row_position = previous_row;
        for (int j = 0; j < map_valley[valleys_ids.front()]->chunk_index * chunk_width; j++) {
            if (c++ == j)
                points.push_back(Point(previous_row, j));
        }
    }

    // Add line points between the valleys.
    for (auto id : valleys_ids) {
        int chunk_index = map_valley[id]->chunk_index;
        int chunk_row = map_valley[id]->position;
        int chunk_start_column = chunk_index * chunk_width;

        for (int j = chunk_start_column; j < chunk_start_column + chunk_width; j++) {
            min_row_position = min(min_row_position, chunk_row);
            max_row_position = max(max_row_position, chunk_row);
            if (c++ == j)
                points.push_back(Point(chunk_row, j));
        }
        if (previous_row != chunk_row) {
            previous_row = chunk_row;
            min_row_position = min(min_row_position, chunk_row);
            max_row_position = max(max_row_position, chunk_row);
        }
    }

    // Add line points in the last chunks having no valleys.
    if (CHUNKS_NUMBER - 1 > map_valley[valleys_ids.back()]->chunk_index) {
        int chunk_index = map_valley[valleys_ids.back()]->chunk_index,
                chunk_row = map_valley[valleys_ids.back()]->position;
        for (int j = chunk_index * chunk_width + chunk_width; j < img_width; j++) {
            if (c++ == j)
                points.push_back(Point(chunk_row, j));
        }
    }
}

bool
Line::comp_min_row_position(const Line *a, const Line *b) {
    return a->min_row_position < b->min_row_position;
}