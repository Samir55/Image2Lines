#include "LineSegmentation.hpp"

map<valley_id, Valley *> all_valleys_ids; ///< A Map from valley id to it's pointer.

LineSegmentation::LineSegmentation(string path_of_image) {
    this->color_img = imread(path_of_image, CV_LOAD_IMAGE_COLOR);
    this->grey_img = imread(path_of_image, CV_LOAD_IMAGE_GRAYSCALE);
}

void
LineSegmentation::pre_process_image() {
    // More filters are about to be applied.
    cv::Mat preprocessed_img, smoothed_img;

    // Noise reduction (Currently a basic filter).
    cv::blur(grey_img, smoothed_img, Size(3, 3), Point(-1, -1));

    // OTSU threshold and Binarization.
    cv::threshold(smoothed_img, binary_img, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
}

void
LineSegmentation::find_contours() {
    cv::Mat img_clone = this->binary_img;

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_clone, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));

    // Initializing rectangular and poly vectors.
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> bound_rect(contours.size() - 1);

    // Getting rectangular boundaries from contours.
    for (size_t i = 0; i < contours.size() - 1; i++) {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true); //apply approximation to polygons with accuracy +-3
        bound_rect[i] = boundingRect(Mat(contours_poly[i]));
    }

    // Merging the rectangular boundaries.
    Rect2d rectangle3;
    vector<Rect> merged_rectangles;
    bool is_repeated = false;
    Mat drawing = Mat::zeros(img_clone.size(), CV_64F);

    // Checking for intersecting rectangles.
    for (int i = 0; i < bound_rect.size(); i++) {
        is_repeated = false;

        for (int j = i + 1; j < bound_rect.size(); j++) {
            rectangle3 = bound_rect[i] & bound_rect[j];

            // Check for intersection/union.
            if (rectangle3.area() > 0) {
                is_repeated = true;
                rectangle3 = bound_rect[i] | bound_rect[j]; //merging
                Rect2d merged_rectangle(rectangle3.tl().x, rectangle3.tl().y, rectangle3.width, rectangle3.height);

                // Push in merged rectangle after checking all the inner loop.
                if (j == bound_rect.size() - 2)
                    merged_rectangles.push_back(merged_rectangle);

                // Update the current vector.
                bound_rect[j] = merged_rectangle;
            }
        }
        // Adding the non repeated (not intersected) rectangles.
        if (!is_repeated)
            merged_rectangles.push_back(bound_rect[i]);
    }
    this->contours = merged_rectangles;
}

void
LineSegmentation::generate_chunks() {
    int width = binary_img.cols;
    int chunk_width = width / CHUNKS_NUMBER;

    for (int i_chunk = 0, start_pixel = 0; i_chunk < CHUNKS_NUMBER; ++i_chunk) {
        this->chunks.push_back(Chunk(i_chunk, start_pixel, chunk_width, cv::Mat(binary_img,
                                                                                cv::Range(0, binary_img.rows), // Rows.
                                                                                cv::Range(start_pixel, start_pixel +
                                                                                                       chunk_width)))); // Cols.
        start_pixel += chunk_width;
    }
}

Line
LineSegmentation::connect_valleys(int i, Valley *current_valley, Line &line, int valleys_min_abs_dist) {
    if (i == 0 || chunks[i].valleys.empty()) return line;

    // Choose the closest valley in right chunk to the start valley.
    int connected_to = -1;
    int min_distance = 100000;
    for (int j = 0; j < this->chunks[i].valleys.size(); j++) {
        Valley *valley = this->chunks[i].valleys[j];
        // Check if the valley is not connected to any other valley.
        if (valley->used) continue;

        int dist = current_valley->position - valley->position;
        dist = dist < 0 ? -dist : dist;
        if (min_distance > dist && dist <= valleys_min_abs_dist) {
            min_distance = dist, connected_to = j;
        }
    }

    // Return line if the current valley is not connected any more to a new valley in the current chunk of index i.
    if (connected_to == -1) {
        return line;
    }

    line.valleys_ids.push_back(this->chunks[i].valleys[connected_to]->valley_id);
    Valley *v = this->chunks[i].valleys[connected_to];
    v->used = true;
    return connect_valleys(i - 1, v, line, valleys_min_abs_dist);
}

