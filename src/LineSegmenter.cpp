#include "LineSegmenter.hpp"

cv::Mat
LineSegmenter::preprocess(const cv::Mat img)
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
LineSegmenter::get_chunks(const cv::Mat img)
{

    int width = img.cols;
    int chunk_width = width / CHUNKS_NUMBER;

    vector<Chunk> chunks(CHUNKS_NUMBER);
    for (int i_chunk = 0, start_pixel = 0; i_chunk < CHUNKS_NUMBER; ++i_chunk) {
        chunks[i_chunk].order = i_chunk + 1;
        chunks[i_chunk].start_col = start_pixel;
        chunks[i_chunk].width = chunk_width;
//        cout << chunk_width <<  " " << start_pixel << " " << img.rows << " " << img.cols << endl; // For debugging.
        chunks[i_chunk].img = cv::Mat(img,
                                      cv::Range(0, img.rows), // rows
                                      cv::Range(start_pixel, start_pixel + chunk_width)); // cols
        start_pixel += chunk_width;
//        cv::imwrite(to_string(i_chunk + 1) + ".jpg", chunks[i_chunk].img); // For debugging.
    }
    return chunks;
}

// ToDo @TheAbzo.
void
Chunk::find_contours()
{
    waitKey(0);
}

void
Chunk::calculate_histogram()
{
    freopen("out.txt", "w", stdout);
    // Assign the size of the hitogram // ToDo @Samir55 later add it to the constructor.
    this->histogram.resize(this->img.rows);

    int count_black = 0; // ToDo @Samir: Remove as it's for debugging.
    int count_white = 0;
    for (int i = 0; i < this->img.rows ; ++i) {
        for (int j = 0; j < this->img.cols ; ++j) {
            if (img.at<uchar>(i, j) == 0) {
                count_black++;
                this->histogram[i]++;
            }
            else count_white++;
        }
    }

//    cout << histogram.size() << " " << count_white << endl;
    for (int i = 0; i < histogram.size(); i++)
        cout << histogram[i] << ' ';
    cout << endl;

}