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

cv::Mat median_filter(const cv::Mat& inputImage, int kernelSize) {
    cv::Mat outputImage;
    cv::medianBlur(inputImage, outputImage, kernelSize);
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

cv::Mat brightnees_contrast(const cv::Mat& inputImage, double contrast, int brightness) {
    cv::Mat output;
    double alpha = contrast / 100.0;
    double beta = brightness - 100;
    inputImage.convertTo(output, -1, alpha, beta);
    return output;
}

cv::Mat erosion_morphology(const cv::Mat& inputImage, int kernel_size) {
    cv::Mat output;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                        cv::Size(2 * kernel_size + 1, 2 * kernel_size + 1),
                        cv::Point(kernel_size, kernel_size));
    cv::erode(inputImage, output, element);
    return output;
}

cv::Mat opening_morphology(const cv::Mat& inputImage, int kernel_size) {
    cv::Mat outputImage;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                        cv::Size(2 * kernel_size + 1, 2 * kernel_size + 1),
                        cv::Point(kernel_size, kernel_size));
    cv::morphologyEx(inputImage, outputImage, cv::MORPH_OPEN, element);
    return outputImage;
}

cv::Mat saturation(const cv::Mat& inputImage, double saturationScale) {
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> hsvChannels;
    cv::split(hsvImage, hsvChannels);
    
    hsvChannels[1] *= saturationScale;

    cv::merge(hsvChannels, hsvImage);
    cv::Mat outputImage;
    cv::cvtColor(hsvImage, outputImage, cv::COLOR_HSV2BGR);

    return outputImage;
}

cv:Mat BGR_channel_changer(const cv::Mat& inputImage, int channelIndex, double scale) {
    cv::Mat outputImage;
    std::vector<cv::Mat> bgrChannels;
    cv::split(inputImage, bgrChannels);

    if (channelIndex >= 0 && channelIndex < 3) {
        bgrChannels[channelIndex] *= scale;
    }

    cv::merge(bgrChannels, outputImage);
    return outputImage;
}