void
LineSegmentation::get_initial_lines() {
    int number_of_heights = 0, valleys_min_abs_dist = 0;

    // Get the histogram of the first CHUNKS_TO_BE_PROCESSED and get the overall average line height.
    for (int i = 0; i < CHUNKS_TO_BE_PROCESSED; i++) {
        int avg_height = this->chunks[i].find_peaks_valleys();
        if (avg_height) number_of_heights++;
        valleys_min_abs_dist += avg_height;
    }
    valleys_min_abs_dist /= number_of_heights;

    // Start form the CHUNKS_TO_BE_PROCESSED chunk.
    for (int i = CHUNKS_TO_BE_PROCESSED - 1; i >= 0; i--) {
        // Check if the chunk is empty.
        if (chunks[i].valleys.empty()) continue;

        // Connect each valley with the nearest ones in the left chunks.
        for (auto &valley : chunks[i].valleys) {
            // Ignore the already connected valleys.
            if (valley->used) continue;
            // ToDo @Samir55 ignore if there one used before having a close level value to it.
            // Start a new line having the current valley and connect it with others in the left.
            valley->used = true;
            Line new_line(this->initial_lines.size(), valley->valley_id);
            new_line = connect_valleys(i - 1, valley, new_line, valleys_min_abs_dist);
            if (new_line.valleys_ids.size() > 1)
                this->initial_lines.push_back(new_line);
        }
    }
}

// ToDo @Samir55 REFACTOR
void
LineSegmentation::generate_initial_points() {
    int last_min_position = 0;
    int chunk_width = chunks.front().width;
    for (auto &line : this->initial_lines) {

        int c = 0, previous_row = 0, min_row_position = 0, max_row_position = 0;
        // Sort the valleys according to their chunk number.
        sort(line.valleys_ids.begin(), line.valleys_ids.end());

        // Add line points in the first chunks having no valleys.
        if (all_valleys_ids[line.valleys_ids.front()]->chunk_order > 0) {
            previous_row = all_valleys_ids[line.valleys_ids.front()]->position;
            max_row_position = min_row_position = previous_row;
            for (int j = 0; j < this->chunks[all_valleys_ids[line.valleys_ids.front()]->chunk_order].start_col; j++) {
                if (c++ == j)
                    line.points.push_back(Point(previous_row, j));
            }
        }

        // Add line points between the valleys.
        for (auto id : line.valleys_ids) {
            int chunk_order = all_valleys_ids[id]->chunk_order, chunk_row = all_valleys_ids[id]->position;
            for (int j = this->chunks[chunk_order].start_col;
                 j < this->chunks[chunk_order].start_col + chunk_width; j++) {
                min_row_position = min(min_row_position, chunk_row);
                max_row_position = max(max_row_position, chunk_row);
                if (c++ == j)
                    line.points.push_back(Point(chunk_row, j));
            }
            if (previous_row != chunk_row) {
                previous_row = chunk_row;
                min_row_position = min(min_row_position, chunk_row);
                max_row_position = max(max_row_position, chunk_row);
            }
        }

        // Add line points in the last chunks having no valleys.
        if (CHUNKS_NUMBER - 1 > all_valleys_ids[line.valleys_ids.back()]->chunk_order) {
            int chunk_order = all_valleys_ids[line.valleys_ids.back()]->chunk_order,
                    chunk_row = all_valleys_ids[line.valleys_ids.back()]->position;
            for (int j = this->chunks[chunk_order].start_col + chunk_width; j < color_img.cols; j++) {
                if (c++ == j)
                    line.points.push_back(Point(chunk_row, j));
            }
        }
        cout << line.index << " " << max_row_position << " " << last_min_position << endl;
        line.start_row_position = last_min_position;
        line.height = max_row_position - last_min_position;
        last_min_position = min_row_position;
    }
}

