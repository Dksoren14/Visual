#include "manipulator.h"



cv::Mat denoise_algorithm(const cv::Mat& inputImage, int h, int hColor, int templateWindowSize, int searchWindowSize) {
    // Placeholder for de-noising algorithm implementation
    cv::Mat outputImage;
    cv::fastNlMeansDenoisingColored(inputImage, outputImage, h, hColor, templateWindowSize, searchWindowSize);
    return outputImage;
}

cv::Mat bilateralFilter(const cv::Mat& inputImage, int d, double sigmaColor, double sigmaSpace) {
    cv::Mat outputImage;
    cv::bilateralFilter(inputImage, outputImage, d, sigmaColor, sigmaSpace);
    return outputImage;
}

cv::Mat canny_edge_detection(const cv::Mat& inputImage, double lowThreshold, double highThreshold) {
    cv::Mat edges;
    cv::Canny(inputImage, edges, lowThreshold, highThreshold);
    return edges;
}


cv::Mat closing_morphology(const cv::Mat& inputImage, int kernelSize) {
    cv::Mat outputImage;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(inputImage, outputImage, cv::MORPH_CLOSE, element);
    return outputImage;
}

