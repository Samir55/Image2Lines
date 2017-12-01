//
// Created by Ibrahim Radwan on 11/29/17.
//

#include "Chunk.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

Chunk::Chunk(int i, int c, int w, Mat img) : valleys(vector<Valley *>()), peaks(vector<Peak>()) {
    this->index = i;
    this->start_col = c;
    this->width = w;
    this->img = img.clone();
    this->histogram.resize((unsigned long) this->img.rows);
    this->avg_height = 0;
    this->lines_count = 0;

    this->calculate_histogram();
}

void
Chunk::calculate_histogram() {
    // Get the smoothed profile by applying a median filter of size 5.
    Mat img_clone;
    medianBlur(this->img, img_clone, 5);
    this->img = img_clone;

    avg_white_height = 0;
    int black_count = 0, current_height = 0, current_white_count = 0, white_lines_count = 0;
    vector<int> white_spaces;
    for (int i = 0; i < img_clone.rows; ++i) {
        black_count = 0;
        for (int j = 0; j < img_clone.cols; ++j) {
            if (img_clone.at<uchar>(i, j) == 0) {
                black_count++;
                this->histogram[i]++;
            }
        }
        if (black_count) {
            current_height++;
            if (current_white_count) {
                white_spaces.push_back(current_white_count);
            }
            current_white_count = 0;
        } else {
            current_white_count++;
            if (current_height) {
                lines_count++;
                avg_height += current_height;
            }
            current_height = 0;
        }
    }

    // Calculate the average height.
    if (lines_count) avg_height /= lines_count;

    // Calculate avg spaces height
    sort(white_spaces.begin(), white_spaces.end());
    for (int i = 0; i < white_spaces.size(); ++i) {
        if (white_spaces[i] > 4 * avg_height) break;
        avg_white_height += white_spaces[i];
        white_lines_count++;
    }

    if (white_lines_count) avg_white_height /= white_lines_count;
    avg_height = max(30, int(avg_height + (avg_height / 2.0)));
}

int
Chunk::find_peaks_valleys(map<int, Valley *> &map_valley) {
    // Detect Peaks.
    for (int i = 1; i < this->histogram.size() - 1; i++) {
        int left_val = this->histogram[i - 1], right_val = this->histogram[i], centre_val = this->histogram[i + 1];

        if (centre_val >= left_val && centre_val >= right_val) { // Peak detected.

            if (peaks.size() > 0 && i - peaks.back().position <= avg_height / 2 &&
                centre_val > peaks.back().value) { // Try to get the largest peak in same region.
                peaks.back().position = i;
                peaks.back().value = centre_val;
            } else if (peaks.empty() || (i - peaks.back().position > avg_height / 2 && centre_val > 0)) {
                peaks.push_back(Peak(i, centre_val));
            }
        }
    }

    // Sort peaks by max value and remove the outliers (the ones with less foreground pixels).
    sort(peaks.begin(), peaks.end(), Peak::comp_value);
    peaks.resize(
            lines_count + 1 <= peaks.size() ? (unsigned long) lines_count + 1 : peaks.size());

    // Sort peaks by least position.
    sort(peaks.begin(), peaks.end(), Peak::comp_position);

    // Search for valleys between 2 peaks.
    for (int i = 1; i <= peaks.size(); i++) {
        int min_position = peaks[i - 1].position;
        int min_value = peaks[i - 1].value;

        for (int j = (peaks[i - 1].position + avg_height / 3);
             j < (i == peaks.size() ? this->img.rows : peaks[i].position - avg_height / 3); j++) {
            int valley_black_count = 0;

            for (int l = 0; l < this->img.cols; ++l) {
                if (this->img.at<uchar>(j, l) == 0) {
                    valley_black_count++;
                }
            }

            if (i == peaks.size() && valley_black_count <= min_value) {
                min_value = valley_black_count;
                min_position = j;
                if (!min_value) {
                    min_position = min(this->img.rows - 10, min_position + avg_height);
                    j = this->img.rows;
                }
            } else if (min_value != 0 && valley_black_count <= min_value) {
                min_value = valley_black_count;
                min_position = j;
            }
        }
        Valley *new_valley = new Valley(this->index, min_position);

        valleys.push_back(new_valley);
        map_valley[new_valley->valley_id] = new_valley;
    }
    return int(ceil(avg_height));
}
