#pragma once
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;



struct CannyParameters {
    struct threshold {
        double low_threshold;
        double max_threshold;
    } threshold;
};
struct DenoiseConfig {
    int h;
    int hColor;
    int templateWindowSize;
    int searchWindowSize;
};
struct BlobDetectionConfig {
    int kernel_size;
    int connectivity;
};

struct BilateralFilterConfig {
    int d;
    int sigmaColor;
    int sigmaSpace;
};

struct BrightnessContrastConfig {
    int brightness;
    int contrast;
};

struct Config {
    DenoiseConfig denoise;
    CannyParameters canny_parameters;
    BlobDetectionConfig blob_detection;
    BilateralFilterConfig bilateral_filter;
    BrightnessContrastConfig brightness_contrast;
};



Config loadConfig(const std::string& path);


struct BlobData {
    cv::Mat labels;      // CV_32S matrix of labels
    cv::Mat stats;       // CV_32S matrix of stats
    cv::Mat centroids;   // CV_64F matrix of centroids
    int numLabels;       // Number of detected labels
};

cv::Mat fetch_image(const std::string& filename);

BlobData blob_detection(const cv::Mat& inputImage, int connectivity = 8);

cv::Mat draw_circles(const cv::Mat& inputImage, const cv::Point& centroids, int radius, int i = 0);

void tuning(const cv::Mat& inputImage, int mode = 0);

cv::Mat brightnees_contrast(const cv::Mat& inputImage, double contrast, int brightness);

cv::Mat erosion_morphology(const cv::Mat& inputImage, int kernel_size);

cv::Mat opening_morphology(const cv::Mat& inputImage, int kernel_size);