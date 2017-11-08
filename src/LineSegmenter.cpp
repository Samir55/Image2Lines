#include "LineSegmenter.hpp"

cv::Mat
LineSegmenter::preprocess (const cv::Mat img)
{
    // More filters are about to be applied. TheAbzo job.
    cv::Mat preprocessed_img, smoothed_img;

    // Noise reduction (Currently a basic filter).
    cv::blur(img, smoothed_img, Size(3, 3), Point(-1, -1));

    // OTSU thresholding and Binarization.
    cv::threshold(smoothed_img, preprocessed_img, 0.0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite(string("preprocessed_test") + ".jpg", preprocessed_img); // ToDo @Samir: Remove as it's for debugging.
    return preprocessed_img;
}

vector<Chunk>
LineSegmenter::get_chunks (const cv::Mat img)
{
    //
    int width = img.cols;
    int chunk_width = width / CHUNKS_NUMBER;

    //
    vector<Chunk> chunks(CHUNKS_NUMBER);
    for (int i_chunk = 0, start_pixel = 0; i_chunk < CHUNKS_NUMBER; ++i_chunk) {
        chunks[i_chunk].order = i_chunk + 1;
        chunks[i_chunk].start_col = start_pixel;
        chunks[i_chunk].width = chunk_width;
        chunks[i_chunk].img = cv::Mat(img,
                                      cv::Range(0, img.rows), // Rows.
                                      cv::Range(start_pixel, start_pixel + chunk_width)); // Cols.
        start_pixel += chunk_width;
        cv::imwrite(to_string(i_chunk + 1) + ".jpg", chunks[i_chunk].img); // For debugging.
    }
    return chunks;
}

// ToDo @TheAbzo.
void
Chunk::find_contours ()
{
    waitKey(0);
}

void
Chunk::calculate_histogram ()
{
//    freopen("out.txt", "w", stdout);

    // Get the smoothed profile by applying a meidan filter of size 5.
    cv::Mat img_clone;
    cv::medianBlur(this->img, img_clone, 5);

    // Assign the size of the hitogram // ToDo @Samir55 later add it to the constructor.
    this->histogram.resize(img_clone.rows);

    int black_count = 0, avg_height = 0, lines_count = 0, current_height = 0;
    for (int i = 0; i < img_clone.rows ; ++i) {
        black_count = 0;
        for (int j = 0; j < img_clone.cols ; ++j) {
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

    cout << "Lines count " << lines_count << endl;
    cout << "Average height " << avg_height << endl;

    // Detect Peaks.
    vector<Peak> initial_peaks;
    for (int i = 1; i < this->histogram.size() - 1; i++) {
        int left_val = this->histogram[i-1], right_val = this->histogram[i], centre_val = this->histogram[i+1];
        if (centre_val > left_val && centre_val > right_val) { // Peak detected.
            if (initial_peaks.size() > 0 && i - initial_peaks.back().position <= avg_height/2 && centre_val >= initial_peaks.back().value) { // Try to get the largest peak in same region.
                initial_peaks.back().position = i;
                initial_peaks.back().value = centre_val;
            } else if (initial_peaks.size() > 0 && i - initial_peaks.back().position <= avg_height/2 && centre_val < initial_peaks.back().value) {

            } else {
                // cout << "LEFT " << left_val << " CENTRE " << centre_val << " RIGHT " << right_val << endl;
                initial_peaks.push_back(Peak(i, centre_val));
            }
        }
    }

    // Sort peaks by max value.
    sort(initial_peaks.begin(), initial_peaks.end());

    // Resize
    initial_peaks.resize(lines_count + 1 <= initial_peaks.size() ? lines_count + 1: initial_peaks.size());

    // Sort peaks by least position.
    sort(initial_peaks.begin(), initial_peaks.end(), Peak::comp);

    // Search for valleys between 2 peaks.
    vector<Valley> initial_valleys;
    for (int i = 1; i < initial_peaks.size(); i++) {
        int min_position = initial_peaks[i-1].position;
        int min_value = initial_peaks[i-1].value;

        for (int j = initial_peaks[i-1].position + 1; j < initial_peaks[i].position - avg_height / 3; j++) {
            int valley_black_count = 0;
            for (int l = 0; l < img_clone.cols ; ++l) {
                if (img_clone.at<uchar>(j, l) == 0) {
                    valley_black_count++;
                }
            }
            if (valley_black_count <= min_value) {
                min_value = valley_black_count;
                min_position = j;
            }
        }
        initial_valleys.push_back(Valley(min_position, min_value));
    }

    cout << "Peaks Count " << initial_peaks.size() << endl;
    for (auto peak:initial_peaks) {
        cout << peak.position <<  " " << peak.value << endl;
    }
    cout << "Valleys Count " << initial_valleys.size() << endl;
    for (auto valley:initial_valleys) {
        cout << valley.position <<  " " << valley.value << endl;
    }

}