void
LineSegmentation::show_lines() {
    cv::Mat img_clone = this->color_img.clone();
    int last_row = -1;
    for (auto line : initial_lines) {
        for (auto point : line.points) {
            img_clone.at<Vec3b>(point.x, point.y) = TEST_LINE_COLOR;
            // Check and draw vertical lines if found.
            if (last_row != -1 && point.x != last_row) {
                for (int i = min(last_row, point.x); i < max(last_row, point.x); i++) {
//                    img_clone.at<Vec3b>(i, point.y) = TEST_LINE_COLOR;
                }
            }
            last_row = point.x;
        }
    }
    cv::imwrite("Final_Lines.jpg", img_clone); // For debugging.
}

void
LineSegmentation::get_regions() {
    for (auto line : this->initial_lines) {
        if (line.valleys_ids.size() <= 1 || line.points.size() <= 1 ||
            line.index == initial_lines.size() - 1)
            continue;

        cv::Mat new_region = Mat::ones(line.height + 1, this->binary_img.cols, CV_8U) * 255;
        vector<int> row_offset;
        // Fill region.
        for (int c = 0; c < binary_img.cols; c++) {
            for (int i = line.points[c].x; i < initial_lines[line.index + 1].points[c].x; i++) {
                row_offset.push_back(line.points[c].x);
                int t = i - line.points[c].x;
                if ( t <= line.height)
                new_region.at<uchar>(t, c) = this->binary_img.at<uchar>(i, c);
            }
        }
        cv::imwrite(string("test") + to_string(line.index) + ".jpg",
                    new_region); // ToDo @Samir: Remove as it's for debugging.

        this->line_regions.push_back(Region(new_region, row_offset));
    }
}

void
LineSegmentation::repair_lines() {
    // Loop over the regions.
    for (auto &line : initial_lines) {
        // ToDo @Samir55 Fix this.
        if (line.index == this->initial_lines.size() - 1) continue;

        for (int i = 0; i < line.points.size(); i++) {
            Point &point = line.points[i];
            if (this->binary_img.at<uchar>(point.x, point.y) == 255) continue;
            int x = line.points[i].x, y = line.points[i].y; // TODO @Samir55 FIX.

            for (auto contour : this->contours) {
                if (y >= contour.tl().x && y <= contour.br().x && x >= contour.tl().y && x <= contour.br().y) {
//                    cout << "HIT Contour at " << point.x << " , " << point.y << endl;
                    // Get the regions.
                    int region_above = line.index, region_below = line.index + 1;

                    // Calculate probabilities.
                    // ToDo @Samir55 Ignore: if the contour height greater than the average height.
                    double prob_above = 1.0, prob_below = 1.0;
                    for (int i = contour.tl().x; i < contour.tl().x + contour.width; i++) {
                        for (int j = contour.tl().y; j < contour.tl().y + contour.height; j++) {
                            if (binary_img.at<uchar>(j, i) == 255) continue;

                            Mat point = Mat::zeros(1, 2, CV_32F);
                            point.at<float>(0, 0) = i;
                            point.at<float>(0, 1) = j;
//                            cout << "Region HERE at line" << line.index << " " << region_above << " " << region_below
//                                 << endl;
                            prob_above *= this->line_regions[region_above].biVarGaussianDensity(point);
                            prob_below *= this->line_regions[region_below].biVarGaussianDensity(point);
//                            cout << "Probability above is " << prob_above << " Probability below is " << prob_below
//                                 << endl;
                        }
                    }
                    // Assign to the highest probability.
//                    cout << "Probability above is " << prob_above << " Probability below is " << prob_below << endl;
                    int new_row;
                    if (prob_above - 0.00000001 > prob_below) {
                        new_row = contour.br().y;
                    } else {
                        new_row = contour.tl().y;
                    }
                    for (int k = point.y; k < point.y + contour.width; k++) {
                        line.points[k].x = new_row;
                    }
                    i += (contour.width - 1);
                }
            }
        }
    }
}

