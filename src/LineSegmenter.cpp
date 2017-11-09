#include "LineSegmenter.hpp"

map<valley_id, Valley*> all_valleys_ids;
void
LineSegmenter::preprocess()
{
    // More filters are about to be applied. TheAbzo job.
    cv::Mat preprocessed_img, smoothed_img;

    // Noise reduction (Currently a basic filter).
    cv::blur(org_img, smoothed_img, Size(3, 3), Point(-1, -1));

    // OTSU threshwolding and Binarization.
    cv::threshold(smoothed_img, preprocessed_img, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite(string("preprocessed_test") + ".jpg", preprocessed_img); // ToDo @Samir: Remove as it's for debugging.
    img = preprocessed_img;
}

void
LineSegmenter::generate_chunks()
{
    //
    int width = img.cols;
    int chunk_width = width / CHUNKS_NUMBER;

    //
    vector<Chunk> generated_chunks(CHUNKS_NUMBER);
    for (int i_chunk = 0, start_pixel = 0; i_chunk < CHUNKS_NUMBER; ++i_chunk) {
        generated_chunks[i_chunk].order = i_chunk + 1;
        generated_chunks[i_chunk].start_col = start_pixel;
        generated_chunks[i_chunk].width = chunk_width;
        generated_chunks[i_chunk].img = cv::Mat(img,
                                                cv::Range(0, img.rows), // Rows.
                                                cv::Range(start_pixel, start_pixel + chunk_width)); // Cols.
        start_pixel += chunk_width;
        cv::imwrite(to_string(i_chunk + 1) + ".jpg", generated_chunks[i_chunk].img); // For debugging.
    }
    this->chunks = generated_chunks;
}

Line
LineSegmenter::connect_line_valleys(int i, Valley current_valley, Line line)
{
    if (i <= 0 || chunks[i].valleys.empty()) return line;

    // Choose the closest valley in right chunk to the start valley.
    int connected_to = -1;
    int min_distance = 100000;

    for (int j = 0; j < this->chunks[i].valleys.size(); j++) {
        Valley valley = this->chunks[i].valleys[j];
        // Check if the valley is not connected to any other valley.
        if (valley.used) continue;

        int dist = current_valley.position - valley.position;
        dist = dist < 0 ? -dist : dist;

        if (min_distance > dist && dist <= 25) {
            min_distance = dist, connected_to = j;
        }
    }

    // Return line if the current valley is not connected any more to a new valley in the current chunk of index i.
    if (connected_to == -1) {
        return line;
    }

    line.valleys_ids.push_back(this->chunks[i].valleys[connected_to].valley_id);
    current_valley = this->chunks[i].valleys[connected_to];
    current_valley.used = true;

    return connect_line_valleys(i-1, current_valley, line);
}

void
LineSegmenter::find_initial_lines()
{
    // Get the histogram of the first 5 chunks.
    for (int i = 0; i < 5; i++) {
        this->chunks[i].calculate_histogram();
    }

    // Start form the 5th chunk.
    for (int i = 4; i > 0; i--) {
        // Check if the chunk is empty.
        if (chunks[i].valleys.empty()) continue;

        // Connect each valley with the nearest ones in the left chunks.
        for (auto valley : chunks[i].valleys) {
            // Ignore the already connected valleys;
            if (valley.used) continue;

            // Start a new line having the current valley and connect it with others in the left.
            Line new_line;
            valley.used = true;
            new_line.valleys_ids.push_back(valley.valley_id);

            this->initial_lines.push_back(connect_line_valleys(i-1, valley, new_line));
        }
    }

}

void
Chunk::find_contours()
{
    cv::Mat img_clone = this->img;
    // ToDO @TheAbzo add more filters

    // Contours detection.
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_clone, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));

    // Initializing rectangular and poly vectors.
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> bound_rect(contours.size() - 1);

    // Getting rectangular boundries from contours.
    for (size_t i = 0; i < contours.size() - 1; i++) {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);//apply approximation to polygons with accuracy +-3
        bound_rect[i] = boundingRect(Mat(contours_poly[i]));
    }

    // Merging the rectangular boundaries.
    Rect2d rectangle3;
    vector<Rect> merged_rectangles;
    bool is_repeated = false;
    Mat drawing = Mat::zeros(img_clone.size(), CV_8UC3);

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
    rectangular_contours = merged_rectangles;
}

Chunk::Chunk()
{} // ToDo @Samir55 move this to header file.

void
Chunk::calculate_histogram()
{
//    freopen("out.txt", "w", stdout);

    // Get the smoothed profile by applying a median filter of size 5.
    cv::Mat img_clone;
    cv::medianBlur(this->img, img_clone, 5);

    // Assign the size of the hitogram // ToDo @Samir55 later add it to the constructor.
    this->histogram.resize(img_clone.rows);

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
//                cout << "Current hieght " << current_height << endl;
                lines_count++;
                avg_height += current_height;
            }
            current_height = 0;
        }
    }

    // Calculate the average height.
    if (lines_count) avg_height /= lines_count;
    avg_height = avg_height * 5 / 3;

    cout << "Lines count " << lines_count << endl;
    cout << "Average height " << avg_height << endl;

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
                       centre_val < initial_peaks.back().value) {

            } else {
                // cout << "LEFT " << left_val << " CENTRE " << centre_val << " RIGHT " << right_val << endl;
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
    vector<Valley> initial_valleys;
    for (int i = 1; i < initial_peaks.size(); i++) {
        int min_position = initial_peaks[i - 1].position;
        int min_value = initial_peaks[i - 1].value;

        for (int j = (initial_peaks[i - 1].position + avg_height / 6);
             j < (initial_peaks[i].position - avg_height / 3); j++) {
            int valley_black_count = 0;
            for (int l = 0; l < img_clone.cols; ++l) {
                if (img_clone.at<uchar>(j, l) == 0) {
                    valley_black_count++;
                }
            }
            if (valley_black_count < min_value) {
                min_value = valley_black_count;
                min_position = j;
            }
        }
        initial_valleys.push_back(Valley(int(all_valleys_ids.size()), min_position, min_value));
    }

    this->valleys = initial_valleys;

    cout << "Peaks Count " << initial_peaks.size() << endl;
    for (auto peak:peaks) {
        cout << peak.position << " " << peak.value << endl;
    }
    cout << "Valleys Count " << initial_valleys.size() << endl;
    for (auto valley:valleys) {
        cout << valley.position << " " << valley.value << endl;
    }

}