vector<cv::Mat>
LineSegmentation::get_lines() {
    this->pre_process_image();
    this->find_contours();
    this->generate_chunks();
    this->get_initial_lines();
    this->generate_initial_points();
    this->get_regions();
    this->repair_lines();
    this->get_regions();
    this->show_lines();
    return vector<cv::Mat>();
}

Chunk::Chunk(int o, int c, int w, cv::Mat i) : valleys(vector<Valley *>()), peaks(vector<Peak>()) {
    this->order = o;
    this->start_col = c;
    this->width = w;
    this->img = i.clone();
    this->histogram.resize(this->img.rows);
}

int
Chunk::find_peaks_valleys() {
    // Get the smoothed profile by applying a median filter of size 5.
    cv::Mat img_clone;
    cv::medianBlur(this->img, img_clone, 5);

    int black_count = 0, avg_height = 0, lines_count = 0, current_height = 0;
    for (int i = 0; i < img_clone.rows; ++i) {
        black_count = 0;
        for (int j = 0; j < img_clone.cols; ++j) {
            if (img_clone.at<uchar>(i, j) == 0) {
                black_count++;
                this->histogram[i]++;
            }
        }
        if (black_count) current_height++;
        else {
            if (current_height) {
                lines_count++;
                avg_height += current_height;
            }
            current_height = 0;
        }
    }

    // Calculate the average height.
    if (lines_count) avg_height /= lines_count;
    avg_height = avg_height + (avg_height / 2.0);

    // Detect Peaks.
    vector<Peak> initial_peaks;
    for (int i = 1; i < this->histogram.size() - 1; i++) {
        int left_val = this->histogram[i - 1], right_val = this->histogram[i], centre_val = this->histogram[i + 1];
        if (centre_val > left_val && centre_val > right_val) { // Peak detected.
            if (initial_peaks.size() > 0 && i - initial_peaks.back().position <= avg_height / 2 &&
                centre_val >= initial_peaks.back().value) { // Try to get the largest peak in same region.
                initial_peaks.back().position = i;
                initial_peaks.back().value = centre_val;
            } else if (initial_peaks.size() > 0 && i - initial_peaks.back().position <= avg_height / 2 &&
                       centre_val < initial_peaks.back().value) {}
            else {
                initial_peaks.push_back(Peak(i, centre_val));
            }
        }
    }

    // Sort peaks by max value.
    sort(initial_peaks.begin(), initial_peaks.end());

    // Resize
    initial_peaks.resize(lines_count + 1 <= initial_peaks.size() ? lines_count + 1 : initial_peaks.size());

    // Sort peaks by least position.
    sort(initial_peaks.begin(), initial_peaks.end(), Peak::comp);
    this->peaks = initial_peaks;

    // Search for valleys between 2 peaks.
    vector<Valley *> initial_valleys;
    for (int i = 1; i < initial_peaks.size(); i++) {
        int min_position = initial_peaks[i - 1].position;
        int min_value = initial_peaks[i - 1].value;

        for (int j = (initial_peaks[i - 1].position + avg_height / 3);
             j < (initial_peaks[i].position - avg_height / 3); j++) {
            int valley_black_count = 0;
            for (int l = 0; l < img_clone.cols; ++l) {
                if (img_clone.at<uchar>(j, l) == 0) {
                    valley_black_count++;
                }
            }
            if (valley_black_count <= min_value && j < initial_peaks[i].position - max(50, avg_height / 2)) {
                min_value = valley_black_count;
                min_position = j;
            }
        }
        Valley *new_valley = new Valley(this->order, int(all_valleys_ids.size()), min_position, min_value);

        initial_valleys.push_back(new_valley);
        initial_valleys.back()->valley_id = int(all_valleys_ids.size());
        all_valleys_ids[initial_valleys.back()->valley_id] = new_valley;
    }
    this->valleys = initial_valleys;
    return int(ceil(avg_height));
}